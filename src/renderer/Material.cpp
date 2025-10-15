#include "Material.h"

namespace Renderer {

bool MaterialSystem::Initialize()
{
    return true;
}

void MaterialSystem::Shutdown()
{
    std::lock_guard<std::mutex> lg(mutex_);
    materials_.clear();
}

Material* MaterialSystem::CreateMaterial(const std::string& name)
{
    std::lock_guard<std::mutex> lg(mutex_);
    auto it = materials_.find(name);
    if (it != materials_.end()) return it->second.get();
    auto m = std::make_unique<Material>();
    m->name = name;
    Material* ptr = m.get();
    materials_.emplace(name, std::move(m));
    return ptr;
}

Material* MaterialSystem::GetMaterial(const std::string& name)
{
    std::lock_guard<std::mutex> lg(mutex_);
    auto it = materials_.find(name);
    return it != materials_.end() ? it->second.get() : nullptr;
}

} // namespace Renderer
