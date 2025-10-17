#include "CommandAllocator.h"
#include "CommandBuffer.h"
#include "core/Log.h"
#include "DX12Device.h"
#include "DX12Renderer.h"
#include "Fence.h"
#include "RenderTarget.h"

#if defined(_WIN32) && defined(_MSC_VER)
#include <d3d12.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <dxgi1_4.h>

// Link DirectX 12 and D3D compiler libraries
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
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
    
    // Create command allocator (one per frame, reused after GPU finishes)
    hr = d3dDevice->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(&m_commandAllocator)
    );
    
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create Command Allocator");
        return;
    }
    
    CORE_LOG_INFO("DX12Renderer: Command Allocator created");
    
    // Create command list (initially closed state)
    hr = d3dDevice->CreateCommandList(
        0, // GPU node (single GPU)
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        m_commandAllocator, // Associated allocator
        nullptr, // Initial pipeline state (none)
        IID_PPV_ARGS(&m_commandList)
    );
    
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create Command List");
        return;
    }
    
    // Command lists are created in recording state, close it immediately
    m_commandList->Close();
    CORE_LOG_INFO("DX12Renderer: Command List created (closed)");
    
    // Create fence for GPU/CPU synchronization
    m_fenceValue = 1;
    hr = d3dDevice->CreateFence(
        0, // Initial value
        D3D12_FENCE_FLAG_NONE,
        IID_PPV_ARGS(&m_fence)
    );
    
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create Fence");
        return;
    }
    
    CORE_LOG_INFO("DX12Renderer: Fence created");
    
    // Create event for fence signaling
    m_fenceEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
    if (m_fenceEvent == nullptr)
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create Fence event");
        return;
    }
    
    CORE_LOG_INFO("DX12Renderer: Fence event created");
    
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
    
    // Create root signature with root constants for MVP matrix (16 floats = 64 bytes)
    D3D12_ROOT_PARAMETER rootParam = {};
    rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
    rootParam.Constants.ShaderRegister = 0; // register(b0) in HLSL
    rootParam.Constants.RegisterSpace = 0;
    rootParam.Constants.Num32BitValues = 16; // 4x4 matrix = 16 floats
    rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // Only visible to vertex shader
    
    D3D12_ROOT_SIGNATURE_DESC rootSigDesc = {};
    rootSigDesc.NumParameters = 1;
    rootSigDesc.pParameters = &rootParam;
    rootSigDesc.NumStaticSamplers = 0;
    rootSigDesc.pStaticSamplers = nullptr;
    rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    
    // Serialize the root signature
    ID3DBlob* signature = nullptr;
    ID3DBlob* error = nullptr;
    hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
    
    if (FAILED(hr))
    {
        if (error)
        {
            CORE_LOG_ERROR("DX12Renderer: Failed to serialize root signature: " + 
                std::string(static_cast<const char*>(error->GetBufferPointer())));
            error->Release();
        }
        else
        {
            CORE_LOG_ERROR("DX12Renderer: Failed to serialize root signature");
        }
        return;
    }
    
    // Create the root signature
    hr = d3dDevice->CreateRootSignature(
        0, // GPU node mask (single GPU)
        signature->GetBufferPointer(),
        signature->GetBufferSize(),
        IID_PPV_ARGS(&m_rootSignature)
    );
    
    signature->Release();
    
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create root signature");
        return;
    }
    
    CORE_LOG_INFO("DX12Renderer: Root Signature created (16 root constants for MVP matrix)");
    
    // Compile vertex shader from HLSL file
    ID3DBlob* vsError = nullptr;
    hr = D3DCompileFromFile(
        L"shaders/quad.hlsl",           // Shader file path
        nullptr,                         // No defines
        nullptr,                         // No include handler
        "VSMain",                        // Entry point function name
        "vs_5_0",                        // Shader target (Shader Model 5.0)
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // Debug flags
        0,                               // No effect flags
        &m_vertexShaderBlob,            // Output bytecode
        &vsError                         // Error messages
    );
    
    if (FAILED(hr))
    {
        if (vsError)
        {
            CORE_LOG_ERROR("DX12Renderer: Failed to compile vertex shader: " + 
                std::string(static_cast<const char*>(vsError->GetBufferPointer())));
            vsError->Release();
        }
        else
        {
            CORE_LOG_ERROR("DX12Renderer: Failed to compile vertex shader (file not found or compilation error)");
        }
        return;
    }
    
    CORE_LOG_INFO("DX12Renderer: Vertex shader compiled successfully");
    
    // Compile pixel shader from HLSL file
    ID3DBlob* psError = nullptr;
    hr = D3DCompileFromFile(
        L"shaders/quad.hlsl",           // Same shader file (contains both VS and PS)
        nullptr,                         // No defines
        nullptr,                         // No include handler
        "PSMain",                        // Entry point function name
        "ps_5_0",                        // Shader target (Shader Model 5.0)
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // Debug flags
        0,                               // No effect flags
        &m_pixelShaderBlob,             // Output bytecode
        &psError                         // Error messages
    );
    
    if (FAILED(hr))
    {
        if (psError)
        {
            CORE_LOG_ERROR("DX12Renderer: Failed to compile pixel shader: " + 
                std::string(static_cast<const char*>(psError->GetBufferPointer())));
            psError->Release();
        }
        else
        {
            CORE_LOG_ERROR("DX12Renderer: Failed to compile pixel shader (file not found or compilation error)");
        }
        return;
    }
    
    CORE_LOG_INFO("DX12Renderer: Pixel shader compiled successfully");
    
    // Define input layout (vertex structure description)
    D3D12_INPUT_ELEMENT_DESC inputElements[] = {
        {
            "POSITION",                              // Semantic name
            0,                                        // Semantic index
            DXGI_FORMAT_R32G32B32_FLOAT,             // Format (3 floats for position)
            0,                                        // Input slot
            0,                                        // Aligned byte offset (0 = start of struct)
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, // Input slot class
            0                                         // Instance data step rate
        },
        {
            "COLOR",                                  // Semantic name
            0,                                        // Semantic index
            DXGI_FORMAT_R32G32B32A32_FLOAT,          // Format (4 floats for color)
            0,                                        // Input slot (same as position)
            12,                                       // Aligned byte offset (after 3 floats = 12 bytes)
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, // Input slot class
            0                                         // Instance data step rate
        }
    };
    
    // Define rasterizer state (default with backface culling)
    D3D12_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
    rasterizerDesc.FrontCounterClockwise = FALSE;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.MultisampleEnable = FALSE;
    rasterizerDesc.AntialiasedLineEnable = FALSE;
    rasterizerDesc.ForcedSampleCount = 0;
    rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
    
    // Define blend state (default with no blending)
    D3D12_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.IndependentBlendEnable = FALSE;
    blendDesc.RenderTarget[0].BlendEnable = FALSE;
    blendDesc.RenderTarget[0].LogicOpEnable = FALSE;
    blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    
    // Create Graphics Pipeline State Object (PSO)
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElements, 2 }; // 2 input elements (POSITION + COLOR)
    psoDesc.pRootSignature = m_rootSignature;
    psoDesc.VS = { m_vertexShaderBlob->GetBufferPointer(), m_vertexShaderBlob->GetBufferSize() };
    psoDesc.PS = { m_pixelShaderBlob->GetBufferPointer(), m_pixelShaderBlob->GetBufferSize() };
    psoDesc.RasterizerState = rasterizerDesc;
    psoDesc.BlendState = blendDesc;
    psoDesc.DepthStencilState.DepthEnable = FALSE; // No depth testing for 2D quad
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // Match swap chain format
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;
    
    hr = d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
    
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create Pipeline State Object");
        return;
    }
    
    CORE_LOG_INFO("DX12Renderer: Pipeline State Object created successfully");
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
    // Release pipeline state object
    if (m_pipelineState)
    {
        m_pipelineState->Release();
        m_pipelineState = nullptr;
        CORE_LOG_INFO("DX12Renderer: Pipeline State Object released");
    }
    
    // Release compiled shaders
    if (m_pixelShaderBlob)
    {
        m_pixelShaderBlob->Release();
        m_pixelShaderBlob = nullptr;
        CORE_LOG_INFO("DX12Renderer: Pixel shader blob released");
    }
    
    if (m_vertexShaderBlob)
    {
        m_vertexShaderBlob->Release();
        m_vertexShaderBlob = nullptr;
        CORE_LOG_INFO("DX12Renderer: Vertex shader blob released");
    }
    
    // Release root signature
    if (m_rootSignature)
    {
        m_rootSignature->Release();
        m_rootSignature = nullptr;
        CORE_LOG_INFO("DX12Renderer: Root Signature released");
    }
    
    // Close fence event handle
    if (m_fenceEvent)
    {
        CloseHandle(m_fenceEvent);
        m_fenceEvent = nullptr;
        CORE_LOG_INFO("DX12Renderer: Fence event closed");
    }
    
    // Release fence
    if (m_fence)
    {
        m_fence->Release();
        m_fence = nullptr;
        CORE_LOG_INFO("DX12Renderer: Fence released");
    }
    
    // Release command list
    if (m_commandList)
    {
        m_commandList->Release();
        m_commandList = nullptr;
        CORE_LOG_INFO("DX12Renderer: Command List released");
    }
    
    // Release command allocator
    if (m_commandAllocator)
    {
        m_commandAllocator->Release();
        m_commandAllocator = nullptr;
        CORE_LOG_INFO("DX12Renderer: Command Allocator released");
    }
    
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
