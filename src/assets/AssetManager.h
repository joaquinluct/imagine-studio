#pragma once

#include <string>
#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>
#include <unordered_set>

// Forward declaration for synchronization fence
namespace Renderer { class Fence; }
namespace Assets { class VFS; }

namespace Assets {

class AssetManager {
public:
    using LoadHandle = unsigned long long;
    enum class Priority { Low = 0, Normal = 1, High = 2 };

    AssetManager();
    ~AssetManager();

    void Initialize();
    void Shutdown();

    // Enqueue an asynchronous load. Returns a handle for cancellation.
    LoadHandle LoadAsync(const std::string& path, std::function<void(const std::string&)> callback, Renderer::Fence* signalFence = nullptr, Priority priority = Priority::Normal);

    // Cancel a pending load. Returns true if cancelled before dispatch.
    bool CancelLoad(LoadHandle handle);

    // Mount a path to the VFS
    bool Mount(const std::string& path);

    // Pop next loaded asset path (if any). Returns false if none.
    bool PopLoaded(std::string& outPath);

private:
    struct Task { LoadHandle handle; Priority priority; std::string path; std::function<void(const std::string&)> callback; Renderer::Fence* fence; };

    struct TaskCmp { bool operator()(Task const& a, Task const& b) const { return static_cast<int>(a.priority) < static_cast<int>(b.priority); } };

    VFS* vfs_ = nullptr;
    std::vector<std::vector<char>> pools_; // simple buffer pool

    // Dispatcher
    std::thread dispatcherThread_;
    std::priority_queue<Task, std::vector<Task>, TaskCmp> taskQueue_;
    std::mutex queueMutex_;
    std::condition_variable queueCv_;
    std::atomic<bool> dispatcherRunning_{false};

    std::atomic<LoadHandle> nextHandle_{1};
    std::mutex cancelMutex_;
    std::unordered_set<LoadHandle> cancelled_;
    // running task cancellation flags and priority tracking
    std::mutex runningMutex_;
    std::unordered_map<LoadHandle, std::shared_ptr<std::atomic<bool>>> runningCancelFlags_;
    std::unordered_map<LoadHandle, Priority> runningPriority_;
    // loaded items queue
    std::mutex loadedMutex_;
    std::queue<std::string> loadedQueue_;
};

} // namespace Assets
