#pragma once

#include "TextureLoader.h"
#include "FileWatcher.h" // v2.1.0 H2.3
#include "AssetDatabase.h" // v2.2.0 H1 - Integration
#include <string>
#include <unordered_map>

#if defined(_WIN32) && defined(_MSC_VER)
#include <d3d12.h>  // For D3D12_GPU_DESCRIPTOR_HANDLE and ID3D12Resource
#endif

namespace Assets {

// v2.1.0 H1.2 - Texture cache for loaded PBR textures
// Stores CPU pixel data + GPU resource handle
// RAII: Rule of Five implemented to prevent double-free
struct LoadedTexture {
    TextureData cpuData;  // CPU pixel data (RGBA8) - owns memory via RAII
    
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12Resource* gpuResource = nullptr;  // GPU texture resource (uploaded) - NOT owned
    D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = {};  // SRV descriptor handle (for shader binding)
    
    // v2.4.0 H3: Thumbnail GPU resources (128x128 downsampled for ImGui preview)
    ID3D12Resource* thumbnailGpuResource = nullptr;  // GPU thumbnail texture (128x128)
    D3D12_GPU_DESCRIPTOR_HANDLE thumbnailSrvHandle = {};  // SRV for ImGui::Image()
    uint32_t thumbnailWidth = 0;
    uint32_t thumbnailHeight = 0;
#else
    void* gpuResource = nullptr;
    void* srvHandle = nullptr;
    void* thumbnailGpuResource = nullptr;
    void* thumbnailSrvHandle = nullptr;
    uint32_t thumbnailWidth = 0;
    uint32_t thumbnailHeight = 0;
#endif
    
    // Constructor
    LoadedTexture() = default;
    
    // Destructor - cpuData handles its own cleanup, GPU resources are managed externally
    ~LoadedTexture() = default;
    
    // Copy constructor - Deep copy via TextureData copy constructor
    LoadedTexture(const LoadedTexture& other) = default;
    
    // Copy assignment - Deep copy via TextureData copy assignment
    LoadedTexture& operator=(const LoadedTexture& other) = default;
    
    // Move constructor - Transfer ownership via TextureData move constructor
    LoadedTexture(LoadedTexture&& other) noexcept = default;
    
    // Move assignment - Transfer ownership via TextureData move assignment
    LoadedTexture& operator=(LoadedTexture&& other) noexcept = default;
    
    bool IsValid() const { return cpuData.IsValid(); }
    bool IsUploadedToGPU() const { return gpuResource != nullptr; }
    bool HasThumbnail() const { return thumbnailGpuResource != nullptr; }
};

// v2.1.0 H1.2 - Material textures (5 PBR textures)
struct MaterialTextures {
    std::string albedoPath;
    std::string normalPath;
    std::string roughnessPath;
    std::string metallicPath;
    std::string aoPath;
    
    LoadedTexture albedo;
    LoadedTexture normal;
    LoadedTexture roughness;
    LoadedTexture metallic;
    LoadedTexture ao;
    
    bool IsFullyLoaded() const {
        return albedo.IsValid() && normal.IsValid() && 
               roughness.IsValid() && metallic.IsValid() && ao.IsValid();
    }
};

// v2.1.0 H1.2 - Texture manager for caching loaded textures
class TextureManager {
public:
    TextureManager() = default;
    ~TextureManager();
    
    // Load material textures from disk (5 PBR textures)
    // Returns MaterialTextures with CPU data loaded
    MaterialTextures LoadMaterialTextures(
        const std::string& albedoPath,
        const std::string& normalPath,
        const std::string& roughnessPath,
        const std::string& metallicPath,
        const std::string& aoPath
    );
    
    // Get loaded texture by path (from cache)
    LoadedTexture* GetTexture(const std::string& path);
    
    // Free all loaded textures
    void Clear();
    
    // v2.1.0 H2.3: Hot-reload support
    void EnableHotReload(bool enable);
    void Update(); // Call every frame to check for file changes
    
    // v2.2.0 H1: Get loaded textures (for AssetBrowser integration)
    const std::unordered_map<std::string, LoadedTexture>& GetLoadedTextures() const { return m_textureCache; }
    
    // v2.4.0 H3: Create GPU thumbnails for Asset Browser preview
    // Requires DX12 device and command list (pass from renderer)
    void CreateThumbnails(void* device, void* commandList, void* descriptorHeap, uint32_t descriptorSize);
    
    // v2.5.0 H4.3: Get thumbnail SRV handle for ImGui::Image() rendering
    // Returns GPU descriptor handle if thumbnail exists, otherwise returns handle with .ptr = 0
    void* GetThumbnailHandle(const std::string& path) const;
    
private:
    std::unordered_map<std::string, LoadedTexture> m_textureCache;
    
    // v2.1.0 H2.3: File watcher for hot-reload
    FileWatcher m_fileWatcher;
    bool m_hotReloadEnabled = false;
    
    // v2.1.0 H2.3: Reload texture callback
    void OnTextureFileChanged(const std::string& filepath);
    
    // v2.2.0 H1: Register texture in AssetDatabase
    void RegisterTextureAsset(const std::string& path, const TextureData& data);
};

} // namespace Assets
