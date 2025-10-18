# Sprint Histories - v1.5.0 (Viewport Real)

> **Referencia**: Ver [`docs/sprint.md`](sprint.md) para el resumen del sprint.

---

## 📋 Historias de Usuario Detalladas

### **H1 - Render Target como Texture**

**ID**: H1  
**Prioridad**: CRÍTICA  
**Complejidad**: Alta

**Descripción**:  
Como desarrollador del motor, quiero exponer el render target de DirectX 12 como SRV (Shader Resource View) para poder usarlo como textura en ImGui y mostrarlo en el panel Viewport del editor.

**Contexto**:  
Actualmente el DX12Renderer renderiza directamente al back buffer, pero no tenemos forma de acceder a esa imagen como textura para mostrarla en ImGui. Necesitamos crear descriptors SRV y realizar transiciones de estado correctas para exponer el render target.

**Criterios de aceptación**:
- ✅ Crear descriptor heap para SRV (si no existe)
- ✅ Crear SRV descriptor apuntando al render target (back buffer)
- ✅ Implementar transiciones de estado: `D3D12_RESOURCE_STATE_RENDER_TARGET` → `D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE`
- ✅ Opcional: Crear texture intermedia si back buffer no se puede usar directamente como SRV
- ✅ API pública: `GetRenderTargetSRV()` retorna `D3D12_GPU_DESCRIPTOR_HANDLE`
- ✅ ImGui::Image() muestra el render target sin artefactos visuales
- ✅ Compilación limpia (0 errores, 0 warnings)

**Tareas derivadas**: H1.1, H1.2, H1.3, H1.4

**Archivos afectados**:
- `src/renderer/DX12Renderer.h/cpp`

**Referencias técnicas**:
- [DirectX 12 Resource Barriers](https://docs.microsoft.com/en-us/windows/win32/direct3d12/using-resource-barriers-to-synchronize-resource-states-in-direct3d-12)
- [Creating Shader Resource Views](https://docs.microsoft.com/en-us/windows/win32/direct3d12/creating-descriptor-heaps)

---

### **H2 - Camera System**

**ID**: H2  
**Prioridad**: CRÍTICA  
**Complejidad**: Media

**Descripción**:  
Como usuario del editor, quiero controlar la cámara del viewport con el mouse para poder orbitar, hacer pan y zoom en la escena 3D de manera intuitiva (similar a Unity/Unreal).

**Contexto**:  
Actualmente el DX12Renderer tiene una cámara hardcoded. Necesitamos una clase `Camera` reutilizable con controles de mouse para:
- **Orbit**: Mouse drag derecho → rotar alrededor del target
- **Pan**: Mouse drag medio → mover cámara lateralmente
- **Zoom**: Mouse scroll → acercar/alejar cámara

**Criterios de aceptación**:
- ✅ Clase `Camera` con propiedades: position, target, up, fov, aspect, near, far
- ✅ `GetViewMatrix()` retorna matriz view (look-at)
- ✅ `GetProjectionMatrix()` retorna matriz projection (perspective)
- ✅ `SetAspectRatio(float)` actualiza aspect ratio dinámicamente
- ✅ Métodos de control:
  - `Orbit(float deltaYaw, float deltaPitch)` - rotar alrededor del target
  - `Pan(float deltaX, float deltaY)` - mover cámara lateralmente
  - `Zoom(float delta)` - acercar/alejar (mover hacia target)
- ✅ Tests unitarios: 15+ tests (matrices, controles, clamp de pitch)
- ✅ Integración con InputManager (mouse drag, scroll)
- ✅ Compilación limpia (0 errores, 0 warnings)

**Tareas derivadas**: H2.1, H2.2, H2.3, H2.4

**Archivos afectados**:
- `src/scene/Camera.h/cpp` (nuevo)
- `src/editor/EditorUI.h/cpp` (integración)
- `tests/camera_test.cpp` (nuevo)

**Referencias técnicas**:
- [Camera Controls in 3D Engines](https://learnopengl.com/Getting-started/Camera)
- DirectXMath: `XMMatrixLookAtLH`, `XMMatrixPerspectiveFovLH`

---

### **H3 - Viewport Integration**

**ID**: H3  
**Prioridad**: ALTA  
**Complejidad**: Media

**Descripción**:  
Como usuario del editor, quiero ver el render 3D en tiempo real dentro del panel Viewport (en lugar de texto placeholder), con resize dinámico y aspect ratio correcto.

**Contexto**:  
El panel Viewport actualmente muestra texto placeholder. Con H1 (SRV del render target) y H2 (Camera), ahora podemos integrar todo para mostrar render 3D real usando `ImGui::Image()`.

**Criterios de aceptación**:
- ✅ `EditorUI::RenderViewport()` usa `ImGui::Image()` con SRV del render target
- ✅ Resize dinámico: cuando el panel cambia de tamaño, se ajusta el render target
  - Detectar cambio con `ImGui::GetContentRegionAvail()`
  - Llamar a `DX12Renderer::Resize(width, height)`
  - Actualizar aspect ratio de Camera
- ✅ Aspect ratio correcto (sin stretching/distorsión)
- ✅ Performance > 200 FPS con viewport activo
- ✅ UV coordinates correctas (imagen no invertida/rotada)
- ✅ Borde del panel visible (ImGui::Begin con flags correctos)
- ✅ Validación visual: usuario puede ver entities en 3D moviéndose
- ✅ Compilación limpia (0 errores, 0 warnings)

**Tareas derivadas**: H3.1, H3.2, H3.3, H3.4

**Archivos afectados**:
- `src/editor/EditorUI.h/cpp`
- `src/renderer/DX12Renderer.h/cpp` (método Resize)
- `src/scene/Scene.h/cpp` (integrar Camera)

**Referencias técnicas**:
- [ImGui::Image documentation](https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples)
- DirectX 12: Resizing swap chain

---

### **H4 - Picking & Selection**

**ID**: H4  
**Prioridad**: ALTA  
**Complejidad**: Alta

**Descripción**:  
Como usuario del editor, quiero hacer click en un objeto del viewport 3D y que se seleccione automáticamente en Hierarchy/Inspector para editarlo rápidamente.

**Contexto**:  
Actualmente solo se puede seleccionar entities desde el panel Hierarchy. Queremos picking desde el viewport usando raycast:
1. Usuario hace click en viewport (screen space coords)
2. Convertir click a ray en world space
3. Intersectar ray con bounding boxes (AABB) de entities
4. Seleccionar entity más cercano
5. Actualizar selección en Hierarchy/Inspector

**Criterios de aceptación**:
- ✅ Función `ScreenToWorldRay(screenX, screenY, camera)` → ray (origin + direction)
- ✅ Función `IntersectRayAABB(ray, aabb)` → bool + distancia
- ✅ `Scene::PickEntity(ray)` retorna entity más cercano (o nullptr)
- ✅ Integración en `EditorUI::RenderViewport()`:
  - Detectar click izquierdo con `ImGui::IsWindowHovered()` + `ImGui::IsMouseClicked(0)`
  - Obtener mouse pos relativa al viewport
  - Llamar a PickEntity y actualizar selección
- ✅ Tests unitarios: 10+ tests (raycast, intersecciones, edge cases)
- ✅ Validación visual: click en cube → se selecciona en Hierarchy/Inspector
- ✅ Compilación limpia (0 errores, 0 warnings)

**Tareas derivadas**: H4.1, H4.2, H4.3

**Archivos afectados**:
- `src/scene/Raycast.h/cpp` (nuevo)
- `src/scene/Scene.h/cpp` (método PickEntity)
- `src/editor/EditorUI.cpp` (integración)
- `tests/raycast_test.cpp` (nuevo)

**Referencias técnicas**:
- [Ray-AABB Intersection](https://gdbooks.gitbooks.io/3dcollisions/content/Chapter3/raycast_aabb.html)
- [Picking in 3D](https://antongerdelan.net/opengl/raycasting.html)

---

## 📊 Resumen de Historias

| ID | Título | Prioridad | Tareas | Estado |
|----|--------|-----------|--------|--------|
| H1 | Render Target como Texture | CRÍTICA | 4 | ⬜ No iniciado |
| H2 | Camera System | CRÍTICA | 4 | ⬜ No iniciado |
| H3 | Viewport Integration | ALTA | 4 | ⬜ No iniciado |
| H4 | Picking & Selection | ALTA | 3 | ⬜ No iniciado |

**Total**: 4 historias, 15 tareas

---

**Versión**: v1.0  
**Última actualización**: 2025-01-18  
**Sprint**: v1.5.0 (Viewport Real)
