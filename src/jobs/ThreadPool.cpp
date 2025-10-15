#include "ThreadPool.h"
namespace Jobs {

ThreadPool::ThreadPool(size_t threadCount)
{
    if (threadCount == 0)
        threadCount = std::max<size_t>(1, std::thread::hardware_concurrency());

    running_.store(true);
    for (size_t i = 0; i < threadCount; ++i)
        workers_.emplace_back(&ThreadPool::WorkerLoop, this);
}

ThreadPool::~ThreadPool()
{
    Shutdown();
}

void ThreadPool::Enqueue(std::function<void()> task)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_.push(std::move(task));
    }
    cv_.notify_one();
}

void ThreadPool::Shutdown()
{
    running_.store(false);
    cv_.notify_all();
    for (auto& t : workers_)
    {
        if (t.joinable())
            t.join();
    }
    workers_.clear();
}

// Suppress potential static analysis warning about mutex unlock in this loop (false positive)
#pragma warning(push)
#pragma warning(disable:26115)
void ThreadPool::WorkerLoop()
{
    while (running_.load())
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [this] { return !tasks_.empty() || !running_.load(); });
            if (!running_.load() && tasks_.empty())
            {
                // no more work and shutting down
                break;
            }

#pragma warning(pop)
            task = std::move(tasks_.front());
            tasks_.pop();
        }

        try {
            task();
        } catch (...) {
            // swallow exceptions in worker
        }
    }
}

} // namespace Jobs
