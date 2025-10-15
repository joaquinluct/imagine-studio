#include "DX12Renderer.h"
#include <iostream>
#include "DX12Device.h"
#include "RenderTarget.h"

namespace Renderer {

DX12Renderer::DX12Renderer() : device_(nullptr), rt_(nullptr) {}
DX12Renderer::~DX12Renderer() { Shutdown(); }

void DX12Renderer::Initialize()
{
    std::cout << "DX12Renderer: Initialize (stub)\n";
    device_ = new DX12Device();
    device_->Initialize();
    rt_ = new RenderTarget();
    rt_->Create(800, 600);
}

// Shutdown implementation moved below (cleanup of device and render target)

void DX12Renderer::RenderFrame()
{
    // Stub: call ComposeUI for composition and present the render target
    ComposeUI();
    if (rt_) rt_->Present();
}

bool DX12Renderer::ComposeUI()
{
    // Stub: prepare composition target for UI
    std::cout << "DX12Renderer: ComposeUI (stub)\n";
    return true;
}

void DX12Renderer::Shutdown()
{
    if (rt_) { rt_->Destroy(); delete rt_; rt_ = nullptr; }
    if (device_) { device_->Shutdown(); delete device_; device_ = nullptr; }
}

} // namespace Renderer
