#pragma once

#include <atomic>

namespace Renderer {

class Fence {
public:
    Fence();
    ~Fence();

    // Signal the fence (increment value)
    void Signal();

    // Wait until fence reaches at least the signaled value (busy-wait stub)
    void Wait();
    unsigned long long GetValue() const { return value_.load(); }
    void WaitForValue(unsigned long long v);

    unsigned long long Value() const { return value_.load(); }

private:
    std::atomic<unsigned long long> value_;
};

} // namespace Renderer
