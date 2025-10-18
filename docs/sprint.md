# Sprint v1.4.0 - Scene Graph & Entity System

> **Estado**: 🚀 En progreso  
> **Fecha inicio**: 2025-01-19  
> **Fecha fin estimada**: 2025-01-26

## 🎯 Objetivo del Sprint

Implementar un **Entity-Component System (ECS) básico** con jerarquía de transformaciones (Scene Graph) que permita:
- Crear/eliminar entities en tiempo real
- Organizar entities en jerarquía padre-hijo
- Actualizar transformaciones (posición, rotación, escala) con propagación
- Conectar el sistema con los panels del Editor (Hierarchy, Inspector)

**Resultado**: Hierarchy funcional (no placeholder), Inspector edita objetos reales, flujo de trabajo de editor profesional.

## 📋 Historias de Usuario

Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias detalladas.

| ID | Historia | Prioridad | Estado |
|----|----------|-----------|--------|
| H1 | Entity System Core | CRÍTICA | ✅ Completada |
| H2 | Transform Component | CRÍTICA | ✅ Completada |
| H3 | Scene Graph Integration | ALTA | ✅ Completada |
| H4 | Editor Integration | ALTA | 🔴 Pendiente |

## 📊 Progreso

**Historias completadas**: 3/4 (75%)  
**Tareas completadas**: 12/16 (75%)

### Desglose por Historia:
- **H1 - Entity System Core**: ✅ 4/4 tareas completadas
  - H1.1 ✅ Entity class (Entity.h/cpp)
  - H1.2 ✅ Component interface (Component.h)
  - H1.3 ✅ EntityManager (factory pattern, O(1) lookup)
  - H1.4 ✅ Tests unitarios (11 tests, 100% PASSED)

- **H2 - Transform Component**: ✅ 4/4 tareas completadas
  - H2.1 ✅ Transform class (pos/rot/scale con DirectXMath)
  - H2.2 ✅ Tests Transform (15 tests, 100% PASSED)
  - H2.3 ✅ Parent-child hierarchy (GetWorldMatrix recursivo)
  - H2.4 ✅ Tests hierarchy (5 tests, 100% PASSED)

- **H3 - Scene Graph Integration**: ✅ 4/4 tareas completadas
  - H3.1 ✅ Scene class (EntityManager, CreateEntity/DestroyEntity, selection)
  - H3.2 ✅ Scene::Update() implementado (propaga deltaTime)
  - H3.3 ✅ Scene::UpdateTransforms() implementado (recalcula matrices)
  - H3.4 ✅ Tests Scene (18 tests, 100% PASSED)

- **H4 - Editor Integration**: 🔴 0/4 tareas

## 🎯 Criterios de Aceptación del Sprint

✅ **Sprint completado cuando**:
1. Sistema de entities permite crear/eliminar objetos en runtime
2. Transform component calcula matrices local→world correctamente
3. Jerarquía padre-hijo funciona (mover padre mueve hijos)
4. Hierarchy panel muestra entities reales (no placeholder)
5. Inspector muestra/edita transform de entity seleccionada
6. Usuario puede crear/eliminar entities desde Hierarchy
7. Compilación limpia (0 errores, 0 warnings)
8. Performance > 200 FPS con 100+ entities

## 📦 Entregables

- `src/scene/Entity.h/cpp` - Entity class (ID, name, components)
- `src/scene/Component.h` - Component base interface
- `src/scene/EntityManager.h/cpp` - Entity factory y storage
- `src/scene/Transform.h/cpp` - Transform component (pos/rot/scale)
- `src/scene/Scene.h/cpp` - Scene graph manager
- `tests/entity_test.cpp` - Tests unitarios ECS
- `docs/ENTITY_SYSTEM.md` - Documentación arquitectura ECS

## 🔄 Comparación con Sprint v1.3.0

| Aspecto | v1.3.0 (ImGui) | v1.4.0 (Scene Graph) |
|---------|----------------|----------------------|
| **Objetivo** | Integrar Editor UI framework | Implementar sistema de escena |
| **Tareas** | 16 | 16 |
| **Complejidad** | Media | Media-Alta |
| **Dependencias externas** | Dear ImGui (external/) | Ninguna (100% código propio) |
| **Impacto en Editor** | Panels básicos funcionales | Panels 100% funcionales (no placeholder) |
| **Performance crítica** | No | Sí (actualizar 100+ entities/frame) |

## 🧪 Validación del Sprint

**Flujo de trabajo que debe funcionar al finalizar**:

```
1. Usuario click "Create Entity" en Hierarchy
   → Aparece nueva entity en lista
   
2. Usuario selecciona entity en Hierarchy
   → Inspector muestra transform (position, rotation, scale)
   
3. Usuario edita transform.position en Inspector
   → Entity se mueve en escena (visible en próximos sprints con Viewport)
   
4. Usuario crea entity "Parent" y "Child"
   → Drag Child bajo Parent en Hierarchy
   → Child.transform hereda Parent.transform
   
5. Usuario mueve Parent
   → Child se mueve automáticamente (transform world propagado)
   
6. Usuario click "Delete Entity" en Hierarchy
   → Entity desaparece de lista y libera memoria
```

## 🚀 Próximo Sprint (v1.5.0)

**Candidato**: Viewport Real (Render Target Texture)

**Justificación**: Con Scene Graph funcional, el siguiente paso natural es ver la escena 3D renderizada dentro del panel Viewport (actualmente placeholder con texto). Esto permitirá visualizar las entities y transformaciones creadas en v1.4.0.

---

*Última actualización*: 2025-01-19
