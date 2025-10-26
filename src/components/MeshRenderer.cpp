#include "MeshRenderer.h"
#include "../core/Log.h"

namespace Components {

MeshRenderer::MeshRenderer()
    : m_meshPath("")
    , m_material(nullptr)
{
    // Default: no mesh, no material
}

MeshRenderer::~MeshRenderer()
{
    // Note: Material is owned by MaterialManager, not by MeshRenderer
    // Do NOT delete m_material here
}

void MeshRenderer::SetMesh(const std::string& meshPath)
{
    m_meshPath = meshPath;
    CORE_LOG_INFO("MeshRenderer: Mesh set to: %s", meshPath.c_str());
}

const std::string& MeshRenderer::GetMesh() const
{
    return m_meshPath;
}

void MeshRenderer::SetMaterial(Materials::Material* material)
{
    m_material = material;
    
    if (material)
    {
        CORE_LOG_INFO("MeshRenderer: Material assigned: %s", material->GetName().c_str());
    }
    else
    {
        CORE_LOG_INFO("MeshRenderer: Material cleared (nullptr)");
    }
}

Materials::Material* MeshRenderer::GetMaterial() const
{
    return m_material;
}

void MeshRenderer::OnUpdate(float deltaTime)
{
    // MeshRenderer doesn't need per-frame updates
    // Rendering is handled by DX12Renderer in render loop
}

void MeshRenderer::OnDestroy()
{
    // Clear references (material is owned by MaterialManager)
    m_material = nullptr;
    m_meshPath.clear();
}

} // namespace Components
