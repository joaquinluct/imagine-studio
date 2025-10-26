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

private:
    // Texture paths (static storage for drag & drop)
    static std::string s_albedoTexture;
    static std::string s_normalTexture;
    static std::string s_roughnessTexture;
    static std::string s_metallicTexture;
    static std::string s_aoTexture;
};

} // namespace Editor
