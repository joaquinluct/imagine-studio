#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <typeindex>
#include <unordered_map>

namespace Scene {

using EntityID = uint64_t;
class Component;

/// @brief Entity representa un objeto en la escena con ID �nico, nombre y components
/// @details Entity-Component pattern: Entity es un contenedor de Components
///          que definen el comportamiento del objeto (Transform, MeshRenderer, etc.)
class Entity {
public:
    /// @brief Constructor de Entity
    /// @param id ID �nico de la entity (generado por EntityManager)
    /// @param name Nombre de la entity (para debugging/UI)
    Entity(EntityID id, const std::string& name);
    
    /// @brief Destructor - libera todos los components
    ~Entity();
    
    // ===== Getters/Setters b�sicos =====
    
    /// @brief Obtener ID �nico de la entity
    EntityID GetID() const { return m_id; }
    
    /// @brief Obtener nombre de la entity
    const std::string& GetName() const { return m_name; }
    
    /// @brief Establecer nombre de la entity
    void SetName(const std::string& name) { m_name = name; }
    
    // ===== Component Management =====
    
    /// @brief A�adir component a la entity
    /// @tparam T Tipo del component (debe heredar de Component)
    /// @return Puntero al component creado
    template<typename T>
    T* AddComponent();
    
    /// @brief Obtener component de tipo T
    /// @tparam T Tipo del component
    /// @return Puntero al component o nullptr si no existe
    template<typename T>
    T* GetComponent();
    
    /// @brief Comprobar si la entity tiene component de tipo T
    /// @tparam T Tipo del component
    /// @return true si tiene el component, false en caso contrario
    template<typename T>
    bool HasComponent() const;
    
    /// @brief Obtener todos los components de la entity
    const std::vector<Component*>& GetComponents() const { return m_components; }
    
    // ===== Lifecycle =====
    
    /// @brief Actualizar entity (llama OnUpdate de todos los components)
    /// @param deltaTime Tiempo transcurrido desde el �ltimo frame (en segundos)
    void Update(float deltaTime);
    
    /// @brief Destruir entity (llama OnDestroy de todos los components)
    void Destroy();
    
private:
    EntityID m_id;                                      ///< ID �nico de la entity
    std::string m_name;                                 ///< Nombre de la entity
    std::vector<Component*> m_components;               ///< Lista de components
    std::unordered_map<std::type_index, Component*> m_componentMap; ///< Lookup r�pido por tipo
};

// ===== Template Implementations =====

template<typename T>
T* Entity::AddComponent() {
    // Verificar que T hereda de Component (compile-time check)
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
    
    // Verificar si ya tiene este component
    std::type_index typeIndex(typeid(T));
    if (m_componentMap.find(typeIndex) != m_componentMap.end()) {
        return static_cast<T*>(m_componentMap[typeIndex]);
    }
    
    // Crear nuevo component
    T* component = new T();
    component->m_entity = this;
    
    // Almacenar en vector y map
    m_components.push_back(component);
    m_componentMap[typeIndex] = component;
    
    return component;
}

template<typename T>
T* Entity::GetComponent() {
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
    
    std::type_index typeIndex(typeid(T));
    auto it = m_componentMap.find(typeIndex);
    
    if (it != m_componentMap.end()) {
        return static_cast<T*>(it->second);
    }
    
    return nullptr;
}

template<typename T>
bool Entity::HasComponent() const {
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
    
    std::type_index typeIndex(typeid(T));
    return m_componentMap.find(typeIndex) != m_componentMap.end();
}

} // namespace Scene
