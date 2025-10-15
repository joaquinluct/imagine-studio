#include "Profiler.h"

namespace Tools {

Profiler& Profiler::Instance()
{
    static Profiler p;
    return p;
}

Profiler::Profiler() {}
Profiler::~Profiler() {}

void Profiler::RecordFrame(double ms)
{
    std::lock_guard<std::mutex> lk(mutex_);
    frameTimeAccum_ += ms;
    ++frameCount_;
    if (frameTimeAccum_ >= 1000.0)
    {
        lastFPS_ = (frameCount_ * 1000.0) / frameTimeAccum_;
        frameTimeAccum_ = 0.0;
        frameCount_ = 0;
    }
}

double Profiler::GetFPS() const
{
    std::lock_guard<std::mutex> lk(mutex_);
    return lastFPS_;
}

} // namespace Tools
