#pragma once

// Editor UI - AAA Editor Panels (Hierarchy, Inspector, Console, Viewport)
// v1.3.0 H4 - Editor Panels & Docking

namespace Editor {

/// <summary>
/// EditorUI class - Manages all editor panels (Unity/Unreal style)
/// </summary>
class EditorUI {
public:
    /// <summary>
    /// Render Hierarchy panel - Shows scene tree with objects (H4.1)
    /// </summary>
    static void RenderHierarchy();
    
    /// <summary>
    /// Render Inspector panel - Shows properties of selected object (H4.2)
    /// </summary>
    static void RenderInspector();
    
    /// <summary>
    /// Render Console panel - Shows log messages (H4.3)
    /// </summary>
    static void RenderConsole();
    
    /// <summary>
    /// Render Viewport panel - Shows 3D scene render target (H4.4)
    /// </summary>
    static void RenderViewport();
    
    /// <summary>
    /// Render all editor panels at once (convenience method for H4.5)
    /// </summary>
    static void RenderAllPanels();
};

} // namespace Editor
