#include "Fence.h"
#include <thread>

namespace Renderer {

Fence::Fence()
{
    value_.store(0);
}

Fence::~Fence() {}

void Fence::Signal()
{
    value_.fetch_add(1);
}

void Fence::Wait()
{
    unsigned long long target = value_.load();
    while (value_.load() < target)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

} // namespace Renderer
