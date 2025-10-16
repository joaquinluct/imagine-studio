#pragma once

#include "CommandAllocator.h"
#include "Fence.h"
#include "IRenderer.h"

#include <string>
// Windows HWND needed for swapchain init
#include <windows.h>
namespace Renderer {

class DX12Renderer : public IRenderer {
public:
    DX12Renderer();
    ~DX12Renderer() override;

    // Initialize without window is deprecated for DX12; prefer Initialize(HWND)
    void Initialize() override;
    // Initialize with HWND for swapchain creation
    void Initialize(HWND hwnd);
    void Shutdown() override;
    void RenderFrame() override;
    // Prepare render target for UI composition
    bool ComposeUI();
    // Notify renderer that an asset was loaded (path)
    void OnAssetLoaded(const std::string& path);

private:
    // stubs for device and render target ownership
    class DX12Device* device_ = nullptr;
    class RenderTarget* rt_ = nullptr;
    class CommandAllocator* allocator_ = nullptr;
    class Fence* fence_ = nullptr;
};

} // namespace Renderer
