#include "MaterialManager.h"
#include "../core/Log.h"

namespace Materials {

MaterialManager& MaterialManager::Instance() {
    static MaterialManager instance;
    return instance;
}

MaterialManager::~MaterialManager() {
    ClearAll();
}

// ===== Material Creation =====

Material* MaterialManager::CreateMaterial(const std::string& name) {
    std::lock_guard<std::mutex> lock(m_mutex);

    // Check if material already exists
    if (m_materials.find(name) != m_materials.end()) {
        CORE_LOG_WARN("MaterialManager: Material '{0}' already exists. Returning existing material.", name.c_str());
        return m_materials[name];
    }

    // Create new material
    Material* material = new Material(name);
    m_materials[name] = material;

    CORE_LOG_INFO("MaterialManager: Material '{0}' created and registered.", name.c_str());
    return material;
}

MaterialInstance* MaterialManager::CreateMaterialInstance(const std::string& name, Material* baseMaterial) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!baseMaterial) {
        CORE_LOG_ERROR("MaterialManager: Cannot create MaterialInstance '{0}' with null base Material.", name.c_str());
        return nullptr;
    }

    // Check if material already exists
    if (m_materials.find(name) != m_materials.end()) {
        CORE_LOG_WARN("MaterialManager: MaterialInstance '{0}' already exists. Returning existing material.", name.c_str());
        return static_cast<MaterialInstance*>(m_materials[name]);
    }

    // Create new material instance
    MaterialInstance* instance = new MaterialInstance(name, baseMaterial);
    m_materials[name] = instance;

    CORE_LOG_INFO("MaterialManager: MaterialInstance '{0}' created (base: '{1}').", 
                  name.c_str(), 
                  baseMaterial->GetName().c_str());
    return instance;
}

// ===== Material Retrieval =====

Material* MaterialManager::GetMaterial(const std::string& name) {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_materials.find(name);
    if (it != m_materials.end()) {
        return it->second;
    }

    CORE_LOG_WARN("MaterialManager: Material '{0}' not found.", name.c_str());
    return nullptr;
}

bool MaterialManager::HasMaterial(const std::string& name) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_materials.find(name) != m_materials.end();
}

// ===== Material Removal =====

void MaterialManager::RemoveMaterial(const std::string& name) {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_materials.find(name);
    if (it != m_materials.end()) {
        delete it->second;
        m_materials.erase(it);
        CORE_LOG_INFO("MaterialManager: Material '{0}' removed.", name.c_str());
    } else {
        CORE_LOG_WARN("MaterialManager: Cannot remove material '{0}' (not found).", name.c_str());
    }
}

// ===== Get All Materials =====

const std::unordered_map<std::string, Material*>& MaterialManager::GetAllMaterials() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_materials;
}

// ===== Hot-Reload (Placeholder) =====

void MaterialManager::ReloadMaterial(const std::string& name) {
    std::lock_guard<std::mutex> lock(m_mutex);

    // TODO: Implement hot-reload logic
    // 1. Detect file change (FileWatcher)
    // 2. Reload textures from disk
    // 3. Update GPU resources
    // 4. Notify renderer

    CORE_LOG_INFO("MaterialManager: ReloadMaterial('{0}') called (placeholder).", name.c_str());
}

void MaterialManager::CheckForChanges() {
    // TODO: Implement FileWatcher
    // Check for file changes in assets/textures/ and assets/materials/
    // Call ReloadMaterial() for changed materials
}

// ===== Cleanup =====

void MaterialManager::ClearAll() {
    std::lock_guard<std::mutex> lock(m_mutex);

    CORE_LOG_INFO("MaterialManager: Clearing all materials ({0} materials).", m_materials.size());

    for (auto& pair : m_materials) {
        delete pair.second;
    }

    m_materials.clear();
}

} // namespace Materials
