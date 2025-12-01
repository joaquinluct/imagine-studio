#include "TextureManager.h"
#include "core/Log.h"

namespace Assets {

TextureManager::~TextureManager()
{
    Clear();
}

MaterialTextures TextureManager::LoadMaterialTextures(
    const std::string& albedoPath,
    const std::string& normalPath,
    const std::string& roughnessPath,
    const std::string& metallicPath,
    const std::string& aoPath
)
{
    MaterialTextures textures;
    textures.albedoPath = albedoPath;
    textures.normalPath = normalPath;
    textures.roughnessPath = roughnessPath;
    textures.metallicPath = metallicPath;
    textures.aoPath = aoPath;
    
    CORE_LOG_INFO("TextureManager: Loading material textures...");
    
    // Load Albedo
    if (!albedoPath.empty())
    {
        auto it = m_textureCache.find(albedoPath);
        if (it != m_textureCache.end())
        {
            textures.albedo = it->second;
            CORE_LOG_INFO("  Albedo: " + albedoPath + " (cached)");
        }
        else
        {
            LoadedTexture loaded;
            loaded.cpuData = TextureLoader::LoadTexture(albedoPath);
            if (loaded.IsValid())
            {
                m_textureCache[albedoPath] = loaded;
                textures.albedo = loaded;
                CORE_LOG_INFO("  Albedo: " + albedoPath + " (" + 
                    std::to_string(loaded.cpuData.width) + "x" + 
                    std::to_string(loaded.cpuData.height) + ")");
            }
            else
            {
                CORE_LOG_ERROR("  Failed to load Albedo: " + albedoPath);
            }
        }
    }
    
    // Load Normal
    if (!normalPath.empty())
    {
        auto it = m_textureCache.find(normalPath);
        if (it != m_textureCache.end())
        {
            textures.normal = it->second;
            CORE_LOG_INFO("  Normal: " + normalPath + " (cached)");
        }
        else
        {
            LoadedTexture loaded;
            loaded.cpuData = TextureLoader::LoadTexture(normalPath);
            if (loaded.IsValid())
            {
                m_textureCache[normalPath] = loaded;
                textures.normal = loaded;
                CORE_LOG_INFO("  Normal: " + normalPath + " (" + 
                    std::to_string(loaded.cpuData.width) + "x" + 
                    std::to_string(loaded.cpuData.height) + ")");
            }
            else
            {
                CORE_LOG_ERROR("  Failed to load Normal: " + normalPath);
            }
        }
    }
    
    // Load Roughness
    if (!roughnessPath.empty())
    {
        auto it = m_textureCache.find(roughnessPath);
        if (it != m_textureCache.end())
        {
            textures.roughness = it->second;
            CORE_LOG_INFO("  Roughness: " + roughnessPath + " (cached)");
        }
        else
        {
            LoadedTexture loaded;
            loaded.cpuData = TextureLoader::LoadTexture(roughnessPath);
            if (loaded.IsValid())
            {
                m_textureCache[roughnessPath] = loaded;
                textures.roughness = loaded;
                CORE_LOG_INFO("  Roughness: " + roughnessPath + " (" + 
                    std::to_string(loaded.cpuData.width) + "x" + 
                    std::to_string(loaded.cpuData.height) + ")");
            }
            else
            {
                CORE_LOG_ERROR("  Failed to load Roughness: " + roughnessPath);
            }
        }
    }
    
    // Load Metallic
    if (!metallicPath.empty())
    {
        auto it = m_textureCache.find(metallicPath);
        if (it != m_textureCache.end())
        {
            textures.metallic = it->second;
            CORE_LOG_INFO("  Metallic: " + metallicPath + " (cached)");
        }
        else
        {
            LoadedTexture loaded;
            loaded.cpuData = TextureLoader::LoadTexture(metallicPath);
            if (loaded.IsValid())
            {
                m_textureCache[metallicPath] = loaded;
                textures.metallic = loaded;
                CORE_LOG_INFO("  Metallic: " + metallicPath + " (" + 
                    std::to_string(loaded.cpuData.width) + "x" + 
                    std::to_string(loaded.cpuData.height) + ")");
            }
            else
            {
                CORE_LOG_ERROR("  Failed to load Metallic: " + metallicPath);
            }
        }
    }
    
    // Load AO
    if (!aoPath.empty())
    {
        auto it = m_textureCache.find(aoPath);
        if (it != m_textureCache.end())
        {
            textures.ao = it->second;
            CORE_LOG_INFO("  AO: " + aoPath + " (cached)");
        }
        else
        {
            LoadedTexture loaded;
            loaded.cpuData = TextureLoader::LoadTexture(aoPath);
            if (loaded.IsValid())
            {
                m_textureCache[aoPath] = loaded;
                textures.ao = loaded;
                CORE_LOG_INFO("  AO: " + aoPath + " (" + 
                    std::to_string(loaded.cpuData.width) + "x" + 
                    std::to_string(loaded.cpuData.height) + ")");
            }
            else
            {
                CORE_LOG_ERROR("  Failed to load AO: " + aoPath);
            }
        }
    }
    
    if (textures.IsFullyLoaded())
    {
        CORE_LOG_INFO("TextureManager: Material textures loaded successfully (5/5)");
    }
    else
    {
        CORE_LOG_WARN("TextureManager: Material textures partially loaded");
    }
    
    return textures;
}

LoadedTexture* TextureManager::GetTexture(const std::string& path)
{
    auto it = m_textureCache.find(path);
    return (it != m_textureCache.end()) ? &it->second : nullptr;
}

void TextureManager::Clear()
{
    // TextureData now uses RAII - memory is automatically freed when m_textureCache is cleared
    // No need to manually call FreeTextureData
    
    // Note: GPU resources are NOT released here
    // They should be released by DX12ResourceManager when no longer needed
    
    m_textureCache.clear();
    
    CORE_LOG_INFO("TextureManager: Cleared all textures");
}

} // namespace Assets
