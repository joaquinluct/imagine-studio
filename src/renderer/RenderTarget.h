#pragma once

namespace Renderer {

class RenderTarget {
public:
    RenderTarget() {}
    ~RenderTarget() {}
    // Create without HWND uses stub path
    bool Create(int width, int height);
    // Create swapchain and render target for a HWND when native device is present
    bool CreateForWindow(void* nativeDevice, void* hwnd, int width, int height);
    void Destroy();
    void Present();

private:
    void* swapchain_ = nullptr; // opaque IDXGISwapChain*
    void* commandQueue_ = nullptr; // opaque ID3D12CommandQueue*
};

} // namespace Renderer
