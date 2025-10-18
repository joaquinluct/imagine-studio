#include "Scene.h"
#include "Transform.h"

namespace Scene {

Scene::Scene(const std::string& name)
    : m_name(name)
    , m_selectedEntity(nullptr)
{
}

Scene::~Scene()
{
    // EntityManager handles cleanup of all entities
}

Entity* Scene::CreateEntity(const std::string& name)
{
    Entity* entity = m_entityManager.CreateEntity(name);
    
    if (entity) {
        // Add Transform component by default
        entity->AddComponent<Transform>();
        
        // Add to root entities (no parent by default)
        m_rootEntities.push_back(entity);
    }
    
    return entity;
}

void Scene::DestroyEntity(EntityID id)
{
    Entity* entity = m_entityManager.GetEntity(id);
    
    if (entity) {
        // Remove from root entities if present
        auto it = std::find(m_rootEntities.begin(), m_rootEntities.end(), entity);
        if (it != m_rootEntities.end()) {
            m_rootEntities.erase(it);
        }
        
        // Clear selection if this entity is selected
        if (m_selectedEntity == entity) {
            m_selectedEntity = nullptr;
        }
        
        // Destroy entity through EntityManager
        m_entityManager.DestroyEntity(id);
    }
}

Entity* Scene::GetEntity(EntityID id)
{
    return m_entityManager.GetEntity(id);
}

Entity* Scene::GetEntityByName(const std::string& name)
{
    // Linear search through all entities
    // TODO: Optimize with name-to-entity map if needed
    for (Entity* entity : m_rootEntities) {
        if (entity->GetName() == name) {
            return entity;
        }
    }
    
    return nullptr;
}

void Scene::Update(float deltaTime)
{
    // Update all root entities
    for (Entity* entity : m_rootEntities) {
        entity->Update(deltaTime);
    }
}

void Scene::UpdateTransforms()
{
    // Update world matrices for all transforms
    for (Entity* entity : m_rootEntities) {
        Transform* transform = entity->GetComponent<Transform>();
        if (transform) {
            // Force recalc of world matrix (propagates to children)
            transform->GetWorldMatrix();
        }
    }
}

void Scene::SetSelectedEntity(EntityID id)
{
    m_selectedEntity = m_entityManager.GetEntity(id);
}

} // namespace Scene
