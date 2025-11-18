#pragma once

#include "TextureLoader.h"
#include <string>
#include <unordered_map>

#if defined(_WIN32) && defined(_MSC_VER)
struct ID3D12Resource;
struct D3D12_GPU_DESCRIPTOR_HANDLE;
#endif

namespace Assets {

// v2.1.0 H1.2 - Texture cache for loaded PBR textures
// Stores CPU pixel data + GPU resource handle
struct LoadedTexture {
    TextureData cpuData;  // CPU pixel data (RGBA8)
    
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12Resource* gpuResource = nullptr;  // GPU texture resource (uploaded)
    D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = {};  // SRV descriptor handle (for shader binding)
#else
    void* gpuResource = nullptr;
    void* srvHandle = nullptr;
#endif
    
    bool IsValid() const { return cpuData.IsValid(); }
    bool IsUploadedToGPU() const { return gpuResource != nullptr; }
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
    
private:
    std::unordered_map<std::string, LoadedTexture> m_textureCache;
};

} // namespace Assets
