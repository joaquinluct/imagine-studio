#include "DX12ResourceManager.h"
#include "core/Log.h"

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
