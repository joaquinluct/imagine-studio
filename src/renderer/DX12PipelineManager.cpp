#include "DX12PipelineManager.h"
#include "core/Log.h"

#if defined(_WIN32) && defined(_MSC_VER)
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#endif

namespace Renderer {

DX12PipelineManager::DX12PipelineManager()
{
}

DX12PipelineManager::~DX12PipelineManager()
{
    Shutdown();
}

bool DX12PipelineManager::Initialize(
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12Device* device
#else
    void* device
#endif
)
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!device)
    {
        CORE_LOG_ERROR("DX12PipelineManager::Initialize: Invalid device");
        return false;
    }

    m_device = device;

    CORE_LOG_INFO("DX12PipelineManager initialized");
    return true;
#else
    CORE_LOG_WARN("DX12PipelineManager::Initialize: Platform not supported");
    return false;
#endif
}

#if defined(_WIN32) && defined(_MSC_VER)
ID3D12RootSignature* DX12PipelineManager::CreateRootSignature(
    const D3D12_ROOT_SIGNATURE_DESC& desc
)
#else
void* DX12PipelineManager::CreateRootSignature(const void* desc)
#endif
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!m_device)
    {
        CORE_LOG_ERROR("DX12PipelineManager::CreateRootSignature: Device not initialized");
        return nullptr;
    }

    // Serialize root signature
    ID3DBlob* signature = nullptr;
    ID3DBlob* error = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(
        &desc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        &signature,
        &error
    );

    if (FAILED(hr))
    {
        if (error)
        {
            CORE_LOG_ERROR("DX12PipelineManager::CreateRootSignature: Failed to serialize - " +
                std::string(static_cast<const char*>(error->GetBufferPointer())));
            error->Release();
        }
        else
        {
            CORE_LOG_ERROR("DX12PipelineManager::CreateRootSignature: Failed to serialize");
        }
        return nullptr;
    }

    // Create root signature
    ID3D12RootSignature* rootSignature = nullptr;
    hr = m_device->CreateRootSignature(
        0, // GPU node mask (single GPU)
        signature->GetBufferPointer(),
        signature->GetBufferSize(),
        IID_PPV_ARGS(&rootSignature)
    );

    signature->Release();

    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12PipelineManager::CreateRootSignature: Failed to create root signature");
        return nullptr;
    }

    CORE_LOG_INFO("DX12PipelineManager: Root signature created");
    return rootSignature;
#else
    return nullptr;
#endif
}

#if defined(_WIN32) && defined(_MSC_VER)
ID3DBlob* DX12PipelineManager::CompileShaderFromFile(
    const wchar_t* filename,
    const char* entryPoint,
    const char* target
)
#else
void* DX12PipelineManager::CompileShaderFromFile(
    const wchar_t* filename,
    const char* entryPoint,
    const char* target
)
#endif
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!filename || !entryPoint || !target)
    {
        CORE_LOG_ERROR("DX12PipelineManager::CompileShaderFromFile: Invalid parameters");
        return nullptr;
    }

    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

    HRESULT hr = D3DCompileFromFile(
        filename,
        nullptr,        // Defines
        nullptr,        // Include handler
        entryPoint,
        target,
        compileFlags,
        0,              // Effect flags
        &shaderBlob,
        &errorBlob
    );

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            CORE_LOG_ERROR("DX12PipelineManager::CompileShaderFromFile: Failed to compile - " +
                std::string(static_cast<const char*>(errorBlob->GetBufferPointer())));
            errorBlob->Release();
        }
        else
        {
            CORE_LOG_ERROR("DX12PipelineManager::CompileShaderFromFile: Failed to compile (file not found or compilation error)");
        }
        return nullptr;
    }

    // Convert filename to narrow string for logging
    char filenameNarrow[256];
    size_t convertedChars = 0;
    wcstombs_s(&convertedChars, filenameNarrow, 256, filename, 255);

    CORE_LOG_INFO("DX12PipelineManager: Shader compiled successfully (" +
        std::string(filenameNarrow) + "::" + std::string(entryPoint) + ", " + std::string(target) + ")");

    return shaderBlob;
#else
    return nullptr;
#endif
}

#if defined(_WIN32) && defined(_MSC_VER)
ID3D12PipelineState* DX12PipelineManager::CreateGraphicsPipelineState(
    const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc
)
#else
void* DX12PipelineManager::CreateGraphicsPipelineState(const void* desc)
#endif
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!m_device)
    {
        CORE_LOG_ERROR("DX12PipelineManager::CreateGraphicsPipelineState: Device not initialized");
        return nullptr;
    }

    ID3D12PipelineState* pipelineState = nullptr;
    HRESULT hr = m_device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipelineState));

    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12PipelineManager::CreateGraphicsPipelineState: Failed to create PSO");
        return nullptr;
    }

    CORE_LOG_INFO("DX12PipelineManager: Graphics pipeline state created");
    return pipelineState;
#else
    return nullptr;
#endif
}

#if defined(_WIN32) && defined(_MSC_VER)
unsigned int DX12PipelineManager::CreateStandardInputLayout(
    D3D12_INPUT_ELEMENT_DESC* outElements,
    unsigned int maxElements
)
#else
unsigned int DX12PipelineManager::CreateStandardInputLayout(void* outElements, unsigned int maxElements)
#endif
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!outElements || maxElements < 2)
    {
        CORE_LOG_ERROR("DX12PipelineManager::CreateStandardInputLayout: Invalid parameters");
        return 0;
    }

    // Standard layout: POSITION (float3) + COLOR (float4)
    outElements[0].SemanticName = "POSITION";
    outElements[0].SemanticIndex = 0;
    outElements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    outElements[0].InputSlot = 0;
    outElements[0].AlignedByteOffset = 0;
    outElements[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
    outElements[0].InstanceDataStepRate = 0;

    outElements[1].SemanticName = "COLOR";
    outElements[1].SemanticIndex = 0;
    outElements[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    outElements[1].InputSlot = 0;
    outElements[1].AlignedByteOffset = 12; // After 3 floats (position)
    outElements[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
    outElements[1].InstanceDataStepRate = 0;

    return 2; // Number of input elements
#else
    return 0;
#endif
}

#if defined(_WIN32) && defined(_MSC_VER)
D3D12_RASTERIZER_DESC DX12PipelineManager::CreateDefaultRasterizerState()
{
    D3D12_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK; // Backface culling enabled
    rasterizerDesc.FrontCounterClockwise = FALSE;   // Clockwise = front face
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.MultisampleEnable = FALSE;
    rasterizerDesc.AntialiasedLineEnable = FALSE;
    rasterizerDesc.ForcedSampleCount = 0;
    rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    return rasterizerDesc;
}
#else
void* DX12PipelineManager::CreateDefaultRasterizerState()
{
    return nullptr;
}
#endif

#if defined(_WIN32) && defined(_MSC_VER)
D3D12_BLEND_DESC DX12PipelineManager::CreateDefaultBlendState()
{
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

    return blendDesc;
}
#else
void* DX12PipelineManager::CreateDefaultBlendState()
{
    return nullptr;
}
#endif

void DX12PipelineManager::ReleaseRootSignature(
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12RootSignature* rootSignature
#else
    void* rootSignature
#endif
)
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (rootSignature)
    {
        rootSignature->Release();
    }
#endif
}

void DX12PipelineManager::ReleasePipelineState(
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12PipelineState* pipelineState
#else
    void* pipelineState
#endif
)
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (pipelineState)
    {
        pipelineState->Release();
    }
#endif
}

void DX12PipelineManager::ReleaseShaderBlob(
#if defined(_WIN32) && defined(_MSC_VER)
    ID3DBlob* blob
#else
    void* blob
#endif
)
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (blob)
    {
        blob->Release();
    }
#endif
}

void DX12PipelineManager::Shutdown()
{
#if defined(_WIN32) && defined(_MSC_VER)
    CORE_LOG_INFO("DX12PipelineManager shutdown");
#endif

    m_device = nullptr;
}

} // namespace Renderer
