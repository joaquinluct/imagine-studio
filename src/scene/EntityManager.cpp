#include "EntityManager.h"

namespace Scene {

EntityManager::EntityManager()
    : m_nextID(1)
{
}

EntityManager::~EntityManager()
{
    Clear();
}

Entity* EntityManager::CreateEntity(const std::string& name)
{
    EntityID id = m_nextID++;
    Entity* entity = new Entity(id, name);
    m_entities[id] = entity;
    return entity;
}

void EntityManager::DestroyEntity(EntityID id)
{
    auto it = m_entities.find(id);
    if (it != m_entities.end())
    {
        delete it->second;
        m_entities.erase(it);
    }
}

Entity* EntityManager::GetEntity(EntityID id)
{
    auto it = m_entities.find(id);
    return (it != m_entities.end()) ? it->second : nullptr;
}

const Entity* EntityManager::GetEntity(EntityID id) const
{
    auto it = m_entities.find(id);
    return (it != m_entities.end()) ? it->second : nullptr;
}

void EntityManager::Clear()
{
    for (auto& pair : m_entities)
    {
        delete pair.second;
    }
    m_entities.clear();
}

} // namespace Scene
