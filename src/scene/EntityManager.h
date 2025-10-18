#pragma once

#include "Entity.h"
#include <unordered_map>
#include <string>

namespace Scene {

class EntityManager {
public:
    EntityManager();
    ~EntityManager();

    // Factory methods
    Entity* CreateEntity(const std::string& name = "Entity");
    void DestroyEntity(EntityID id);

    // Query methods
    Entity* GetEntity(EntityID id);
    const Entity* GetEntity(EntityID id) const;

    // Batch operations
    void Clear();

    // Info
    size_t GetEntityCount() const { return m_entities.size(); }

private:
    EntityID m_nextID;
    std::unordered_map<EntityID, Entity*> m_entities;
};

} // namespace Scene
