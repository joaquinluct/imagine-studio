#pragma once

#if defined(_WIN32) && defined(_MSC_VER)
#include <d3d12.h>
#include <d3dcommon.h>
#include <dxgiformat.h>
#endif

namespace Renderer {

// v1.6.0 DEV-002.4 - Pipeline Manager (AAA Architecture)
// Encapsulates pipeline state creation (PSO, Root Signature, Shaders)
// Inspired by Unreal Engine FD3D12PipelineState and Unity ShaderPass
class DX12PipelineManager {
public:
    DX12PipelineManager();
    ~DX12PipelineManager();

    // Initialize pipeline manager with device
    bool Initialize(
#if defined(_WIN32) && defined(_MSC_VER)
        ID3D12Device* device
#else
        void* device
#endif
    );

    // === ROOT SIGNATURE CREATION ===

    // Create root signature from descriptor
    // Returns nullptr on failure
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12RootSignature* CreateRootSignature(
        const D3D12_ROOT_SIGNATURE_DESC& desc
    );
#else
    void* CreateRootSignature(const void* desc);
#endif

    // === SHADER COMPILATION ===

    // Compile shader from HLSL file
    // Returns nullptr on failure
#if defined(_WIN32) && defined(_MSC_VER)
    ID3DBlob* CompileShaderFromFile(
        const wchar_t* filename,
        const char* entryPoint,
        const char* target
    );
#else
    void* CompileShaderFromFile(
        const wchar_t* filename,
        const char* entryPoint,
        const char* target
    );
#endif

    // === PIPELINE STATE OBJECT (PSO) CREATION ===

    // Create graphics pipeline state
    // Returns nullptr on failure
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12PipelineState* CreateGraphicsPipelineState(
        const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc
    );
#else
    void* CreateGraphicsPipelineState(const void* desc);
#endif

    // === HELPER: INPUT LAYOUT ===

    // Create standard input layout for vertex with Position + Color
    // Returns input element count
#if defined(_WIN32) && defined(_MSC_VER)
    unsigned int CreateStandardInputLayout(
        D3D12_INPUT_ELEMENT_DESC* outElements,
        unsigned int maxElements
    );
#else
    unsigned int CreateStandardInputLayout(void* outElements, unsigned int maxElements);
#endif

    // === HELPER: RASTERIZER STATE ===

    // Create default rasterizer state (backface culling, solid fill)
#if defined(_WIN32) && defined(_MSC_VER)
    D3D12_RASTERIZER_DESC CreateDefaultRasterizerState();
#else
    void* CreateDefaultRasterizerState();
#endif

    // === HELPER: BLEND STATE ===

    // Create default blend state (no blending)
#if defined(_WIN32) && defined(_MSC_VER)
    D3D12_BLEND_DESC CreateDefaultBlendState();
#else
    void* CreateDefaultBlendState();
#endif

    // === RESOURCE RELEASE ===

    // Release root signature
    void ReleaseRootSignature(
#if defined(_WIN32) && defined(_MSC_VER)
        ID3D12RootSignature* rootSignature
#else
        void* rootSignature
#endif
    );

    // Release pipeline state
    void ReleasePipelineState(
#if defined(_WIN32) && defined(_MSC_VER)
        ID3D12PipelineState* pipelineState
#else
        void* pipelineState
#endif
    );

    // Release shader blob
    void ReleaseShaderBlob(
#if defined(_WIN32) && defined(_MSC_VER)
        ID3DBlob* blob
#else
        void* blob
#endif
    );

    // Shutdown and release resources
    void Shutdown();

private:
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12Device* m_device = nullptr;
#else
    void* m_device = nullptr;
#endif
};

} // namespace Renderer
