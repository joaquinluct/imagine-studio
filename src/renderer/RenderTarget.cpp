#include "RenderTarget.h"

#include <iostream>

namespace Renderer {

// Simple stub implementations

bool RenderTarget::Create(int width, int height)
{
    (void)width; (void)height;
    std::cout << "RenderTarget: Create (stub)\n";
    return true;
}

void RenderTarget::Destroy()
{
    std::cout << "RenderTarget: Destroy (stub)\n";
}

void RenderTarget::Present()
{
    std::cout << "RenderTarget: Present (stub)\n";
}

} // namespace Renderer
