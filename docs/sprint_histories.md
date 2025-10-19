# Sprint Histories - v1.8.0 (Active)

> **Referencia**: Ver [`docs/sprint_tasks.md`](sprint_tasks.md) para tareas detalladas del sprint.

Este archivo contiene las historias de usuario (alto nivel) del sprint activo v1.8.0 - Scene Graph & Entity System.

---

## Historia 1: Entity System Core (H1)

### H1 - Entity System Core
**ID**: H1  
**Prioridad**: 🔴 Crítica  
**Estado**: ⏳ En progreso  

**Descripción**: Implementar clases base del Entity System (Entity, Component, EntityManager) con funcionalidad core de crear/destruir entities y añadir/remover componentes.

**Criterios de aceptación**:
- [x] Entity class con ID, nombre, parent/children
- [ ] Component base class con virtual Update()
- [ ] EntityManager singleton funcional
- [ ] CreateEntity() y DestroyEntity() funcionan correctamente
- [ ] AddComponent() y GetComponent<T>() type-safe
- [ ] Tests unitarios básicos

**Tareas derivadas**: H1.1, H1.2, H1.3, H1.4

---

## Historia 2: Transform Component (H2)

### H2 - Transform Component
**ID**: H2  
**Prioridad**: 🔴 Crítica  
**Estado**: ⏳ Pendiente  

**Descripción**: Implementar Transform Component con position, rotation, scale y jerarquía padre-hijo con transforms heredados.

**Criterios de aceptación**:
- [ ] Transform Component con local position/rotation/scale
- [ ] GetWorldMatrix() calcula world transform correctamente
- [ ] SetParent() actualiza jerarquía
- [ ] Transforms se heredan de padre a hijo
- [ ] GLM integration para math (vec3, quat, mat4)

**Tareas derivadas**: H2.1, H2.2, H2.3, H2.4

---

## Historia 3: Scene Graph Integration (H3)

### H3 - Scene Graph Integration
**ID**: H3  
**Prioridad**: 🟡 Alta  
**Estado**: ⏳ Pendiente  

**Descripción**: Integrar Entity System con Scene class existente y sistema de renderizado para que entities se procesen y rendericen correctamente.

**Criterios de aceptación**:
- [ ] Scene class refactorizada con EntityManager
- [ ] Scene::Update() procesa todas las entities
- [ ] Renderer puede iterar entities y renderizar meshes
- [ ] Scene serialization básica (save/load)

**Tareas derivadas**: H3.1, H3.2, H3.3, H3.4

---

## Historia 4: Editor Integration (H4)

### H4 - Editor Integration
**ID**: H4  
**Prioridad**: 🟡 Alta  
**Estado**: ⏳ Pendiente  

**Descripción**: Conectar Entity System con panels del editor (Hierarchy, Inspector) para crear/seleccionar/editar entities en tiempo real.

**Criterios de aceptación**:
- [ ] Hierarchy muestra entities reales (no placeholder)
- [ ] Click en entity en Hierarchy selecciona entity
- [ ] Inspector muestra Transform de entity seleccionada
- [ ] Editar Transform en Inspector actualiza entity
- [ ] Botones Create Entity / Delete Entity funcionales

**Tareas derivadas**: H4.1, H4.2, H4.3, H4.4

---

## 📊 Resumen de Historias

| ID | Historia | Tareas | Completadas | Estado |
|----|----------|--------|-------------|--------|
| H1 | Entity System Core | 4 | 1 | ⏳ En progreso |
| H2 | Transform Component | 4 | 0 | ⏳ Pendiente |
| H3 | Scene Graph Integration | 4 | 0 | ⏳ Pendiente |
| H4 | Editor Integration | 4 | 0 | ⏳ Pendiente |

**Total historias**: 4 (0 completadas, 1 en progreso, 3 pendientes)

---

*Última actualización*: 2025-01-18  
*Sprint*: v1.8.0 - Scene Graph & Entity System
