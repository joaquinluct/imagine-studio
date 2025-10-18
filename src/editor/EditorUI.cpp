#include "EditorUI.h"
#include "../scene/Scene.h"
#include "../scene/Entity.h"
#include "../scene/Transform.h"

#include "imgui.h"

namespace Editor {

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
        }
    }
    
    ImGui::End();
}

void EditorUI::RenderConsole()
{
    ImGui::Begin("Console");
    
    // Placeholder: Logs hardcoded (TODO: integrar con Log.h en futuro)
    // Formato: [NIVEL] Mensaje
    
    // INFO logs (verde)
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "[INFO]");
    ImGui::SameLine();
    ImGui::Text("Application started successfully");
    
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "[INFO]");
    ImGui::SameLine();
    ImGui::Text("DX12Renderer initialized");
    
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "[INFO]");
    ImGui::SameLine();
    ImGui::Text("ImGui context created (v1.3.0)");
    
    // WARNING logs (amarillo)
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[WARNING]");
    ImGui::SameLine();
    ImGui::Text("Asset streaming: Low priority task cancelled");
    
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[WARNING]");
    ImGui::SameLine();
    ImGui::Text("Shader cache not found, compiling from source");
    
    // ERROR logs (rojo)
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "[ERROR]");
    ImGui::SameLine();
    ImGui::Text("Failed to load texture: missing_file.png");
    
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "[ERROR]");
    ImGui::SameLine();
    ImGui::Text("Network connection timeout (retrying...)");
    
    // Más INFO logs para demostrar scroll
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "[INFO]");
    ImGui::SameLine();
    ImGui::Text("Editor UI panels loaded: Hierarchy, Inspector, Console");
    
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "[INFO]");
    ImGui::SameLine();
    ImGui::Text("Frame rendering at 220 FPS");
    
    // Auto-scroll al final (mantiene los logs más recientes visibles)
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    
    ImGui::End();
}

void EditorUI::RenderViewport()
{
    ImGui::Begin("Viewport");
    
    // Placeholder: Información del viewport
    ImGui::Text("3D Viewport");
    ImGui::Separator();
    
    // Información técnica del render target
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Render Target:");
    ImGui::Text("  Resolution: 1920x1080 (Full HD)");
    ImGui::Text("  Format: DXGI_FORMAT_R8G8B8A8_UNORM");
    ImGui::Text("  Samples: 1 (no MSAA)");
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    // Placeholder para el render target como textura
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "TODO:");
    ImGui::TextWrapped("Expose back buffer as SRV texture and render with ImGui::Image()");
    
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Future implementation:");
    ImGui::BulletText("Create SRV for render target");
    ImGui::BulletText("Transition render target to SHADER_RESOURCE state");
    ImGui::BulletText("Render with ImGui::Image(srvGpuHandle, size)");
    ImGui::BulletText("Handle viewport resizing");
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    // Estadísticas de renderizado
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Rendering Stats:");
    ImGui::Text("  Draw Calls: 1");
    ImGui::Text("  Vertices: 6");
    ImGui::Text("  Triangles: 2");
    ImGui::Text("  Primitives: Quad");
    
    // Nota: El render 3D actualmente se muestra detrás de los panels
    // con fondo semitransparente (70%% opacidad)
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Note:");
    ImGui::TextWrapped("Currently, the 3D scene is rendered behind editor panels with semi-transparent background (70%% opacity).");
    
    ImGui::End();
}

void EditorUI::RenderAllPanels(Scene::Scene* scene)
{
    // Render all editor panels in order (H4.5)
    RenderHierarchy(scene);
    RenderInspector(scene);
    RenderConsole();
    RenderViewport();
}

} // namespace Editor
