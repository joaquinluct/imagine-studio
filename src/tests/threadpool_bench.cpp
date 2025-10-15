#include <iostream>
#include <vector>
#include <chrono>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <filesystem>
#include "../jobs/ThreadPool.h"

int main()
{
    const size_t TASKS = 100000; // number of tasks per run
    std::vector<int> threadCounts = {1,2,4,8};

    struct Result { int threads; double seconds; size_t tasks; };
    std::vector<Result> results;

    for (int tc : threadCounts)
    {
        Jobs::ThreadPool pool((size_t)tc);
        std::atomic<size_t> remaining(TASKS);
        std::mutex m;
        std::condition_variable cv;

        auto task = [&remaining, &m, &cv]() {
            // tiny work
            int x = 0; for (int i=0;i<10;++i) x += i;
            if (remaining.fetch_sub(1) == 1)
            {
                std::lock_guard<std::mutex> lk(m);
                cv.notify_one();
            }
        };

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i=0;i<TASKS;++i) pool.Enqueue(task);

        // wait
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&remaining](){ return remaining.load() == 0; });
        auto end = std::chrono::high_resolution_clock::now();
        double secs = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
        results.push_back({tc, secs, TASKS});
        std::cout << "Threads=" << tc << " tasks=" << TASKS << " time=" << secs << "s\n";
        // pool destructor will join
    }

    // ensure output directory exists and write JSON results
    std::filesystem::create_directories("build/bench");
    std::ofstream ofs("build/bench/threadpool_bench_results.json");
    if (ofs)
    {
        ofs << "[\n";
        for (size_t i=0;i<results.size();++i)
        {
            ofs << "  {\"threads\": " << results[i].threads << ", \"tasks\": " << results[i].tasks << ", \"seconds\": " << results[i].seconds << " }";
            if (i+1<results.size()) ofs << ",\n";
            else ofs << "\n";
        }
        ofs << "]\n";
    }

    return 0;
}
