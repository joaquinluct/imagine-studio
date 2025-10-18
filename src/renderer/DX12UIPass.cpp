#include "DX12UIPass.h"
#include "DX12CommandContext.h"
#include "core/Log.h"

// ImGui headers (Dear ImGui - Editor UI Framework)
#include "imgui.h"
#include "imgui_impl_dx12.h"

namespace Renderer {

DX12UIPass::DX12UIPass()
{
}

DX12UIPass::~DX12UIPass()
{
    Shutdown();
}

bool DX12UIPass::Initialize()
{
    CORE_LOG_INFO("DX12UIPass initialized");
    return true;
}

void DX12UIPass::Shutdown()
{
    m_renderTarget = nullptr;
    m_imguiSrvHeap = nullptr;

#if defined(_WIN32) && defined(_MSC_VER)
    m_rtv = {};
#endif

    CORE_LOG_INFO("DX12UIPass shutdown");
}

void DX12UIPass::Execute(DX12CommandContext& ctx)
{
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12GraphicsCommandList* commandList = ctx.GetCommandList();
    if (!commandList)
    {
        CORE_LOG_ERROR("DX12UIPass::Execute: Invalid command list");
        return;
    }

    // Set render target
    commandList->OMSetRenderTargets(1, &m_rtv, FALSE, nullptr);
    
    // If UI is hidden, we need to show the scene RT in fullscreen
    // (copy scene RT to back buffer instead of just clearing)
    if (!m_uiVisible)
    {
        // TODO DEV-003: Implement fullscreen scene RT blit
        // For now, clear with dark gray (editor background)
        const float clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
        commandList->ClearRenderTargetView(m_rtv, clearColor, 0, nullptr);
        return;
    }
    
    // UI is visible - clear back buffer with editor background
    const float clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    commandList->ClearRenderTargetView(m_rtv, clearColor, 0, nullptr);
    
    // Get ImGui draw data
    ImDrawData* draw_data = ImGui::GetDrawData();
    
    // If we have draw data, render ImGui
    if (draw_data && draw_data->TotalVtxCount > 0)
    {
        // Set ImGui SRV descriptor heap (CRITICAL for ImGui to access textures)
        if (m_imguiSrvHeap)
        {
            commandList->SetDescriptorHeaps(1, &m_imguiSrvHeap);
        }
        else
        {
            CORE_LOG_ERROR("DX12UIPass::Execute: ImGui SRV heap not set");
            return;
        }

        // Call ImGui DX12 backend to render draw data
        // The backend handles pipeline, vertex buffers, etc.
        ImGui_ImplDX12_RenderDrawData(draw_data, commandList);
    }
#endif
}

void DX12UIPass::SetRenderTarget(
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

void DX12UIPass::SetImGuiSrvHeap(
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12DescriptorHeap* imguiSrvHeap
#else
    void* imguiSrvHeap
#endif
)
{
    m_imguiSrvHeap = imguiSrvHeap;
}

} // namespace Renderer
