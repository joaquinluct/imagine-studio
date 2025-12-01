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

// v2.4.0 H3: Create GPU thumbnails for Asset Browser
void TextureManager::CreateThumbnails(void* devicePtr, void* commandListPtr, void* descriptorHeapPtr, uint32_t descriptorSize)
{
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12Device* device = static_cast<ID3D12Device*>(devicePtr);
    ID3D12GraphicsCommandList* commandList = static_cast<ID3D12GraphicsCommandList*>(commandListPtr);
    ID3D12DescriptorHeap* descriptorHeap = static_cast<ID3D12DescriptorHeap*>(descriptorHeapPtr);
    
    if (!device || !commandList || !descriptorHeap) {
        CORE_LOG_ERROR("TextureManager::CreateThumbnails: Invalid DX12 resources");
        return;
    }
    
    CORE_LOG_INFO("TextureManager: Creating thumbnails for " + std::to_string(m_textureCache.size()) + " textures");
    
    uint32_t descriptorIndex = 0; // Start from first available descriptor in heap
    D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
    
    for (auto& pair : m_textureCache) {
        const std::string& path = pair.first;
        LoadedTexture& texture = pair.second;
        
        if (!texture.IsValid()) {
            continue;
        }
        
        // Skip if thumbnail already created
        if (texture.HasThumbnail()) {
            descriptorIndex++;
            cpuHandle.ptr += descriptorSize;
            gpuHandle.ptr += descriptorSize;
            continue;
        }
        
        // Downsample to 128x128 (simple box filter on CPU)
        const uint32_t thumbnailSize = 128;
        texture.thumbnailWidth = thumbnailSize;
        texture.thumbnailHeight = thumbnailSize;
        
        // Calculate downsample ratios
        float scaleX = static_cast<float>(texture.cpuData.width) / thumbnailSize;
        float scaleY = static_cast<float>(texture.cpuData.height) / thumbnailSize;
        
        // Allocate thumbnail pixel data (RGBA8)
        std::vector<unsigned char> thumbnailPixels(thumbnailSize * thumbnailSize * 4);
        
        // Simple box filter downsampling
        for (uint32_t y = 0; y < thumbnailSize; ++y) {
            for (uint32_t x = 0; x < thumbnailSize; ++x) {
                // Map thumbnail pixel to source texture
                uint32_t srcX = static_cast<uint32_t>(x * scaleX);
                uint32_t srcY = static_cast<uint32_t>(y * scaleY);
                
                // Clamp to texture bounds
                srcX = (srcX < texture.cpuData.width) ? srcX : (texture.cpuData.width - 1);
                srcY = (srcY < texture.cpuData.height) ? srcY : (texture.cpuData.height - 1);
                
                // Sample pixel from source
                uint32_t srcIndex = (srcY * texture.cpuData.width + srcX) * 4;
                uint32_t dstIndex = (y * thumbnailSize + x) * 4;
                
                // Copy RGBA
                thumbnailPixels[dstIndex + 0] = texture.cpuData.pixels[srcIndex + 0]; // R
                thumbnailPixels[dstIndex + 1] = texture.cpuData.pixels[srcIndex + 1]; // G
                thumbnailPixels[dstIndex + 2] = texture.cpuData.pixels[srcIndex + 2]; // B
                thumbnailPixels[dstIndex + 3] = texture.cpuData.pixels[srcIndex + 3]; // A
            }
        }
        
        // Create GPU texture for thumbnail (128x128 RGBA8)
        D3D12_RESOURCE_DESC textureDesc = {};
        textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        textureDesc.Alignment = 0;
        textureDesc.Width = thumbnailSize;
        textureDesc.Height = thumbnailSize;
        textureDesc.DepthOrArraySize = 1;
        textureDesc.MipLevels = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        
        D3D12_HEAP_PROPERTIES heapProps = {};
        heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
        heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heapProps.CreationNodeMask = 1;
        heapProps.VisibleNodeMask = 1;
        
        ID3D12Resource* thumbnailResource = nullptr;
        HRESULT hr = device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &textureDesc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(&thumbnailResource)
        );
        
        if (FAILED(hr) || !thumbnailResource) {
            CORE_LOG_ERROR("TextureManager: Failed to create thumbnail GPU resource for: " + path);
            continue;
        }
        
        // Create upload buffer for thumbnail
        const UINT64 uploadBufferSize = thumbnailSize * thumbnailSize * 4;
        
        D3D12_HEAP_PROPERTIES uploadHeapProps = {};
        uploadHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        uploadHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        uploadHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        uploadHeapProps.CreationNodeMask = 1;
        uploadHeapProps.VisibleNodeMask = 1;
        
        D3D12_RESOURCE_DESC uploadBufferDesc = {};
        uploadBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        uploadBufferDesc.Alignment = 0;
        uploadBufferDesc.Width = uploadBufferSize;
        uploadBufferDesc.Height = 1;
        uploadBufferDesc.DepthOrArraySize = 1;
        uploadBufferDesc.MipLevels = 1;
        uploadBufferDesc.Format = DXGI_FORMAT_UNKNOWN;
        uploadBufferDesc.SampleDesc.Count = 1;
        uploadBufferDesc.SampleDesc.Quality = 0;
        uploadBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        uploadBufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        
        ID3D12Resource* uploadBuffer = nullptr;
        hr = device->CreateCommittedResource(
            &uploadHeapProps,
            D3D12_HEAP_FLAG_NONE,
            &uploadBufferDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&uploadBuffer)
        );
        
        if (FAILED(hr) || !uploadBuffer) {
            CORE_LOG_ERROR("TextureManager: Failed to create thumbnail upload buffer");
            thumbnailResource->Release();
            continue;
        }
        
        // Map and copy pixel data to upload buffer
        void* mappedData = nullptr;
        hr = uploadBuffer->Map(0, nullptr, &mappedData);
        if (SUCCEEDED(hr)) {
            memcpy(mappedData, thumbnailPixels.data(), uploadBufferSize);
            uploadBuffer->Unmap(0, nullptr);
        }
        
        // Copy from upload buffer to GPU texture
        D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint = {};
        footprint.Offset = 0;
        footprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        footprint.Footprint.Width = thumbnailSize;
        footprint.Footprint.Height = thumbnailSize;
        footprint.Footprint.Depth = 1;
        footprint.Footprint.RowPitch = thumbnailSize * 4; // RGBA = 4 bytes per pixel
        
        D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
        srcLocation.pResource = uploadBuffer;
        srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
        srcLocation.PlacedFootprint = footprint;
        
        D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
        dstLocation.pResource = thumbnailResource;
        dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        dstLocation.SubresourceIndex = 0;
        
        commandList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, nullptr);
        
        // Transition to PIXEL_SHADER_RESOURCE for ImGui rendering
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = thumbnailResource;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        
        commandList->ResourceBarrier(1, &barrier);
        
        // Create SRV for ImGui::Image()
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Texture2D.MipLevels = 1;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.PlaneSlice = 0;
        srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
        
        device->CreateShaderResourceView(thumbnailResource, &srvDesc, cpuHandle);
        
        // Store thumbnail data
        texture.thumbnailGpuResource = thumbnailResource;
        texture.thumbnailSrvHandle = gpuHandle;
        
        // Release upload buffer (GPU copy is done)
        uploadBuffer->Release();
        
        // Advance descriptor handles
        descriptorIndex++;
        cpuHandle.ptr += descriptorSize;
        gpuHandle.ptr += descriptorSize;
        
        CORE_LOG_INFO("TextureManager: Thumbnail created for: " + path + " (128x128)");
    }
    
    CORE_LOG_INFO("TextureManager: Thumbnails creation completed");
#endif
}

} // namespace Assets
