#pragma once

#if defined(_WIN32) && defined(_MSC_VER)
#include <d3d12.h>
#include <windows.h>
#endif

namespace Renderer {

// v1.7.0 H1 - Frame pipelining (double buffering)
// Each FrameContext has its own command allocator, list, and fence
// Allows GPU to work on frame N while CPU prepares frame N+1
#if defined(_WIN32) && defined(_MSC_VER)
struct FrameContext {
    ID3D12CommandAllocator* commandAllocator = nullptr;
    ID3D12GraphicsCommandList* commandList = nullptr;
    ID3D12Fence* fence = nullptr;
    HANDLE fenceEvent = nullptr;
    unsigned long long fenceValue = 0;
};
#endif

// v1.6.0 DEV-002.2 - Command Context (AAA Architecture)
// Encapsulates command list recording, execution, and GPU/CPU synchronization
// Inspired by Unreal Engine FD3D12CommandContext and Unity CommandBuffer
class DX12CommandContext {
public:
    DX12CommandContext();
    ~DX12CommandContext();

    // Initialize command context with device and command queue
    bool Initialize(
#if defined(_WIN32) && defined(_MSC_VER)
        ID3D12Device* device,
        ID3D12CommandQueue* commandQueue
#else
        void* device,
        void* commandQueue
#endif
    );

    // Begin frame (reset command allocator and list)
    bool BeginFrame(
#if defined(_WIN32) && defined(_MSC_VER)
        ID3D12PipelineState* initialPSO = nullptr
#else
        void* initialPSO = nullptr
#endif
    );

    // End frame (close command list)
    bool EndFrame();

    // Execute command list on command queue
    bool Execute();

    // Wait for GPU to finish all pending work (synchronous)
    void WaitForGPU();

    // Get command list for recording
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12GraphicsCommandList* GetCommandList() const { return m_commandList; }
#else
    void* GetCommandList() const { return m_commandList; }
#endif

    // Get current fence value (for tracking frame completion)
    unsigned long long GetCurrentFenceValue() const { return m_fenceValue; }

    // Shutdown and release resources
    void Shutdown();

private:
#if defined(_WIN32) && defined(_MSC_VER)
    // v1.7.0 H1 - Frame pipelining (ring buffer)
    static const int FRAME_LATENCY = 2; // Double buffering (CPU can be 2 frames ahead of GPU)
    FrameContext m_frameContexts[FRAME_LATENCY];
    int m_currentFrameIndex = 0;
    
    // Device and command queue (shared across frames)
    ID3D12Device* m_device = nullptr;
    ID3D12CommandQueue* m_commandQueue = nullptr;
    
    // Legacy single-frame members (deprecated, kept for compatibility during migration)
    ID3D12CommandAllocator* m_commandAllocator = nullptr;
    ID3D12GraphicsCommandList* m_commandList = nullptr;
    ID3D12Fence* m_fence = nullptr;
    HANDLE m_fenceEvent = nullptr;
#else
    void* m_device = nullptr;
    void* m_commandQueue = nullptr;
    void* m_commandAllocator = nullptr;
    void* m_commandList = nullptr;
    void* m_fence = nullptr;
    void* m_fenceEvent = nullptr;
#endif

    unsigned long long m_fenceValue = 0;
    bool m_isRecording = false;
};

} // namespace Renderer
