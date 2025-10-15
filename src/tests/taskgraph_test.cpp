#include "../jobs/TaskGraph.h"
#include "../jobs/ThreadPool.h"

#include <chrono>
#include <iostream>
#ifdef BUILD_TEST_EXE
int main()
{
    const int N = 1000; // nodes
    Jobs::ThreadPool pool(4);
    Jobs::TaskGraph tg;

    // create N leaf tasks that just increment an atomic
    std::atomic<int> counter{0};
    std::vector<int> ids;
    ids.reserve(N);
    for (int i = 0; i < N; ++i)
    {
        ids.push_back(tg.AddTask([&counter](){ counter.fetch_add(1); }));
    }

    // create a final task that depends on all leaf tasks
    int final = tg.AddTask([&counter](){ std::cout << "Final task running, counter=" << counter.load() << "\n"; });
    for (int id : ids)
        tg.AddDependency(id, final);

    auto t0 = std::chrono::high_resolution_clock::now();
    tg.Execute(&pool);
    auto t1 = std::chrono::high_resolution_clock::now();
    double secs = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();
    std::cout << "TaskGraph test completed in " << secs << "s, counter=" << counter.load() << "\n";

    return 0;
}

#endif // BUILD_TEST_EXE
