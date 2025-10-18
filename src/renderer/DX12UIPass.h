#pragma once

#include "DX12RenderPass.h"

#if defined(_WIN32) && defined(_MSC_VER)
#include <d3d12.h>
#endif

namespace Renderer {

// v1.6.0 DEV-002.5 - UI Pass (AAA Architecture)
// Renders ImGui UI on top of scene render target
// Inspired by Unreal Engine FUIPass and Unity UIPass
class DX12UIPass : public DX12RenderPass {
public:
    DX12UIPass();
    ~DX12UIPass() override;

    // Execute UI pass
    void Execute(DX12CommandContext& ctx) override;

    // Initialize pass
    bool Initialize() override;

    // Shutdown pass
    void Shutdown() override;

    // Get pass name
    const char* GetPassName() const override { return "UIPass"; }

    // Set render target and RTV
    void SetRenderTarget(
#if defined(_WIN32) && defined(_MSC_VER)
        ID3D12Resource* renderTarget,
        D3D12_CPU_DESCRIPTOR_HANDLE rtv
#else
        void* renderTarget,
        void* rtv
#endif
    );

    // Set ImGui SRV descriptor heap
    void SetImGuiSrvHeap(
#if defined(_WIN32) && defined(_MSC_VER)
        ID3D12DescriptorHeap* imguiSrvHeap
#else
        void* imguiSrvHeap
#endif
    );

    // Set UI visible flag
    void SetUIVisible(bool visible) { m_uiVisible = visible; }

private:
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12Resource* m_renderTarget = nullptr;
    D3D12_CPU_DESCRIPTOR_HANDLE m_rtv = {};
    ID3D12DescriptorHeap* m_imguiSrvHeap = nullptr;
#else
    void* m_renderTarget = nullptr;
    void* m_rtv = nullptr;
    void* m_imguiSrvHeap = nullptr;
#endif

    bool m_uiVisible = true; // UI visible by default
};

} // namespace Renderer
