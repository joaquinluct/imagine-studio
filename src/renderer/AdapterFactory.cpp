#include "AdapterFactory.h"
#ifdef BUILD_TEST_EXE
#include "IRenderer.h"
#include <memory>

namespace Renderer {

// Simple stub renderer used for lightweight tests to avoid pulling heavy DX12 dependencies
class StubRenderer : public IRenderer {
public:
    void Initialize() override {}
    void Shutdown() override {}
    void RenderFrame() override {}
};

std::unique_ptr<IRenderer> AdapterFactory::CreateAdapter(Backend /*b*/)
{
    return std::make_unique<StubRenderer>();
}

} // namespace Renderer
#else
#include "DX12Renderer.h"

namespace Renderer {

std::unique_ptr<IRenderer> AdapterFactory::CreateAdapter(Backend b)
{
    switch (b)
    {
    case Backend::DX12:
        return std::make_unique<DX12Renderer>();
    default:
        // fallback to DX12 for now
        return std::make_unique<DX12Renderer>();
    }
}

} // namespace Renderer
#endif
