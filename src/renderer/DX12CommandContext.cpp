#include "DX12CommandContext.h"
#include "core/Log.h"

namespace Renderer {

DX12CommandContext::DX12CommandContext()
{
}

DX12CommandContext::~DX12CommandContext()
{
    Shutdown();
}

bool DX12CommandContext::Initialize(
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12Device* device,
    ID3D12CommandQueue* commandQueue
#else
    void* device,
    void* commandQueue
#endif
)
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!device || !commandQueue)
    {
        CORE_LOG_ERROR("DX12CommandContext::Initialize: Invalid device or command queue");
        return false;
    }

    m_device = device;
    m_commandQueue = commandQueue;

    // v1.7.0 H1.3 - Initialize FrameContext array (ring buffer)
    for (int i = 0; i < FRAME_LATENCY; ++i)
    {
        FrameContext& frameCtx = m_frameContexts[i];
        
        // Create command allocator for this frame
        HRESULT hr = m_device->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            IID_PPV_ARGS(&frameCtx.commandAllocator)
        );
        
        if (FAILED(hr))
        {
            CORE_LOG_ERROR("DX12CommandContext::Initialize: Failed to create command allocator for frame " + std::to_string(i));
            return false;
        }
        
        // Create command list for this frame (initially closed state)
        hr = m_device->CreateCommandList(
            0, // GPU node mask (single GPU)
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            frameCtx.commandAllocator,
            nullptr, // Initial PSO (none)
            IID_PPV_ARGS(&frameCtx.commandList)
        );
        
        if (FAILED(hr))
        {
            CORE_LOG_ERROR("DX12CommandContext::Initialize: Failed to create command list for frame " + std::to_string(i));
            frameCtx.commandAllocator->Release();
            frameCtx.commandAllocator = nullptr;
            return false;
        }
        
        // Command lists are created in recording state, close it immediately
        frameCtx.commandList->Close();
        
        // Create fence for this frame
        frameCtx.fenceValue = 0;
        hr = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&frameCtx.fence));
        
        if (FAILED(hr))
        {
            CORE_LOG_ERROR("DX12CommandContext::Initialize: Failed to create fence for frame " + std::to_string(i));
            frameCtx.commandList->Release();
            frameCtx.commandList = nullptr;
            frameCtx.commandAllocator->Release();
            frameCtx.commandAllocator = nullptr;
            return false;
        }
        
        // Create fence event for this frame
        frameCtx.fenceEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
        if (frameCtx.fenceEvent == nullptr)
        {
            CORE_LOG_ERROR("DX12CommandContext::Initialize: Failed to create fence event for frame " + std::to_string(i));
            frameCtx.fence->Release();
            frameCtx.fence = nullptr;
            frameCtx.commandList->Release();
            frameCtx.commandList = nullptr;
            frameCtx.commandAllocator->Release();
            frameCtx.commandAllocator = nullptr;
            return false;
        }
    }
    
    // Set compatibility pointers to first frame context (for GetCommandList())
    m_commandList = m_frameContexts[0].commandList;

    CORE_LOG_INFO("DX12CommandContext initialized (frame pipelining with " + std::to_string(FRAME_LATENCY) + " frames)");
    return true;
#else
    CORE_LOG_WARN("DX12CommandContext::Initialize: Platform not supported");
    return false;
#endif
}

bool DX12CommandContext::BeginFrame(
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12PipelineState* initialPSO
#else
    void* initialPSO
#endif
)
{
#if defined(_WIN32) && defined(_MSC_VER)
    // v1.7.0 H1.4 - Use current frame context from ring buffer
    FrameContext& frameCtx = m_frameContexts[m_currentFrameIndex];
    
    if (!frameCtx.commandAllocator || !frameCtx.commandList)
    {
        CORE_LOG_ERROR("DX12CommandContext::BeginFrame: Command context not initialized");
        return false;
    }

    if (m_isRecording)
    {
        CORE_LOG_ERROR("DX12CommandContext::BeginFrame: Already recording");
        return false;
    }

    // v1.7.0 H1.4 - Wait ONLY if GPU is still using THIS frame's allocator
    // (i.e., CPU is more than FRAME_LATENCY frames ahead)
    if (frameCtx.fence->GetCompletedValue() < frameCtx.fenceValue)
    {
        // GPU hasn't finished with this frame context yet, wait for it
        HRESULT hr = frameCtx.fence->SetEventOnCompletion(frameCtx.fenceValue, frameCtx.fenceEvent);
        if (FAILED(hr))
        {
            CORE_LOG_ERROR("DX12CommandContext::BeginFrame: Failed to set event on fence completion");
            return false;
        }
        
        WaitForSingleObject(frameCtx.fenceEvent, INFINITE);
    }

    // Reset command allocator (reuse memory for new frame)
    HRESULT hr = frameCtx.commandAllocator->Reset();
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12CommandContext::BeginFrame: Failed to reset command allocator");
        return false;
    }

    // Reset command list with optional initial PSO
    hr = frameCtx.commandList->Reset(frameCtx.commandAllocator, initialPSO);
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12CommandContext::BeginFrame: Failed to reset command list");
        return false;
    }
    
    // Update compatibility pointer for GetCommandList()
    m_commandList = frameCtx.commandList;

    m_isRecording = true;
    return true;
#else
    return false;
#endif
}

bool DX12CommandContext::EndFrame()
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!m_commandList)
    {
        CORE_LOG_ERROR("DX12CommandContext::EndFrame: Command list not initialized");
        return false;
    }

    if (!m_isRecording)
    {
        CORE_LOG_ERROR("DX12CommandContext::EndFrame: Not recording");
        return false;
    }

    // Close command list (finish recording)
    HRESULT hr = m_commandList->Close();
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12CommandContext::EndFrame: Failed to close command list");
        return false;
    }

    m_isRecording = false;
    return true;
#else
    return false;
#endif
}

bool DX12CommandContext::Execute()
{
#if defined(_WIN32) && defined(_MSC_VER)
    // v1.7.0 H1.5 - Use current frame context
    FrameContext& frameCtx = m_frameContexts[m_currentFrameIndex];
    
    if (!m_commandQueue || !frameCtx.commandList || !frameCtx.fence)
    {
        CORE_LOG_ERROR("DX12CommandContext::Execute: Command context not initialized");
        return false;
    }

    if (m_isRecording)
    {
        CORE_LOG_ERROR("DX12CommandContext::Execute: Cannot execute while recording (call EndFrame first)");
        return false;
    }

    // Execute command list on command queue
    ID3D12CommandList* ppCommandLists[] = { frameCtx.commandList };
    m_commandQueue->ExecuteCommandLists(1, ppCommandLists);

    // v1.7.0 H1.5 - Signal fence for THIS frame with unique fence value
    frameCtx.fenceValue++;
    HRESULT hr = m_commandQueue->Signal(frameCtx.fence, frameCtx.fenceValue);
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12CommandContext::Execute: Failed to signal fence");
        return false;
    }
    
    // v1.7.0 H1.5 - Advance to next frame in ring buffer
    m_currentFrameIndex = (m_currentFrameIndex + 1) % FRAME_LATENCY;

    return true;
#else
    return false;
#endif
}

void DX12CommandContext::WaitForGPU()
{
#if defined(_WIN32) && defined(_MSC_VER)
    // v1.7.0 H1 FIX: Wait for ALL frames in ring buffer to complete
    for (int i = 0; i < FRAME_LATENCY; ++i)
    {
        FrameContext& frameCtx = m_frameContexts[i];
        
        if (!frameCtx.fence || !frameCtx.fenceEvent)
        {
            continue; // Skip uninitialized frames
        }
        
        // Wait for this frame's GPU work to complete
        if (frameCtx.fenceValue > 0 && frameCtx.fence->GetCompletedValue() < frameCtx.fenceValue)
        {
            HRESULT hr = frameCtx.fence->SetEventOnCompletion(frameCtx.fenceValue, frameCtx.fenceEvent);
            if (FAILED(hr))
            {
                CORE_LOG_ERROR("DX12CommandContext::WaitForGPU: Failed to set event on fence completion for frame " + std::to_string(i));
                continue;
            }
            
            WaitForSingleObject(frameCtx.fenceEvent, INFINITE);
        }
    }
#endif
}

void DX12CommandContext::Shutdown()
{
#if defined(_WIN32) && defined(_MSC_VER)
    // v1.7.0 H1.6 - Wait for GPU to finish all frames before releasing resources
    for (int i = 0; i < FRAME_LATENCY; ++i)
    {
        FrameContext& frameCtx = m_frameContexts[i];
        
        if (frameCtx.fence && frameCtx.fenceEvent && frameCtx.fenceValue > 0)
        {
            // Wait for this frame's GPU work to complete
            if (frameCtx.fence->GetCompletedValue() < frameCtx.fenceValue)
            {
                frameCtx.fence->SetEventOnCompletion(frameCtx.fenceValue, frameCtx.fenceEvent);
                WaitForSingleObject(frameCtx.fenceEvent, INFINITE);
            }
        }
        
        // Close fence event handle
        if (frameCtx.fenceEvent)
        {
            CloseHandle(frameCtx.fenceEvent);
            frameCtx.fenceEvent = nullptr;
        }
        
        // Release fence
        if (frameCtx.fence)
        {
            frameCtx.fence->Release();
            frameCtx.fence = nullptr;
        }
        
        // Release command list
        if (frameCtx.commandList)
        {
            frameCtx.commandList->Release();
            frameCtx.commandList = nullptr;
        }
        
        // Release command allocator
        if (frameCtx.commandAllocator)
        {
            frameCtx.commandAllocator->Release();
            frameCtx.commandAllocator = nullptr;
        }
        
        frameCtx.fenceValue = 0;
    }

    CORE_LOG_INFO("DX12CommandContext shutdown (frame pipelining)");
#endif

    m_device = nullptr;
    m_commandQueue = nullptr;
    m_commandList = nullptr; // Compatibility pointer (no release, points to frame context)
    m_fenceValue = 0;
    m_isRecording = false;
    m_currentFrameIndex = 0;
}

} // namespace Renderer
