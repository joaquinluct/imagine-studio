#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include <string>
#include <vector>

namespace Scene {

/// <summary>
/// Scene class - Manages collection of entities (Unity/Unreal style)
/// Provides entity lifecycle, selection, and hierarchy access
/// </summary>
class Scene {
public:
    Scene(const std::string& name = "Untitled Scene");
    ~Scene();

    // Entity management
    Entity* CreateEntity(const std::string& name = "Entity");
    void DestroyEntity(EntityID id);
    Entity* GetEntity(EntityID id);
    Entity* GetEntityByName(const std::string& name);

    // Scene lifecycle
    void Update(float deltaTime);
    void UpdateTransforms();

    // Selection (for Editor)
    void SetSelectedEntity(EntityID id);
    Entity* GetSelectedEntity() { return m_selectedEntity; }

    // Root entities (for Hierarchy panel)
    const std::vector<Entity*>& GetRootEntities() const { return m_rootEntities; }

    // Scene info
    const std::string& GetName() const { return m_name; }
    void SetName(const std::string& name) { m_name = name; }

private:
    std::string m_name;
    EntityManager m_entityManager;
    std::vector<Entity*> m_rootEntities;
    Entity* m_selectedEntity;
};

} // namespace Scene
