#pragma once

#if defined(_WIN32) && defined(_MSC_VER)
#include <d3d12.h>
#include <dxgiformat.h>
#endif

namespace Renderer {

// v1.6.0 DEV-002.3 - Resource Manager (AAA Architecture)
// Encapsulates resource creation (buffers, textures, descriptor heaps)
// Inspired by Unreal Engine FD3D12ResourceAllocator and Unity ResourceManager
class DX12ResourceManager {
public:
    DX12ResourceManager();
    ~DX12ResourceManager();

    // Initialize resource manager with device
    bool Initialize(
#if defined(_WIN32) && defined(_MSC_VER)
        ID3D12Device* device
#else
        void* device
#endif
    );

    // === BUFFER CREATION ===

    // Create vertex buffer (GPU-only memory, optimal performance)
    // Returns nullptr on failure
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12Resource* CreateVertexBuffer(
        const void* data,
        unsigned int sizeInBytes,
        ID3D12GraphicsCommandList* uploadCommandList = nullptr
    );
#else
    void* CreateVertexBuffer(
        const void* data,
        unsigned int sizeInBytes,
        void* uploadCommandList = nullptr
    );
#endif

    // Create constant buffer (CPU-writable, persistently mapped)
    // Returns nullptr on failure
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12Resource* CreateConstantBuffer(unsigned int sizeInBytes);
#else
    void* CreateConstantBuffer(unsigned int sizeInBytes);
#endif

    // === TEXTURE CREATION ===

    // Create 2D texture (render target, depth buffer, etc.)
    // Returns nullptr on failure
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12Resource* CreateTexture2D(
        unsigned int width,
        unsigned int height,
        DXGI_FORMAT format,
        D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
        D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_COMMON,
        const D3D12_CLEAR_VALUE* clearValue = nullptr
    );
#else
    void* CreateTexture2D(
        unsigned int width,
        unsigned int height,
        unsigned int format,
        unsigned int flags = 0,
        unsigned int initialState = 0,
        const void* clearValue = nullptr
    );
#endif

    // === DESCRIPTOR HEAP CREATION ===

    // Create descriptor heap (RTV, DSV, SRV, UAV, etc.)
    // Returns nullptr on failure
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12DescriptorHeap* CreateDescriptorHeap(
        D3D12_DESCRIPTOR_HEAP_TYPE type,
        unsigned int numDescriptors,
        bool shaderVisible = false
    );
#else
    void* CreateDescriptorHeap(
        unsigned int type,
        unsigned int numDescriptors,
        bool shaderVisible = false
    );
#endif

    // Get descriptor size for a given heap type
    unsigned int GetDescriptorSize(
#if defined(_WIN32) && defined(_MSC_VER)
        D3D12_DESCRIPTOR_HEAP_TYPE type
#else
        unsigned int type
#endif
    ) const;

    // === RESOURCE RELEASE ===

    // Release a resource (decrements ref count)
    void ReleaseResource(
#if defined(_WIN32) && defined(_MSC_VER)
        ID3D12Resource* resource
#else
        void* resource
#endif
    );

    // Release a descriptor heap (decrements ref count)
    void ReleaseDescriptorHeap(
#if defined(_WIN32) && defined(_MSC_VER)
        ID3D12DescriptorHeap* heap
#else
        void* heap
#endif
    );

    // Shutdown and release all resources
    void Shutdown();

private:
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12Device* m_device = nullptr;

    // Descriptor sizes (cached for performance)
    unsigned int m_descriptorSizeRTV = 0;
    unsigned int m_descriptorSizeDSV = 0;
    unsigned int m_descriptorSizeCBV_SRV_UAV = 0;
    unsigned int m_descriptorSizeSampler = 0;

    // Helper: Create upload heap for staging
    ID3D12Resource* CreateUploadBuffer(unsigned int sizeInBytes);

    // Helper: Copy data from upload to default heap
    void CopyBufferRegion(
        ID3D12Resource* dest,
        ID3D12Resource* src,
        unsigned int sizeInBytes,
        ID3D12GraphicsCommandList* commandList
    );
#else
    void* m_device = nullptr;
    unsigned int m_descriptorSizeRTV = 0;
    unsigned int m_descriptorSizeDSV = 0;
    unsigned int m_descriptorSizeCBV_SRV_UAV = 0;
    unsigned int m_descriptorSizeSampler = 0;
#endif
};

} // namespace Renderer
