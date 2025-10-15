#pragma once

namespace Renderer {

class RenderTarget {
public:
    RenderTarget() {}
    ~RenderTarget() {}

    bool Create(int width, int height) { (void)width; (void)height; return true; }
    void Destroy() {}
};

} // namespace Renderer
