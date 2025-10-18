#pragma once

#include "DX12RenderPass.h"

#if defined(_WIN32) && defined(_MSC_VER)
#include <d3d12.h>
#endif

namespace Renderer {

// v1.6.0 DEV-002.5 - Opaque Pass (AAA Architecture)
// Renders opaque geometry to scene render target
// Inspired by Unreal Engine FOpaqueBasePass and Unity OpaquePass
class DX12OpaquePass : public DX12RenderPass {
public:
    DX12OpaquePass();
    ~DX12OpaquePass() override;

    // Execute opaque pass
    void Execute(DX12CommandContext& ctx) override;

    // Initialize pass
    bool Initialize() override;

    // Shutdown pass
    void Shutdown() override;

    // Get pass name
    const char* GetPassName() const override { return "OpaquePass"; }

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

    // Set pipeline state and root signature
    void SetPipeline(
#if defined(_WIN32) && defined(_MSC_VER)
        ID3D12PipelineState* pipelineState,
        ID3D12RootSignature* rootSignature
#else
        void* pipelineState,
        void* rootSignature
#endif
    );

    // Set vertex buffer and view
    void SetVertexBuffer(
#if defined(_WIN32) && defined(_MSC_VER)
        const D3D12_VERTEX_BUFFER_VIEW& vertexBufferView
#else
        const void* vertexBufferView
#endif
    );

    // Set MVP matrix (root constants)
    void SetMVPMatrix(const float* mvpMatrix);

    // Set camera (for MVP calculation)
    void SetCamera(class Camera* camera);

private:
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12Resource* m_renderTarget = nullptr;
    D3D12_CPU_DESCRIPTOR_HANDLE m_rtv = {};
    ID3D12PipelineState* m_pipelineState = nullptr;
    ID3D12RootSignature* m_rootSignature = nullptr;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView = {};
#else
    void* m_renderTarget = nullptr;
    void* m_rtv = nullptr;
    void* m_pipelineState = nullptr;
    void* m_rootSignature = nullptr;
    void* m_vertexBufferView = nullptr;
#endif

    float m_mvpMatrix[16] = {}; // MVP matrix for rendering
    class Camera* m_camera = nullptr;
};

} // namespace Renderer
