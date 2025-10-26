#pragma once

#include <string>
#include <cstdint>
#include "../assets/AssetDatabase.h"

namespace Materials {

using Assets::AssetID;

// Material properties aligned to 16 bytes for GPU constant buffer
struct MaterialProperties {
    float albedoColor[4];      // RGBA (default: 1.0, 1.0, 1.0, 1.0)
    float metallic;            // 0.0-1.0 (default: 0.0)
    float roughness;           // 0.0-1.0 (default: 0.5)
    float padding[2];          // Padding to 16-byte alignment

    MaterialProperties()
        : albedoColor{1.0f, 1.0f, 1.0f, 1.0f}
        , metallic(0.0f)
        , roughness(0.5f)
        , padding{0.0f, 0.0f}
    {
    }
};

// Material class with PBR texture slots and properties
class Material {
public:
    explicit Material(const std::string& name);
    virtual ~Material() = default;

    // Texture slots (PBR workflow)
    void SetAlbedoTexture(AssetID id);
    void SetNormalTexture(AssetID id);
    void SetRoughnessTexture(AssetID id);
    void SetMetallicTexture(AssetID id);
    void SetAOTexture(AssetID id);

    // Material properties
    void SetAlbedoColor(float r, float g, float b, float a = 1.0f);
    void SetMetallic(float value);
    void SetRoughness(float value);

    // Getters - Properties
    const MaterialProperties& GetProperties() const { return m_properties; }
    const std::string& GetName() const { return m_name; }

    // Getters - Texture IDs
    AssetID GetAlbedoTexture() const { return m_albedoTexture; }
    AssetID GetNormalTexture() const { return m_normalTexture; }
    AssetID GetRoughnessTexture() const { return m_roughnessTexture; }
    AssetID GetMetallicTexture() const { return m_metallicTexture; }
    AssetID GetAOTexture() const { return m_aoTexture; }

    // Validation
    bool IsValid() const;
    bool HasAlbedoTexture() const { return m_albedoTexture != 0; }
    bool HasNormalTexture() const { return m_normalTexture != 0; }
    bool HasRoughnessTexture() const { return m_roughnessTexture != 0; }
    bool HasMetallicTexture() const { return m_metallicTexture != 0; }
    bool HasAOTexture() const { return m_aoTexture != 0; }

protected:
    std::string m_name;
    MaterialProperties m_properties;

    // Texture slots (AssetID references)
    AssetID m_albedoTexture;
    AssetID m_normalTexture;
    AssetID m_roughnessTexture;
    AssetID m_metallicTexture;
    AssetID m_aoTexture;
};

} // namespace Materials
