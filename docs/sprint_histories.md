# Sprint Histories - v1.4.0 (Scene Graph & Entity System)

> **Referencia**: Ver [`docs/sprint.md`](sprint.md) para el objetivo general del sprint.

Este archivo contiene las historias de usuario del Sprint v1.4.0, que implementa el Entity-Component System (ECS) básico con jerarquía de transformaciones (Scene Graph).

---

## H1 - Entity System Core

**ID**: H1  
**Prioridad**: CRÍTICA  
**Estado**: 🔴 Pendiente

**Descripción**:  
Como desarrollador del engine, necesito un sistema básico de entities y components para poder crear objetos de juego con comportamiento modular (ECS pattern).

**Contexto técnico**:
- Entity = ID único + nombre + lista de components
- Component = interfaz base con `OnUpdate()`, `OnDestroy()`
- EntityManager = factory para crear/eliminar entities, storage con `std::unordered_map<EntityID, Entity>`
- IDs generados con contador incremental (uint64_t)

**Criterios de aceptación**:
- ✅ Clase `Entity` con ID, nombre, y lista de components (`std::vector<Component*>`)
- ✅ Interfaz `Component` con métodos virtuales `OnUpdate()`, `OnDestroy()`
- ✅ Clase `EntityManager` para crear/eliminar entities
- ✅ Método `EntityManager::GetEntity(EntityID)` con `O(1)` lookup
- ✅ Tests unitarios (crear 1000 entities, eliminar 500, verificar memoria)

**Tareas derivadas**: H1.1, H1.2, H1.3, H1.4

---

## H2 - Transform Component

**ID**: H2  
**Prioridad**: CRÍTICA  
**Estado**: 🔴 Pendiente

**Descripción**:  
Como desarrollador del engine, necesito un componente Transform que gestione posición, rotación y escala de entities, con soporte para jerarquía padre-hijo (cálculo de matrices local→world).

**Contexto técnico**:
- Transform almacena: `position` (vec3), `rotation` (quaternion), `scale` (vec3)
- Matriz local = TRS (Translation × Rotation × Scale)
- Matriz world = parent.matrixWorld × matrixLocal (propagación recursiva)
- Dirty flag para recalcular solo cuando cambien valores

**Criterios de aceptación**:
- ✅ Clase `Transform : public Component` con pos/rot/scale
- ✅ Métodos `SetPosition()`, `SetRotation()`, `SetScale()`
- ✅ Método `GetLocalMatrix()` calcula TRS matrix
- ✅ Método `GetWorldMatrix()` calcula con propagación de padre
- ✅ Soporte para parent/child hierarchy (punteros Entity*)
- ✅ Dirty flag optimization (no recalcular si no cambió)
- ✅ Tests unitarios (verificar matrices, jerarquía 3 niveles)

**Tareas derivadas**: H2.1, H2.2, H2.3, H2.4

---

## H3 - Scene Graph Integration

**ID**: H3  
**Prioridad**: ALTA  
**Estado**: 🔴 Pendiente

**Descripción**:  
Como desarrollador del engine, necesito una clase Scene que gestione la colección de entities y actualice sus transforms en cada frame (scene graph).

**Contexto técnico**:
- Scene almacena lista de root entities (`std::vector<Entity*>`)
- Método `Scene::Update()` llama `OnUpdate()` de todos components
- Método `Scene::UpdateTransforms()` recalcula matrices world de jerarquía
- Método `Scene::Render()` itera entities con MeshRenderer component (futuro sprint)

**Criterios de aceptación**:
- ✅ Clase `Scene` con lista de entities
- ✅ Métodos `AddEntity()`, `RemoveEntity()`, `GetEntity(name)`
- ✅ Método `Update()` llama `OnUpdate()` de components
- ✅ Método `UpdateTransforms()` propaga matrices en jerarquía
- ✅ Performance: actualizar 100+ entities < 1ms
- ✅ Tests unitarios (crear scene, añadir entities, update)

**Tareas derivadas**: H3.1, H3.2, H3.3, H3.4

---

## H4 - Editor Integration

**ID**: H4  
**Prioridad**: ALTA  
**Estado**: 🔴 Pendiente

**Descripción**:  
Como usuario del editor, necesito que el panel Hierarchy muestre entities reales de la escena (no placeholder), y el panel Inspector permita editar sus transforms en tiempo real.

**Contexto técnico**:
- Hierarchy obtiene lista de entities de `Scene::GetRootEntities()`
- Inspector muestra transform de entity seleccionada (`Scene::GetSelectedEntity()`)
- Botones "Create Entity" y "Delete Entity" llaman `Scene::AddEntity()` / `RemoveEntity()`
- ImGui widgets: `ImGui::DragFloat3()` para position/scale, `ImGui::SliderAngle()` para rotation

**Criterios de aceptación**:
- ✅ Hierarchy muestra entities reales (no hardcoded)
- ✅ Click en entity en Hierarchy selecciona entity (`Scene::SetSelectedEntity()`)
- ✅ Inspector muestra transform (position, rotation, scale) de entity seleccionada
- ✅ Editar valores en Inspector actualiza transform real
- ✅ Botón "Create Entity" crea nueva entity en scene
- ✅ Botón "Delete Entity" elimina entity seleccionada
- ✅ Jerarquía visual (indentación) muestra parent/child relationships

**Tareas derivadas**: H4.1, H4.2, H4.3, H4.4

---

## 📊 Resumen de Historias

| ID | Título | Tareas | Prioridad | Complejidad |
|----|--------|--------|-----------|-------------|
| H1 | Entity System Core | 4 | CRÍTICA | Media |
| H2 | Transform Component | 4 | CRÍTICA | Media-Alta |
| H3 | Scene Graph Integration | 4 | ALTA | Media |
| H4 | Editor Integration | 4 | ALTA | Baja-Media |

**Total**: 4 historias, 16 tareas

---

*Última actualización*: 2025-01-19
