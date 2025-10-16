#include "DX12Device.h"

#include <iostream>
#if defined(_WIN32) && defined(_MSC_VER)
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#endif

namespace Renderer {

DX12Device::DX12Device()
{
}

DX12Device::~DX12Device()
{
    Shutdown();
}

bool DX12Device::Initialize()
{
#if defined(_WIN32) && defined(_MSC_VER)
    // Try to create a D3D12 device. If any step fails, fall back to stub.
    IDXGIFactory4* factory = nullptr;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
    {
        std::cout << "DX12Device: CreateDXGIFactory1 failed, falling back to stub\n";
        return true;
    }

    IDXGIAdapter1* adapter = nullptr;
    for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
    {
        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);
        // Skip software adapters
        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        {
            adapter->Release();
            adapter = nullptr;
            continue;
        }

        ID3D12Device* d3dDevice = nullptr;
        if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3dDevice))))
        {
            // Store the device pointer as opaque and mark native device available
            nativeDevice_ = reinterpret_cast<void*>(d3dDevice);
            hasNativeDevice_ = true;
            adapter->Release();
            factory->Release();
            std::cout << "DX12Device: D3D12 device created\n";
            return true;
        }

        if (d3dDevice) d3dDevice->Release();
        adapter->Release();
        adapter = nullptr;
    }

    if (factory) factory->Release();
    std::cout << "DX12Device: No suitable D3D12 adapter found, falling back to stub\n";
    hasNativeDevice_ = false;
    nativeDevice_ = nullptr;
    return true;
#else
    std::cout << "DX12Device: Initialize (stub)\n";
    hasNativeDevice_ = false;
    nativeDevice_ = nullptr;
    return true;
#endif
}

void DX12Device::Shutdown()
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (hasNativeDevice_ && nativeDevice_)
    {
        ID3D12Device* d3d = reinterpret_cast<ID3D12Device*>(nativeDevice_);
        d3d->Release();
        nativeDevice_ = nullptr;
        hasNativeDevice_ = false;
    }
#endif
    std::cout << "DX12Device: Shutdown (stub)\n";
}

bool DX12Device::HasNativeDevice() const { return hasNativeDevice_; }
void* DX12Device::NativeDevice() const { return nativeDevice_; }

} // namespace Renderer
