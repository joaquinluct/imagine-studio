#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
namespace Jobs {

class ThreadPool {
public:
    explicit ThreadPool(size_t threadCount = 0);
    ~ThreadPool();

    // Enqueue a task to be executed by the pool
    void Enqueue(std::function<void()> task);

    // Stop accepting tasks and join all threads
    void Shutdown();

private:
    void WorkerLoop();

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> running_{false};
};

} // namespace Jobs
