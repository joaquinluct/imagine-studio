#pragma once

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
private:
    bool hasNativeDevice_ = false;
    void* nativeDevice_ = nullptr;
};

} // namespace Renderer
