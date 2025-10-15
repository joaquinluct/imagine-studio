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
};

} // namespace Renderer
