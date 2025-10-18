#include "Entity.h"
#include "Component.h"

namespace Scene {

Entity::Entity(EntityID id, const std::string& name)
    : m_id(id)
    , m_name(name)
{
}

Entity::~Entity() {
    Destroy();
}

void Entity::Update(float deltaTime) {
    // Llamar OnUpdate() de todos los components
    for (Component* component : m_components) {
        if (component) {
            component->OnUpdate(deltaTime);
        }
    }
}

void Entity::Destroy() {
    // Llamar OnDestroy() y liberar memoria de todos los components
    for (Component* component : m_components) {
        if (component) {
            component->OnDestroy();
            delete component;
        }
    }
    
    m_components.clear();
    m_componentMap.clear();
}

} // namespace Scene
