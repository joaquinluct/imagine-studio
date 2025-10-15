#pragma once

#include <thread>
#include <vector>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace Jobs {

class ThreadPool {
public:
    ThreadPool(size_t threadCount = std::thread::hardware_concurrency()) {}
    ~ThreadPool() {}

    void Enqueue(std::function<void()> task) {}
};

} // namespace Jobs
