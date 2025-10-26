#pragma once

#include "Material.h"
#include "MaterialInstance.h"
#include <unordered_map>
#include <string>
#include <mutex>

namespace Materials {

// MaterialManager: Singleton for managing all materials in the engine
// Features:
// - Material cache (avoid duplicates)
// - Hot-reload support (detect file changes)
// - Thread-safe access
class MaterialManager {
public:
    // Get singleton instance
    static MaterialManager& Instance();

    // Material creation
    Material* CreateMaterial(const std::string& name);
    MaterialInstance* CreateMaterialInstance(const std::string& name, Material* baseMaterial);

    // Material retrieval
    Material* GetMaterial(const std::string& name);
    bool HasMaterial(const std::string& name) const;

    // Material removal
    void RemoveMaterial(const std::string& name);

    // Get all materials
    const std::unordered_map<std::string, Material*>& GetAllMaterials() const;

    // Hot-reload (future implementation)
    void ReloadMaterial(const std::string& name);
    void CheckForChanges(); // Call every frame to detect file changes

    // Cleanup
    void ClearAll();

    // Delete copy/move constructors (singleton)
    MaterialManager(const MaterialManager&) = delete;
    MaterialManager& operator=(const MaterialManager&) = delete;

private:
    MaterialManager() = default;
    ~MaterialManager();

    // Material storage
    std::unordered_map<std::string, Material*> m_materials;

    // Thread safety
    mutable std::mutex m_mutex;
};

} // namespace Materials
