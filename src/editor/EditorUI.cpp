#include "EditorUI.h"
#include "Viewport.h"       // v1.5.0 H3.1
#include "AssetBrowser.h"   // v1.9.0 H4.1
#include "../scene/Scene.h"
#include "../scene/Entity.h"
#include "../scene/Transform.h"
#include "../core/Log.h"    // v1.9.1 H2.1 - Console logs reales

#include "imgui.h"

namespace Editor {

// v1.5.0 H3.1 - Viewport singleton
Viewport* EditorUI::s_viewport = nullptr;

// v1.9.0 H4.1 - AssetBrowser singleton
AssetBrowser* EditorUI::s_assetBrowser = nullptr;

void EditorUI::RenderHierarchy(Scene::Scene* scene)
{
    ImGui::Begin("Hierarchy");
    
    if (!scene) {
        ImGui::Text("No active scene");
        ImGui::End();
        return;
    }
    
    // Buttons: Create Entity and Delete Entity
    if (ImGui::Button("Create Entity")) {
        scene->CreateEntity("New Entity");
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Delete Entity")) {
        Scene::Entity* selected = scene->GetSelectedEntity();
        if (selected) {
            scene->DestroyEntity(selected->GetID());
        }
    }
    
    ImGui::Separator();
    
    // Render tree of root entities
    const std::vector<Scene::Entity*>& rootEntities = scene->GetRootEntities();
    
    if (rootEntities.empty()) {
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Scene is empty");
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Click 'Create Entity' to add objects");
    } else {
        for (Scene::Entity* entity : rootEntities) {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
            
            // Highlight if selected
            if (entity == scene->GetSelectedEntity()) {
                flags |= ImGuiTreeNodeFlags_Selected;
            }
            
            // Leaf node (no children for now - TODO: H4 future task)
            flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            
            ImGui::TreeNodeEx(entity->GetName().c_str(), flags);
            
            // Click to select
            if (ImGui::IsItemClicked()) {
                scene->SetSelectedEntity(entity->GetID());
            }
        }
    }
    
    ImGui::End();
}

void EditorUI::RenderInspector(Scene::Scene* scene)
{
    ImGui::Begin("Inspector");
    
    if (!scene) {
        ImGui::Text("No active scene");
        ImGui::End();
        return;
    }
    
    Scene::Entity* selected = scene->GetSelectedEntity();
    
    if (!selected) {
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "No entity selected");
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Select an entity in Hierarchy panel");
        
        // Drag & Drop Target (H4.3) - Show hint when no selection
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM")) {
                const char* assetPath = static_cast<const char*>(payload->Data);
                // Cannot apply asset without selection
                ImGui::OpenPopup("NoSelectionWarning");
            }
            ImGui::EndDragDropTarget();
        }
        
        ImGui::End();
        return;
    }
    
    // Entity name
    ImGui::Text("Entity: %s", selected->GetName().c_str());
    ImGui::Text("ID: %llu", selected->GetID());
    ImGui::Separator();
    
    // Transform component
    Scene::Transform* transform = selected->GetComponent<Scene::Transform>();
    if (transform) {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            // Position
            DirectX::XMFLOAT3 pos = transform->GetPosition();
            if (ImGui::DragFloat3("Position", &pos.x, 0.1f)) {
                transform->SetPosition(pos);
            }
            
            // Rotation (Euler angles in degrees for UI)
            DirectX::XMFLOAT3 rot = transform->GetRotation();
            float rotDegrees[3] = {
                DirectX::XMConvertToDegrees(rot.x),
                DirectX::XMConvertToDegrees(rot.y),
                DirectX::XMConvertToDegrees(rot.z)
            };
            
            if (ImGui::DragFloat3("Rotation", rotDegrees, 1.0f, -180.0f, 180.0f)) {
                transform->SetRotation(
                    DirectX::XMConvertToRadians(rotDegrees[0]),
                    DirectX::XMConvertToRadians(rotDegrees[1]),
                    DirectX::XMConvertToRadians(rotDegrees[2])
                );
            }
            
            // Scale
            DirectX::XMFLOAT3 scale = transform->GetScale();
            if (ImGui::DragFloat3("Scale", &scale.x, 0.01f, 0.01f, 10.0f)) {
                transform->SetScale(scale);
            }
            
            // Drag & Drop Target (H4.3) - Apply asset to selected entity
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM")) {
                    const char* assetPath = static_cast<const char*>(payload->Data);
                    
                    // Placeholder: Log drop event
                    // TODO (future): Apply asset to entity
                    // - Texture: Add/Update MeshRenderer material
                    // - Mesh: Replace mesh component
                    // - Shader: Update material shader
                }
                ImGui::EndDragDropTarget();
            }
        }
    }
    
    ImGui::End();
}

void EditorUI::RenderConsole()
{
    ImGui::Begin("Console");
    
    // v1.9.1 H2.1-H2.2: Get logs from Log.h (real logs, not hardcoded)
    std::vector<Core::LogEntry> logs = Core::GetLogs();
    
    // Render each log entry with color by level
    for (const Core::LogEntry& entry : logs) {
        ImVec4 color;
        const char* levelStr = Core::ToString(entry.level);
        
        // Set color by log level
        switch (entry.level) {
            case Core::LogLevel::Info:  color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); break; // Green
            case Core::LogLevel::Warn:  color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); break; // Yellow
            case Core::LogLevel::Error: color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); break; // Red
            default:                    color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); break; // White
        }
        
        // Render: [LEVEL] Message
        ImGui::TextColored(color, "[%s]", levelStr);
        ImGui::SameLine();
        ImGui::Text("%s", entry.message.c_str());
    }
    
    // Auto-scroll to bottom (keeps recent logs visible)
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    
    ImGui::End();
}

void EditorUI::RenderViewport()
{
    // v1.5.0 H3.1 - Use real Viewport class
    Viewport* viewport = GetViewport();
    if (viewport)
    {
        viewport->Render();
    }
}

void EditorUI::RenderAssetBrowser()
{
    // v1.9.0 H4.1 - Use real AssetBrowser class
    AssetBrowser* assetBrowser = GetAssetBrowser();
    if (assetBrowser)
    {
        assetBrowser->Render();
    }
}

void EditorUI::RenderAllPanels(Scene::Scene* scene)
{
    // Render all editor panels in order (H4.5)
    RenderHierarchy(scene);
    RenderInspector(scene);
    RenderConsole();
    RenderViewport();
    RenderAssetBrowser(); // v1.9.0 H4.1
}

// v1.5.0 H3.1 - Get/create viewport singleton
Viewport* EditorUI::GetViewport()
{
    if (!s_viewport)
    {
        s_viewport = new Viewport();
    }
    return s_viewport;
}

// v1.9.0 H4.1 - Get/create asset browser singleton
AssetBrowser* EditorUI::GetAssetBrowser()
{
    if (!s_assetBrowser)
    {
        s_assetBrowser = new AssetBrowser();
    }
    return s_assetBrowser;
}

} // namespace Editor
