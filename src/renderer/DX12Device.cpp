#include "DX12Device.h"
#include <iostream>

namespace Renderer {

DX12Device::DX12Device() {}
DX12Device::~DX12Device() {}

bool DX12Device::Initialize()
{
    std::cout << "DX12Device: Initialize (stub)\n";
    return true;
}

void DX12Device::Shutdown()
{
    std::cout << "DX12Device: Shutdown (stub)\n";
}

} // namespace Renderer
