#pragma once

#if defined(_WIN32) && defined(_MSC_VER)
#include <d3d12.h>
#include <dxgi1_4.h>
#include <windows.h>
#endif

namespace Renderer {

// v1.6.0 DEV-002.1 - SwapChain Management (AAA Architecture)
// Encapsulates swap chain creation, presentation, and back buffer management
// Inspired by Unreal Engine FDXGISwapChain and Unity SwapChain abstraction
class DX12SwapChain {
public:
    DX12SwapChain();
    ~DX12SwapChain();

    // Create swap chain for window
    bool Create(
#if defined(_WIN32) && defined(_MSC_VER)
        ID3D12CommandQueue* commandQueue,
        IDXGIFactory4* factory,
#else
        void* commandQueue,
        void* factory,
#endif
        HWND hwnd,
        unsigned int width,
        unsigned int height,
        unsigned int bufferCount = 2
    );

    // Present frame with optional VSync
    void Present(bool vsync = true);

    // Get back buffer resource
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12Resource* GetBackBuffer(unsigned int index) const;
#else
    void* GetBackBuffer(unsigned int index) const;
#endif

    // Get current back buffer index
    unsigned int GetCurrentBackBufferIndex() const;

    // Get buffer count
    unsigned int GetBufferCount() const { return m_bufferCount; }

    // Resize swap chain (for window resize)
    bool Resize(unsigned int width, unsigned int height);

    // Shutdown and release resources
    void Shutdown();

private:
#if defined(_WIN32) && defined(_MSC_VER)
    IDXGISwapChain3* m_swapChain = nullptr;
    ID3D12Resource* m_backBuffers[3] = {}; // Max 3 buffers (double/triple buffering)
#else
    void* m_swapChain = nullptr;
    void* m_backBuffers[3] = {};
#endif

    unsigned int m_bufferCount = 2;
    unsigned int m_width = 0;
    unsigned int m_height = 0;
};

} // namespace Renderer
