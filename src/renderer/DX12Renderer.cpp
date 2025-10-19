#include "DX12Renderer.h"
#include "core/Log.h"

// === AAA SUBSYSTEMS (v1.6.0 DEV-002) ===
#include "DX12Device.h"
#include "DX12SwapChain.h"
#include "DX12CommandContext.h"
#include "DX12ResourceManager.h"
#include "DX12PipelineManager.h"
#include "DX12OpaquePass.h"
#include "DX12UIPass.h"
#include "Camera.h"

// Legacy stubs
#include "CommandAllocator.h"
#include "CommandBuffer.h"
#include "Fence.h"
#include "RenderTarget.h"

// ImGui headers (Dear ImGui - Editor UI Framework)
#include "imgui.h"
#include "imgui_impl_dx12.h"

#if defined(_WIN32) && defined(_MSC_VER)
#include <d3d12.h>
#include <dxgi1_4.h>

// Link DirectX 12 libraries
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#endif

#include <iostream>
#include <sstream>

namespace Renderer {

// v1.6.0 DEV-002.6 - Constructor (AAA Architecture)
DX12Renderer::DX12Renderer()
    : m_device(nullptr)
    , m_swapChain(nullptr)
    , m_commandContext(nullptr)
    , m_resourceManager(nullptr)
    , m_pipelineManager(nullptr)
    , m_opaquePass(nullptr)
    , m_uiPass(nullptr)
    , m_camera(nullptr)
    , rt_(nullptr)
    , allocator_(nullptr)
    , fence_(nullptr)
{
}

DX12Renderer::~DX12Renderer()
{
    Shutdown();
}

#if defined(_WIN32) && defined(_MSC_VER)
ID3D12Device* DX12Renderer::GetDevice() const
{
    return m_device ? static_cast<ID3D12Device*>(m_device->NativeDevice()) : nullptr;
}
#endif

void DX12Renderer::Initialize()
{
    std::cout << "DX12Renderer: Initialize (stub)\n";
    
    // Create DX12Device
    m_device = new DX12Device();
    m_device->Initialize();
    
    // Legacy stubs for non-DX12 platforms
    rt_ = new RenderTarget();
    rt_->Create(800, 600);
    allocator_ = new CommandAllocator();
    fence_ = new Fence();
}

void DX12Renderer::Initialize(HWND hwnd)
{
    CORE_LOG_INFO("DX12Renderer: Initialize(HWND) - AAA Architecture");
    
    // === STEP 1: Initialize Device ===
    m_device = new DX12Device();
    m_device->Initialize();
    
    if (!m_device->HasNativeDevice())
    {
        CORE_LOG_WARN("DX12Renderer: No native device, using stub render target");
        rt_ = new RenderTarget();
        rt_->Create(800, 600);
        allocator_ = new CommandAllocator();
        fence_ = new Fence();
        return;
    }
    
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12Device* d3dDevice = static_cast<ID3D12Device*>(m_device->NativeDevice());
    IDXGIFactory4* factory = static_cast<IDXGIFactory4*>(m_device->NativeFactory());
    ID3D12CommandQueue* commandQueue = static_cast<ID3D12CommandQueue*>(m_device->NativeCommandQueue());
    
    if (!d3dDevice || !factory || !commandQueue)
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to get Device, Factory or CommandQueue");
        return;
    }
    
    // === STEP 2: Create Descriptor Heaps ===
    // RTV heap (2 back buffers + 1 scene RT)
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = BACK_BUFFER_COUNT + 1;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    
    HRESULT hr = d3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create RTV heap");
        return;
    }
    
    m_rtvDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    
    // ImGui SRV heap (font atlas + scene RT SRV)
    D3D12_DESCRIPTOR_HEAP_DESC imguiSrvHeapDesc = {};
    imguiSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    imguiSrvHeapDesc.NumDescriptors = 2;
    imguiSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    
    hr = d3dDevice->CreateDescriptorHeap(&imguiSrvHeapDesc, IID_PPV_ARGS(&m_imguiSrvHeap));
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create ImGui SRV heap");
        return;
    }
    
    CORE_LOG_INFO("DX12Renderer: Descriptor heaps created (RTV + ImGui SRV)");
    
    // === STEP 3: Initialize AAA Subsystems ===
    
    // SwapChain
    m_swapChain = new DX12SwapChain();
    if (!m_swapChain->Create(commandQueue, factory, hwnd, 1920, 1080, BACK_BUFFER_COUNT))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create SwapChain");
        return;
    }
    
    // CommandContext
    m_commandContext = new DX12CommandContext();
    if (!m_commandContext->Initialize(d3dDevice, commandQueue))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to initialize CommandContext");
        return;
    }
    
    // ResourceManager
    m_resourceManager = new DX12ResourceManager();
    if (!m_resourceManager->Initialize(d3dDevice))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to initialize ResourceManager");
        return;
    }
    
    // PipelineManager
    m_pipelineManager = new DX12PipelineManager();
    if (!m_pipelineManager->Initialize(d3dDevice))
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to initialize PipelineManager");
        return;
    }
    
    CORE_LOG_INFO("DX12Renderer: AAA subsystems initialized (SwapChain, CommandContext, ResourceManager, PipelineManager)");
    
    // === STEP 4: Create Back Buffer RTVs ===
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
    
    for (unsigned int i = 0; i < BACK_BUFFER_COUNT; ++i)
    {
        ID3D12Resource* backBuffer = m_swapChain->GetBackBuffer(i);
        if (backBuffer)
        {
            d3dDevice->CreateRenderTargetView(backBuffer, nullptr, rtvHandle);
            rtvHandle.ptr += m_rtvDescriptorSize;
        }
    }
    
    CORE_LOG_INFO("DX12Renderer: Created " + std::to_string(BACK_BUFFER_COUNT) + " back buffer RTVs");
    
    // === STEP 5: Create Root Signature ===
    D3D12_ROOT_PARAMETER rootParam = {};
    rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
    rootParam.Constants.ShaderRegister = 0; // register(b0)
    rootParam.Constants.RegisterSpace = 0;
    rootParam.Constants.Num32BitValues = 16; // 4x4 MVP matrix
    rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
    
    D3D12_ROOT_SIGNATURE_DESC rootSigDesc = {};
    rootSigDesc.NumParameters = 1;
    rootSigDesc.pParameters = &rootParam;
    rootSigDesc.NumStaticSamplers = 0;
    rootSigDesc.pStaticSamplers = nullptr;
    rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    
    m_rootSignature = m_pipelineManager->CreateRootSignature(rootSigDesc);
    if (!m_rootSignature)
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create root signature");
        return;
    }
    
    // === STEP 6: Compile Shaders ===
    ID3DBlob* vertexShaderBlob = m_pipelineManager->CompileShaderFromFile(
        L"shaders/quad.hlsl", "VSMain", "vs_5_0"
    );
    
    ID3DBlob* pixelShaderBlob = m_pipelineManager->CompileShaderFromFile(
        L"shaders/quad.hlsl", "PSMain", "ps_5_0"
    );
    
    if (!vertexShaderBlob || !pixelShaderBlob)
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to compile shaders");
        return;
    }
    
    // === STEP 7: Create Pipeline State ===
    D3D12_INPUT_ELEMENT_DESC inputElements[2];
    m_pipelineManager->CreateStandardInputLayout(inputElements, 2);
    
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElements, 2 };
    psoDesc.pRootSignature = m_rootSignature;
    psoDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
    psoDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };
    psoDesc.RasterizerState = m_pipelineManager->CreateDefaultRasterizerState();
    psoDesc.BlendState = m_pipelineManager->CreateDefaultBlendState();
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;
    
    m_pipelineState = m_pipelineManager->CreateGraphicsPipelineState(psoDesc);
    if (!m_pipelineState)
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create pipeline state");
        return;
    }
    
    // Release shader blobs
    m_pipelineManager->ReleaseShaderBlob(vertexShaderBlob);
    m_pipelineManager->ReleaseShaderBlob(pixelShaderBlob);
    
    CORE_LOG_INFO("DX12Renderer: Pipeline state created (root signature + shaders + PSO)");
    
    // === STEP 8: Create Vertex Buffer ===
    struct Vertex {
        float pos[3];
        float col[4];
    };
    
    Vertex vertices[] = {
        {{-0.75f, -0.75f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // Bottom-left, red
        {{-0.75f,  0.75f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // Top-left, blue
        {{ 0.75f, -0.75f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // Bottom-right, green
        {{ 0.75f, -0.75f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // Bottom-right, green
        {{-0.75f,  0.75f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // Top-left, blue
        {{ 0.75f,  0.75f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, // Top-right, yellow
    };
    
    const unsigned int vertexBufferSize = sizeof(vertices);
    
    // Use command context for vertex buffer upload
    m_commandContext->BeginFrame(nullptr);
    
    // BUG-4 FIX INTENTO #5: Keep upload buffer alive until GPU finishes copying
    ID3D12Resource* uploadBuffer = nullptr;
    m_vertexBuffer = m_resourceManager->CreateVertexBuffer(
        vertices,
        vertexBufferSize,
        m_commandContext->GetCommandList(),
        &uploadBuffer
    );
    
    if (!m_vertexBuffer)
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create vertex buffer");
        return;
    }
    
    // Execute and wait for vertex buffer upload
    m_commandContext->EndFrame();
    m_commandContext->Execute();
    m_commandContext->WaitForGPU();
    
    // BUG-4 FIX INTENTO #5: NOW it's safe to release upload buffer (GPU finished copying)
    if (uploadBuffer)
    {
        uploadBuffer->Release();
    }
    
    // Configure vertex buffer view
    m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = sizeof(Vertex);
    m_vertexBufferView.SizeInBytes = vertexBufferSize;
    
    CORE_LOG_INFO("DX12Renderer: Vertex buffer created (6 vertices, 28 bytes stride)");
    
    // === STEP 9: Initialize Camera ===
    m_camera = new Camera();
    m_camera->SetPosition(0.0f, 2.0f, -5.0f);
    m_camera->SetTarget(0.0f, 0.0f, 0.0f);
    m_camera->SetPerspective(45.0f, 1920.0f / 1080.0f, 0.1f, 1000.0f);
    
    CORE_LOG_INFO("DX12Renderer: Camera initialized");
    
    // === STEP 10: Create Scene Render Target ===
    CreateSceneRenderTarget();
    
    // === STEP 11: Initialize Render Passes ===
    m_opaquePass = new DX12OpaquePass();
    m_opaquePass->Initialize();
    m_opaquePass->SetRenderTarget(m_sceneRenderTarget, m_sceneRTV);
    m_opaquePass->SetPipeline(m_pipelineState, m_rootSignature);
    m_opaquePass->SetVertexBuffer(m_vertexBufferView);
    m_opaquePass->SetCamera(m_camera);
    
    m_uiPass = new DX12UIPass();
    m_uiPass->Initialize();
    
    CORE_LOG_INFO("DX12Renderer: Render passes initialized (OpaquePass, UIPass)");
    
    CORE_LOG_INFO("DX12Renderer: Initialization complete - AAA Architecture (v1.6.0 DEV-002.6)");
#endif
    
    // Legacy stubs
    allocator_ = new CommandAllocator();
    fence_ = new Fence();
}

#if defined(_WIN32) && defined(_MSC_VER)
void DX12Renderer::CreateSceneRenderTarget()
{
    if (!m_device || !m_device->HasNativeDevice())
    {
        return;
    }
    
    ID3D12Device* d3dDevice = static_cast<ID3D12Device*>(m_device->NativeDevice());
    
    // Create scene RT with ResourceManager
    D3D12_CLEAR_VALUE clearValue = {};
    clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    clearValue.Color[0] = 0.0f;
    clearValue.Color[1] = 0.2f;
    clearValue.Color[2] = 0.4f;
    clearValue.Color[3] = 1.0f;
    
    m_sceneRenderTarget = m_resourceManager->CreateTexture2D(
        1920, 1080,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        &clearValue
    );
    
    if (!m_sceneRenderTarget)
    {
        CORE_LOG_ERROR("DX12Renderer: Failed to create scene render target");
        return;
    }
    
    // Create RTV (slot 2 in RTV heap)
    m_sceneRTV = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
    m_sceneRTV.ptr += BACK_BUFFER_COUNT * m_rtvDescriptorSize;
    
    d3dDevice->CreateRenderTargetView(m_sceneRenderTarget, nullptr, m_sceneRTV);
    
    // Create SRV (slot 1 in ImGui SRV heap)
    unsigned int srvDescriptorSize = m_resourceManager->GetDescriptorSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    
    m_sceneSRV_CPU = m_imguiSrvHeap->GetCPUDescriptorHandleForHeapStart();
    m_sceneSRV_CPU.ptr += srvDescriptorSize;
    
    m_sceneSRV_GPU = m_imguiSrvHeap->GetGPUDescriptorHandleForHeapStart();
    m_sceneSRV_GPU.ptr += srvDescriptorSize;
    
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    
    d3dDevice->CreateShaderResourceView(m_sceneRenderTarget, &srvDesc, m_sceneSRV_CPU);
    
    CORE_LOG_INFO("DX12Renderer: Scene render target created (1920x1080, offscreen)");
    CORE_LOG_INFO("Scene RT SRV GPU handle: " + std::to_string(m_sceneSRV_GPU.ptr));
    CORE_LOG_INFO("Scene RT SRV CPU handle: " + std::to_string(m_sceneSRV_CPU.ptr));
}

void DX12Renderer::CalculateMVPMatrix()
{
    if (!m_camera)
    {
        // Identity matrix
        for (int i = 0; i < 16; ++i)
        {
            m_mvpMatrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
        }
        return;
    }
    
    // Model matrix (identity - quad at origin)
    float modelMatrix[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    
    const float* viewMatrix = m_camera->GetViewMatrix();
    const float* projectionMatrix = m_camera->GetProjectionMatrix();
    
    // BUG-4 DEBUG: Log camera position
    static int frameCount = 0;
    if (frameCount++ == 0) // Solo primer frame
    {
        CORE_LOG_INFO("BUG-4 DEBUG: Camera view matrix:");
        for (int i = 0; i < 4; ++i)
        {
            std::ostringstream ss;
            ss << "[" << viewMatrix[i*4+0] << ", " << viewMatrix[i*4+1] << ", " << viewMatrix[i*4+2] << ", " << viewMatrix[i*4+3] << "]";
            CORE_LOG_INFO(ss.str());
        }
    }
    
    // Calculate ModelView = Model * View
    float modelView[16];
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            modelView[row * 4 + col] = 0.0f;
            for (int k = 0; k < 4; ++k)
            {
                modelView[row * 4 + col] += modelMatrix[row * 4 + k] * viewMatrix[k * 4 + col];
            }
        }
    }
    
    // Calculate MVP = ModelView * Projection
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            m_mvpMatrix[row * 4 + col] = 0.0f;
            for (int k = 0; k < 4; ++k)
            {
                m_mvpMatrix[row * 4 + col] += modelView[row * 4 + k] * projectionMatrix[k * 4 + col];
            }
        }
    }
}
#endif

void DX12Renderer::RenderFrame()
{
    RenderForwardPass();
}

void DX12Renderer::RenderForwardPass()
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!m_device || !m_device->HasNativeDevice())
    {
        // Fallback to stub rendering
        Renderer::CommandBuffer cmd;
        cmd.Initialize(1024);
        cmd.Submit();
        if (fence_) fence_->Signal();
        ComposeUI();
        if (rt_) rt_->Present();
        return;
    }
    
    if (!m_commandContext || !m_opaquePass || !m_uiPass || !m_swapChain)
    {
        CORE_LOG_ERROR("DX12Renderer::RenderForwardPass: Subsystems not initialized");
        return;
    }
    
    // Calculate MVP matrix
    CalculateMVPMatrix();
    m_opaquePass->SetMVPMatrix(m_mvpMatrix);
    
    // v1.7.0 H1.8 - FRAME PIPELINING: ONE command list for entire frame
    // BeginFrame() now waits only if CPU is >FRAME_LATENCY frames ahead (built into FrameContext)
    m_commandContext->BeginFrame(nullptr);
    ID3D12GraphicsCommandList* commandList = m_commandContext->GetCommandList();
    
    if (!commandList)
    {
        CORE_LOG_ERROR("DX12Renderer::RenderForwardPass: Failed to get command list");
        return;
    }
    
    // Get back buffer for this frame
    unsigned int backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
    ID3D12Resource* backBuffer = m_swapChain->GetBackBuffer(backBufferIndex);
    D3D12_CPU_DESCRIPTOR_HANDLE backBufferRTV = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
    backBufferRTV.ptr += backBufferIndex * m_rtvDescriptorSize;
    
    // v1.7.0 H1.8 + H2 (Barrier Batching) - Batch ALL initial barriers
    static bool isFirstFrame = true;
    
    if (isFirstFrame)
    {
        // FRAME 1: Scene RT already in RENDER_TARGET, back buffer in PRESENT
        // Barrier: Back buffer PRESENT ? RENDER_TARGET (needed for UIPass later)
        D3D12_RESOURCE_BARRIER initialBarriers[1] = {};
        initialBarriers[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        initialBarriers[0].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        initialBarriers[0].Transition.pResource = backBuffer;
        initialBarriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        initialBarriers[0].Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        initialBarriers[0].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        
        commandList->ResourceBarrier(1, initialBarriers);
    }
    else
    {
        // FRAME N+1: Scene RT in PSR (from previous frame), back buffer in PRESENT
        // Batch barriers: Scene RT PSR?RT, Back buffer PRESENT?RT
        D3D12_RESOURCE_BARRIER initialBarriers[2] = {};
        
        // Barrier 1: Scene RT PSR ? RT
        initialBarriers[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        initialBarriers[0].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        initialBarriers[0].Transition.pResource = m_sceneRenderTarget;
        initialBarriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        initialBarriers[0].Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        initialBarriers[0].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        
        // Barrier 2: Back buffer PRESENT ? RT
        initialBarriers[1].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        initialBarriers[1].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        initialBarriers[1].Transition.pResource = backBuffer;
        initialBarriers[1].Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        initialBarriers[1].Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        initialBarriers[1].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        
        commandList->ResourceBarrier(2, initialBarriers);
    }
    
    // === PASS 1: OPAQUE PASS (render 3D scene to scene RT) ===
    // Set pipeline state for OpaquePass
    commandList->SetPipelineState(m_pipelineState);
    
    // Execute OpaquePass commands (inline, no separate command list)
    m_opaquePass->Execute(*m_commandContext);
    
    // === MID-FRAME BARRIER: Scene RT ? PIXEL_SHADER_RESOURCE ===
    // Transition scene RT so ImGui can sample it in Viewport
    D3D12_RESOURCE_BARRIER midBarrier = {};
    midBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    midBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    midBarrier.Transition.pResource = m_sceneRenderTarget;
    midBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    midBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    midBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    
    commandList->ResourceBarrier(1, &midBarrier);
    
    // === PASS 2: UI PASS (render ImGui to back buffer) ===
    m_uiPass->SetRenderTarget(backBuffer, backBufferRTV);
    m_uiPass->SetImGuiSrvHeap(m_imguiSrvHeap);
    m_uiPass->SetUIVisible(m_uiVisible);
    
    // Execute UIPass commands (inline, same command list)
    m_uiPass->Execute(*m_commandContext);
    
    // === END-FRAME BARRIER: Back Buffer ? PRESENT ===
    D3D12_RESOURCE_BARRIER endBarrier = {};
    endBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    endBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    endBarrier.Transition.pResource = backBuffer;
    endBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    endBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    endBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    
    commandList->ResourceBarrier(1, &endBarrier);
    
    // v1.7.0 H1.8 - END FRAME: Close and execute command list ONCE
    m_commandContext->EndFrame();
    m_commandContext->Execute(); // GPU starts working, CPU continues to next frame
    
    // v1.7.0 H1.8 - NO WaitForGPU() here!
    // Frame pipelining: GPU works on frame N, CPU prepares frame N+1
    // Wait only happens in BeginFrame() if CPU is >FRAME_LATENCY frames ahead
    
    // Mark first frame complete
    isFirstFrame = false;
    
    // Present (swap buffers)
    m_swapChain->Present(true); // VSync on
#else
    // Stub rendering
    Renderer::CommandBuffer cmd;
    cmd.Initialize(1024);
    cmd.Submit();
    if (fence_) fence_->Signal();
    ComposeUI();
    if (rt_) rt_->Present();
#endif
}

bool DX12Renderer::ComposeUI()
{
    // Legacy stub
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
    // Wait for GPU before releasing resources
    if (m_commandContext)
    {
        m_commandContext->WaitForGPU();
    }
    
    // Shutdown render passes
    if (m_uiPass)
    {
        m_uiPass->Shutdown();
        delete m_uiPass;
        m_uiPass = nullptr;
    }
    
    if (m_opaquePass)
    {
        m_opaquePass->Shutdown();
        delete m_opaquePass;
        m_opaquePass = nullptr;
    }
    
    // Release camera
    if (m_camera)
    {
        delete m_camera;
        m_camera = nullptr;
    }
    
    // Release resources (managed by subsystems)
    if (m_pipelineManager)
    {
        m_pipelineManager->ReleasePipelineState(m_pipelineState);
        m_pipelineManager->ReleaseRootSignature(m_rootSignature);
    }
    
    if (m_resourceManager)
    {
        m_resourceManager->ReleaseResource(m_vertexBuffer);
        m_resourceManager->ReleaseResource(m_sceneRenderTarget);
        m_resourceManager->ReleaseDescriptorHeap(m_rtvHeap);
        m_resourceManager->ReleaseDescriptorHeap(m_imguiSrvHeap);
    }
    
    // Shutdown subsystems
    if (m_pipelineManager)
    {
        m_pipelineManager->Shutdown();
        delete m_pipelineManager;
        m_pipelineManager = nullptr;
    }
    
    if (m_resourceManager)
    {
        m_resourceManager->Shutdown();
        delete m_resourceManager;
        m_resourceManager = nullptr;
    }
    
    if (m_commandContext)
    {
        m_commandContext->Shutdown();
        delete m_commandContext;
        m_commandContext = nullptr;
    }
    
    if (m_swapChain)
    {
        m_swapChain->Shutdown();
        delete m_swapChain;
        m_swapChain = nullptr;
    }
    
    CORE_LOG_INFO("DX12Renderer: AAA subsystems shutdown");
#endif
    
    // Legacy stubs
    if (rt_) { rt_->Destroy(); delete rt_; rt_ = nullptr; }
    if (m_device) { m_device->Shutdown(); delete m_device; m_device = nullptr; }
    if (allocator_) { delete allocator_; allocator_ = nullptr; }
    if (fence_) { delete fence_; fence_ = nullptr; }
    
    CORE_LOG_INFO("DX12Renderer: Shutdown complete");
}

} // namespace Renderer
