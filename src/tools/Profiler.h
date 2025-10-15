#pragma once

#include <atomic>
#include <mutex>

namespace Tools {

class Profiler {
public:
    static Profiler& Instance();

    // Record frame time in milliseconds
    void RecordFrame(double ms);

    // Get averaged FPS
    double GetFPS() const;

private:
    Profiler();
    ~Profiler();

    mutable std::mutex mutex_;
    double frameTimeAccum_ = 0.0;
    int frameCount_ = 0;
    double lastFPS_ = 0.0;
};

} // namespace Tools
