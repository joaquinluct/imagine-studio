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
std::string MaterialEditor::s_currentMaterialName = "Default_Material";  // H4.2

void MaterialEditor::Render()
{
    // Create Material Editor window (dockable panel)
    ImGui::Begin("Material Editor", nullptr);
    
    // Título y descripción
    ImGui::TextColored(ImVec4(0, 1, 1, 1), "Material Editor");
    ImGui::Text("Create and edit PBR materials");
    ImGui::Separator();
    
    // Material name input (H4.2)
    static char materialNameBuffer[256] = "Default_Material";
    if (ImGui::InputText("Material Name", materialNameBuffer, sizeof(materialNameBuffer))) {
        s_currentMaterialName = std::string(materialNameBuffer);
    }
    
    // Drag source for material (H4.2)
    if (ImGui::Button("Drag Material to Inspector")) {
        // This button acts as drag source
    }
    
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        // Set payload to material name
        ImGui::SetDragDropPayload("MATERIAL_EDITOR_ITEM", s_currentMaterialName.c_str(), s_currentMaterialName.size() + 1);
        
        // Preview while dragging
        ImGui::Text("Material: %s", s_currentMaterialName.c_str());
        
        ImGui::EndDragDropSource();
    }
    
    ImGui::Separator();
    
    // Botón "New Material"
    if (ImGui::Button("New Material"))
    {
        CORE_LOG_INFO("MaterialEditor: New Material button clicked (placeholder)");
    }
    
    ImGui::SameLine();
    
    // Botón "Save Material"
    if (ImGui::Button("Save Material"))
    {
        CORE_LOG_INFO("MaterialEditor: Save Material button clicked (placeholder)");
    }
    
    ImGui::Separator();
    
    // Sección de propiedades del material
    if (ImGui::CollapsingHeader("Material Properties", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // Albedo color picker
        static float albedoColor[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // Blanco por defecto
        ImGui::ColorEdit4("Albedo Color", albedoColor);
        
        // Deslizadores de Metallic y Roughness
        static float metallic = 0.0f; // Por defecto: no metálico
        ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f);
        
        static float roughness = 0.5f; // Por defecto: rugosidad media
        ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);
    }
    
    ImGui::Separator();
    
    // Sección de slots de textura con drag & drop
    if (ImGui::CollapsingHeader("Texture Slots", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // Lambda para renderizar slot de textura con drag & drop
        auto RenderTextureSlot = [](const char* label, const char* slotID, std::string& texturePath)
        {
            ImGui::Text("%s:", label);
            ImGui::SameLine();
            
            // Label del botón: mostrar nombre de textura o "None"
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
            
            // Área de Drag & Drop
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM"))
                {
                    // El payload contiene la ruta del asset (cadena null-terminated)
                    const char* droppedAsset = static_cast<const char*>(payload->Data);
                    texturePath = std::string(droppedAsset);
                    
                    CORE_LOG_INFO("MaterialEditor: Texture dropped on %s slot: %s", label, droppedAsset);
                }
                ImGui::EndDragDropTarget();
            }
            
            // Menú contextual de clic derecho: Borrar textura
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
        
        // Renderizar 5 slots de textura con drag & drop
        RenderTextureSlot("Albedo (t0)", "Albedo", s_albedoTexture);
        RenderTextureSlot("Normal (t1)", "Normal", s_normalTexture);
        RenderTextureSlot("Roughness (t2)", "Roughness", s_roughnessTexture);
        RenderTextureSlot("Metallic (t3)", "Metallic", s_metallicTexture);
        RenderTextureSlot("AO (t4)", "AO", s_aoTexture);
    }
    
    ImGui::Separator();
    
    // Sección de vista previa (H3.4) - Marcador de posición visual con info del material
    if (ImGui::CollapsingHeader("Preview", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // Obtener propiedades actuales del material
        static float albedoColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        static float metallic = 0.0f;
        static float roughness = 0.5f;
        
        // Caja de vista previa (128x128 marcador de posición)
        ImVec2 previewSize(128.0f, 128.0f);
        ImVec2 cursorPos = ImGui::GetCursorScreenPos();
        
        // Dibujar fondo (simulación de patrón de tablero de ajedrez con color)
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        
        // Dibujar rectángulo coloreado representando el albedo del material
        ImU32 albedoColorU32 = ImGui::ColorConvertFloat4ToU32(
            ImVec4(albedoColor[0], albedoColor[1], albedoColor[2], albedoColor[3])
        );
        drawList->AddRectFilled(cursorPos, 
            ImVec2(cursorPos.x + previewSize.x, cursorPos.y + previewSize.y), 
            albedoColorU32);
        
        // Dibujar borde
        drawList->AddRect(cursorPos, 
            ImVec2(cursorPos.x + previewSize.x, cursorPos.y + previewSize.y), 
            IM_COL32(255, 255, 255, 255));
        
        // Avanzar cursor
        ImGui::Dummy(previewSize);
        
        // Información resumen de material
        ImGui::Text("Material Properties:");
        ImGui::BulletText("Albedo: (%.2f, %.2f, %.2f, %.2f)", 
            albedoColor[0], albedoColor[1], albedoColor[2], albedoColor[3]);
        ImGui::BulletText("Metallic: %.2f", metallic);
        ImGui::BulletText("Roughness: %.2f", roughness);
        
        // Resumen de slots de textura
        int textureCount = 0;
        if (!s_albedoTexture.empty()) textureCount++;
        if (!s_normalTexture.empty()) textureCount++;
        if (!s_roughnessTexture.empty()) textureCount++;
        if (!s_metallicTexture.empty()) textureCount++;
        if (!s_aoTexture.empty()) textureCount++;
        
        ImGui::BulletText("Textures: %d/5 assigned", textureCount);
        
        ImGui::Separator();
        
        // Nota sobre la vista previa real (futura implementación)
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Note: Real-time 3D preview will be implemented in future task");
        ImGui::TextWrapped("Full preview requires render-to-texture with PBR lighting (deferred to H4 when materials are applied to meshes)");
    }
    
    ImGui::End();
}

} // namespace Editor
