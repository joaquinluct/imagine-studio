#include "EditorUI.h"

#include "imgui.h"

namespace Editor {

void EditorUI::RenderHierarchy()
{
    ImGui::Begin("Hierarchy");
    
    // Placeholder: Árbol de objetos hardcoded (placeholder scene structure)
    // Este árbol simula una estructura típica de escena 3D
    if (ImGui::TreeNode("Scene Root"))
    {
        // Camera node
        if (ImGui::TreeNode("Camera"))
        {
            ImGui::Selectable("Main Camera");
            ImGui::TreePop();
        }
        
        // Lights group
        if (ImGui::TreeNode("Lights"))
        {
            ImGui::Selectable("Directional Light");
            ImGui::Selectable("Point Light 1");
            ImGui::Selectable("Point Light 2");
            ImGui::TreePop();
        }
        
        // Geometry group
        if (ImGui::TreeNode("Geometry"))
        {
            ImGui::Selectable("Quad");
            ImGui::Selectable("Cube");
            ImGui::Selectable("Sphere");
            ImGui::TreePop();
        }
        
        ImGui::TreePop();
    }
    
    ImGui::End();
}

void EditorUI::RenderInspector()
{
    ImGui::Begin("Inspector");
    
    // Placeholder: Propiedades del objeto seleccionado
    // En el futuro, esto se sincronizará con la selección del Hierarchy
    ImGui::Text("Selected: Main Camera");
    ImGui::Separator();
    
    // Transform properties
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        static float position[3] = { 0.0f, 0.0f, -5.0f };
        static float rotation[3] = { 0.0f, 0.0f, 0.0f };
        static float scale[3] = { 1.0f, 1.0f, 1.0f };
        
        ImGui::DragFloat3("Position", position, 0.1f);
        ImGui::DragFloat3("Rotation", rotation, 1.0f, -360.0f, 360.0f);
        ImGui::DragFloat3("Scale", scale, 0.01f, 0.01f, 10.0f);
    }
    
    // Camera properties
    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
    {
        static float fov = 60.0f;
        static float nearPlane = 0.1f;
        static float farPlane = 1000.0f;
        static bool orthographic = false;
        
        ImGui::SliderFloat("FOV", &fov, 30.0f, 120.0f);
        ImGui::DragFloat("Near Plane", &nearPlane, 0.01f, 0.01f, 10.0f);
        ImGui::DragFloat("Far Plane", &farPlane, 10.0f, 10.0f, 10000.0f);
        ImGui::Checkbox("Orthographic", &orthographic);
    }
    
    ImGui::End();
}

void EditorUI::RenderConsole()
{
    ImGui::Begin("Console");
    
    // Placeholder: Logs del sistema
    // TODO H4.3: Implementar panel Console con logs funcionales
    ImGui::Text("(Console panel - coming soon in H4.3)");
    
    ImGui::End();
}

void EditorUI::RenderViewport()
{
    ImGui::Begin("Viewport");
    
    // Placeholder: Render target de la escena 3D
    // TODO H4.4: Implementar panel Viewport con render target
    ImGui::Text("(Viewport panel - coming soon in H4.4)");
    
    ImGui::End();
}

void EditorUI::RenderAllPanels()
{
    // Render all editor panels in order (H4.5)
    RenderHierarchy();
    RenderInspector();
    RenderConsole();
    RenderViewport();
}

} // namespace Editor
