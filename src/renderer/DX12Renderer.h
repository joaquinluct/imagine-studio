#pragma once

#include "IRenderer.h"

#include <string>
// Windows HWND needed for swapchain init
#include <windows.h>

#if defined(_WIN32) && defined(_MSC_VER)
#include <d3d12.h>
#endif

namespace Renderer {

// Forward declarations of AAA subsystems (v1.6.0 DEV-002)
class DX12Device;
class DX12SwapChain;
class DX12CommandContext;
class DX12ResourceManager;
class DX12PipelineManager;
class DX12OpaquePass;
class DX12UIPass;
class Camera;
class RenderTarget;
class CommandAllocator;
class Fence;

} // namespace Renderer

// v1.8.0 H3.1 - Scene Graph forward declaration
namespace Scene { class Scene; }

namespace Renderer {

// v1.6.0 DEV-002.6 - DX12Renderer (AAA Architecture - Orchestrator)
// Orchestrates render passes and coordinates subsystems
// Reduced from 1100+ lines to ~300 lines
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
    
    // Forward rendering (v1.2.0)
    void RenderForwardPass();
    
    // Prepare render target for UI composition (legacy stub)
    bool ComposeUI();
    
    // Notify renderer that an asset loaded (path)
    void OnAssetLoaded(const std::string& path);
    
    // UI visibility control (H2.3 - Toggle UI with F1)
    void SetUIVisible(bool visible) { m_uiVisible = visible; }
    bool IsUIVisible() const { return m_uiVisible; }
    void ToggleUI() { m_uiVisible = !m_uiVisible; }
    
    // Camera accessor (v1.5.0 - H2.3)
    Camera* GetCamera() { return m_camera; }
    
    // v1.8.0 H3.1 - Scene accessor
    void SetScene(Scene::Scene* scene) { m_scene = scene; }
    Scene::Scene* GetScene() { return m_scene; }
    
    // ImGui SRV heap accessor (v1.3.0 - H2.2)
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12DescriptorHeap* GetImGuiSrvHeap() const { return m_imguiSrvHeap; }
    ID3D12Device* GetDevice() const;
    
    // v1.6.0 H1.1 - Scene RT SRV accessor (for Viewport panel)
    D3D12_GPU_DESCRIPTOR_HANDLE GetSceneRenderTargetSRV() const {
        return m_sceneSRV_GPU;
    }
#else
    void* GetImGuiSrvHeap() const { return nullptr; }
    void* GetDevice() const { return nullptr; }
#endif

private:
    // === AAA SUBSYSTEMS (v1.6.0 DEV-002) ===
    DX12Device* m_device = nullptr;
    DX12SwapChain* m_swapChain = nullptr;
    DX12CommandContext* m_commandContext = nullptr;
    DX12ResourceManager* m_resourceManager = nullptr;
    DX12PipelineManager* m_pipelineManager = nullptr;
    
    // Render passes
    DX12OpaquePass* m_opaquePass = nullptr;
    DX12UIPass* m_uiPass = nullptr;
    
    // Camera system
    Camera* m_camera = nullptr;
    
    // v1.8.0 H3.1 - Scene Graph integration
    Scene::Scene* m_scene = nullptr;
    
    // Legacy stubs (for non-DX12 platforms)
    RenderTarget* rt_ = nullptr;
    CommandAllocator* allocator_ = nullptr;
    Fence* fence_ = nullptr;
    
#if defined(_WIN32) && defined(_MSC_VER)
    // Back buffer count
    static const unsigned int BACK_BUFFER_COUNT = 2;
    
    // Descriptor heaps
    ID3D12DescriptorHeap* m_rtvHeap = nullptr;
    ID3D12DescriptorHeap* m_imguiSrvHeap = nullptr;
    unsigned int m_rtvDescriptorSize = 0;
    
    // Scene render target (offscreen - AAA standard)
    ID3D12Resource* m_sceneRenderTarget = nullptr;
    D3D12_CPU_DESCRIPTOR_HANDLE m_sceneRTV = {};
    D3D12_GPU_DESCRIPTOR_HANDLE m_sceneSRV_GPU = {};
    D3D12_CPU_DESCRIPTOR_HANDLE m_sceneSRV_CPU = {};
    
    // Render resources (managed by subsystems)
    ID3D12RootSignature* m_rootSignature = nullptr;
    ID3D12PipelineState* m_pipelineState = nullptr;
    ID3D12Resource* m_vertexBuffer = nullptr;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView = {};
    
    // MVP matrix
    float m_mvpMatrix[16] = {};
#endif
    
    // UI visibility state
    bool m_uiVisible = true;
    
    // Helper methods
#if defined(_WIN32) && defined(_MSC_VER)
    void CreateSceneRenderTarget();
    void CalculateMVPMatrix();
#endif
};

} // namespace Renderer
