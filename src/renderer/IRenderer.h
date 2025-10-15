#pragma once

namespace Renderer {

class IRenderer {
public:
    virtual ~IRenderer() {}
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;
    virtual void RenderFrame() = 0;
};

} // namespace Renderer
