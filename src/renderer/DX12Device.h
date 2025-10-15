#pragma once

namespace Renderer {

class DX12Device {
public:
    DX12Device();
    ~DX12Device();

    bool Initialize();
    void Shutdown();
};

} // namespace Renderer
