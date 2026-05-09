#pragma once

#include <string>

namespace Editor {

/// <summary>
/// MaterialEditor - Panel ImGui para crear/editar materiales PBR
/// </summary>
class MaterialEditor {
public:
    /// <summary>
    /// Render Material Editor panel (ImGui window)
    /// </summary>
    static void Render();
    
    // Material property getters (for other systems to access)
    static const float* GetAlbedoColor() { return s_albedoColor; }
    static float GetMetallic() { return s_metallic; }
    static float GetRoughness() { return s_roughness; }
    
    static const std::string& GetAlbedoTexture() { return s_albedoTexture; }
    static const std::string& GetNormalTexture() { return s_normalTexture; }
    static const std::string& GetRoughnessTexture() { return s_roughnessTexture; }
    static const std::string& GetMetallicTexture() { return s_metallicTexture; }
    static const std::string& GetAOTexture() { return s_aoTexture; }
    static const std::string& GetCurrentMaterialName() { return s_currentMaterialName; }

private:
    // Texture paths (static storage for drag & drop)
    static std::string s_albedoTexture;
    static std::string s_normalTexture;
    static std::string s_roughnessTexture;
    static std::string s_metallicTexture;
    static std::string s_aoTexture;
    
    // Material properties (shared between Material Properties and Preview sections)
    static float s_albedoColor[4];  // RGBA
    static float s_metallic;
    static float s_roughness;
    
    // Current material name (H4.2)
    static std::string s_currentMaterialName;
};

} // namespace Editor
