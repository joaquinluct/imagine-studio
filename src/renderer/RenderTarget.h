#pragma once

namespace Renderer {

class RenderTarget {
public:
    RenderTarget() {}
    ~RenderTarget() {}

    bool Create(int width, int height);
    void Destroy();
    void Present();
};

} // namespace Renderer
