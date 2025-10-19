# Sprint Tasks - v1.8.0 (Active)

> **Referencia**: Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias de usuario del sprint.

Este archivo contiene las tareas detalladas (bajo nivel) del sprint activo v1.8.0 - Scene Graph & Entity System.

---

## Historia 1: Entity System Core (H1)

### Tarea H1.1: Crear Entity.h con clase base
**Estado**: ✅ Completada  
**Archivos afectados**: `src/scene/Entity.h` (nuevo)

**Descripción**: Crear archivo `Entity.h` con clase Entity básica incluyendo ID, nombre, parent/children pointers.

**Cambios**:
```cpp
#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace Scene {

using EntityID = uint64_t;

class Component;

class Entity {
public:
    Entity(EntityID id, const std::string& name);
    ~Entity();
    
    EntityID GetID() const { return m_id; }
    const std::string& GetName() const { return m_name; }
    void SetName(const std::string& name) { m_name = name; }
    
    // Hierarchy
    Entity* GetParent() const { return m_parent; }
    void SetParent(Entity* parent);
    const std::vector<Entity*>& GetChildren() const { return m_children; }
    
    // Components
    void AddComponent(Component* component);
    void RemoveComponent(Component* component);
    template<typename T> T* GetComponent() const;
    
private:
    EntityID m_id;
    std::string m_name;
    Entity* m_parent = nullptr;
    std::vector<Entity*> m_children;
    std::vector<Component*> m_components;
};

} // namespace Scene
```
**Commit**: Pendiente (después de compilación limpia)

---

### Tarea H1.2: Crear Entity.cpp con implementación
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/scene/Entity.cpp` (nuevo)

**Descripción**: Implementar métodos de Entity (constructor, destructor, SetParent, Add/RemoveComponent).

**Pasos**:
1. [ ] Constructor/Destructor
2. [ ] SetParent() actualiza m_parent y m_children
3. [ ] AddComponent() añade a m_components
4. [ ] GetComponent<T>() usa dynamic_cast

---

### Tarea H1.3: Crear Component.h base class
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/scene/Component.h` (nuevo)

**Descripción**: Crear Component base class abstract con virtual Update().

**Cambios**:
```cpp
#pragma once

namespace Scene {

class Entity;

class Component {
public:
    Component(Entity* owner) : m_owner(owner) {}
    virtual ~Component() = default;
    
    Entity* GetOwner() const { return m_owner; }
    virtual void Update(float deltaTime) = 0;
    
protected:
    Entity* m_owner;
};

} // namespace Scene
```

---

### Tarea H1.4: Crear EntityManager.h/cpp singleton
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/scene/EntityManager.h`, `src/scene/EntityManager.cpp` (nuevos)

**Descripción**: Implementar EntityManager singleton para crear/destruir entities y asignar IDs únicos.

**Cambios**:
```cpp
#pragma once
#include "Entity.h"
#include <unordered_map>

namespace Scene {

class EntityManager {
public:
    static EntityManager& Instance();
    
    Entity* CreateEntity(const std::string& name);
    void DestroyEntity(EntityID id);
    Entity* GetEntity(EntityID id) const;
    
private:
    EntityManager() = default;
    EntityID GenerateID();
    
    std::unordered_map<EntityID, Entity*> m_entities;
    EntityID m_nextID = 1;
};

} // namespace Scene
```

---

## Historia 2: Transform Component (H2)

### Tarea H2.1: Crear Transform.h con position/rotation/scale
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/scene/Transform.h` (nuevo)

**Descripción**: Crear Transform Component con GLM vec3/quat/mat4 para position, rotation, scale.

---

### Tarea H2.2: Implementar GetWorldMatrix()
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/scene/Transform.cpp` (nuevo)

**Descripción**: Calcular world matrix multiplicando local matrix × parent world matrix (recursivo).

---

### Tarea H2.3: Implementar SetParent() con transforms
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/scene/Entity.cpp`, `src/scene/Transform.cpp`

**Descripción**: Al cambiar parent, recalcular local transform para mantener world position.

---

### Tarea H2.4: Testing jerarquía transforms
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `tests/scene_test.cpp`

**Descripción**: Tests unitarios para verificar que transforms se heredan correctamente.

---

## Historia 3: Scene Graph Integration (H3)

### Tarea H3.1: Refactorizar Scene class con EntityManager
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/scene/Scene.h`, `src/scene/Scene.cpp`

**Descripción**: Integrar EntityManager en Scene, añadir Scene::Update() que llama Component::Update().

---

### Tarea H3.2: Renderizar entities con Transform
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Iterar entities en Scene, obtener Transform, renderizar mesh si existe.

---

### Tarea H3.3: Scene serialization básica
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/scene/SceneSerializer.h/cpp` (nuevos)

**Descripción**: Serializar/deserializar Scene a JSON (entities, transforms, jerarquía).

---

### Tarea H3.4: Testing Scene integration
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `tests/scene_test.cpp`

**Descripción**: Tests de integración (crear entities, añadir a scene, update, render).

---

## Historia 4: Editor Integration (H4)

### Tarea H4.1: Conectar Hierarchy con EntityManager
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/editor/EditorUI.cpp`

**Descripción**: Refactorizar RenderHierarchy() para iterar entities reales en lugar de placeholder.

---

### Tarea H4.2: Sistema de selección de entities
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/editor/EditorUI.h/cpp`

**Descripción**: Click en entity en Hierarchy selecciona entity (m_selectedEntity).

---

### Tarea H4.3: Inspector edita Transform
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/editor/EditorUI.cpp`

**Descripción**: RenderInspector() muestra Transform de m_selectedEntity con ImGui::DragFloat3.

---

### Tarea H4.4: Botones Create/Delete Entity
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/editor/EditorUI.cpp`

**Descripción**: Añadir botones "Create Entity" y "Delete Entity" funcionales en Hierarchy.

---

## 📊 Resumen de Tareas

| Historia | Tarea | Título | Estado |
|----------|-------|--------|--------|
| H1 | H1.1 | Crear Entity.h | ✅ Completada |
| H1 | H1.2 | Crear Entity.cpp | ⏳ Pendiente |
| H1 | H1.3 | Crear Component.h | ⏳ Pendiente |
| H1 | H1.4 | Crear EntityManager | ⏳ Pendiente |
| H2 | H2.1 | Transform.h | ⏳ Pendiente |
| H2 | H2.2 | GetWorldMatrix() | ⏳ Pendiente |
| H2 | H2.3 | SetParent() transforms | ⏳ Pendiente |
| H2 | H2.4 | Testing transforms | ⏳ Pendiente |
| H3 | H3.1 | Scene + EntityManager | ⏳ Pendiente |
| H3 | H3.2 | Renderizar entities | ⏳ Pendiente |
| H3 | H3.3 | Scene serialization | ⏳ Pendiente |
| H3 | H3.4 | Testing integration | ⏳ Pendiente |
| H4 | H4.1 | Hierarchy real | ⏳ Pendiente |
| H4 | H4.2 | Selección entities | ⏳ Pendiente |
| H4 | H4.3 | Inspector Transform | ⏳ Pendiente |
| H4 | H4.4 | Create/Delete Entity | ⏳ Pendiente |

**Total**: 16 tareas (1 completada, 15 pendientes)

---

*Última actualización*: 2025-01-18  
*Sprint*: v1.8.0 - Scene Graph & Entity System
