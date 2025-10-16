#include "RenderTarget.h"

#if defined(_WIN32) && defined(_MSC_VER)
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#endif

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

    // Create a fence for CPU/GPU synchronization
    ID3D12Fence* fence = nullptr;
    if (FAILED(d3d->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence))))
    {
        std::cout << "RenderTarget: CreateForWindow - CreateFence failed, proceeding without native fence\n";
        fenceNative_ = nullptr;
        fenceEvent_ = nullptr;
    }
    else
    {
        fenceNative_ = reinterpret_cast<void*>(fence);
        fenceValue_ = 0;
        // Create an event for waiting on the fence
        HANDLE ev = CreateEvent(NULL, FALSE, FALSE, NULL);
        fenceEvent_ = reinterpret_cast<void*>(ev);
    }

    factory->Release();

    std::cout << "RenderTarget: CreateForWindow - swapchain, command queue and optional fence created\n";
    return true;
#else
    return Create(width, height);
#endif
}

void RenderTarget::Destroy()
{
    std::cout << "RenderTarget: Destroy (stub)\n";
#if defined(_WIN32) && defined(_MSC_VER)
    if (swapchain_)
    {
        IDXGISwapChain* sc = reinterpret_cast<IDXGISwapChain*>(swapchain_);
        sc->Release();
        swapchain_ = nullptr;
    }
    if (commandQueue_)
    {
        ID3D12CommandQueue* cq = reinterpret_cast<ID3D12CommandQueue*>(commandQueue_);
        cq->Release();
        commandQueue_ = nullptr;
    }
#endif
}

void RenderTarget::Present()
{
    std::cout << "RenderTarget: Present\n";
#if defined(_WIN32) && defined(_MSC_VER)
    if (swapchain_)
    {
        IDXGISwapChain* sc = reinterpret_cast<IDXGISwapChain*>(swapchain_);
        // Present with 1 sync interval, no flags
        sc->Present(1, 0);

        // If we have a native fence and command queue, increment and signal
        if (commandQueue_ && fenceNative_)
        {
            ID3D12CommandQueue* cq = reinterpret_cast<ID3D12CommandQueue*>(commandQueue_);
            ID3D12Fence* fence = reinterpret_cast<ID3D12Fence*>(fenceNative_);
            fenceValue_++;
            cq->Signal(fence, fenceValue_);
            // Wait for the fence on CPU with event (brief wait)
            if (fenceEvent_)
            {
                fence->SetEventOnCompletion(fenceValue_, reinterpret_cast<HANDLE>(fenceEvent_));
                WaitForSingleObject(reinterpret_cast<HANDLE>(fenceEvent_), 1); // short wait
            }
        }
        return;
    }
#endif
    std::cout << "RenderTarget: Present (stub fallback)\n";
}

} // namespace Renderer
