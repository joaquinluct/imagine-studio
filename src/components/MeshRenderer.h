#pragma once

#include "../scene/Component.h"
#include <string>

// Forward declarations
namespace Renderer {
    class Material;
}

namespace Components {

/// <summary>
/// MeshRenderer Component - Renders 3D mesh with material
/// Stores mesh asset ID and material pointer
/// </summary>
class MeshRenderer : public Scene::Component {
public:
    MeshRenderer();
    ~MeshRenderer();
    
    // Mesh asset
    void SetMesh(const std::string& meshPath);
    const std::string& GetMesh() const;
    
    // Material (H4.1)
    void SetMaterial(Renderer::Material* material);
    Renderer::Material* GetMaterial() const;
    
    // Component lifecycle
    void OnUpdate(float deltaTime) override;
    void OnDestroy() override;
    
private:
    std::string m_meshPath;              // Path to mesh asset (e.g., "assets/meshes/sphere.obj")
    Renderer::Material* m_material;      // Material pointer (H4.1)
};

} // namespace Components
