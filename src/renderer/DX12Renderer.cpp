#include "DX12Renderer.h"
#include <iostream>

namespace Renderer {

DX12Renderer::DX12Renderer() {}
DX12Renderer::~DX12Renderer() {}

void DX12Renderer::Initialize()
{
    std::cout << "DX12Renderer: Initialize (stub)\n";
}

void DX12Renderer::Shutdown()
{
    std::cout << "DX12Renderer: Shutdown (stub)\n";
}

void DX12Renderer::RenderFrame()
{
    // Stub: no real rendering yet
}

} // namespace Renderer
