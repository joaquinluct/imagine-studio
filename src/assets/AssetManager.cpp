#include "AssetManager.h"
#include <thread>
#include <chrono>
#include "../jobs/ThreadPool.h"
#include "../core/Log.h"

namespace Assets {

static Jobs::ThreadPool* s_threadPool = nullptr;

AssetManager::AssetManager(){}
AssetManager::~AssetManager(){}

void AssetManager::Initialize()
{
    if (!s_threadPool)
        s_threadPool = new Jobs::ThreadPool(2);
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
    CORE_LOG_INFO("AssetManager shutdown");
}

void AssetManager::LoadAsync(const std::string& path, std::function<void(const std::string&)> callback)
{
    if (!s_threadPool)
    {
        CORE_LOG_ERROR("ThreadPool not initialized for AssetManager");
        return;
    }

    s_threadPool->Enqueue([path, callback]() {
        CORE_LOG_INFO(std::string("Loading asset: ") + path);
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // simulate load
        if (callback)
            callback(path);
        CORE_LOG_INFO(std::string("Loaded asset: ") + path);
    });
}

} // namespace Assets
