#include "DX12OpaquePass.h"
#include "DX12CommandContext.h"
#include "Camera.h"
#include "core/Log.h"

namespace Renderer {

DX12OpaquePass::DX12OpaquePass()
{
    // Initialize MVP matrix to identity
    m_mvpMatrix[0] = 1.0f;  m_mvpMatrix[1] = 0.0f;  m_mvpMatrix[2] = 0.0f;  m_mvpMatrix[3] = 0.0f;
    m_mvpMatrix[4] = 0.0f;  m_mvpMatrix[5] = 1.0f;  m_mvpMatrix[6] = 0.0f;  m_mvpMatrix[7] = 0.0f;
    m_mvpMatrix[8] = 0.0f;  m_mvpMatrix[9] = 0.0f;  m_mvpMatrix[10] = 1.0f; m_mvpMatrix[11] = 0.0f;
    m_mvpMatrix[12] = 0.0f; m_mvpMatrix[13] = 0.0f; m_mvpMatrix[14] = 0.0f; m_mvpMatrix[15] = 1.0f;
}

DX12OpaquePass::~DX12OpaquePass()
{
    Shutdown();
}

bool DX12OpaquePass::Initialize()
{
    CORE_LOG_INFO("DX12OpaquePass initialized");
    return true;
}

void DX12OpaquePass::Shutdown()
{
    m_renderTarget = nullptr;
    m_pipelineState = nullptr;
    m_rootSignature = nullptr;
    m_camera = nullptr;

#if defined(_WIN32) && defined(_MSC_VER)
    m_rtv = {};
    m_vertexBufferView = {};
#endif

    CORE_LOG_INFO("DX12OpaquePass shutdown");
}

void DX12OpaquePass::Execute(DX12CommandContext& ctx)
{
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12GraphicsCommandList* commandList = ctx.GetCommandList();
    if (!commandList)
    {
        CORE_LOG_ERROR("DX12OpaquePass::Execute: Invalid command list");
        return;
    }

    // Set pipeline state and root signature
    if (m_pipelineState && m_rootSignature)
    {
        commandList->SetPipelineState(m_pipelineState);
        commandList->SetGraphicsRootSignature(m_rootSignature);
    }
    else
    {
        CORE_LOG_ERROR("DX12OpaquePass::Execute: Pipeline state or root signature not set");
        return;
    }

    // Set viewport and scissor rect (1920x1080 Full HD)
    SetViewportAndScissor(ctx, 1920, 1080);

    // Set render target with dark blue clear color
    const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    DX12RenderPass::SetRenderTarget(ctx, m_rtv, clearColor);

    // Set primitive topology
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set vertex buffer
    if (m_vertexBufferView.BufferLocation == 0)
    {
        CORE_LOG_ERROR("DX12OpaquePass::Execute: Vertex buffer view not set");
        return;
    }
    
    commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);

    // Set root constants (MVP matrix)
    commandList->SetGraphicsRoot32BitConstants(0, 16, m_mvpMatrix, 0);

    // DRAW CALL - 6 vertices (2 triangles forming a quad)
    commandList->DrawInstanced(6, 1, 0, 0);
    
    CORE_LOG_INFO("DX12OpaquePass: Draw call executed (6 vertices)");
#endif
}

void DX12OpaquePass::SetRenderTarget(
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12Resource* renderTarget,
    D3D12_CPU_DESCRIPTOR_HANDLE rtv
#else
    void* renderTarget,
    void* rtv
#endif
)
{
    m_renderTarget = renderTarget;
#if defined(_WIN32) && defined(_MSC_VER)
    m_rtv = rtv;
#else
    m_rtv = rtv;
#endif
}

void DX12OpaquePass::SetPipeline(
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12PipelineState* pipelineState,
    ID3D12RootSignature* rootSignature
#else
    void* pipelineState,
    void* rootSignature
#endif
)
{
    m_pipelineState = pipelineState;
    m_rootSignature = rootSignature;
}

void DX12OpaquePass::SetVertexBuffer(
#if defined(_WIN32) && defined(_MSC_VER)
    const D3D12_VERTEX_BUFFER_VIEW& vertexBufferView
#else
    const void* vertexBufferView
#endif
)
{
#if defined(_WIN32) && defined(_MSC_VER)
    m_vertexBufferView = vertexBufferView;
#endif
}

void DX12OpaquePass::SetMVPMatrix(const float* mvpMatrix)
{
    if (mvpMatrix)
    {
        for (int i = 0; i < 16; ++i)
        {
            m_mvpMatrix[i] = mvpMatrix[i];
        }
    }
}

void DX12OpaquePass::SetCamera(Camera* camera)
{
    m_camera = camera;
}

} // namespace Renderer
