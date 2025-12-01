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

// v2.1.0 H1.6 - Assets includes (need full definition for MaterialTextures member variable)
namespace Assets { class TextureManager; }  // Forward declare TextureManager (pointer only)
#include "../assets/TextureManager.h"  // Include for MaterialTextures struct (full definition needed)

namespace Renderer {

// v1.6.0 DEV-002 - DirectX 12 Renderer (AAA Architecture)
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

    // === PUBLIC ACCESSORS (v2.1.0 H1.3) ===
    
    // Get material SRV heap for binding during rendering
    ID3D12DescriptorHeap* GetMaterialSrvHeap() const { return m_materialSrvHeap; }
    
    // Allocate next available SRV slot in material heap (returns GPU handle)
    // Returns: GPU descriptor handle for the allocated SRV slot
    // Returns: .ptr = 0 if heap is full (80 slots exhausted)
    D3D12_GPU_DESCRIPTOR_HANDLE AllocateMaterialSrv();
    
    // Get CPU descriptor handle for writing SRV at specific index
    D3D12_CPU_DESCRIPTOR_HANDLE GetMaterialSrvCpuHandle(unsigned int index) const;
    
    // Reset material SRV allocator (for scene unload/reload)
    void ResetMaterialSrvAllocator() { m_nextMaterialSrvIndex = 0; }

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
    
    // === H2.5 - PBR Pipeline State Object ===
    ID3D12RootSignature* m_pbrRootSignature = nullptr;
    ID3D12PipelineState* m_pbrPipelineState = nullptr;
    
    // MVP matrix
    float m_mvpMatrix[16] = {};
#endif
    
    // UI visibility state
    bool m_uiVisible = true;
    
#if defined(_WIN32) && defined(_MSC_VER)
    // === DESCRIPTOR HEAPS (v2.1.0 H1.3) ===
    ID3D12DescriptorHeap* m_materialSrvHeap = nullptr; // 80 descriptors (material textures: albedo, normal, metallic, roughness, ao)
    unsigned int m_materialSrvDescriptorSize = 0;
    unsigned int m_nextMaterialSrvIndex = 0; // Track next available SRV slot (0-79)
    
    // === PBR TEXTURES (v2.1.0 H1.6) ===
    
    // GPU resources for brick textures (uploaded to GPU)
    ID3D12Resource* m_brickAlbedoGPU = nullptr;
    ID3D12Resource* m_brickNormalGPU = nullptr;
    ID3D12Resource* m_brickRoughnessGPU = nullptr;
    ID3D12Resource* m_brickMetallicGPU = nullptr;
    ID3D12Resource* m_brickAoGPU = nullptr;
    
    // GPU descriptor handles for brick textures (SRVs in material heap)
    D3D12_GPU_DESCRIPTOR_HANDLE m_brickAlbedoSRV = {};
    D3D12_GPU_DESCRIPTOR_HANDLE m_brickNormalSRV = {};
    D3D12_GPU_DESCRIPTOR_HANDLE m_brickRoughnessSRV = {};
    D3D12_GPU_DESCRIPTOR_HANDLE m_brickMetallicSRV = {};
    D3D12_GPU_DESCRIPTOR_HANDLE m_brickAoSRV = {};
#endif
    
    // v2.1.0 H1.6: Texture Manager and Brick Material Textures (cross-platform)
    Assets::TextureManager* m_textureManager = nullptr;
    Assets::MaterialTextures m_brickTextures;
    
    // Helper methods
#if defined(_WIN32) && defined(_MSC_VER)
    void CreateSceneRenderTarget();
    void CalculateMVPMatrix();
#endif
};

} // namespace Renderer
