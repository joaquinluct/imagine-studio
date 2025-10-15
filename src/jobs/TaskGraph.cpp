#include "TaskGraph.h"
#include "ThreadPool.h"
#include <queue>
#include <iostream>

namespace Jobs {

TaskGraph::TaskGraph() {}
TaskGraph::~TaskGraph() {}

int TaskGraph::AddTask(TaskFunc fn)
{
    std::lock_guard<std::mutex> lock(mutex_);
    nodes_.push_back({fn, {}, 0});
    return (int)nodes_.size() - 1;
}

void TaskGraph::AddDependency(int parent, int child)
{
    std::lock_guard<std::mutex> lock(mutex_);
    nodes_[parent].children.push_back(child);
    nodes_[child].deps.fetch_add(1);
}

void TaskGraph::Execute(ThreadPool* pool)
{
    if (!pool) return;

    remaining_.store((int)nodes_.size());

    // Submit tasks that have no dependencies
    for (int i = 0; i < (int)nodes_.size(); ++i)
    {
        if (nodes_[i].deps.load() == 0)
        {
            // capture index by value
            int id = i;
            pool->Enqueue([this, id, pool]() {
                // Execute the task
                nodes_[id].fn();

                // For each child, decrement deps and if it reaches zero, submit the child
                for (int c : nodes_[id].children)
                {
                    int prev = nodes_[c].deps.fetch_sub(1);
                    if (prev == 1)
                    {
                        int childId = c;
                        pool->Enqueue([this, childId, pool]() {
                            nodes_[childId].fn();
                            // recursively handle grandchildren by posting their children
                            for (int gc : nodes_[childId].children)
                            {
                                int prev2 = nodes_[gc].deps.fetch_sub(1);
                                if (prev2 == 1)
                                {
                                    int gid = gc;
                                    pool->Enqueue([this, gid, pool]() {
                                        // execute and continue chain
                                        nodes_[gid].fn();
                                        for (int ggc : nodes_[gid].children)
                                        {
                                            int prev3 = nodes_[ggc].deps.fetch_sub(1);
                                            if (prev3 == 1)
                                            {
                                                int ggId = ggc;
                                                pool->Enqueue([this, ggId, pool]() {
                                                    nodes_[ggId].fn();
                                                    remaining_.fetch_sub(1);
                                                });
                                            }
                                        }
                                        remaining_.fetch_sub(1);
                                    });
                                }
                            }
                            remaining_.fetch_sub(1);
                        });
                    }
                }

                remaining_.fetch_sub(1);
            });
        }
    }

    // Wait until all tasks have completed
    std::mutex m;
    std::unique_lock<std::mutex> lk(m);
    while (remaining_.load() > 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

} // namespace Jobs
