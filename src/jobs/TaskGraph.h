#pragma once

#include <functional>
#include <vector>
#include <mutex>
#include <atomic>
#include <condition_variable>

namespace Jobs {

class ThreadPool;

// Simple TaskGraph for expressing dependencies between tasks and executing them on a ThreadPool.
class TaskGraph {
public:
    using TaskFunc = std::function<void()>;

    TaskGraph();
    ~TaskGraph();

    // Add a task and return its id
    int AddTask(TaskFunc fn);

    // Add a dependency: task `child` depends on `parent` (parent must run before child)
    void AddDependency(int parent, int child);

    // Execute the graph on the provided ThreadPool. This call blocks until all tasks complete.
    void Execute(ThreadPool* pool);

private:
    struct Node {
        TaskFunc fn;
        std::vector<int> children;
        std::atomic<int> deps{0};
    };

    std::vector<Node> nodes_;
    std::mutex mutex_;
    std::atomic<int> remaining_{0};
    std::condition_variable cv_;
};

} // namespace Jobs
