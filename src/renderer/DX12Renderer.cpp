#include "CommandAllocator.h"
#include "CommandBuffer.h"
#include "core/Log.h"
#include "DX12Device.h"
#include "DX12Renderer.h"
#include "Fence.h"
#include "RenderTarget.h"

// Camera system (v1.5.0 - H2.2)
#include "Camera.h"

// ImGui headers (Dear ImGui - v1.3.0 H2.3)
#include "imgui.h"
#include "imgui_impl_dx12.h"

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

#if defined(_WIN32) && defined(_MSC_VER)
ID3D12Device* DX12Renderer::GetDevice() const
{
    return device_ ? static_cast<ID3D12Device*>(device_->NativeDevice()) : nullptr;
}
#endif

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
    
    // Create descriptor heap for ImGui (SRV for font atlas - v1.3.0, expanded v1.5.0 H1.1)
    // Slot 0: ImGui font atlas SRV
    // Slot 1: Render target SRV (for viewport texture - v1.5.0)
    D3D12_DESCRIPTOR_HEAP_DESC imguiSrvHeapDesc = {};
    imguiSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    imguiSrvHeapDesc.NumDescriptors = 2;  // Expanded from 1 to 2 (font atlas + render target SRV - v1.5.0 H1.1)
    imguiSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    hr = d3dDevice->CreateDescriptorHeap(&imguiSrvHeapDesc, IID_PPV_ARGS(&m_imguiSrvHeap));
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("Failed to create ImGui SRV descriptor heap");
        return;
    }
    
    // Store descriptor size for SRV heap (v1.5.0 H1.1)
    m_imguiSrvDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    CORE_LOG_INFO("ImGui SRV descriptor heap created (2 descriptors: font atlas + render target SRV)");
    
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
    swapChainDesc.Width = 1920;  // Full HD width
    swapChainDesc.Height = 1080; // Full HD height
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
    
    // Define rasterizer state (backface culling enabled for optimization)
    D3D12_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK; // Backface culling enabled
    rasterizerDesc.FrontCounterClockwise = FALSE; // Clockwise = front face
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
    
    // Create vertex buffer with 6 vertices (2 triangles for a quad)
    struct Vertex {
        float pos[3];  // Position (x, y, z)
        float col[4];  // Color (r, g, b, a)
    };
    
    // Define quad vertices (2 triangles, CLOCKWISE winding for front faces)
    // Triangle 1: bottom-left, top-left, bottom-right (clockwise)
    // Triangle 2: bottom-right, top-left, top-right (clockwise)
    // Using NDC coordinates: -1.0 to 1.0 (full screen quad)
    Vertex vertices[] = {
        // Triangle 1 (clockwise winding)
        {{-0.75f, -0.75f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // Bottom-left, red
        {{-0.75f,  0.75f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // Top-left, blue
        {{ 0.75f, -0.75f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // Bottom-right, green
        
        // Triangle 2 (clockwise winding)
        {{ 0.75f, -0.75f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // Bottom-right, green
        {{-0.75f,  0.75f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // Top-left, blue
        {{ 0.75f,  0.75f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, // Top-right, yellow
    };
    
    const UINT vertexBufferSize = sizeof(vertices);
    
    // Create vertex buffer in upload heap (staging for CPU to GPU transfer)
    D3D12_HEAP_PROPERTIES uploadHeapProps = {};
    uploadHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    uploadHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    uploadHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    uploadHeapProps.CreationNodeMask = 1;
    uploadHeapProps.VisibleNodeMask = 1;
    
    D3D12_RESOURCE_DESC uploadBufferDesc = {};
    uploadBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    uploadBufferDesc.Alignment = 0;
    uploadBufferDesc.Width = vertexBufferSize;
    uploadBufferDesc.Height = 1;
    uploadBufferDesc.DepthOrArraySize = 1;
    uploadBufferDesc.MipLevels = 1;
    uploadBufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    uploadBufferDesc.SampleDesc.Count = 1;
    uploadBufferDesc.SampleDesc.Quality = 0;
    uploadBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    uploadBufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    
    hr = d3dDevice->CreateCommittedResource(
        &uploadHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &uploadBufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, // Upload heap must be in GENERIC_READ state
        nullptr,
        IID_PPV_ARGS(&m_vertexBufferUpload)
    );
    
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create vertex buffer upload heap");
        return;
    }
    
    CORE_LOG_INFO("DX12Renderer: Vertex buffer upload heap created");
    
    // Copy vertex data to upload heap
    void* pVertexDataBegin = nullptr;
    D3D12_RANGE readRange = { 0, 0 }; // We do not intend to read from this resource on the CPU
    hr = m_vertexBufferUpload->Map(0, &readRange, &pVertexDataBegin);
    
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to map vertex buffer upload heap");
        return;
    }
    
    memcpy(pVertexDataBegin, vertices, vertexBufferSize);
    m_vertexBufferUpload->Unmap(0, nullptr);
    
    CORE_LOG_INFO("DX12Renderer: Vertex data copied to upload heap");
    
    // Create vertex buffer in default heap (GPU-only memory, optimal performance)
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
    
    hr = d3dDevice->CreateCommittedResource(
        &defaultHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &vertexBufferDesc,
        D3D12_RESOURCE_STATE_COPY_DEST, // Start in copy destination state
        nullptr,
        IID_PPV_ARGS(&m_vertexBuffer)
    );
    
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create vertex buffer (default heap)");
        return;
    }
    
    CORE_LOG_INFO("DX12Renderer: Vertex buffer (default heap) created");
    
    // Record copy command from upload heap to default heap
    // Reset command allocator and command list for initial setup
    m_commandAllocator->Reset();
    m_commandList->Reset(m_commandAllocator, nullptr);
    
    // Copy buffer region from upload to default heap
    m_commandList->CopyBufferRegion(m_vertexBuffer, 0, m_vertexBufferUpload, 0, vertexBufferSize);
    
    // Transition vertex buffer from COPY_DEST to VERTEX_AND_CONSTANT_BUFFER state
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_vertexBuffer;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    
    m_commandList->ResourceBarrier(1, &barrier);
    
    // Close command list
    m_commandList->Close();
    
    // Execute copy command
    ID3D12CommandList* ppCommandLists[] = { m_commandList };
    commandQueue->ExecuteCommandLists(1, ppCommandLists);
    
    // Wait for copy to complete (synchronous initialization)
    const UINT64 fenceValueForCopy = m_fenceValue;
    hr = commandQueue->Signal(m_fence, fenceValueForCopy);
    m_fenceValue++;
    
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to signal fence for vertex buffer copy");
        return;
    }
    
    // Wait until the fence value is reached (GPU finished copying)
    if (m_fence->GetCompletedValue() < fenceValueForCopy)
    {
        hr = m_fence->SetEventOnCompletion(fenceValueForCopy, m_fenceEvent);
        if (FAILED(hr))
        {
            CORE_LOG_ERROR("DX12Renderer: Failed to set event on completion for vertex buffer copy");
            return;
        }
        WaitForSingleObject(m_fenceEvent, INFINITE);
    }
    
    CORE_LOG_INFO("DX12Renderer: Vertex buffer copy completed");
    
    // Configure vertex buffer view for pipeline binding
    m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = sizeof(Vertex); // 28 bytes (3 floats + 4 floats)
    m_vertexBufferView.SizeInBytes = vertexBufferSize;  // 6 * 28 = 168 bytes
    
    CORE_LOG_INFO("DX12Renderer: Vertex buffer view configured (6 vertices, stride 28 bytes)");
    
    // Create constant buffer for MVP matrix (identity matrix for "Hola Mundo")
    // Initialize MVP matrix to identity
    m_mvpMatrix[0] = 1.0f;  m_mvpMatrix[1] = 0.0f;  m_mvpMatrix[2] = 0.0f;  m_mvpMatrix[3] = 0.0f;
    m_mvpMatrix[4] = 0.0f;  m_mvpMatrix[5] = 1.0f;  m_mvpMatrix[6] = 0.0f;  m_mvpMatrix[7] = 0.0f;
    m_mvpMatrix[8] = 0.0f;  m_mvpMatrix[9] = 0.0f;  m_mvpMatrix[10] = 1.0f; m_mvpMatrix[11] = 0.0f;
    m_mvpMatrix[12] = 0.0f; m_mvpMatrix[13] = 0.0f; m_mvpMatrix[14] = 0.0f; m_mvpMatrix[15] = 1.0f;
    
    // Constant buffers must be 256-byte aligned
    const UINT constantBufferSize = (sizeof(m_mvpMatrix) + 255) & ~255; // Align to 256 bytes
    
    // Create constant buffer in upload heap (CPU writable, persistently mapped)
    D3D12_HEAP_PROPERTIES cbUploadHeapProps = {};
    cbUploadHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    cbUploadHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    cbUploadHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    cbUploadHeapProps.CreationNodeMask = 1;
    cbUploadHeapProps.VisibleNodeMask = 1;
    
    D3D12_RESOURCE_DESC cbDesc = {};
    cbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    cbDesc.Alignment = 0;
    cbDesc.Width = constantBufferSize;
    cbDesc.Height = 1;
    cbDesc.DepthOrArraySize = 1;
    cbDesc.MipLevels = 1;
    cbDesc.Format = DXGI_FORMAT_UNKNOWN;
    cbDesc.SampleDesc.Count = 1;
    cbDesc.SampleDesc.Quality = 0;
    cbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    cbDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    
    hr = d3dDevice->CreateCommittedResource(
        &cbUploadHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &cbDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, // Upload heap must be in GENERIC_READ state
        nullptr,
        IID_PPV_ARGS(&m_constantBuffer)
    );
    
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create constant buffer");
        return;
    }
    
    CORE_LOG_INFO("DX12Renderer: Constant buffer created (256-byte aligned)");
    
    // Map constant buffer persistently (will remain mapped for updates)
    D3D12_RANGE cbReadRange = { 0, 0 }; // We do not intend to read from this resource on the CPU
    hr = m_constantBuffer->Map(0, &cbReadRange, &m_cbMappedData);
    
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to map constant buffer");
        return;
    }
    
    // Copy initial MVP matrix data to constant buffer
    memcpy(m_cbMappedData, m_mvpMatrix, sizeof(m_mvpMatrix));
    
    CORE_LOG_INFO("DX12Renderer: Constant buffer mapped and initialized with identity matrix");
    
    // v1.5.0 H1.1 - Create Render Target SRV for viewport texture
    CreateRenderTargetSRV();
    
    // v1.5.0 H2.2 - Initialize Camera
    m_camera = new Camera();
    // Configure camera for default scene view
    m_camera->SetPosition(0.0f, 2.0f, -5.0f);  // Slightly above and back
    m_camera->SetTarget(0.0f, 0.0f, 0.0f);     // Looking at origin
    m_camera->SetPerspective(45.0f, 1920.0f / 1080.0f, 0.1f, 1000.0f);  // Match viewport aspect ratio
    CORE_LOG_INFO("DX12Renderer: Camera initialized at position (0, 2, -5)");
#endif // defined(_WIN32) && defined(_MSC_VER) - END OF Initialize(HWND) DX12 CODE BLOCK
    
    allocator_ = new CommandAllocator();
    fence_ = new Fence();
}

#if defined(_WIN32) && defined(_MSC_VER)
// v1.5.0 H1.1 - Create SRV descriptor for render target (viewport texture)
// v1.5.0 H1.2 - Updated to regenerate SRV for current back buffer (m_frameIndex)
// v1.5.0 H1.4 - Added validation and logging for SRV readiness
void DX12Renderer::CreateRenderTargetSRV()
{
    if (!device_ || !device_->HasNativeDevice())
    {
        CORE_LOG_WARN("DX12Renderer::CreateRenderTargetSRV: No native device available");
        return;
    }
    
    ID3D12Device* d3dDevice = static_cast<ID3D12Device*>(device_->NativeDevice());
    
    // v1.5.0 H1.4 - Validate render target before creating SRV
    if (!m_renderTargets[m_frameIndex])
    {
        CORE_LOG_ERROR("DX12Renderer::CreateRenderTargetSRV: Render target at frame index " + 
            std::to_string(m_frameIndex) + " is null");
        return;
    }
    
    // v1.5.0 H1.4 - Validate ImGui SRV heap
    if (!m_imguiSrvHeap)
    {
        CORE_LOG_ERROR("DX12Renderer::CreateRenderTargetSRV: ImGui SRV heap is null");
        return;
    }
    
    // Get SRV handle for render target (slot 1 in ImGui SRV heap)
    // Slot 0 is reserved for ImGui font atlas
    m_renderTargetSRV_CPU = m_imguiSrvHeap->GetCPUDescriptorHandleForHeapStart();
    m_renderTargetSRV_CPU.ptr += m_imguiSrvDescriptorSize; // Offset to slot 1
    
    m_renderTargetSRV_GPU = m_imguiSrvHeap->GetGPUDescriptorHandleForHeapStart();
    m_renderTargetSRV_GPU.ptr += m_imguiSrvDescriptorSize; // Offset to slot 1
    
    // Create SRV descriptor for CURRENT render target (back buffer at m_frameIndex)
    // This will be called after Present() when m_frameIndex changes (H1.2)
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Match swap chain format
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    
    // Create SRV for current back buffer (m_frameIndex may have changed after Present)
    d3dDevice->CreateShaderResourceView(
        m_renderTargets[m_frameIndex], // Use CURRENT frame index
        &srvDesc,
        m_renderTargetSRV_CPU
    );
    
    // v1.5.0 H1.4 - Log SRV validation details
    CORE_LOG_INFO("DX12Renderer: Render Target SRV updated for frame index " + std::to_string(m_frameIndex));
    CORE_LOG_INFO("DX12Renderer: SRV GPU handle ptr: " + std::to_string(m_renderTargetSRV_GPU.ptr));
    CORE_LOG_INFO("DX12Renderer: SRV format: DXGI_FORMAT_R8G8B8A8_UNORM (28)");
    CORE_LOG_INFO("DX12Renderer: SRV ready for ImGui::Image() usage in H3.1");
}

// v1.5.0 H1.3 - Helper method for resource state transitions
// Simplifies barrier creation and ensures correct transition handling
void DX12Renderer::TransitionResource(
    ID3D12Resource* resource,
    D3D12_RESOURCE_STATES stateBefore,
    D3D12_RESOURCE_STATES stateAfter
)
{
    if (!resource || !m_commandList)
    {
        CORE_LOG_ERROR("DX12Renderer::TransitionResource: Invalid resource or command list");
        return;
    }
    
    // Skip transition if states are the same (optimization)
    if (stateBefore == stateAfter)
    {
        return;
    }
    
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = resource;
    barrier.Transition.StateBefore = stateBefore;
    barrier.Transition.StateAfter = stateAfter;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    
    m_commandList->ResourceBarrier(1, &barrier);
}

// v1.5.0 H2.2 - Helper for matrix multiplication (4x4 row-major)
namespace {
    // Multiply two 4x4 row-major matrices: result = a * b
    inline void MultiplyMatrix4x4(const float* a, const float* b, float* result)
    {
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                result[row * 4 + col] = 0.0f;
                for (int k = 0; k < 4; ++k)
                {
                    result[row * 4 + col] += a[row * 4 + k] * b[k * 4 + col];
                }
            }
        }
    }
    
    // Transpose 4x4 matrix (convert column-major to row-major or vice versa)
    inline void Transpose4x4(const float* src, float* dst)
    {
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                dst[row * 4 + col] = src[col * 4 + row];
            }
        }
    }
}
#endif

void DX12Renderer::RenderFrame()
{
    // Forward Rendering (v1.2.0)
    RenderForwardPass();
}

void DX12Renderer::RenderForwardPass()
{
    // Pass 1: Opaque
    OpaquePass();
    
    // Pass 2: UI (condicional - H2.3)
    if (m_uiVisible)
    {
        UIPass();
    }
    
#if defined(_WIN32) && defined(_MSC_VER)
    // Present frame once at the end of all passes
    if (device_ && device_->HasNativeDevice() && m_swapChain)
    {
        HRESULT hr = m_swapChain->Present(1, 0); // 1 = VSync on, 0 = no flags
        if (FAILED(hr))
        {
            CORE_LOG_ERROR("DX12Renderer::RenderForwardPass: Failed to present frame");
            return;
        }
        
        // Update frame index for next frame
        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
        
        // v1.5.0 H1.2 - Update SRV descriptor to point to new back buffer
        // This ensures GetRenderTargetSRV() always returns the correct texture
        CreateRenderTargetSRV();
    }
#endif
}

void DX12Renderer::OpaquePass()
{
#if defined(_WIN32) && defined(_MSC_VER)
    // Only render if we have native DX12 device
    if (!device_ || !device_->HasNativeDevice())
    {
        // Fallback to stub rendering
        Renderer::CommandBuffer cmd;
        cmd.Initialize(1024);
        unsigned long long submitIndex = cmd.Submit();
        if (fence_) fence_->Signal();
        ComposeUI();
        if (rt_) rt_->Present();
        return;
    }
    
    // Get command queue from device
    ID3D12CommandQueue* commandQueue = static_cast<ID3D12CommandQueue*>(device_->NativeCommandQueue());
    if (!commandQueue)
    {
        CORE_LOG_ERROR("DX12Renderer: No command queue available for rendering");
        return;
    }
    
    // Reset command allocator (reuse memory for new frame)
    HRESULT hr = m_commandAllocator->Reset();
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to reset command allocator");
        return;
    }
    
    // Reset command list with PSO (starts recording)
    hr = m_commandList->Reset(m_commandAllocator, m_pipelineState);
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to reset command list");
        return;
    }
    
    // Set root signature
    m_commandList->SetGraphicsRootSignature(m_rootSignature);
    
    // Set viewport (1920x1080 Full HD)
    D3D12_VIEWPORT viewport = {};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = 1920.0f;
    viewport.Height = 1080.0f;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    m_commandList->RSSetViewports(1, &viewport);
    
    // Set scissor rect (full viewport)
    D3D12_RECT scissorRect = {};
    scissorRect.left = 0;
    scissorRect.top = 0;
    scissorRect.right = 1920;
    scissorRect.bottom = 1080;
    m_commandList->RSSetScissorRects(1, &scissorRect);
    
    // v1.5.0 H1.3 - Transition render target: PRESENT -> RENDER_TARGET
    TransitionResource(
        m_renderTargets[m_frameIndex],
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET
    );
    
    // Get RTV handle for current back buffer
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += m_frameIndex * m_rtvDescriptorSize;
    
    // Set render target
    m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
    
    // Clear render target with dark blue color
    const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    
    // Set primitive topology
    m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    // Set vertex buffer
    m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
    
    // v1.5.0 H2.2 - Calculate MVP matrix: Model * View * Projection
    if (m_camera)
    {
        // Model matrix (identity for now - quad at origin, row-major)
        float modelMatrix[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,  // Row 0
            0.0f, 1.0f, 0.0f, 0.0f,  // Row 1
            0.0f, 0.0f, 1.0f, 0.0f,  // Row 2
            0.0f, 0.0f, 0.0f, 1.0f   // Row 3
        };
        
        // Get View and Projection from camera (row-major)
        const float* viewMatrix = m_camera->GetViewMatrix();
        const float* projectionMatrix = m_camera->GetProjectionMatrix();
        
        // Calculate ModelView = Model * View (row-major multiplication)
        float modelView[16];
        MultiplyMatrix4x4(modelMatrix, viewMatrix, modelView);
        
        // Calculate MVP = ModelView * Projection (row-major multiplication)
        MultiplyMatrix4x4(modelView, projectionMatrix, m_mvpMatrix);
    }
    
    // Set root constants (MVP matrix)
    m_commandList->SetGraphicsRoot32BitConstants(0, 16, m_mvpMatrix, 0);
    
    // DRAW CALL - 6 vertices (2 triangles forming a quad)
    m_commandList->DrawInstanced(6, 1, 0, 0);
    
    // v1.5.0 H3.2 - Transition render target: RENDER_TARGET -> PIXEL_SHADER_RESOURCE
    // This allows ImGui::Image() in Viewport to read the render target as a texture
    // NOTE: Previously this was RENDER_TARGET -> PRESENT, but now UIPass reads the RT
    TransitionResource(
        m_renderTargets[m_frameIndex],
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );
    
    // Close command list (finish recording)
    hr = m_commandList->Close();
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to close command list");
        return;
    }
    
    // Execute command list on command queue
    ID3D12CommandList* ppCommandLists[] = { m_commandList };
    commandQueue->ExecuteCommandLists(1, ppCommandLists);
    
    // Signal fence after GPU work submission
    const UINT64 fenceValueForThisFrame = m_fenceValue;
    hr = commandQueue->Signal(m_fence, fenceValueForThisFrame);
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to signal fence");
        return;
    }
    
    m_fenceValue++; // Increment for next frame
    
    // Wait for GPU to finish rendering (synchronous for now, optimize later with double buffering)
    if (m_fence->GetCompletedValue() < fenceValueForThisFrame)
    {
        // GPU hasn't reached this fence value yet, wait for it
        hr = m_fence->SetEventOnCompletion(fenceValueForThisFrame, m_fenceEvent);
        if (FAILED(hr))
        {
            CORE_LOG_ERROR("DX12Renderer: Failed to set event on fence completion");
            return;
        }
        
        WaitForSingleObject(m_fenceEvent, INFINITE);
    }
    
    // NOTE: Present() is called once at the end of RenderForwardPass(), not here
#else
    // Stub: call ComposeUI for composition and present the render target
    // Simulate recording commands
    Renderer::CommandBuffer cmd;
    cmd.Initialize(1024);
    unsigned long long submitIndex = cmd.Submit();
    if (fence_) fence_->Signal();
    ComposeUI();
    if (rt_) rt_->Present();
#endif
}

void DX12Renderer::UIPass()
{
    // Early exit if UI is not visible (controlled by F1 toggle)
    if (!m_uiVisible)
    {
        return;
    }
    
#if defined(_WIN32) && defined(_MSC_VER)
    // Only render if we have native DX12 device
    if (!device_ || !device_->HasNativeDevice())
    {
        return;
    }
    
    // ✅ RENDERIZAR IMGUI DRAW DATA (v1.3.0 - H2.3)
    ImDrawData* draw_data = ImGui::GetDrawData();
    if (draw_data == nullptr || draw_data->TotalVtxCount == 0)
    {
        return; // Nada que renderizar
    }
    
    // Get command queue from device
    ID3D12CommandQueue* commandQueue = static_cast<ID3D12CommandQueue*>(device_->NativeCommandQueue());
    if (!commandQueue)
    {
        CORE_LOG_ERROR("DX12Renderer::UIPass: No command queue available");
        return;
    }
    
    // Reset command allocator for UI pass
    HRESULT hr = m_commandAllocator->Reset();
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer::UIPass: Failed to reset command allocator");
        return;
    }
    
    // Reset command list WITHOUT PSO (ImGui backend will set its own PSO)
    hr = m_commandList->Reset(m_commandAllocator, nullptr);
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer::UIPass: Failed to reset command list");
        return;
    }
    
    // v1.5.0 H3.2 - Transition render target: PIXEL_SHADER_RESOURCE -> RENDER_TARGET
    // OpaquePass left the RT in PSR state so ImGui::Image() could read it
    TransitionResource(
        m_renderTargets[m_frameIndex],
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        D3D12_RESOURCE_STATE_RENDER_TARGET
    );
    
    // Get RTV handle for current back buffer
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += m_frameIndex * m_rtvDescriptorSize;
    
    // Set render target (no clear, render on top of opaque pass)
    m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
    
    // ✅ CRÍTICO: Configurar descriptor heap ANTES de RenderDrawData
    // ImGui necesita el SRV heap para acceder a las texturas (font atlas)
    m_commandList->SetDescriptorHeaps(1, &m_imguiSrvHeap);
    
    // ✅ LLAMAR AL BACKEND IMGUI PARA RENDERIZAR
    // El backend ImGui_ImplDX12 se encarga de configurar pipeline, vertex buffers, etc.
    ImGui_ImplDX12_RenderDrawData(draw_data, m_commandList);
    
    // v1.5.0 H1.3 - Transition render target: RENDER_TARGET -> PRESENT
    TransitionResource(
        m_renderTargets[m_frameIndex],
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT
    );
    
    // Close command list
    hr = m_commandList->Close();
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer::UIPass: Failed to close command list");
        return;
    }
    
    // Execute command list
    ID3D12CommandList* ppCommandLists[] = { m_commandList };
    commandQueue->ExecuteCommandLists(1, ppCommandLists);
    
    // Signal fence
    const UINT64 fenceValueForUIPass = m_fenceValue;
    hr = commandQueue->Signal(m_fence, fenceValueForUIPass);
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer::UIPass: Failed to signal fence");
        return;
    }
    
    m_fenceValue++;
    
    // Wait for GPU to finish (synchronous for now)
    if (m_fence->GetCompletedValue() < fenceValueForUIPass)
    {
        hr = m_fence->SetEventOnCompletion(fenceValueForUIPass, m_fenceEvent);
        if (FAILED(hr))
        {
            CORE_LOG_ERROR("DX12Renderer::UIPass: Failed to set event on fence completion");
            return;
        }
        
        WaitForSingleObject(m_fenceEvent, INFINITE);
    }
    
    // NOTE: Present() is called once at the end of RenderForwardPass(), not here
#endif
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
    // Wait for GPU to finish all pending work before releasing resources
    if (device_ && device_->HasNativeDevice() && m_fence && m_fenceEvent)
    {
        ID3D12CommandQueue* commandQueue = static_cast<ID3D12CommandQueue*>(device_->NativeCommandQueue());
        if (commandQueue)
        {
            // Signal fence with a high value
            const UINT64 fenceValueForShutdown = m_fenceValue;
            HRESULT hr = commandQueue->Signal(m_fence, fenceValueForShutdown);
            
            if (SUCCEEDED(hr))
            {
                // Wait for GPU to reach this fence value
                if (m_fence->GetCompletedValue() < fenceValueForShutdown)
                {
                    m_fence->SetEventOnCompletion(fenceValueForShutdown, m_fenceEvent);
                    WaitForSingleObject(m_fenceEvent, INFINITE);
                }
                
                CORE_LOG_INFO("DX12Renderer: GPU work completed, safe to release resources");
            }
        }
    }
    
    // Unmap and release constant buffer
    if (m_constantBuffer)
    {
        if (m_cbMappedData)
        {
            m_constantBuffer->Unmap(0, nullptr);
            m_cbMappedData = nullptr;
        }
        m_constantBuffer->Release();
        m_constantBuffer = nullptr;
        CORE_LOG_INFO("DX12Renderer: Constant buffer released");
    }
    
    // Release vertex buffer resources
    if (m_vertexBufferUpload)
    {
        m_vertexBufferUpload->Release();
        m_vertexBufferUpload = nullptr;
        CORE_LOG_INFO("DX12Renderer: Vertex buffer upload heap released");
    }
    
    if (m_vertexBuffer)
    {
        m_vertexBuffer->Release();
        m_vertexBuffer = nullptr;
        CORE_LOG_INFO("DX12Renderer: Vertex buffer released");
    }
    
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
    
    // Release ImGui SRV descriptor heap (v1.3.0)
    if (m_imguiSrvHeap)
    {
        m_imguiSrvHeap->Release();
        m_imguiSrvHeap = nullptr;
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
    
    // v1.5.0 H2.2 - Release Camera
    if (m_camera)
    {
        delete m_camera;
        m_camera = nullptr;
        CORE_LOG_INFO("DX12Renderer: Camera released");
    }
    
    if (rt_) { rt_->Destroy(); delete rt_; rt_ = nullptr; }
    if (device_) { device_->Shutdown(); delete device_; device_ = nullptr; }
}

} // namespace Renderer
