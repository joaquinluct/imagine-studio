#pragma once

#include <windows.h>

// Forward declarations
#if defined(_WIN32) && defined(_MSC_VER)
struct D3D12_GPU_DESCRIPTOR_HANDLE;
#endif

namespace Editor {

// v1.5.0 H3.1 - Viewport panel for 3D scene rendering
// Shows the render target texture using ImGui::Image()
class Viewport {
public:
    Viewport();
    ~Viewport() = default;

    // Render the viewport panel (call from EditorUI::RenderAllPanels)
    void Render();
    
    // Set the GPU descriptor handle for the render target SRV
    void SetRenderTargetSRV(void* gpuHandle); // D3D12_GPU_DESCRIPTOR_HANDLE as void*
    
    // Check if viewport is currently hovered (for camera controls)
    bool IsHovered() const { return m_isHovered; }
    
    // Get viewport size (for aspect ratio calculation)
    void GetSize(float& width, float& height) const;

private:
    void* m_renderTargetSRV = nullptr; // D3D12_GPU_DESCRIPTOR_HANDLE stored as void*
    bool m_isHovered = false;
    float m_width = 1920.0f;  // Default viewport size
    float m_height = 1080.0f;
};

} // namespace Editor
