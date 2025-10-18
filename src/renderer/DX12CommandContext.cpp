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

    // Create command allocator
    HRESULT hr = m_device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(&m_commandAllocator)
    );

    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12CommandContext::Initialize: Failed to create command allocator");
        return false;
    }

    // Create command list (initially closed state)
    hr = m_device->CreateCommandList(
        0, // GPU node mask (single GPU)
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        m_commandAllocator,
        nullptr, // Initial PSO (none)
        IID_PPV_ARGS(&m_commandList)
    );

    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12CommandContext::Initialize: Failed to create command list");
        m_commandAllocator->Release();
        m_commandAllocator = nullptr;
        return false;
    }

    // Command lists are created in recording state, close it immediately
    m_commandList->Close();

    // Create fence for GPU/CPU synchronization
    m_fenceValue = 1;
    hr = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));

    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12CommandContext::Initialize: Failed to create fence");
        m_commandList->Release();
        m_commandList = nullptr;
        m_commandAllocator->Release();
        m_commandAllocator = nullptr;
        return false;
    }

    // Create fence event
    m_fenceEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
    if (m_fenceEvent == nullptr)
    {
        CORE_LOG_ERROR("DX12CommandContext::Initialize: Failed to create fence event");
        m_fence->Release();
        m_fence = nullptr;
        m_commandList->Release();
        m_commandList = nullptr;
        m_commandAllocator->Release();
        m_commandAllocator = nullptr;
        return false;
    }

    CORE_LOG_INFO("DX12CommandContext initialized (command allocator, list, fence)");
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
    if (!m_commandAllocator || !m_commandList)
    {
        CORE_LOG_ERROR("DX12CommandContext::BeginFrame: Command context not initialized");
        return false;
    }

    if (m_isRecording)
    {
        CORE_LOG_ERROR("DX12CommandContext::BeginFrame: Already recording");
        return false;
    }

    // Reset command allocator (reuse memory for new frame)
    HRESULT hr = m_commandAllocator->Reset();
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12CommandContext::BeginFrame: Failed to reset command allocator");
        return false;
    }

    // Reset command list with optional initial PSO
    hr = m_commandList->Reset(m_commandAllocator, initialPSO);
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12CommandContext::BeginFrame: Failed to reset command list");
        return false;
    }

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
    if (!m_commandQueue || !m_commandList || !m_fence)
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
    ID3D12CommandList* ppCommandLists[] = { m_commandList };
    m_commandQueue->ExecuteCommandLists(1, ppCommandLists);

    // Signal fence after GPU work submission
    const unsigned long long fenceValueForThisFrame = m_fenceValue;
    HRESULT hr = m_commandQueue->Signal(m_fence, fenceValueForThisFrame);
    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12CommandContext::Execute: Failed to signal fence");
        return false;
    }

    m_fenceValue++;

    return true;
#else
    return false;
#endif
}

void DX12CommandContext::WaitForGPU()
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!m_fence || !m_fenceEvent)
    {
        CORE_LOG_ERROR("DX12CommandContext::WaitForGPU: Fence not initialized");
        return;
    }

    // Wait for the previous frame's fence value
    const unsigned long long fenceValueToWaitFor = m_fenceValue - 1;

    if (m_fence->GetCompletedValue() < fenceValueToWaitFor)
    {
        // GPU hasn't reached this fence value yet, wait for it
        HRESULT hr = m_fence->SetEventOnCompletion(fenceValueToWaitFor, m_fenceEvent);
        if (FAILED(hr))
        {
            CORE_LOG_ERROR("DX12CommandContext::WaitForGPU: Failed to set event on fence completion");
            return;
        }

        WaitForSingleObject(m_fenceEvent, INFINITE);
    }
#endif
}

void DX12CommandContext::Shutdown()
{
#if defined(_WIN32) && defined(_MSC_VER)
    // Wait for GPU to finish before releasing resources
    if (m_fence && m_fenceEvent && m_fenceValue > 1)
    {
        WaitForGPU();
    }

    // Close fence event handle
    if (m_fenceEvent)
    {
        CloseHandle(m_fenceEvent);
        m_fenceEvent = nullptr;
    }

    // Release fence
    if (m_fence)
    {
        m_fence->Release();
        m_fence = nullptr;
    }

    // Release command list
    if (m_commandList)
    {
        m_commandList->Release();
        m_commandList = nullptr;
    }

    // Release command allocator
    if (m_commandAllocator)
    {
        m_commandAllocator->Release();
        m_commandAllocator = nullptr;
    }

    CORE_LOG_INFO("DX12CommandContext shutdown");
#endif

    m_device = nullptr;
    m_commandQueue = nullptr;
    m_fenceValue = 0;
    m_isRecording = false;
}

} // namespace Renderer
