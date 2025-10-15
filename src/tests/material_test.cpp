#ifdef BUILD_TEST_EXE
#include <iostream>
#include "../renderer/Material.h"

int main()
{
    Renderer::MaterialSystem ms;
    if (!ms.Initialize())
    {
        std::cerr << "MaterialSystem failed to init\n";
        return 1;
    }
    auto* m = ms.CreateMaterial("Default");
    m->properties["diffuse"] = "white";
    auto* m2 = ms.GetMaterial("Default");
    std::cout << "Material: " << m2->name << " diffuse=" << m2->properties["diffuse"] << "\n";
    ms.Shutdown();
    return 0;
}
#endif
