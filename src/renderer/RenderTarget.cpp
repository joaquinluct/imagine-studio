#include "RenderTarget.h"

#include <iostream>
namespace Renderer {

// Simple stub implementations

bool RenderTarget::Create(int width, int height)
{
    (void)width; (void)height;
    std::cout << "RenderTarget: Create (stub)\n";
    return true;
}

bool RenderTarget::CreateForWindow(void* nativeDevice, void* hwnd, int width, int height)
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!nativeDevice || !hwnd)
        return Create(width, height);

    ID3D12Device* d3d = reinterpret_cast<ID3D12Device*>(nativeDevice);

    // Create a simple command queue
    D3D12_COMMAND_QUEUE_DESC qdesc = {};
    qdesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    qdesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    ID3D12CommandQueue* cq = nullptr;
    if (FAILED(d3d->CreateCommandQueue(&qdesc, IID_PPV_ARGS(&cq))))
    {
        std::cout << "RenderTarget: CreateCommandQueue failed, falling back to stub\n";
        return Create(width, height);
    }

    // Create swapchain
    IDXGIFactory4* factory = nullptr;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
    {
        std::cout << "RenderTarget: CreateDXGIFactory1 failed, falling back to stub\n";
        cq->Release();
        return Create(width, height);
    }

    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 2;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.OutputWindow = reinterpret_cast<HWND>(hwnd);
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;

    IDXGISwapChain* sc = nullptr;
    HRESULT hr = factory->CreateSwapChain(cq, &sd, &sc);
    if (FAILED(hr))
    {
        std::cout << "RenderTarget: CreateSwapChain failed, falling back to stub\n";
        factory->Release();
        cq->Release();
        return Create(width, height);
    }

    swapchain_ = reinterpret_cast<void*>(sc);
    commandQueue_ = reinterpret_cast<void*>(cq);
    factory->Release();

    std::cout << "RenderTarget: CreateForWindow - swapchain and command queue created\n";
    return true;
#else
    return Create(width, height);
#endif
}

void RenderTarget::Destroy()
{
    std::cout << "RenderTarget: Destroy (stub)\n";
}

void RenderTarget::Present()
{
    std::cout << "RenderTarget: Present (stub)\n";
}

} // namespace Renderer
