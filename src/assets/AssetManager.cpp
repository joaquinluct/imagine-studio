#include "AssetManager.h"
#include <thread>
#include <chrono>
#include "../jobs/ThreadPool.h"
#include "../core/Log.h"
#include "../renderer/Fence.h"
#include "VFS.h"

using namespace Assets;

namespace Assets {

static Jobs::ThreadPool* s_threadPool = nullptr;

AssetManager::AssetManager(){}
AssetManager::~AssetManager(){}

void AssetManager::Initialize()
{
    if (!s_threadPool)
        s_threadPool = new Jobs::ThreadPool(2);
    vfs_ = new VFS();
    dispatcherRunning_.store(true);
    dispatcherThread_ = std::thread([this](){
        while (dispatcherRunning_.load()) {
            Task task;
            {
                std::unique_lock<std::mutex> lock(queueMutex_);
                queueCv_.wait(lock, [this]{ return !taskQueue_.empty() || !dispatcherRunning_.load(); });
                if (!dispatcherRunning_.load()) break;
                task = taskQueue_.top(); taskQueue_.pop();
            }

            // Check cancellation
            {
                std::lock_guard<std::mutex> lock(cancelMutex_);
                if (cancelled_.count(task.handle)) continue;
            }

            // Dispatch to thread pool
            s_threadPool->Enqueue([this, task]() {
                CORE_LOG_INFO(std::string("Loading asset (dispatched): ") + task.path);
                std::vector<char> buf;
                bool ok = false;
                if (vfs_) ok = vfs_->ReadFile(task.path, buf);
                if (!ok) { std::this_thread::sleep_for(std::chrono::milliseconds(200)); buf.assign(128,0); }
                pools_.push_back(std::move(buf));
                if (task.callback) task.callback(task.path);
                CORE_LOG_INFO(std::string("Loaded asset (dispatched): ") + task.path);
                if (task.fence) task.fence->Signal();
            });
        }
    });
    CORE_LOG_INFO("AssetManager initialized");
}

void AssetManager::Shutdown()
{
    if (s_threadPool)
    {
        s_threadPool->Shutdown();
        delete s_threadPool;
        s_threadPool = nullptr;
    }
    if (vfs_) { delete vfs_; vfs_ = nullptr; }
    // stop dispatcher
    dispatcherRunning_.store(false);
    queueCv_.notify_all();
    if (dispatcherThread_.joinable()) dispatcherThread_.join();
    CORE_LOG_INFO("AssetManager shutdown");
}

AssetManager::LoadHandle AssetManager::LoadAsync(const std::string& path, std::function<void(const std::string&)> callback, Renderer::Fence* signalFence, AssetManager::Priority priority)
{
    if (!s_threadPool)
    {
        CORE_LOG_ERROR("ThreadPool not initialized for AssetManager");
        return 0;
    }

    // enqueue into dispatcher with normal priority
    Task t;
    t.handle = nextHandle_.fetch_add(1);
    t.priority = priority;
    t.path = path; t.callback = callback; t.fence = signalFence;
    {
        std::lock_guard<std::mutex> lock(queueMutex_);
        taskQueue_.push(t);
    }
    queueCv_.notify_one();
    return t.handle;
}

bool AssetManager::Mount(const std::string& path)
{
    if (!vfs_) return false;
    return vfs_->Mount(path);
}
bool AssetManager::CancelLoad(LoadHandle handle)
{
    std::lock_guard<std::mutex> lock(cancelMutex_);
    if (cancelled_.count(handle)) return false;
    cancelled_.insert(handle);
    return true;
}

} // namespace Assets
