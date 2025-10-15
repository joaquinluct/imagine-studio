#pragma once

#include "IRenderer.h"
#include <memory>

namespace Renderer {

enum class Backend { DX12, DX11, Vulkan, OpenGL };

// Factory for renderer adapters
class AdapterFactory {
public:
    static std::unique_ptr<IRenderer> CreateAdapter(Backend b);
};

} // namespace Renderer
