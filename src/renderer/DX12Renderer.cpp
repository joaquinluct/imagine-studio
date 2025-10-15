#include "CommandAllocator.h"
#include "CommandBuffer.h"
#include "DX12Device.h"
#include "DX12Renderer.h"
#include "Fence.h"
#include "RenderTarget.h"

#include <iostream>
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
    allocator_ = new CommandAllocator();
    fence_ = new Fence();
}

// Shutdown implementation moved below (cleanup of device and render target)

void DX12Renderer::RenderFrame()
{
    // Stub: call ComposeUI for composition and present the render target
    // Simulate recording commands
    Renderer::CommandBuffer cmd;
    cmd.Initialize(1024);
    // ... allocate and record
    unsigned long long submitIndex = cmd.Submit();

    // signal fence and wait (stub)
    if (fence_) fence_->Signal();
    if (fence_) fence_->Wait();

    ComposeUI();
    if (rt_) rt_->Present();
}

bool DX12Renderer::ComposeUI()
{
    // Stub: prepare composition target for UI
    std::cout << "DX12Renderer: ComposeUI (stub)\n";
    return true;
}

void DX12Renderer::OnAssetLoaded(const std::string& path)
{
    std::cout << "DX12Renderer: Asset loaded: " << path << "\n";
}

void DX12Renderer::Shutdown()
{
    if (rt_) { rt_->Destroy(); delete rt_; rt_ = nullptr; }
    if (device_) { device_->Shutdown(); delete device_; device_ = nullptr; }
}

} // namespace Renderer
