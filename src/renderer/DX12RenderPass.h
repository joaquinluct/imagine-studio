#pragma once

#if defined(_WIN32) && defined(_MSC_VER)
#include <d3d12.h>
#endif

namespace Renderer {

// Forward declarations
class DX12CommandContext;

// v1.6.0 DEV-002.5 - Render Pass Abstraction (AAA Architecture)
// Base class for all render passes (OpaquePass, UIPass, ShadowPass, etc.)
// Inspired by Unreal Engine FRenderPass and Unity ScriptableRenderPass
class DX12RenderPass {
public:
    DX12RenderPass() = default;
    virtual ~DX12RenderPass() = default;

    // Execute render pass (pure virtual - must be implemented by derived classes)
    virtual void Execute(DX12CommandContext& ctx) = 0;

    // Initialize render pass (optional override)
    virtual bool Initialize() { return true; }

    // Shutdown render pass (optional override)
    virtual void Shutdown() {}

    // Get pass name for debugging (optional override)
    virtual const char* GetPassName() const { return "UnnamedPass"; }

protected:
    // Helper: Set render target
#if defined(_WIN32) && defined(_MSC_VER)
    void SetRenderTarget(
        DX12CommandContext& ctx,
        D3D12_CPU_DESCRIPTOR_HANDLE rtv,
        const float* clearColor = nullptr
    );
#else
    void SetRenderTarget(
        DX12CommandContext& ctx,
        void* rtv,
        const float* clearColor = nullptr
    );
#endif

    // Helper: Set viewport and scissor rect
    void SetViewportAndScissor(
        DX12CommandContext& ctx,
        unsigned int width,
        unsigned int height
    );

    // Helper: Transition resource state
#if defined(_WIN32) && defined(_MSC_VER)
    void TransitionResource(
        DX12CommandContext& ctx,
        ID3D12Resource* resource,
        D3D12_RESOURCE_STATES stateBefore,
        D3D12_RESOURCE_STATES stateAfter
    );
#else
    void TransitionResource(
        DX12CommandContext& ctx,
        void* resource,
        unsigned int stateBefore,
        unsigned int stateAfter
    );
#endif
};

} // namespace Renderer
