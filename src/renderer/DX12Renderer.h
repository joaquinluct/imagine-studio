#pragma once

#include "CommandAllocator.h"
#include "Fence.h"
#include "IRenderer.h"

#include <string>
// Windows HWND needed for swapchain init
#include <windows.h>

#if defined(_WIN32) && defined(_MSC_VER)
#include <d3dcommon.h> // For ID3DBlob

struct IDXGISwapChain3;
struct ID3D12Resource;
struct ID3D12DescriptorHeap;
struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
struct ID3D12Fence;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
#endif

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
    // Notify renderer that an asset loaded (path)
    void OnAssetLoaded(const std::string& path);

private:
    // stubs for device and render target ownership
    class DX12Device* device_ = nullptr;
    class RenderTarget* rt_ = nullptr;
    class CommandAllocator* allocator_ = nullptr;
    class Fence* fence_ = nullptr;
    
#if defined(_WIN32) && defined(_MSC_VER)
    // SwapChain and render targets
    static const UINT BACK_BUFFER_COUNT = 2;
    IDXGISwapChain3* m_swapChain = nullptr;
    ID3D12Resource* m_renderTargets[BACK_BUFFER_COUNT] = {};
    ID3D12DescriptorHeap* m_rtvHeap = nullptr;
    UINT m_rtvDescriptorSize = 0;
    UINT m_frameIndex = 0;
    
    // CBV/SRV/UAV descriptor heap (for constant buffers and textures)
    ID3D12DescriptorHeap* m_cbvSrvUavHeap = nullptr;
    UINT m_cbvSrvUavDescriptorSize = 0;
    
    // Command recording and synchronization
    ID3D12CommandAllocator* m_commandAllocator = nullptr;
    ID3D12GraphicsCommandList* m_commandList = nullptr;
    ID3D12Fence* m_fence = nullptr;
    UINT64 m_fenceValue = 0;
    HANDLE m_fenceEvent = nullptr;
    
    // Root signature (defines shader parameter layout)
    ID3D12RootSignature* m_rootSignature = nullptr;
    
    // Compiled shaders (vertex and pixel shader bytecode)
    ID3DBlob* m_vertexShaderBlob = nullptr;
    ID3DBlob* m_pixelShaderBlob = nullptr;
    
    // Pipeline State Object (defines complete graphics pipeline state)
    ID3D12PipelineState* m_pipelineState = nullptr;
#endif
};

} // namespace Renderer
