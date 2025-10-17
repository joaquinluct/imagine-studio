#pragma once

#if defined(_WIN32) && defined(_MSC_VER)
struct IDXGIFactory4;
struct IDXGIAdapter1;
struct ID3D12Device;
struct ID3D12CommandQueue;
#endif

namespace Renderer {

class DX12Device {
public:
    DX12Device();
    ~DX12Device();

    bool Initialize();
    void Shutdown();
    
    // Returns true if a real D3D12 device was created
    bool HasNativeDevice() const;
    
    // Opaque pointer to native device (ID3D12Device*) when available
    void* NativeDevice() const;
    
    // Opaque pointer to DXGI Factory (IDXGIFactory4*) when available
    void* NativeFactory() const;
    
    // Opaque pointer to Command Queue (ID3D12CommandQueue*) when available
    void* NativeCommandQueue() const;

private:
    bool hasNativeDevice_ = false;
    
#if defined(_WIN32) && defined(_MSC_VER)
    IDXGIFactory4* m_factory = nullptr;
    IDXGIAdapter1* m_adapter = nullptr;
    ID3D12Device* m_device = nullptr;
    ID3D12CommandQueue* m_commandQueue = nullptr;
#else
    void* nativeDevice_ = nullptr;
#endif
};

} // namespace Renderer
