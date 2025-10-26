#include "Material.h"
#include "../core/Log.h"
#include <algorithm>

namespace Materials {

Material::Material(const std::string& name)
    : m_name(name)
    , m_properties()
    , m_albedoTexture(0)
    , m_normalTexture(0)
    , m_roughnessTexture(0)
    , m_metallicTexture(0)
    , m_aoTexture(0)
{
    CORE_LOG_INFO("Material created: {0}", name.c_str());
}

// ===== Texture Slots =====

void Material::SetAlbedoTexture(AssetID id) {
    m_albedoTexture = id;
    if (id != 0) {
        CORE_LOG_INFO("Material '{0}': Albedo texture set (AssetID: {1})", m_name.c_str(), id);
    }
}

void Material::SetNormalTexture(AssetID id) {
    m_normalTexture = id;
    if (id != 0) {
        CORE_LOG_INFO("Material '{0}': Normal texture set (AssetID: {1})", m_name.c_str(), id);
    }
}

void Material::SetRoughnessTexture(AssetID id) {
    m_roughnessTexture = id;
    if (id != 0) {
        CORE_LOG_INFO("Material '{0}': Roughness texture set (AssetID: {1})", m_name.c_str(), id);
    }
}

void Material::SetMetallicTexture(AssetID id) {
    m_metallicTexture = id;
    if (id != 0) {
        CORE_LOG_INFO("Material '{0}': Metallic texture set (AssetID: {1})", m_name.c_str(), id);
    }
}

void Material::SetAOTexture(AssetID id) {
    m_aoTexture = id;
    if (id != 0) {
        CORE_LOG_INFO("Material '{0}': AO texture set (AssetID: {1})", m_name.c_str(), id);
    }
}

// ===== Material Properties =====

void Material::SetAlbedoColor(float r, float g, float b, float a) {
    m_properties.albedoColor[0] = std::max(0.0f, std::min(1.0f, r));
    m_properties.albedoColor[1] = std::max(0.0f, std::min(1.0f, g));
    m_properties.albedoColor[2] = std::max(0.0f, std::min(1.0f, b));
    m_properties.albedoColor[3] = std::max(0.0f, std::min(1.0f, a));
}

void Material::SetMetallic(float value) {
    m_properties.metallic = std::max(0.0f, std::min(1.0f, value));
}

void Material::SetRoughness(float value) {
    m_properties.roughness = std::max(0.0f, std::min(1.0f, value));
}

// ===== Validation =====

bool Material::IsValid() const {
    // Material is valid if name is not empty
    return !m_name.empty();
}

} // namespace Materials
