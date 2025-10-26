#pragma once

// Forward declaration
namespace Scene { class Scene; }

namespace Editor {

// Forward declaration (v1.5.0 H3.1)
class Viewport;

// Forward declaration (v1.9.0 H4.1)
class AssetBrowser;

// Forward declaration (v2.0.0 H3.1)
class MaterialEditor;

/// <summary>
/// EditorUI - Renders all editor panels (Hierarchy, Inspector, Console, Viewport, Asset Browser, Material Editor)
/// Integrated with Scene for real entity management
/// </summary>
class EditorUI {
public:
    /// <summary>
    /// Render Hierarchy panel - Shows scene tree with objects (H4.1)
    /// </summary>
    static void RenderHierarchy(Scene::Scene* scene);
    
    /// <summary>
    /// Render Inspector panel - Shows properties of selected object (H4.2)
    /// </summary>
    static void RenderInspector(Scene::Scene* scene);
    
    /// <summary>
    /// Render Console panel - Shows log messages (H4.3)
    /// </summary>
    static void RenderConsole();
    
    /// <summary>
    /// Render Viewport panel - Shows 3D scene render target (v1.5.0 H3.1)
    /// </summary>
    static void RenderViewport();
    
    /// <summary>
    /// Render Asset Browser panel - Navigate and manage project assets (v1.9.0 H4.1)
    /// </summary>
    static void RenderAssetBrowser();
    
    /// <summary>
    /// Render Material Editor panel - Create/edit PBR materials (v2.0.0 H3.1)
    /// </summary>
    static void RenderMaterialEditor();
    
    /// <summary>
    /// Render all editor panels at once (convenience method for H4.5)
    /// </summary>
    static void RenderAllPanels(Scene::Scene* scene);
    
    // v1.5.0 H3.1 - Get viewport instance (singleton pattern)
    static Viewport* GetViewport();
    
    // v1.9.0 H4.1 - Get asset browser instance (singleton pattern)
    static AssetBrowser* GetAssetBrowser();

private:
    static Viewport* s_viewport;           // Singleton instance (v1.5.0 H3.1)
    static AssetBrowser* s_assetBrowser;   // Singleton instance (v1.9.0 H4.1)
};

} // namespace Editor
