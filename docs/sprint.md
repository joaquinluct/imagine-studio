# Sprint v1.5.0 - Viewport Real (Render Target Texture)

> **Referencia**: Ver [`docs/MAIN.md`](MAIN.md) para los pilares fundamentales del proyecto.

---

## 🎯 Objetivo del Sprint

Implementar **Viewport Real** en el Editor, exponiendo el render target como SRV texture y renderizándolo dentro del panel Viewport usando `ImGui::Image()`. Incluye controles básicos de cámara (orbit, pan, zoom) y picking de objetos para selección desde el viewport.

**Resultado esperado**: Viewport 100% funcional tipo Unity/Unreal con render 3D en tiempo real dentro del panel del editor.

---

## 📅 Información del Sprint

- **Versión**: v1.5.0
- **Sprint anterior**: v1.4.0 (Scene Graph & Entity System) ✅
- **Duración estimada**: 4-6 horas (15 tareas)
- **Prioridad**: Alta (complementa v1.4.0)

---

## 🎨 Comparación con Versión Anterior (v1.4.0)

| Aspecto | v1.4.0 (Scene Graph) | v1.5.0 (Viewport Real) |
|---------|---------------------|------------------------|
| **Viewport** | ❌ Placeholder con texto | ✅ Render 3D real en tiempo real |
| **Cámara** | ❌ Hardcoded en DX12Renderer | ✅ Camera class con controles (orbit/pan/zoom) |
| **Picking** | ❌ No disponible | ✅ Raycast desde viewport para selección |
| **Resize** | ❌ No maneja resize | ✅ Resize dinámico de render target |
| **Experiencia** | ⚠️ Editor funcional pero sin vista 3D | ✅ Editor profesional tipo Unity/Unreal |

---

## 📊 Historias de Usuario (User Stories)

### **H1 - Render Target como Texture** (4 tareas)
Exponer el render target del DX12Renderer como SRV (Shader Resource View) para poder usarlo como textura en ImGui.

**Criterios de aceptación**:
- ✅ Crear SRV descriptor para render target (back buffer)
- ✅ Transiciones de estado: RENDER_TARGET → SHADER_RESOURCE
- ✅ Copiar render target a texture intermedia (si necesario)
- ✅ ImGui::Image() muestra render target sin artefactos

**Complejidad**: Alta (trabajo con descriptors DX12 y resource transitions)

---

### **H2 - Camera System** (4 tareas)
Implementar sistema de cámara con controles básicos (orbit, pan, zoom) para manipular la vista del viewport.

**Criterios de aceptación**:
- ✅ Camera class (position, target, up, fov, aspect, near/far)
- ✅ GetViewMatrix() y GetProjectionMatrix()
- ✅ Controles orbit (mouse drag derecho)
- ✅ Controles pan (mouse drag medio) y zoom (scroll)

**Complejidad**: Media (matemáticas de cámara 3D)

---

### **H3 - Viewport Integration** (4 tareas)
Integrar render target texture en el panel Viewport del editor con resize dinámico y aspect ratio correcto.

**Criterios de aceptación**:
- ✅ RenderViewport() usa ImGui::Image() con SRV del render target
- ✅ Resize dinámico del render target cuando el panel cambia de tamaño
- ✅ Aspect ratio correcto (evitar stretching)
- ✅ Performance > 200 FPS con viewport activo

**Complejidad**: Media (integración UI + render)

---

### **H4 - Picking & Selection** (3 tareas)
Implementar sistema de picking para seleccionar entities desde el viewport con mouse click.

**Criterios de aceptación**:
- ✅ Raycast desde viewport (screen space → world space)
- ✅ Intersección ray-AABB (bounding box de entities)
- ✅ Selección en Hierarchy/Inspector al hacer click en viewport

**Complejidad**: Alta (matemáticas de raycast + integración)

---

## 📈 Estado del Sprint

**Progreso**: 0/15 tareas completadas (0.0%)

| Historia | Tareas Completadas | Total Tareas | Estado |
|----------|-------------------|--------------|--------|
| H1 - Render Target como Texture | 0 | 4 | ⬜ No iniciado |
| H2 - Camera System | 0 | 4 | ⬜ No iniciado |
| H3 - Viewport Integration | 0 | 4 | ⬜ No iniciado |
| H4 - Picking & Selection | 0 | 3 | ⬜ No iniciado |

**Barra de progreso**:
```
┌────────────────────────────────────────────────────────────────────┐
│⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛ 0.0%│
└────────────────────────────────────────────────────────────────────┘
```

---

## 🎯 Objetivos de Calidad

- **Compilación limpia**: 0 errores, 0 warnings (CMake + MSBuild)
- **Performance**: > 200 FPS con viewport activo
- **Tests unitarios**: Mínimo 15 tests (Camera, Raycast, Intersection)
- **Validación visual**: Viewport muestra render 3D sin artefactos
- **Código AAA**: Siguiendo `docs/AAA_STANDARDS.md` y `docs/MAIN.md`

---

## 📦 Beneficios del Sprint

- ✅ **Viewport 100% funcional**: Ver render 3D dentro del panel del editor
- ✅ **Experiencia profesional**: Editor tipo Unity/Unreal (no solo placeholder)
- ✅ **Base para futuras features**: Gizmos, grid, manipuladores 3D, asset preview
- ✅ **Workflow completo**: Crear entity → Ver en 3D → Click → Editar en Inspector

---

## 🚀 Flujo de Trabajo Resultante

```
Usuario abre editor → Panel Viewport muestra render 3D en tiempo real →
Usuario arrastra mouse (orbit) → Cámara rota alrededor de la escena →
Usuario hace click en cube en Viewport → Cube se selecciona en Hierarchy/Inspector →
Usuario edita transform en Inspector → Cube se actualiza en Viewport 3D
```

---

## 📚 Archivos Principales a Modificar/Crear

### Nuevos archivos:
- `src/scene/Camera.h/cpp` - Camera class con controles
- `src/scene/Raycast.h/cpp` - Raycast utilities (screen → world, ray-AABB)
- `tests/camera_test.cpp` - Tests unitarios Camera
- `tests/raycast_test.cpp` - Tests unitarios Raycast

### Modificar:
- `src/renderer/DX12Renderer.h/cpp` - Crear SRV, transiciones de estado
- `src/editor/EditorUI.h/cpp` - RenderViewport con ImGui::Image()
- `src/scene/Scene.h/cpp` - Integrar Camera + Raycast

---

## 🔗 Referencias

- **Backlog**: `docs/backlog.md` - Opción A (RECOMENDADO)
- **Sprint anterior**: `docs/sprints/sprint_v1.4.0.md`
- **Pilares**: `docs/MAIN.md`
- **Estándares AAA**: `docs/AAA_STANDARDS.md`
- **Metodología**: `docs/TEMPLATE.md`

---

**Versión**: v1.0  
**Última actualización**: 2025-01-18  
**Estado**: 📋 Planificado
