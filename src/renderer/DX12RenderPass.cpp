#include "DX12RenderPass.h"
#include "DX12CommandContext.h"
#include "core/Log.h"

namespace Renderer {

#if defined(_WIN32) && defined(_MSC_VER)
void DX12RenderPass::SetRenderTarget(
    DX12CommandContext& ctx,
    D3D12_CPU_DESCRIPTOR_HANDLE rtv,
    const float* clearColor
)
{
    ID3D12GraphicsCommandList* commandList = ctx.GetCommandList();
    if (!commandList)
    {
        CORE_LOG_ERROR("DX12RenderPass::SetRenderTarget: Invalid command list");
        return;
    }

    // Set render target
    commandList->OMSetRenderTargets(1, &rtv, FALSE, nullptr);

    // Clear render target if color provided
    if (clearColor)
    {
        commandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
    }
}
#else
void DX12RenderPass::SetRenderTarget(
    DX12CommandContext& ctx,
    void* rtv,
    const float* clearColor
)
{
    // Stub for non-Windows platforms
}
#endif

void DX12RenderPass::SetViewportAndScissor(
    DX12CommandContext& ctx,
    unsigned int width,
    unsigned int height
)
{
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12GraphicsCommandList* commandList = ctx.GetCommandList();
    if (!commandList)
    {
        CORE_LOG_ERROR("DX12RenderPass::SetViewportAndScissor: Invalid command list");
        return;
    }

    // Set viewport
    D3D12_VIEWPORT viewport = {};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    commandList->RSSetViewports(1, &viewport);

    // Set scissor rect
    D3D12_RECT scissorRect = {};
    scissorRect.left = 0;
    scissorRect.top = 0;
    scissorRect.right = static_cast<LONG>(width);
    scissorRect.bottom = static_cast<LONG>(height);
    commandList->RSSetScissorRects(1, &scissorRect);
#endif
}

#if defined(_WIN32) && defined(_MSC_VER)
void DX12RenderPass::TransitionResource(
    DX12CommandContext& ctx,
    ID3D12Resource* resource,
    D3D12_RESOURCE_STATES stateBefore,
    D3D12_RESOURCE_STATES stateAfter
)
{
    ID3D12GraphicsCommandList* commandList = ctx.GetCommandList();
    if (!commandList || !resource)
    {
        CORE_LOG_ERROR("DX12RenderPass::TransitionResource: Invalid command list or resource");
        return;
    }

    // Skip transition if states are the same (optimization)
    if (stateBefore == stateAfter)
    {
        return;
    }

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = resource;
    barrier.Transition.StateBefore = stateBefore;
    barrier.Transition.StateAfter = stateAfter;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    commandList->ResourceBarrier(1, &barrier);
}
#else
void DX12RenderPass::TransitionResource(
    DX12CommandContext& ctx,
    void* resource,
    unsigned int stateBefore,
    unsigned int stateAfter
)
{
    // Stub for non-Windows platforms
}
#endif

} // namespace Renderer
