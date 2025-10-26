#include "DX12ResourceManager.h"
#include "core/Log.h"
#include "assets/MeshData.h"  // v1.9.0 H3.3 - MeshData for CreateMeshBuffers

#if defined(_WIN32) && defined(_MSC_VER)
#include <d3d12.h>
#include <dxgi1_4.h>

// Link DirectX 12 libraries
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#endif

#include <iostream>
#include <sstream>

namespace Renderer {

DX12ResourceManager::DX12ResourceManager()
{
}

DX12ResourceManager::~DX12ResourceManager()
{
    Shutdown();
}

bool DX12ResourceManager::Initialize(
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12Device* device
#else
    void* device
#endif
)
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!device)
    {
        CORE_LOG_ERROR("DX12ResourceManager::Initialize: Invalid device");
        return false;
    }

    m_device = device;

    // Cache descriptor sizes for performance
    m_descriptorSizeRTV = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    m_descriptorSizeDSV = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    m_descriptorSizeCBV_SRV_UAV = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    m_descriptorSizeSampler = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

    CORE_LOG_INFO("DX12ResourceManager initialized");
    CORE_LOG_INFO("  RTV descriptor size: " + std::to_string(m_descriptorSizeRTV));
    CORE_LOG_INFO("  DSV descriptor size: " + std::to_string(m_descriptorSizeDSV));
    CORE_LOG_INFO("  CBV/SRV/UAV descriptor size: " + std::to_string(m_descriptorSizeCBV_SRV_UAV));
    CORE_LOG_INFO("  Sampler descriptor size: " + std::to_string(m_descriptorSizeSampler));

    return true;
#else
    CORE_LOG_WARN("DX12ResourceManager::Initialize: Platform not supported");
    return false;
#endif
}

#if defined(_WIN32) && defined(_MSC_VER)
ID3D12Resource* DX12ResourceManager::CreateVertexBuffer(
    const void* data,
    unsigned int sizeInBytes,
    ID3D12GraphicsCommandList* uploadCommandList,
    ID3D12Resource** outUploadBuffer
)
#else
void* DX12ResourceManager::CreateVertexBuffer(
    const void* data,
    unsigned int sizeInBytes,
    void* uploadCommandList,
    void** outUploadBuffer
)
#endif
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!m_device || !data || sizeInBytes == 0)
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateVertexBuffer: Invalid parameters");
        return nullptr;
    }

    // Create vertex buffer in default heap (GPU-only, optimal performance)
    D3D12_HEAP_PROPERTIES defaultHeapProps = {};
    defaultHeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
    defaultHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    defaultHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    defaultHeapProps.CreationNodeMask = 1;
    defaultHeapProps.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC bufferDesc = {};
    bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    bufferDesc.Alignment = 0;
    bufferDesc.Width = sizeInBytes;
    bufferDesc.Height = 1;
    bufferDesc.DepthOrArraySize = 1;
    bufferDesc.MipLevels = 1;
    bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    bufferDesc.SampleDesc.Count = 1;
    bufferDesc.SampleDesc.Quality = 0;
    bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    bufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    ID3D12Resource* vertexBuffer = nullptr;
    HRESULT hr = m_device->CreateCommittedResource(
        &defaultHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_COPY_DEST, // Start in copy destination state
        nullptr,
        IID_PPV_ARGS(&vertexBuffer)
    );

    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateVertexBuffer: Failed to create vertex buffer");
        return nullptr;
    }

    // If command list provided, upload data immediately
    if (uploadCommandList)
    {
        // Create upload buffer for staging
        ID3D12Resource* uploadBuffer = CreateUploadBuffer(sizeInBytes);
        if (!uploadBuffer)
        {
            vertexBuffer->Release();
            return nullptr;
        }

        // Map and copy data to upload buffer
        void* pMappedData = nullptr;
        D3D12_RANGE readRange = { 0, 0 };
        hr = uploadBuffer->Map(0, &readRange, &pMappedData);
        if (FAILED(hr))
        {
            CORE_LOG_ERROR("DX12ResourceManager::CreateVertexBuffer: Failed to map upload buffer");
            uploadBuffer->Release();
            vertexBuffer->Release();
            return nullptr;
        }

        memcpy(pMappedData, data, sizeInBytes);
        uploadBuffer->Unmap(0, nullptr);

        // Copy from upload to default heap
        CopyBufferRegion(vertexBuffer, uploadBuffer, sizeInBytes, uploadCommandList);

        // Transition vertex buffer to VERTEX_AND_CONSTANT_BUFFER state
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = vertexBuffer;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        uploadCommandList->ResourceBarrier(1, &barrier);

        // v1.7.0 H3.5: Return upload buffer to caller for backward compatibility
        // In future versions, this will be removed and upload buffer will be auto-enqueued
        if (outUploadBuffer)
        {
            *outUploadBuffer = uploadBuffer;
            CORE_LOG_INFO("DX12ResourceManager: Upload buffer returned to caller (manual management)");
        }
        else
        {
            // Legacy path: release immediately (DEPRECATED - causes errors)
            uploadBuffer->Release();
            CORE_LOG_WARN("DX12ResourceManager: Upload buffer released immediately (DEPRECATED)");
        }
    }

    CORE_LOG_INFO("DX12ResourceManager: Vertex buffer created (" + std::to_string(sizeInBytes) + " bytes)");
    return vertexBuffer;
#else
    return nullptr;
#endif
}

#if defined(_WIN32) && defined(_MSC_VER)
ID3D12Resource* DX12ResourceManager::CreateConstantBuffer(unsigned int sizeInBytes)
#else
void* DX12ResourceManager::CreateConstantBuffer(unsigned int sizeInBytes)
#endif
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!m_device || sizeInBytes == 0)
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateConstantBuffer: Invalid parameters");
        return nullptr;
    }

    // Constant buffers must be 256-byte aligned
    unsigned int alignedSize = (sizeInBytes + 255) & ~255;

    // Create constant buffer in upload heap (CPU-writable, persistently mapped)
    D3D12_HEAP_PROPERTIES uploadHeapProps = {};
    uploadHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    uploadHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    uploadHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    uploadHeapProps.CreationNodeMask = 1;
    uploadHeapProps.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC bufferDesc = {};
    bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    bufferDesc.Alignment = 0;
    bufferDesc.Width = alignedSize;
    bufferDesc.Height = 1;
    bufferDesc.DepthOrArraySize = 1;
    bufferDesc.MipLevels = 1;
    bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    bufferDesc.SampleDesc.Count = 1;
    bufferDesc.SampleDesc.Quality = 0;
    bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    bufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    ID3D12Resource* constantBuffer = nullptr;
    HRESULT hr = m_device->CreateCommittedResource(
        &uploadHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, // Upload heap must be in GENERIC_READ state
        nullptr,
        IID_PPV_ARGS(&constantBuffer)
    );

    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateConstantBuffer: Failed to create constant buffer");
        return nullptr;
    }

    CORE_LOG_INFO("DX12ResourceManager: Constant buffer created (" + std::to_string(alignedSize) + " bytes, 256-byte aligned)");
    return constantBuffer;
#else
    return nullptr;
#endif
}

#if defined(_WIN32) && defined(_MSC_VER)
ID3D12Resource* DX12ResourceManager::CreateTexture2D(
    unsigned int width,
    unsigned int height,
    DXGI_FORMAT format,
    D3D12_RESOURCE_FLAGS flags,
    D3D12_RESOURCE_STATES initialState,
    const D3D12_CLEAR_VALUE* clearValue
)
#else
void* DX12ResourceManager::CreateTexture2D(
    unsigned int width,
    unsigned int height,
    unsigned int format,
    unsigned int flags,
    unsigned int initialState,
    const void* clearValue
)
#endif
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!m_device || width == 0 || height == 0)
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateTexture2D: Invalid parameters");
        return nullptr;
    }

    // Create texture resource
    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.MipLevels = 1;
    textureDesc.Format = format;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    textureDesc.Flags = flags;

    // Heap properties (default heap = GPU only)
    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
    heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProps.CreationNodeMask = 1;
    heapProps.VisibleNodeMask = 1;

    ID3D12Resource* texture = nullptr;
    HRESULT hr = m_device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &textureDesc,
        initialState,
        clearValue,
        IID_PPV_ARGS(&texture)
    );

    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateTexture2D: Failed to create texture");
        return nullptr;
    }

    CORE_LOG_INFO("DX12ResourceManager: Texture2D created (" + std::to_string(width) + "x" + std::to_string(height) + ", format " + std::to_string(format) + ")");
    return texture;
#else
    return nullptr;
#endif
}

#if defined(_WIN32) && defined(_MSC_VER)
ID3D12Resource* DX12ResourceManager::CreateTexture2DFromData(
    const unsigned char* pixels,
    unsigned int width,
    unsigned int height,
    DXGI_FORMAT format,
    ID3D12GraphicsCommandList* uploadCommandList,
    ID3D12Resource** outUploadBuffer
)
#else
void* DX12ResourceManager::CreateTexture2DFromData(
    const void* pixels,
    unsigned int width,
    unsigned int height,
    unsigned int format,
    void* uploadCommandList,
    void** outUploadBuffer
)
#endif
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!m_device || !pixels || width == 0 || height == 0 || !uploadCommandList)
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateTexture2DFromData: Invalid parameters");
        return nullptr;
    }

    // Calculate texture size (bytes per pixel depends on format)
    unsigned int bytesPerPixel = 4; // RGBA = 4 bytes
    if (format == DXGI_FORMAT_R8G8B8A8_UNORM)
    {
        bytesPerPixel = 4;
    }
    // Add more formats as needed

    // Create texture resource in default heap (GPU-only, optimal performance)
    D3D12_HEAP_PROPERTIES defaultHeapProps = {};
    defaultHeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
    defaultHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    defaultHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    defaultHeapProps.CreationNodeMask = 1;
    defaultHeapProps.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureDesc.Alignment = 0;
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.MipLevels = 1;
    textureDesc.Format = format;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    ID3D12Resource* texture = nullptr;
    HRESULT hr = m_device->CreateCommittedResource(
        &defaultHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &textureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST, // Start in copy destination state
        nullptr,
        IID_PPV_ARGS(&texture)
    );

    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateTexture2DFromData: Failed to create texture resource");
        return nullptr;
    }

    // Get upload buffer size from device
    UINT64 uploadBufferSize = 0;
    m_device->GetCopyableFootprints(&textureDesc, 0, 1, 0, nullptr, nullptr, nullptr, &uploadBufferSize);

    // Create upload buffer (staging heap for CPU-to-GPU transfer)
    ID3D12Resource* uploadBuffer = CreateUploadBuffer(static_cast<unsigned int>(uploadBufferSize));
    if (!uploadBuffer)
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateTexture2DFromData: Failed to create upload buffer");
        texture->Release();
        return nullptr;
    }

    // Map upload buffer and copy pixel data
    void* pMappedData = nullptr;
    D3D12_RANGE readRange = { 0, 0 }; // We won't read from this resource on CPU
    hr = uploadBuffer->Map(0, &readRange, &pMappedData);
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateTexture2DFromData: Failed to map upload buffer");
        uploadBuffer->Release();
        texture->Release();
        return nullptr;
    }

    // Get placed footprint for subresource layout
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint = {};
    UINT numRows = 0;
    UINT64 rowSizeInBytes = 0;
    UINT64 totalBytes = 0;
    m_device->GetCopyableFootprints(&textureDesc, 0, 1, 0, &footprint, &numRows, &rowSizeInBytes, &totalBytes);

    // Copy pixel data row by row (respecting D3D12 row pitch alignment)
    unsigned char* destPtr = static_cast<unsigned char*>(pMappedData);
    const unsigned char* srcPtr = pixels;
    unsigned int srcRowPitch = width * bytesPerPixel;

    for (unsigned int row = 0; row < height; ++row)
    {
        memcpy(destPtr + row * footprint.Footprint.RowPitch, 
               srcPtr + row * srcRowPitch, 
               srcRowPitch);
    }

    uploadBuffer->Unmap(0, nullptr);

    // Record copy command from upload buffer to texture
    D3D12_TEXTURE_COPY_LOCATION destLocation = {};
    destLocation.pResource = texture;
    destLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    destLocation.SubresourceIndex = 0;

    D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
    srcLocation.pResource = uploadBuffer;
    srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    srcLocation.PlacedFootprint = footprint;

    uploadCommandList->CopyTextureRegion(&destLocation, 0, 0, 0, &srcLocation, nullptr);

    // Transition texture from COPY_DEST to PIXEL_SHADER_RESOURCE
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = texture;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    uploadCommandList->ResourceBarrier(1, &barrier);

    // Return upload buffer to caller for deferred release
    if (outUploadBuffer)
    {
        *outUploadBuffer = uploadBuffer;
        CORE_LOG_INFO("DX12ResourceManager: Texture2D created from data (" + std::to_string(width) + "x" + std::to_string(height) + ", upload buffer returned to caller)");
    }
    else
    {
        // Legacy path: release immediately (DEPRECATED - may cause GPU errors)
        uploadBuffer->Release();
        CORE_LOG_WARN("DX12ResourceManager: Upload buffer released immediately (DEPRECATED - may cause errors)");
    }

    return texture;
#else
    return nullptr;
#endif
}

#if defined(_WIN32) && defined(_MSC_VER)
MeshBuffers DX12ResourceManager::CreateMeshBuffers(
    const Assets::MeshData& mesh,
    ID3D12GraphicsCommandList* uploadCommandList,
    ID3D12Resource** outUploadBuffers  // Array[2]: [0]=vertexUpload, [1]=indexUpload
)
#else
void* DX12ResourceManager::CreateMeshBuffers(
    const void* mesh,
    void* uploadCommandList,
    void** outUploadBuffers
)
#endif
{
#if defined(_WIN32) && defined(_MSC_VER)
    MeshBuffers meshBuffers;
    
    if (!m_device || !mesh.IsValid() || !uploadCommandList)
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateMeshBuffers: Invalid parameters");
        return meshBuffers;
    }
    
    CORE_LOG_INFO("DX12ResourceManager: Creating mesh buffers for " + mesh.name);
    
    // === CREATE VERTEX BUFFER ===
    
    unsigned int vertexBufferSize = mesh.GetVertexCount() * sizeof(Assets::Vertex);
    
    // Create vertex buffer in default heap (GPU-only)
    D3D12_HEAP_PROPERTIES defaultHeapProps = {};
    defaultHeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
    defaultHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    defaultHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    defaultHeapProps.CreationNodeMask = 1;
    defaultHeapProps.VisibleNodeMask = 1;
    
    D3D12_RESOURCE_DESC vertexBufferDesc = {};
    vertexBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    vertexBufferDesc.Alignment = 0;
    vertexBufferDesc.Width = vertexBufferSize;
    vertexBufferDesc.Height = 1;
    vertexBufferDesc.DepthOrArraySize = 1;
    vertexBufferDesc.MipLevels = 1;
    vertexBufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    vertexBufferDesc.SampleDesc.Count = 1;
    vertexBufferDesc.SampleDesc.Quality = 0;
    vertexBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    vertexBufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    
    HRESULT hr = m_device->CreateCommittedResource(
        &defaultHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &vertexBufferDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&meshBuffers.vertexBuffer)
    );
    
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateMeshBuffers: Failed to create vertex buffer");
        return meshBuffers;
    }
    
    // === CREATE INDEX BUFFER ===
    
    unsigned int indexBufferSize = mesh.GetIndexCount() * sizeof(unsigned int);
    
    D3D12_RESOURCE_DESC indexBufferDesc = {};
    indexBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    indexBufferDesc.Alignment = 0;
    indexBufferDesc.Width = indexBufferSize;
    indexBufferDesc.Height = 1;
    indexBufferDesc.DepthOrArraySize = 1;
    indexBufferDesc.MipLevels = 1;
    indexBufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    indexBufferDesc.SampleDesc.Count = 1;
    indexBufferDesc.SampleDesc.Quality = 0;
    indexBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    indexBufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    
    hr = m_device->CreateCommittedResource(
        &defaultHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &indexBufferDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&meshBuffers.indexBuffer)
    );
    
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateMeshBuffers: Failed to create index buffer");
        meshBuffers.vertexBuffer->Release();
        meshBuffers.vertexBuffer = nullptr;
        return meshBuffers;
    }
    
    // === UPLOAD VERTEX DATA ===
    
    // Create vertex upload buffer
    ID3D12Resource* vertexUploadBuffer = CreateUploadBuffer(vertexBufferSize);
    if (!vertexUploadBuffer)
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateMeshBuffers: Failed to create vertex upload buffer");
        meshBuffers.vertexBuffer->Release();
        meshBuffers.indexBuffer->Release();
        meshBuffers.vertexBuffer = nullptr;
        meshBuffers.indexBuffer = nullptr;
        return meshBuffers;
    }
    
    // Map and copy vertex data
    void* vertexMappedData = nullptr;
    D3D12_RANGE readRange = { 0, 0 };
    hr = vertexUploadBuffer->Map(0, &readRange, &vertexMappedData);
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateMeshBuffers: Failed to map vertex upload buffer");
        vertexUploadBuffer->Release();
        meshBuffers.vertexBuffer->Release();
        meshBuffers.indexBuffer->Release();
        meshBuffers.vertexBuffer = nullptr;
        meshBuffers.indexBuffer = nullptr;
        return meshBuffers;
    }
    
    memcpy(vertexMappedData, mesh.vertices.data(), vertexBufferSize);
    vertexUploadBuffer->Unmap(0, nullptr);
    
    // === UPLOAD INDEX DATA ===
    
    // Create index upload buffer
    ID3D12Resource* indexUploadBuffer = CreateUploadBuffer(indexBufferSize);
    if (!indexUploadBuffer)
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateMeshBuffers: Failed to create index upload buffer");
        vertexUploadBuffer->Release();
        meshBuffers.vertexBuffer->Release();
        meshBuffers.indexBuffer->Release();
        meshBuffers.vertexBuffer = nullptr;
        meshBuffers.indexBuffer = nullptr;
        return meshBuffers;
    }
    
    // Map and copy index data
    void* indexMappedData = nullptr;
    hr = indexUploadBuffer->Map(0, &readRange, &indexMappedData);
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateMeshBuffers: Failed to map index upload buffer");
        indexUploadBuffer->Release();
        vertexUploadBuffer->Release();
        meshBuffers.vertexBuffer->Release();
        meshBuffers.indexBuffer->Release();
        meshBuffers.vertexBuffer = nullptr;
        meshBuffers.indexBuffer = nullptr;
        return meshBuffers;
    }
    
    memcpy(indexMappedData, mesh.indices.data(), indexBufferSize);
    indexUploadBuffer->Unmap(0, nullptr);
    
    // === COPY TO GPU ===
    
    // Copy vertex buffer
    CopyBufferRegion(meshBuffers.vertexBuffer, vertexUploadBuffer, vertexBufferSize, uploadCommandList);
    
    // Copy index buffer
    CopyBufferRegion(meshBuffers.indexBuffer, indexUploadBuffer, indexBufferSize, uploadCommandList);
    
    // === RESOURCE BARRIERS ===
    
    D3D12_RESOURCE_BARRIER barriers[2] = {};
    
    // Vertex buffer: COPY_DEST ? VERTEX_AND_CONSTANT_BUFFER
    barriers[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barriers[0].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barriers[0].Transition.pResource = meshBuffers.vertexBuffer;
    barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barriers[0].Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    barriers[0].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    
    // Index buffer: COPY_DEST ? INDEX_BUFFER
    barriers[1].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barriers[1].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barriers[1].Transition.pResource = meshBuffers.indexBuffer;
    barriers[1].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barriers[1].Transition.StateAfter = D3D12_RESOURCE_STATE_INDEX_BUFFER;
    barriers[1].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    
    uploadCommandList->ResourceBarrier(2, barriers);
    
    // === CREATE BUFFER VIEWS ===
    
    meshBuffers.vertexBufferView.BufferLocation = meshBuffers.vertexBuffer->GetGPUVirtualAddress();
    meshBuffers.vertexBufferView.StrideInBytes = sizeof(Assets::Vertex);
    meshBuffers.vertexBufferView.SizeInBytes = vertexBufferSize;
    
    meshBuffers.indexBufferView.BufferLocation = meshBuffers.indexBuffer->GetGPUVirtualAddress();
    meshBuffers.indexBufferView.Format = DXGI_FORMAT_R32_UINT;  // 32-bit unsigned int indices
    meshBuffers.indexBufferView.SizeInBytes = indexBufferSize;
    
    meshBuffers.vertexCount = mesh.GetVertexCount();
    meshBuffers.indexCount = mesh.GetIndexCount();
    
    // === RETURN UPLOAD BUFFERS ===
    
    if (outUploadBuffers)
    {
        outUploadBuffers[0] = vertexUploadBuffer;
        outUploadBuffers[1] = indexUploadBuffer;
        
        CORE_LOG_INFO("DX12ResourceManager: Mesh buffers created for " + mesh.name);
        CORE_LOG_INFO("  Vertex buffer: " + std::to_string(mesh.GetVertexCount()) + " vertices (" + std::to_string(vertexBufferSize) + " bytes)");
        CORE_LOG_INFO("  Index buffer: " + std::to_string(mesh.GetIndexCount()) + " indices (" + std::to_string(indexBufferSize) + " bytes)");
        CORE_LOG_INFO("  Triangles: " + std::to_string(mesh.GetTriangleCount()));
        CORE_LOG_INFO("  Upload buffers returned to caller (manual management)");
    }
    else
    {
        // Legacy path: release immediately (DEPRECATED)
        vertexUploadBuffer->Release();
        indexUploadBuffer->Release();
        CORE_LOG_WARN("DX12ResourceManager: Upload buffers released immediately (DEPRECATED)");
    }
    
    return meshBuffers;
#else
    return nullptr;
#endif
}

#if defined(_WIN32) && defined(_MSC_VER)
ID3D12DescriptorHeap* DX12ResourceManager::CreateDescriptorHeap(
    D3D12_DESCRIPTOR_HEAP_TYPE type,
    unsigned int numDescriptors,
    bool shaderVisible
)
#else
void* DX12ResourceManager::CreateDescriptorHeap(
    unsigned int type,
    unsigned int numDescriptors,
    bool shaderVisible
)
#endif
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!m_device || numDescriptors == 0)
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateDescriptorHeap: Invalid parameters");
        return nullptr;
    }

    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.NumDescriptors = numDescriptors;
    heapDesc.Type = type;
    heapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    ID3D12DescriptorHeap* heap = nullptr;
    HRESULT hr = m_device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap));

    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateDescriptorHeap: Failed to create descriptor heap");
        return nullptr;
    }

    // Get type name for logging
    const char* typeName = "UNKNOWN";
    switch (type)
    {
    case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV: typeName = "CBV/SRV/UAV"; break;
    case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER: typeName = "SAMPLER"; break;
    case D3D12_DESCRIPTOR_HEAP_TYPE_RTV: typeName = "RTV"; break;
    case D3D12_DESCRIPTOR_HEAP_TYPE_DSV: typeName = "DSV"; break;
    }

    CORE_LOG_INFO("DX12ResourceManager: Descriptor heap created (" + std::string(typeName) + ", " + std::to_string(numDescriptors) + " descriptors, shader visible: " + (shaderVisible ? "true" : "false") + ")");
    return heap;
#else
    return nullptr;
#endif
}

unsigned int DX12ResourceManager::GetDescriptorSize(
#if defined(_WIN32) && defined(_MSC_VER)
    D3D12_DESCRIPTOR_HEAP_TYPE type
#else
    unsigned int type
#endif
) const
{
#if defined(_WIN32) && defined(_MSC_VER)
    switch (type)
    {
    case D3D12_DESCRIPTOR_HEAP_TYPE_RTV: return m_descriptorSizeRTV;
    case D3D12_DESCRIPTOR_HEAP_TYPE_DSV: return m_descriptorSizeDSV;
    case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV: return m_descriptorSizeCBV_SRV_UAV;
    case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER: return m_descriptorSizeSampler;
    default: return 0;
    }
#else
    return 0;
#endif
}

void DX12ResourceManager::ReleaseResource(
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12Resource* resource
#else
    void* resource
#endif
)
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (resource)
    {
        resource->Release();
    }
#endif
}

void DX12ResourceManager::ReleaseDescriptorHeap(
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12DescriptorHeap* heap
#else
    void* heap
#endif
)
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (heap)
    {
        heap->Release();
    }
#endif
}

void DX12ResourceManager::Shutdown()
{
#if defined(_WIN32) && defined(_MSC_VER)
    // v1.7.0 H3: Release ALL pending resources before shutdown
    for (auto& pending : m_pendingReleases)
    {
        if (pending.resource)
        {
            pending.resource->Release();
        }
    }
    m_pendingReleases.clear();

    CORE_LOG_INFO("DX12ResourceManager shutdown");
#endif

    m_device = nullptr;
    m_descriptorSizeRTV = 0;
    m_descriptorSizeDSV = 0;
    m_descriptorSizeCBV_SRV_UAV = 0;
    m_descriptorSizeSampler = 0;
}

#if defined(_WIN32) && defined(_MSC_VER)
ID3D12Resource* DX12ResourceManager::CreateUploadBuffer(unsigned int sizeInBytes)
{
    if (!m_device || sizeInBytes == 0)
    {
        return nullptr;
    }

    D3D12_HEAP_PROPERTIES uploadHeapProps = {};
    uploadHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    uploadHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    uploadHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    uploadHeapProps.CreationNodeMask = 1;
    uploadHeapProps.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC bufferDesc = {};
    bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    bufferDesc.Alignment = 0;
    bufferDesc.Width = sizeInBytes;
    bufferDesc.Height = 1;
    bufferDesc.DepthOrArraySize = 1;
    bufferDesc.MipLevels = 1;
    bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    bufferDesc.SampleDesc.Count = 1;
    bufferDesc.SampleDesc.Quality = 0;
    bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    bufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    ID3D12Resource* uploadBuffer = nullptr;
    HRESULT hr = m_device->CreateCommittedResource(
        &uploadHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&uploadBuffer)
    );

    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12ResourceManager::CreateUploadBuffer: Failed to create upload buffer");
        return nullptr;
    }

    return uploadBuffer;
}

void DX12ResourceManager::CopyBufferRegion(
    ID3D12Resource* dest,
    ID3D12Resource* src,
    unsigned int sizeInBytes,
    ID3D12GraphicsCommandList* commandList
)
{
    if (!dest || !src || !commandList || sizeInBytes == 0)
    {
        CORE_LOG_ERROR("DX12ResourceManager::CopyBufferRegion: Invalid parameters");
        return;
    }

    commandList->CopyBufferRegion(dest, 0, src, 0, sizeInBytes);
}
#endif

void DX12ResourceManager::ProcessDeferredReleases(
#if defined(_WIN32) && defined(_MSC_VER)
    unsigned long long completedFenceValue
#else
    unsigned long long completedFenceValue
#endif
)
{
#if defined(_WIN32) && defined(_MSC_VER)
    // v1.7.0 H3.4 - Process pending releases
    // Release resources whose fence value has been reached by GPU
    for (auto it = m_pendingReleases.begin(); it != m_pendingReleases.end(); )
    {
        if (completedFenceValue >= it->fenceValue)
        {
            // GPU finished using this resource, safe to release
            if (it->resource)
            {
                it->resource->Release();
            }
            
            // Remove from pending list
            it = m_pendingReleases.erase(it);
        }
        else
        {
            // GPU still using this resource, keep it
            ++it;
        }
    }
#endif
}

} // namespace Renderer
