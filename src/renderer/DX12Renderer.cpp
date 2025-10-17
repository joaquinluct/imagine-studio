#include "CommandAllocator.h"
#include "CommandBuffer.h"
#include "core/Log.h"
#include "DX12Device.h"
#include "DX12Renderer.h"
#include "Fence.h"
#include "RenderTarget.h"

#if defined(_WIN32) && defined(_MSC_VER)
#include <d3d12.h>
#include <dxgi1_4.h>
#endif

#include <iostream>

namespace Renderer {

DX12Renderer::DX12Renderer() : device_(nullptr), rt_(nullptr) {}
DX12Renderer::~DX12Renderer() { Shutdown(); }

void DX12Renderer::Initialize()
{
    std::cout << "DX12Renderer: Initialize (stub)\n";
    device_ = new DX12Device();
    device_->Initialize();
    rt_ = new RenderTarget();
    if (device_->HasNativeDevice())
    {
        // No HWND provided: create fallback sized RT
        rt_->Create(800, 600);
    }
    else
    {
        rt_->Create(800, 600);
    }
    allocator_ = new CommandAllocator();
    fence_ = new Fence();
}

void DX12Renderer::Initialize(HWND hwnd)
{
    CORE_LOG_INFO("DX12Renderer: Initialize(HWND)");
    
    device_ = new DX12Device();
    device_->Initialize();
    
    if (!device_->HasNativeDevice())
    {
        CORE_LOG_WARN("DX12Renderer: No native device, using stub render target");
        rt_ = new RenderTarget();
        rt_->Create(800, 600);
        allocator_ = new CommandAllocator();
        fence_ = new Fence();
        return;
    }
    
#if defined(_WIN32) && defined(_MSC_VER)
    // Get device, factory and command queue from DX12Device
    ID3D12Device* d3dDevice = static_cast<ID3D12Device*>(device_->NativeDevice());
    IDXGIFactory4* factory = static_cast<IDXGIFactory4*>(device_->NativeFactory());
    ID3D12CommandQueue* commandQueue = static_cast<ID3D12CommandQueue*>(device_->NativeCommandQueue());
    
    if (!d3dDevice || !factory || !commandQueue)
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to get Device, Factory or CommandQueue");
        return;
    }
    
    // Create descriptor heap for render target views (RTVs)
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = BACK_BUFFER_COUNT;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    
    HRESULT hr = d3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create RTV descriptor heap");
        return;
    }
    
    m_rtvDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    CORE_LOG_INFO("DX12Renderer: RTV descriptor heap created");
    
    // Create descriptor heap for CBV/SRV/UAV (Constant Buffers, Shader Resources, Unordered Access)
    D3D12_DESCRIPTOR_HEAP_DESC cbvSrvUavHeapDesc = {};
    cbvSrvUavHeapDesc.NumDescriptors = 1; // Start with 1 descriptor for constant buffer
    cbvSrvUavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvSrvUavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // Must be shader visible
    
    hr = d3dDevice->CreateDescriptorHeap(&cbvSrvUavHeapDesc, IID_PPV_ARGS(&m_cbvSrvUavHeap));
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create CBV/SRV/UAV descriptor heap");
        return;
    }
    
    m_cbvSrvUavDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    CORE_LOG_INFO("DX12Renderer: CBV/SRV/UAV descriptor heap created (shader visible)");
    
    // Create swap chain
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = 800;  // TODO: Get from window
    swapChainDesc.Height = 600; // TODO: Get from window
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferCount = BACK_BUFFER_COUNT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    
    IDXGISwapChain1* swapChain1 = nullptr;
    hr = factory->CreateSwapChainForHwnd(
        commandQueue,
        hwnd,
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain1
    );
    
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create SwapChain");
        return;
    }
    
    // Query for IDXGISwapChain3 interface
    hr = swapChain1->QueryInterface(IID_PPV_ARGS(&m_swapChain));
    swapChain1->Release();
    
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to get IDXGISwapChain3 interface");
        return;
    }
    
    CORE_LOG_INFO("DX12Renderer: SwapChain created successfully");
    
    // Get current back buffer index
    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
    
    // Create render target views (RTVs) for each back buffer
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
    
    for (UINT i = 0; i < BACK_BUFFER_COUNT; ++i)
    {
        hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
        if (FAILED(hr))
        {
            CORE_LOG_ERROR("DX12Renderer: Failed to get back buffer " + std::to_string(i));
            return;
        }
        
        d3dDevice->CreateRenderTargetView(m_renderTargets[i], nullptr, rtvHandle);
        rtvHandle.ptr += m_rtvDescriptorSize;
    }
    
    CORE_LOG_INFO("DX12Renderer: Created " + std::to_string(BACK_BUFFER_COUNT) + " render target views");
#endif
    
    allocator_ = new CommandAllocator();
    fence_ = new Fence();
}

// Rest of the implementation...

void DX12Renderer::RenderFrame()
{
    // Stub: call ComposeUI for composition and present the render target
    // Simulate recording commands
    Renderer::CommandBuffer cmd;
    cmd.Initialize(1024);
    // ... allocate and record
    unsigned long long submitIndex = cmd.Submit();

    // signal fence (stub). Do NOT wait here on the same thread — the
    // stubbed Fence implementation would deadlock because Signal and Wait
    // are on the same thread. In real GPU code the GPU would signal the
    // fence and the CPU could wait asynchronously or poll for completion.
    if (fence_) fence_->Signal();
    else
        (void)0;

    ComposeUI();
    if (rt_) rt_->Present();
}

bool DX12Renderer::ComposeUI()
{
    // Stub: prepare composition target for UI
    std::cout << "DX12Renderer: ComposeUI (stub)\n";
    return true;
}

void DX12Renderer::OnAssetLoaded(const std::string& path)
{
    std::cout << "DX12Renderer: Asset loaded: " << path << "\n";
}

void DX12Renderer::Shutdown()
{
#if defined(_WIN32) && defined(_MSC_VER)
    // Release CBV/SRV/UAV descriptor heap
    if (m_cbvSrvUavHeap)
    {
        m_cbvSrvUavHeap->Release();
        m_cbvSrvUavHeap = nullptr;
        CORE_LOG_INFO("DX12Renderer: CBV/SRV/UAV descriptor heap released");
    }
    
    // Release render targets
    for (UINT i = 0; i < BACK_BUFFER_COUNT; ++i)
    {
        if (m_renderTargets[i])
        {
            m_renderTargets[i]->Release();
            m_renderTargets[i] = nullptr;
        }
    }
    
    // Release RTV descriptor heap
    if (m_rtvHeap)
    {
        m_rtvHeap->Release();
        m_rtvHeap = nullptr;
        CORE_LOG_INFO("DX12Renderer: RTV descriptor heap released");
    }
    
    // Release swap chain
    if (m_swapChain)
    {
        m_swapChain->Release();
        m_swapChain = nullptr;
        CORE_LOG_INFO("DX12Renderer: SwapChain released");
    }
#endif
    
    if (rt_) { rt_->Destroy(); delete rt_; rt_ = nullptr; }
    if (device_) { device_->Shutdown(); delete device_; device_ = nullptr; }
}

} // namespace Renderer
