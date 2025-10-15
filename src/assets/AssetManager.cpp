#include "AssetManager.h"
#include <thread>
#include <chrono>
#include <memory>
#include <unordered_map>
#include "../jobs/ThreadPool.h"
#include "../core/Log.h"
#include "../renderer/Fence.h"
#include "VFS.h"
#include <array>
#include <sstream>

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
    for (int i = 0; i < 3; ++i)
    {
        metrics_requested_by_prio_[i].store(0);
        metrics_completed_by_prio_[i].store(0);
        metrics_cancelled_by_prio_[i].store(0);
        metrics_total_time_ms_by_prio_[i].store(0);
    }
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

            // mark start
            metrics_started_.fetch_add(1);
            metrics_requested_by_prio_[static_cast<int>(task.priority)].fetch_add(1);

            // Dispatch to thread pool with cooperative cancellation support
            auto cancelFlag = std::make_shared<std::atomic<bool>>(false);
            {
                std::lock_guard<std::mutex> rl(runningMutex_);
                runningCancelFlags_[task.handle] = cancelFlag;
                runningPriority_[task.handle] = task.priority;
            }

            s_threadPool->Enqueue([this, task, cancelFlag]() {
                CORE_LOG_INFO(std::string("Loading asset (dispatched): ") + task.path);
                auto tstart = std::chrono::steady_clock::now();
                std::vector<char> buf;
                bool ok = false;
                if (vfs_) ok = vfs_->ReadFile(task.path, buf);

                // Simulate streaming in chunks and check cancelFlag for preemption
                if (!ok) {
                    for (int i = 0; i < 20; ++i) {
                        if (cancelFlag->load()) {
                            CORE_LOG_INFO(std::string("Load cancelled during streaming: ") + task.path);
                            // metrics
                            metrics_cancelled_.fetch_add(1);
                            metrics_cancelled_by_prio_[static_cast<int>(task.priority)].fetch_add(1);
                            // cleanup running flags
                            std::lock_guard<std::mutex> rl(runningMutex_);
                            runningCancelFlags_.erase(task.handle);
                            runningPriority_.erase(task.handle);
                            return;
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                    buf.assign(128,0);
                }

                pools_.push_back(std::move(buf));
                if (task.callback) task.callback(task.path);
                CORE_LOG_INFO(std::string("Loaded asset (dispatched): ") + task.path);
                if (task.fence) task.fence->Signal();
                {
                    std::lock_guard<std::mutex> l(loadedMutex_);
                    loadedQueue_.push(task.path);
                }

                // metrics completed
                metrics_completed_.fetch_add(1);
                metrics_completed_by_prio_[static_cast<int>(task.priority)].fetch_add(1);
                auto tend = std::chrono::steady_clock::now();
                auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(tend - tstart).count();
                metrics_total_time_ms_by_prio_[static_cast<int>(task.priority)].fetch_add(static_cast<uint64_t>(ms));

                // cleanup running flags
                std::lock_guard<std::mutex> rl(runningMutex_);
                runningCancelFlags_.erase(task.handle);
                runningPriority_.erase(task.handle);
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
    // dump metrics at shutdown
    DumpMetrics();
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
        // update metrics
        metrics_requested_.fetch_add(1);
        metrics_requested_by_prio_[static_cast<int>(t.priority)].fetch_add(1);

        // If this is a high priority task, request cancellation of lower-priority running tasks
        if (t.priority == Priority::High)
        {
            std::lock_guard<std::mutex> rl(runningMutex_);
            for (auto &kv : runningPriority_)
            {
                if (static_cast<int>(kv.second) < static_cast<int>(t.priority))
                {
                    auto it = runningCancelFlags_.find(kv.first);
                    if (it != runningCancelFlags_.end()) it->second->store(true);
                }
            }
        }

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

bool AssetManager::PopLoaded(std::string& outPath)
{
    std::lock_guard<std::mutex> l(loadedMutex_);
    if (loadedQueue_.empty()) return false;
    outPath = loadedQueue_.front(); loadedQueue_.pop();
    return true;
}

void AssetManager::DumpMetrics() const
{
    std::ostringstream ss;
    ss << "AssetManager Metrics:\n";
    ss << "  requested=" << metrics_requested_.load() << " started=" << metrics_started_.load() << " completed=" << metrics_completed_.load() << " cancelled=" << metrics_cancelled_.load() << "\n";
    for (int i = 0; i < 3; ++i)
    {
        ss << "  prio[" << i << "] requested=" << metrics_requested_by_prio_[i].load() << " completed=" << metrics_completed_by_prio_[i].load() << " cancelled=" << metrics_cancelled_by_prio_[i].load() << " total_ms=" << metrics_total_time_ms_by_prio_[i].load() << "\n";
    }
    CORE_LOG_INFO(ss.str());
}

} // namespace Assets
