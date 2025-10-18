#include "DX12SwapChain.h"
#include "core/Log.h"

namespace Renderer {

DX12SwapChain::DX12SwapChain()
{
}

DX12SwapChain::~DX12SwapChain()
{
    Shutdown();
}

bool DX12SwapChain::Create(
#if defined(_WIN32) && defined(_MSC_VER)
    ID3D12CommandQueue* commandQueue,
    IDXGIFactory4* factory,
#else
    void* commandQueue,
    void* factory,
#endif
    HWND hwnd,
    unsigned int width,
    unsigned int height,
    unsigned int bufferCount
)
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!commandQueue || !factory || !hwnd)
    {
        CORE_LOG_ERROR("DX12SwapChain::Create: Invalid parameters");
        return false;
    }

    if (bufferCount < 2 || bufferCount > 3)
    {
        CORE_LOG_ERROR("DX12SwapChain::Create: Buffer count must be 2 or 3");
        return false;
    }

    m_bufferCount = bufferCount;
    m_width = width;
    m_height = height;

    // Describe swap chain
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferCount = bufferCount;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

    // Create swap chain
    IDXGISwapChain1* swapChain1 = nullptr;
    HRESULT hr = factory->CreateSwapChainForHwnd(
        commandQueue,
        hwnd,
        &swapChainDesc,
        nullptr, // Fullscreen desc
        nullptr, // Restrict output
        &swapChain1
    );

    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12SwapChain::Create: Failed to create swap chain");
        return false;
    }

    // Query for IDXGISwapChain3 interface
    hr = swapChain1->QueryInterface(IID_PPV_ARGS(&m_swapChain));
    swapChain1->Release();

    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12SwapChain::Create: Failed to get IDXGISwapChain3 interface");
        return false;
    }

    // Get back buffers
    for (unsigned int i = 0; i < bufferCount; ++i)
    {
        hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i]));
        if (FAILED(hr))
        {
            CORE_LOG_ERROR("DX12SwapChain::Create: Failed to get back buffer " + std::to_string(i));
            Shutdown();
            return false;
        }
    }

    CORE_LOG_INFO("DX12SwapChain created (" + std::to_string(width) + "x" + std::to_string(height) + 
        ", " + std::to_string(bufferCount) + " buffers)");
    return true;
#else
    CORE_LOG_WARN("DX12SwapChain::Create: Platform not supported");
    return false;
#endif
}

void DX12SwapChain::Present(bool vsync)
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!m_swapChain)
    {
        CORE_LOG_ERROR("DX12SwapChain::Present: Swap chain not created");
        return;
    }

    // Present with VSync (1) or without (0)
    UINT syncInterval = vsync ? 1 : 0;
    HRESULT hr = m_swapChain->Present(syncInterval, 0);

    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12SwapChain::Present: Failed to present frame");
    }
#endif
}

#if defined(_WIN32) && defined(_MSC_VER)
ID3D12Resource* DX12SwapChain::GetBackBuffer(unsigned int index) const
#else
void* DX12SwapChain::GetBackBuffer(unsigned int index) const
#endif
{
    if (index >= m_bufferCount)
    {
        CORE_LOG_ERROR("DX12SwapChain::GetBackBuffer: Index out of range");
        return nullptr;
    }

    return m_backBuffers[index];
}

unsigned int DX12SwapChain::GetCurrentBackBufferIndex() const
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!m_swapChain)
    {
        return 0;
    }

    return m_swapChain->GetCurrentBackBufferIndex();
#else
    return 0;
#endif
}

bool DX12SwapChain::Resize(unsigned int width, unsigned int height)
{
#if defined(_WIN32) && defined(_MSC_VER)
    if (!m_swapChain)
    {
        CORE_LOG_ERROR("DX12SwapChain::Resize: Swap chain not created");
        return false;
    }

    // Release back buffers before resizing
    for (unsigned int i = 0; i < m_bufferCount; ++i)
    {
        if (m_backBuffers[i])
        {
            m_backBuffers[i]->Release();
            m_backBuffers[i] = nullptr;
        }
    }

    // Resize swap chain buffers
    HRESULT hr = m_swapChain->ResizeBuffers(
        m_bufferCount,
        width,
        height,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        0 // Flags
    );

    if (FAILED(hr))
    {
        CORE_LOG_ERROR("DX12SwapChain::Resize: Failed to resize swap chain");
        return false;
    }

    // Re-acquire back buffers
    for (unsigned int i = 0; i < m_bufferCount; ++i)
    {
        hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i]));
        if (FAILED(hr))
        {
            CORE_LOG_ERROR("DX12SwapChain::Resize: Failed to get back buffer " + std::to_string(i));
            return false;
        }
    }

    m_width = width;
    m_height = height;

    CORE_LOG_INFO("DX12SwapChain resized to " + std::to_string(width) + "x" + std::to_string(height));
    return true;
#else
    return false;
#endif
}

void DX12SwapChain::Shutdown()
{
#if defined(_WIN32) && defined(_MSC_VER)
    // Release back buffers
    for (unsigned int i = 0; i < m_bufferCount; ++i)
    {
        if (m_backBuffers[i])
        {
            m_backBuffers[i]->Release();
            m_backBuffers[i] = nullptr;
        }
    }

    // Release swap chain
    if (m_swapChain)
    {
        m_swapChain->Release();
        m_swapChain = nullptr;
        CORE_LOG_INFO("DX12SwapChain shutdown");
    }
#endif

    m_bufferCount = 0;
    m_width = 0;
    m_height = 0;
}

} // namespace Renderer
