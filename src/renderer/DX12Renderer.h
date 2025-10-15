#pragma once

#include "IRenderer.h"

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

private:
    // stubs for device and render target ownership
    class DX12Device* device_ = nullptr;
    class RenderTarget* rt_ = nullptr;
};

} // namespace Renderer
