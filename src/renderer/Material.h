#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>

namespace Renderer {

struct Material {
    std::string name;
    // simple property map (could be enums, textures, values, etc.)
    std::unordered_map<std::string, std::string> properties;
};

class MaterialSystem {
public:
    MaterialSystem() = default;
    ~MaterialSystem() = default;

    bool Initialize();
    void Shutdown();

    Material* CreateMaterial(const std::string& name);
    Material* GetMaterial(const std::string& name);

private:
    std::mutex mutex_;
    std::unordered_map<std::string, std::unique_ptr<Material>> materials_;
};

} // namespace Renderer
