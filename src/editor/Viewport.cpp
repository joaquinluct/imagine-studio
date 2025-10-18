#include "Viewport.h"
#include "core/Log.h"

// ImGui for viewport panel rendering
#include "imgui.h"

namespace Editor {

// v1.5.0 H3.1 - Viewport implementation

Viewport::Viewport()
{
    CORE_LOG_INFO("Viewport created (default size: 1920x1080)");
}

void Viewport::Render()
{
    // Create viewport window (dockable panel)
    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    // Get available content region size
    ImVec2 availableSize = ImGui::GetContentRegionAvail();
    m_width = availableSize.x;
    m_height = availableSize.y;
    
    // Ensure minimum size to avoid division by zero
    if (m_width < 1.0f) m_width = 1.0f;
    if (m_height < 1.0f) m_height = 1.0f;
    
    // Render the 3D scene texture using ImGui::Image
    if (m_renderTargetSRV)
    {
#if defined(_WIN32) && defined(_MSC_VER)
        // Cast void* to ImTextureID (GPU descriptor handle ptr stored as uintptr_t in ImGui)
        ImTextureID texID = reinterpret_cast<ImTextureID>(m_renderTargetSRV);
        
        // Display the render target texture
        // ImGui::Image expects: texture ID, size, uv0, uv1, tint color, border color
        ImGui::Image(
            texID,
            ImVec2(m_width, m_height),  // Size (fill content region)
            ImVec2(0, 0),                // UV top-left
            ImVec2(1, 1),                // UV bottom-right
            ImVec4(1, 1, 1, 1),         // Tint color (white = no tint)
            ImVec4(0, 0, 0, 0)          // Border color (transparent)
        );
        
        // Check if viewport is hovered (for camera controls)
        m_isHovered = ImGui::IsItemHovered();
#else
        // Fallback for non-Windows platforms (show placeholder)
        ImGui::Text("Viewport (3D rendering not available on this platform)");
        m_isHovered = false;
#endif
    }
    else
    {
        // Show placeholder if no render target is set
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Viewport: No render target available");
        ImGui::Text("Size: %.0fx%.0f", m_width, m_height);
        m_isHovered = false;
    }
    
    ImGui::End();
}

void Viewport::SetRenderTargetSRV(void* gpuHandle)
{
    m_renderTargetSRV = gpuHandle;
}

void Viewport::GetSize(float& width, float& height) const
{
    width = m_width;
    height = m_height;
}

} // namespace Editor
