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
    // TODO H4.2: Implementar panel Inspector con propiedades editables
    ImGui::Text("(Inspector panel - coming soon in H4.2)");
    
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
