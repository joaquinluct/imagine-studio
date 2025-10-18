#pragma once

#if defined(_WIN32) && defined(_MSC_VER)
#include <d3d12.h>
#include <windows.h>
#endif

namespace Renderer {

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
    ID3D12Device* m_device = nullptr;
    ID3D12CommandQueue* m_commandQueue = nullptr;
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
