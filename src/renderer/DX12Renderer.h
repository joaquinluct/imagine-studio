#pragma once

#include "IRenderer.h"
#include "CommandAllocator.h"
#include "Fence.h"
#include <string>

namespace Renderer {

class DX12Renderer : public IRenderer {
public:
    DX12Renderer();
    ~DX12Renderer() override;

    void Initialize() override;
    void Shutdown() override;
    void RenderFrame() override;
    // Prepare render target for UI composition
    bool ComposeUI();
    // Notify renderer that an asset was loaded (path)
    void OnAssetLoaded(const std::string& path);

private:
    // stubs for device and render target ownership
    class DX12Device* device_ = nullptr;
    class RenderTarget* rt_ = nullptr;
    class CommandAllocator* allocator_ = nullptr;
    class Fence* fence_ = nullptr;
};

} // namespace Renderer
