#pragma once

namespace Scene {

class Entity;

/// @brief Interfaz base para todos los components del ECS
/// @details Components definen el comportamiento de las entities.
///          Ejemplos: Transform, MeshRenderer, Camera, etc.
///          Cada component debe heredar de esta clase e implementar OnUpdate/OnDestroy.
class Component {
public:
    /// @brief Destructor virtual
    virtual ~Component() = default;
    
    /// @brief Actualizar component (llamado cada frame)
    /// @param deltaTime Tiempo transcurrido desde el último frame (en segundos)
    virtual void OnUpdate(float deltaTime) {}
    
    /// @brief Destruir component (llamado al eliminar entity)
    virtual void OnDestroy() {}
    
    /// @brief Obtener entity propietaria de este component
    /// @return Puntero a la entity
    Entity* GetEntity() const { return m_entity; }
    
protected:
    friend class Entity; ///< Entity puede acceder a m_entity para asignarlo
    Entity* m_entity = nullptr; ///< Entity propietaria de este component
};

} // namespace Scene
