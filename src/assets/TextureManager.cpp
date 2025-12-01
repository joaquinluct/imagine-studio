#include "TextureManager.h"
#include "core/Log.h"
#include <sys/stat.h> // For file size
#include <functional> // For std::hash

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
                
                // v2.2.0 H1: Register in AssetDatabase
                RegisterTextureAsset(albedoPath, loaded.cpuData);
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
                
                // v2.2.0 H1: Register in AssetDatabase
                RegisterTextureAsset(normalPath, loaded.cpuData);
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
                
                // v2.2.0 H1: Register in AssetDatabase
                RegisterTextureAsset(roughnessPath, loaded.cpuData);
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
                
                // v2.2.0 H1: Register in AssetDatabase
                RegisterTextureAsset(metallicPath, loaded.cpuData);
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
                
                // v2.2.0 H1: Register in AssetDatabase
                RegisterTextureAsset(aoPath, loaded.cpuData);
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

// v2.1.0 H2.3: Hot-reload support

void TextureManager::EnableHotReload(bool enable)
{
    m_hotReloadEnabled = enable;
    
    if (enable)
    {
        // Start watching all loaded textures
        for (const auto& pair : m_textureCache)
        {
            const std::string& filepath = pair.first;
            m_fileWatcher.WatchFile(filepath, [this](const std::string& path) {
                this->OnTextureFileChanged(path);
            });
        }
        
        CORE_LOG_INFO("TextureManager: Hot-reload enabled");
    }
    else
    {
        // Stop watching all files
        m_fileWatcher.Clear();
        
        CORE_LOG_INFO("TextureManager: Hot-reload disabled");
    }
}

void TextureManager::Update()
{
    if (m_hotReloadEnabled)
    {
        m_fileWatcher.Update();
    }
}

void TextureManager::OnTextureFileChanged(const std::string& filepath)
{
    CORE_LOG_INFO("TextureManager: Texture file modified detected: " + filepath);
    
    // Find texture in cache
    auto it = m_textureCache.find(filepath);
    if (it == m_textureCache.end())
    {
        CORE_LOG_WARN("TextureManager: File modified but not in cache: " + filepath);
        return;
    }
    
    // Reload texture from disk
    CORE_LOG_INFO("TextureManager: Reloading texture: " + filepath);
    
    LoadedTexture& loadedTexture = it->second;
    
    // Free old CPU data (RAII will handle this automatically)
    // Load new CPU data
    TextureData newData = TextureLoader::LoadTexture(filepath);
    
    if (!newData.IsValid())
    {
        CORE_LOG_ERROR("TextureManager: Failed to reload texture: " + filepath);
        return;
    }
    
    // Update CPU data (RAII handles old data cleanup)
    loadedTexture.cpuData = std::move(newData);
    
    // TODO: Upload to GPU and recreate SRV (requires DX12ResourceManager integration)
    // For now, just update CPU data
    // Full GPU hot-reload will be implemented when DX12ResourceManager supports it
    
    CORE_LOG_INFO("TextureManager: Texture reloaded successfully (CPU): " + filepath);
    CORE_LOG_INFO("  New size: " + std::to_string(loadedTexture.cpuData.width) + "x" + 
                  std::to_string(loadedTexture.cpuData.height));
}

// v2.2.0 H1: Register texture in AssetDatabase
void TextureManager::RegisterTextureAsset(const std::string& path, const TextureData& data)
{
    // Get file size for metadata
    struct stat fileStat;
    uint64_t fileSize = 0;
    uint64_t lastModified = 0;
    
    if (stat(path.c_str(), &fileStat) == 0)
    {
        fileSize = static_cast<uint64_t>(fileStat.st_size);
        lastModified = static_cast<uint64_t>(fileStat.st_mtime);
    }
    
    // Generate AssetID from path hash (simple hash function)
    AssetID assetID = std::hash<std::string>{}(path);
    
    // Extract filename from path for asset name
    size_t lastSlash = path.find_last_of("/\\");
    std::string filename = (lastSlash != std::string::npos) ? path.substr(lastSlash + 1) : path;
    
    // Remove extension for clean name
    size_t lastDot = filename.find_last_of('.');
    std::string assetName = (lastDot != std::string::npos) ? filename.substr(0, lastDot) : filename;
    
    // Create metadata struct
    AssetMetadata metadata;
    metadata.id = assetID;
    metadata.type = AssetType::Texture;
    metadata.path = path;
    metadata.name = assetName;
    metadata.fileSize = fileSize;
    metadata.lastModified = lastModified;
    
    // Register in AssetDatabase
    AssetDatabase& db = AssetDatabase::GetInstance();
    db.RegisterAsset(metadata);
    
    CORE_LOG_INFO("TextureManager: Registered in AssetDatabase: " + path + 
                  " (ID: " + std::to_string(assetID) + ", Name: " + assetName + ")");
}

} // namespace Assets
