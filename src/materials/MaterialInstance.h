#pragma once

#include "Material.h"

namespace Materials {

// MaterialInstance: Lightweight variant of a Material
// Shares shader/PSO with base Material, but has unique properties and textures
// Use case: Multiple objects with same Material but different colors/properties
// Example: Red brick wall, Blue brick wall (same Material, different instances)
class MaterialInstance : public Material {
public:
    // Create instance from base Material
    explicit MaterialInstance(const std::string& name, Material* baseMaterial);
    virtual ~MaterialInstance() = default;

    // Get base Material (shader source)
    Material* GetBaseMaterial() const { return m_baseMaterial; }

    // Validation (must have valid base Material)
    bool IsValid() const;

private:
    Material* m_baseMaterial; // Base Material that defines shader/PSO
};

} // namespace Materials
