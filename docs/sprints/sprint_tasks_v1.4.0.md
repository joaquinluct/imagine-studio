# Sprint Tasks - v1.4.0 (Scene Graph & Entity System)

> **Referencia**: Ver [`docs/sprint_histories.md`](sprint_histories.md) para las historias de usuario completas.

Este archivo contiene las tareas granulares del Sprint v1.4.0, organizadas por historia.

---

## H1 - Entity System Core (4 tareas)

### H1.1 - Crear Entity class básica

**ID**: H1.1  
**Historia**: H1  
**Estado**: 🔴 Pendiente

**Descripción**:  
Implementar clase `Entity` con ID único, nombre, y lista de components.

**Implementación**:
```cpp
// src/scene/Entity.h
#pragma once
#include <cstdint>
#include <string>
#include <vector>

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
    
    template<typename T>
    T* AddComponent();
    
    template<typename T>
    T* GetComponent();
    
    template<typename T>
    bool HasComponent() const;
    
    void Update(float deltaTime);
    void Destroy();
    
private:
    EntityID m_id;
    std::string m_name;
    std::vector<Component*> m_components;
};

} // namespace Scene
```

**Archivos afectados**:
- `src/scene/Entity.h` (nuevo)
- `src/scene/Entity.cpp` (nuevo)

**Criterio de éxito**:
- Compilación limpia (0 errores, 0 warnings)
- Entity creada con ID y nombre
- `GetID()` y `GetName()` devuelven valores correctos

---

### H1.2 - Crear Component interface

**ID**: H1.2  
**Historia**: H1  
**Estado**: 🔴 Pendiente

**Descripción**:  
Implementar interfaz base `Component` con métodos virtuales para update y destrucción.

**Implementación**:
```cpp
// src/scene/Component.h
#pragma once

namespace Scene {

class Entity;

class Component {
public:
    virtual ~Component() = default;
    
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnDestroy() {}
    
    Entity* GetEntity() const { return m_entity; }
    
protected:
    friend class Entity;
    Entity* m_entity = nullptr;
};

} // namespace Scene
```

**Archivos afectados**:
- `src/scene/Component.h` (nuevo)

**Criterio de éxito**:
- Compilación limpia
- Interfaz Component es heredable
- `OnUpdate()` y `OnDestroy()` son virtuales

---

### H1.3 - Implementar EntityManager

**ID**: H1.3  
**Historia**: H1  
**Estado**: 🔴 Pendiente

**Descripción**:  
Implementar factory pattern para crear/eliminar entities con storage eficiente.

**Implementación**:
```cpp
// src/scene/EntityManager.h
#pragma once
#include "Entity.h"
#include <unordered_map>

namespace Scene {

class EntityManager {
public:
    EntityManager();
    ~EntityManager();
    
    Entity* CreateEntity(const std::string& name = "Entity");
    void DestroyEntity(EntityID id);
    
    Entity* GetEntity(EntityID id);
    const Entity* GetEntity(EntityID id) const;
    
    void Clear();
    
    size_t GetEntityCount() const { return m_entities.size(); }
    
private:
    EntityID m_nextID = 1;
    std::unordered_map<EntityID, Entity*> m_entities;
};

} // namespace Scene
```

**Archivos afectados**:
- `src/scene/EntityManager.h` (nuevo)
- `src/scene/EntityManager.cpp` (nuevo)

**Criterio de éxito**:
- `CreateEntity()` genera ID único incremental
- `GetEntity()` lookup O(1)
- `DestroyEntity()` libera memoria correctamente
- `Clear()` elimina todas las entities

---

### H1.4 - Tests unitarios Entity System

**ID**: H1.4  
**Historia**: H1  
**Estado**: 🔴 Pendiente

**Descripción**:  
Crear tests unitarios para validar Entity, Component y EntityManager.

**Implementación**:
```cpp
// tests/entity_test.cpp
#include "../src/scene/Entity.h"
#include "../src/scene/EntityManager.h"
#include <cassert>
#include <iostream>

void TestEntityCreation() {
    Scene::EntityManager mgr;
    
    // Test 1: Create entity
    Scene::Entity* e1 = mgr.CreateEntity("Player");
    assert(e1 != nullptr);
    assert(e1->GetName() == "Player");
    assert(e1->GetID() == 1);
    
    // Test 2: Create multiple entities
    Scene::Entity* e2 = mgr.CreateEntity("Enemy");
    assert(e2->GetID() == 2);
    
    // Test 3: Get entity by ID
    Scene::Entity* found = mgr.GetEntity(1);
    assert(found == e1);
    
    // Test 4: Destroy entity
    mgr.DestroyEntity(1);
    assert(mgr.GetEntity(1) == nullptr);
    
    std::cout << "EntityTest: All tests passed!" << std::endl;
}

int main() {
    TestEntityCreation();
    return 0;
}
```

**Archivos afectados**:
- `tests/entity_test.cpp` (nuevo)
- `CMakeLists.txt` (añadir entity_test executable)

**Criterio de éxito**:
- Test compila y ejecuta sin errores
- Todos los asserts pasan
- Memory leaks = 0 (validar con valgrind/ASAN)

---

## H2 - Transform Component (4 tareas)

### H2.1 - Crear Transform class con pos/rot/scale

**ID**: H2.1  
**Historia**: H2  
**Estado**: 🔴 Pendiente

**Descripción**:  
Implementar component Transform con position, rotation (quaternion), scale.

**Implementación**:
```cpp
// src/scene/Transform.h
#pragma once
#include "Component.h"
#include <DirectXMath.h>

namespace Scene {

using namespace DirectX;

class Transform : public Component {
public:
    Transform();
    ~Transform() override = default;
    
    // Getters
    XMFLOAT3 GetPosition() const { return m_position; }
    XMFLOAT4 GetRotation() const { return m_rotation; }
    XMFLOAT3 GetScale() const { return m_scale; }
    
    // Setters
    void SetPosition(const XMFLOAT3& pos);
    void SetRotation(const XMFLOAT4& rot);
    void SetScale(const XMFLOAT3& scale);
    
    // Matrix calculations
    XMMATRIX GetLocalMatrix() const;
    XMMATRIX GetWorldMatrix() const;
    
    // Hierarchy
    void SetParent(Transform* parent);
    Transform* GetParent() const { return m_parent; }
    const std::vector<Transform*>& GetChildren() const { return m_children; }
    
private:
    XMFLOAT3 m_position = {0.0f, 0.0f, 0.0f};
    XMFLOAT4 m_rotation = {0.0f, 0.0f, 0.0f, 1.0f}; // identity quaternion
    XMFLOAT3 m_scale = {1.0f, 1.0f, 1.0f};
    
    Transform* m_parent = nullptr;
    std::vector<Transform*> m_children;
    
    mutable bool m_isDirty = true;
    mutable XMMATRIX m_cachedLocalMatrix;
    mutable XMMATRIX m_cachedWorldMatrix;
};

} // namespace Scene
```

**Archivos afectados**:
- `src/scene/Transform.h` (nuevo)
- `src/scene/Transform.cpp` (nuevo)

**Criterio de éxito**:
- Transform almacena pos/rot/scale correctamente
- Setters marcan `m_isDirty = true`
- Compilación limpia

---

### H2.2 - Implementar cálculo de matriz local (TRS)

**ID**: H2.2  
**Historia**: H2  
**Estado**: 🔴 Pendiente

**Descripción**:  
Calcular matriz local = Translation × Rotation × Scale con DirectXMath.

**Implementación**:
```cpp
// src/scene/Transform.cpp
#include "Transform.h"

namespace Scene {

XMMATRIX Transform::GetLocalMatrix() const {
    if (!m_isDirty) {
        return m_cachedLocalMatrix;
    }
    
    // TRS matrix: Translation × Rotation × Scale
    XMVECTOR pos = XMLoadFloat3(&m_position);
    XMVECTOR rot = XMLoadFloat4(&m_rotation);
    XMVECTOR scale = XMLoadFloat3(&m_scale);
    
    XMMATRIX matTranslation = XMMatrixTranslationFromVector(pos);
    XMMATRIX matRotation = XMMatrixRotationQuaternion(rot);
    XMMATRIX matScale = XMMatrixScalingFromVector(scale);
    
    m_cachedLocalMatrix = matScale * matRotation * matTranslation;
    m_isDirty = false;
    
    return m_cachedLocalMatrix;
}

} // namespace Scene
```

**Archivos afectados**:
- `src/scene/Transform.cpp`

**Criterio de éxito**:
- Matriz local calculada correctamente (TRS order)
- Dirty flag evita recalcular si no cambió
- Tests: verificar matriz identidad, matriz con valores

---

### H2.3 - Implementar jerarquía padre-hijo

**ID**: H2.3  
**Historia**: H2  
**Estado**: 🔴 Pendiente

**Descripción**:  
Permitir establecer parent/child relationships entre transforms.

**Implementación**:
```cpp
// src/scene/Transform.cpp (continuación)

void Transform::SetParent(Transform* parent) {
    // Remove from previous parent
    if (m_parent) {
        auto& siblings = m_parent->m_children;
        siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
    }
    
    // Set new parent
    m_parent = parent;
    
    // Add to new parent's children
    if (m_parent) {
        m_parent->m_children.push_back(this);
    }
    
    m_isDirty = true;
}

XMMATRIX Transform::GetWorldMatrix() const {
    XMMATRIX localMatrix = GetLocalMatrix();
    
    if (m_parent) {
        XMMATRIX parentWorldMatrix = m_parent->GetWorldMatrix();
        return localMatrix * parentWorldMatrix;
    }
    
    return localMatrix;
}
```

**Archivos afectados**:
- `src/scene/Transform.cpp`

**Criterio de éxito**:
- `SetParent()` actualiza parent/child correctamente
- `GetWorldMatrix()` propaga matrices recursivamente
- Tests: jerarquía 3 niveles (grand-parent → parent → child)

---

### H2.4 - Tests unitarios Transform

**ID**: H2.4  
**Historia**: H2  
**Estado**: 🔴 Pendiente

**Descripción**:  
Validar cálculos de matrices y jerarquía con tests unitarios.

**Implementación**:
```cpp
// tests/transform_test.cpp
#include "../src/scene/Transform.h"
#include <cassert>
#include <iostream>

void TestTransformMatrices() {
    Scene::Transform t;
    
    // Test 1: Identity matrix
    t.SetPosition({0.0f, 0.0f, 0.0f});
    t.SetScale({1.0f, 1.0f, 1.0f});
    XMMATRIX mat = t.GetLocalMatrix();
    // Verify identity matrix
    
    // Test 2: Translation
    t.SetPosition({10.0f, 5.0f, 0.0f});
    mat = t.GetLocalMatrix();
    XMFLOAT4X4 matFloat;
    XMStoreFloat4x4(&matFloat, mat);
    assert(matFloat._41 == 10.0f && matFloat._42 == 5.0f);
    
    // Test 3: Hierarchy
    Scene::Transform parent, child;
    child.SetParent(&parent);
    parent.SetPosition({10.0f, 0.0f, 0.0f});
    child.SetPosition({5.0f, 0.0f, 0.0f});
    
    XMMATRIX childWorld = child.GetWorldMatrix();
    XMStoreFloat4x4(&matFloat, childWorld);
    assert(matFloat._41 == 15.0f); // 10 + 5
    
    std::cout << "TransformTest: All tests passed!" << std::endl;
}

int main() {
    TestTransformMatrices();
    return 0;
}
```

**Archivos afectados**:
- `tests/transform_test.cpp` (nuevo)
- `CMakeLists.txt`

**Criterio de éxito**:
- Tests pasan correctamente
- Matrices calculadas son correctas
- Jerarquía propaga transformaciones

---

## H3 - Scene Graph Integration (4 tareas)

### H3.1 - Crear Scene class

**ID**: H3.1  
**Historia**: H3  
**Estado**: 🔴 Pendiente

**Descripción**:  
Implementar clase Scene para gestionar colección de entities.

**Implementación**:
```cpp
// src/scene/Scene.h
#pragma once
#include "Entity.h"
#include "EntityManager.h"
#include <vector>
#include <string>

namespace Scene {

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
    void Render(); // Placeholder (render loop en futuro sprint)
    
    // Selection
    void SetSelectedEntity(EntityID id);
    Entity* GetSelectedEntity() { return m_selectedEntity; }
    
    // Root entities (para hierarchy visual)
    const std::vector<Entity*>& GetRootEntities() const { return m_rootEntities; }
    
private:
    std::string m_name;
    EntityManager m_entityManager;
    std::vector<Entity*> m_rootEntities;
    Entity* m_selectedEntity = nullptr;
};

} // namespace Scene
```

**Archivos afectados**:
- `src/scene/Scene.h` (nuevo)
- `src/scene/Scene.cpp` (nuevo)

**Criterio de éxito**:
- Scene gestiona entities correctamente
- `CreateEntity()` / `DestroyEntity()` funcionales
- Compilación limpia

---

### H3.2 - Implementar Scene::Update()

**ID**: H3.2  
**Historia**: H3  
**Estado**: 🔴 Pendiente

**Descripción**:  
Llamar `OnUpdate()` de todos los components en cada frame.

**Implementación**:
```cpp
// src/scene/Scene.cpp

void Scene::Update(float deltaTime) {
    // Update all entities
    for (Entity* entity : m_rootEntities) {
        entity->Update(deltaTime);
        
        // Recursively update children (via Transform hierarchy)
        // TODO: implement in Entity::Update()
    }
}
```

**Archivos afectados**:
- `src/scene/Scene.cpp`

**Criterio de éxito**:
- `Update()` llama `Entity::Update()` de todas las entities
- DeltaTime se propaga correctamente
- Performance < 1ms para 100+ entities

---

### H3.3 - Implementar Scene::UpdateTransforms()

**ID**: H3.3  
**Historia**: H3  
**Estado**: 🔴 Pendiente

**Descripción**:  
Recalcular matrices world de toda la jerarquía de transforms.

**Implementación**:
```cpp
// src/scene/Scene.cpp

void Scene::UpdateTransforms() {
    // Traverse hierarchy and update world matrices
    for (Entity* entity : m_rootEntities) {
        Transform* t = entity->GetComponent<Transform>();
        if (t) {
            // Force recalc of world matrix (propagates to children)
            t->GetWorldMatrix();
        }
    }
}
```

**Archivos afectados**:
- `src/scene/Scene.cpp`

**Criterio de éxito**:
- Matrices world se recalculan en orden (parent → children)
- Dirty flags funcionan correctamente
- Tests: mover parent, verificar child se movió

---

### H3.4 - Tests unitarios Scene

**ID**: H3.4  
**Historia**: H3  
**Estado**: 🔴 Pendiente

**Descripción**:  
Validar Scene con tests unitarios (crear/eliminar entities, update).

**Implementación**:
```cpp
// tests/scene_test.cpp
#include "../src/scene/Scene.h"
#include <cassert>
#include <iostream>

void TestScene() {
    Scene::Scene scene("Test Scene");
    
    // Test 1: Create entity
    Scene::Entity* e1 = scene.CreateEntity("Player");
    assert(e1 != nullptr);
    
    // Test 2: Get entity
    Scene::Entity* found = scene.GetEntity(e1->GetID());
    assert(found == e1);
    
    // Test 3: Update scene
    scene.Update(0.016f); // 60 FPS
    scene.UpdateTransforms();
    
    // Test 4: Selection
    scene.SetSelectedEntity(e1->GetID());
    assert(scene.GetSelectedEntity() == e1);
    
    std::cout << "SceneTest: All tests passed!" << std::endl;
}

int main() {
    TestScene();
    return 0;
}
```

**Archivos afectados**:
- `tests/scene_test.cpp` (nuevo)
- `CMakeLists.txt`

**Criterio de éxito**:
- Tests pasan sin errores
- Scene funciona correctamente
- Memory leaks = 0

---

## H4 - Editor Integration (4 tareas)

### H4.1 - Conectar Hierarchy con Scene

**ID**: H4.1  
**Historia**: H4  
**Estado**: 🔴 Pendiente

**Descripción**:  
Reemplazar Hierarchy placeholder con entities reales de Scene.

**Implementación**:
```cpp
// src/editor/EditorUI.cpp (modificar RenderHierarchy)

void EditorUI::RenderHierarchy() {
    ImGui::Begin("Hierarchy");
    
    // Obtener scene actual (global o singleton)
    extern Scene::Scene* g_currentScene;
    
    if (g_currentScene) {
        // Botón Create Entity
        if (ImGui::Button("Create Entity")) {
            g_currentScene->CreateEntity("New Entity");
        }
        
        ImGui::SameLine();
        
        // Botón Delete Entity
        if (ImGui::Button("Delete Entity")) {
            Scene::Entity* selected = g_currentScene->GetSelectedEntity();
            if (selected) {
                g_currentScene->DestroyEntity(selected->GetID());
            }
        }
        
        ImGui::Separator();
        
        // Listar entities reales
        for (Scene::Entity* entity : g_currentScene->GetRootEntities()) {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
            
            // Highlight si está seleccionada
            if (entity == g_currentScene->GetSelectedEntity()) {
                flags |= ImGuiTreeNodeFlags_Selected;
            }
            
            bool nodeOpen = ImGui::TreeNodeEx(entity->GetName().c_str(), flags);
            
            // Click para seleccionar
            if (ImGui::IsItemClicked()) {
                g_currentScene->SetSelectedEntity(entity->GetID());
            }
            
            if (nodeOpen) {
                // TODO: Renderizar hijos (Transform children)
                ImGui::TreePop();
            }
        }
    } else {
        ImGui::Text("No active scene");
    }
    
    ImGui::End();
}
```

**Archivos afectados**:
- `src/editor/EditorUI.cpp`
- `src/main.cpp` (crear Scene global)

**Criterio de éxito**:
- Hierarchy muestra entities reales
- Create/Delete buttons funcionales
- Click selecciona entity

---

### H4.2 - Conectar Inspector con Transform

**ID**: H4.2  
**Historia**: H4  
**Estado**: 🔴 Pendiente

**Descripción**:  
Inspector muestra/edita Transform de entity seleccionada.

**Implementación**:
```cpp
// src/editor/EditorUI.cpp (modificar RenderInspector)

void EditorUI::RenderInspector() {
    ImGui::Begin("Inspector");
    
    extern Scene::Scene* g_currentScene;
    
    if (g_currentScene) {
        Scene::Entity* selected = g_currentScene->GetSelectedEntity();
        
        if (selected) {
            // Nombre entity
            ImGui::Text("Entity: %s", selected->GetName().c_str());
            ImGui::Separator();
            
            // Transform component
            Scene::Transform* t = selected->GetComponent<Scene::Transform>();
            if (t) {
                ImGui::Text("Transform");
                
                // Position
                XMFLOAT3 pos = t->GetPosition();
                if (ImGui::DragFloat3("Position", &pos.x, 0.1f)) {
                    t->SetPosition(pos);
                }
                
                // Rotation (euler angles para UI)
                XMFLOAT4 rot = t->GetRotation();
                // TODO: Convert quaternion → euler for display
                ImGui::DragFloat3("Rotation", &rot.x, 1.0f, -180.0f, 180.0f);
                
                // Scale
                XMFLOAT3 scale = t->GetScale();
                if (ImGui::DragFloat3("Scale", &scale.x, 0.01f)) {
                    t->SetScale(scale);
                }
            }
        } else {
            ImGui::Text("No entity selected");
        }
    } else {
        ImGui::Text("No active scene");
    }
    
    ImGui::End();
}
```

**Archivos afectados**:
- `src/editor/EditorUI.cpp`

**Criterio de éxito**:
- Inspector muestra transform de entity seleccionada
- Editar valores actualiza transform real
- ImGui::DragFloat3 funciona correctamente

---

### H4.3 - Implementar Create/Delete Entity

**ID**: H4.3  
**Historia**: H4  
**Estado**: 🔴 Pendiente

**Descripción**:  
Botones en Hierarchy para crear/eliminar entities.

**Implementación**:  
(Ya implementado en H4.1, solo validar)

**Archivos afectados**:
- `src/editor/EditorUI.cpp`

**Criterio de éxito**:
- Click "Create Entity" → nueva entity aparece en Hierarchy
- Click "Delete Entity" → entity seleccionada desaparece
- Memory leaks = 0 al eliminar entities

---

### H4.4 - Tests de integración Editor

**ID**: H4.4  
**Historia**: H4  
**Estado**: 🔴 Pendiente

**Descripción**:  
Validación manual del flujo completo en editor.

**Tests manuales**:
1. Ejecutar aplicación (F5)
2. Click "Create Entity" → verificar aparece en Hierarchy
3. Click en entity → verificar Inspector muestra transform
4. Editar position en Inspector → verificar valores actualizados
5. Crear 2nd entity, seleccionar 1st entity, click "Delete" → verificar desaparece
6. Crear parent y child, mover parent → verificar child se mueve (validar en logs)

**Archivos afectados**:
- N/A (tests manuales)

**Criterio de éxito**:
- Todos los tests manuales pasan
- No crashes, no memory leaks
- Performance > 200 FPS con 100+ entities

---

## 📊 Resumen de Tareas

| Historia | Tareas | Complejidad | Archivos Nuevos | Archivos Modificados |
|----------|--------|-------------|-----------------|----------------------|
| H1 | 4 | Media | 6 | 1 (CMakeLists.txt) |
| H2 | 4 | Media-Alta | 4 | 1 (CMakeLists.txt) |
| H3 | 4 | Media | 4 | 1 (CMakeLists.txt) |
| H4 | 4 | Baja-Media | 0 | 2 (EditorUI.cpp, main.cpp) |

**Total**: 16 tareas, ~14 archivos nuevos, ~5 archivos modificados

---

*Última actualización*: 2025-01-19
