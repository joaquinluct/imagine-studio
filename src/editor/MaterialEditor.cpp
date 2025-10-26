#include "MaterialEditor.h"
#include "core/Log.h"

// ImGui for material editor panel rendering
#include "imgui.h"

namespace Editor {

void MaterialEditor::Render()
{
    // Create Material Editor window (dockable panel)
    ImGui::Begin("Material Editor", nullptr);
    
    // Placeholder: Título y descripción
    ImGui::TextColored(ImVec4(0, 1, 1, 1), "Material Editor");
    ImGui::Text("Create and edit PBR materials");
    ImGui::Separator();
    
    // Placeholder: Botón "New Material"
    if (ImGui::Button("New Material"))
    {
        CORE_LOG_INFO("MaterialEditor: New Material button clicked (placeholder)");
    }
    
    ImGui::SameLine();
    
    // Placeholder: Botón "Save Material"
    if (ImGui::Button("Save Material"))
    {
        CORE_LOG_INFO("MaterialEditor: Save Material button clicked (placeholder)");
    }
    
    ImGui::Separator();
    
    // Placeholder: Material properties section
    if (ImGui::CollapsingHeader("Material Properties", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // Albedo color picker
        static float albedoColor[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // White default
        ImGui::ColorEdit4("Albedo Color", albedoColor);
        
        // Metallic slider
        static float metallic = 0.0f; // Default: non-metallic
        ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f);
        
        // Roughness slider
        static float roughness = 0.5f; // Default: medium roughness
        ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);
    }
    
    ImGui::Separator();
    
    // Placeholder: Texture slots section
    if (ImGui::CollapsingHeader("Texture Slots", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // Albedo texture slot
        ImGui::Text("Albedo (t0):");
        ImGui::SameLine();
        if (ImGui::Button("None##Albedo"))
        {
            CORE_LOG_INFO("MaterialEditor: Albedo texture slot clicked (placeholder)");
        }
        
        // Normal texture slot
        ImGui::Text("Normal (t1):");
        ImGui::SameLine();
        if (ImGui::Button("None##Normal"))
        {
            CORE_LOG_INFO("MaterialEditor: Normal texture slot clicked (placeholder)");
        }
        
        // Roughness texture slot
        ImGui::Text("Roughness (t2):");
        ImGui::SameLine();
        if (ImGui::Button("None##Roughness"))
        {
            CORE_LOG_INFO("MaterialEditor: Roughness texture slot clicked (placeholder)");
        }
        
        // Metallic texture slot
        ImGui::Text("Metallic (t3):");
        ImGui::SameLine();
        if (ImGui::Button("None##Metallic"))
        {
            CORE_LOG_INFO("MaterialEditor: Metallic texture slot clicked (placeholder)");
        }
        
        // AO texture slot
        ImGui::Text("AO (t4):");
        ImGui::SameLine();
        if (ImGui::Button("None##AO"))
        {
            CORE_LOG_INFO("MaterialEditor: AO texture slot clicked (placeholder)");
        }
    }
    
    ImGui::Separator();
    
    // Placeholder: Preview section
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Preview: Not implemented yet (H3.4)");
    
    ImGui::End();
}

} // namespace Editor
