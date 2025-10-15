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
    CORE_LOG_INFO("AssetManager shutdown");
}

void AssetManager::LoadAsync(const std::string& path, std::function<void(const std::string&)> callback, class Renderer::Fence* signalFence)
{
    if (!s_threadPool)
    {
        CORE_LOG_ERROR("ThreadPool not initialized for AssetManager");
        return;
    }

    s_threadPool->Enqueue([this, path, callback, signalFence]() {
        CORE_LOG_INFO(std::string("Loading asset: ") + path);
        // Try VFS read (non-blocking simulation)
        std::vector<char> buf;
        bool ok = false;
        if (vfs_) ok = vfs_->ReadFile(path, buf);

        if (!ok)
        {
            // simulate load from disk
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            buf.assign(128, 0);
        }

        // store into pool
        pools_.push_back(std::move(buf));

        if (callback) callback(path);
        CORE_LOG_INFO(std::string("Loaded asset: ") + path);
        if (signalFence) signalFence->Signal();
    });
}

bool AssetManager::Mount(const std::string& path)
{
    if (!vfs_) return false;
    return vfs_->Mount(path);
}

} // namespace Assets
