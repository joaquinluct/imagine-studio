#include "MaterialEditor.h"
#include "core/Log.h"

// ImGui for material editor panel rendering
#include "imgui.h"

namespace Editor {

// Static member initialization
std::string MaterialEditor::s_albedoTexture = "";
std::string MaterialEditor::s_normalTexture = "";
std::string MaterialEditor::s_roughnessTexture = "";
std::string MaterialEditor::s_metallicTexture = "";
std::string MaterialEditor::s_aoTexture = "";

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
        // Helper lambda: Render texture slot with drag & drop
        auto RenderTextureSlot = [](const char* label, const char* slotID, std::string& texturePath)
        {
            ImGui::Text("%s:", label);
            ImGui::SameLine();
            
            // Button label: show texture name or "None"
            std::string buttonLabel = texturePath.empty() ? "None" : texturePath;
            buttonLabel += "##" + std::string(slotID);
            
            if (ImGui::Button(buttonLabel.c_str()))
            {
                if (!texturePath.empty())
                {
                    CORE_LOG_INFO("MaterialEditor: %s texture clicked: %s", label, texturePath.c_str());
                }
                else
                {
                    CORE_LOG_INFO("MaterialEditor: %s texture slot clicked (empty)", label);
                }
            }
            
            // Drag & Drop Target
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM"))
                {
                    // Payload contains asset path (null-terminated string)
                    const char* droppedAsset = static_cast<const char*>(payload->Data);
                    texturePath = std::string(droppedAsset);
                    
                    CORE_LOG_INFO("MaterialEditor: Texture dropped on %s slot: %s", label, droppedAsset);
                }
                ImGui::EndDragDropTarget();
            }
            
            // Right-click context menu: Clear texture
            if (ImGui::BeginPopupContextItem(slotID))
            {
                if (ImGui::MenuItem("Clear Texture"))
                {
                    if (!texturePath.empty())
                    {
                        CORE_LOG_INFO("MaterialEditor: Cleared %s texture: %s", label, texturePath.c_str());
                        texturePath.clear();
                    }
                }
                ImGui::EndPopup();
            }
        };
        
        // Render 5 texture slots with drag & drop
        RenderTextureSlot("Albedo (t0)", "Albedo", s_albedoTexture);
        RenderTextureSlot("Normal (t1)", "Normal", s_normalTexture);
        RenderTextureSlot("Roughness (t2)", "Roughness", s_roughnessTexture);
        RenderTextureSlot("Metallic (t3)", "Metallic", s_metallicTexture);
        RenderTextureSlot("AO (t4)", "AO", s_aoTexture);
    }
    
    ImGui::Separator();
    
    // Placeholder: Preview section
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Preview: Not implemented yet (H3.4)");
    
    ImGui::End();
}

} // namespace Editor
