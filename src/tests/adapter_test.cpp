#ifdef BUILD_TEST_EXE
#include <iostream>
#include "../renderer/AdapterFactory.h"

int main()
{
    auto r = Renderer::AdapterFactory::CreateAdapter(Renderer::Backend::DX12);
    if (r) std::cout << "Adapter created\n";
    else std::cout << "Adapter creation failed\n";
    return 0;
}
#endif
