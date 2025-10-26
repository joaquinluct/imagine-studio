#include "MaterialInstance.h"
#include "../core/Log.h"

namespace Materials {

MaterialInstance::MaterialInstance(const std::string& name, Material* baseMaterial)
    : Material(name)
    , m_baseMaterial(baseMaterial)
{
    if (!baseMaterial) {
        CORE_LOG_ERROR("MaterialInstance '{0}': Base Material is null!", name.c_str());
        return;
    }

    // Copy properties from base Material
    m_properties = baseMaterial->GetProperties();

    // Copy texture IDs from base Material
    m_albedoTexture = baseMaterial->GetAlbedoTexture();
    m_normalTexture = baseMaterial->GetNormalTexture();
    m_roughnessTexture = baseMaterial->GetRoughnessTexture();
    m_metallicTexture = baseMaterial->GetMetallicTexture();
    m_aoTexture = baseMaterial->GetAOTexture();

    CORE_LOG_INFO("MaterialInstance created: {0} (base: {1})", 
                  name.c_str(), 
                  baseMaterial->GetName().c_str());
}

bool MaterialInstance::IsValid() const {
    // MaterialInstance is valid if:
    // 1. Name is not empty
    // 2. Base Material exists and is valid
    return !m_name.empty() && m_baseMaterial != nullptr && m_baseMaterial->IsValid();
}

} // namespace Materials
