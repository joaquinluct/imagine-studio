# Sprint Histories - v1.6.0

> **Referencia**: Ver [`docs/sprint.md`](sprint.md) para objetivo general del sprint.

Este archivo contiene las historias de usuario del **Sprint v1.6.0 - Viewport AAA (Offscreen Render Target)**.

---

## Historia 1: Offscreen Render Target Creation

**ID**: H1  
**Título**: Crear render target offscreen para escena 3D  
**Prioridad**: 🔴 Crítica  
**Estimación**: 1.5 horas

### Descripción

Como desarrollador del engine, quiero crear un render target offscreen dedicado para la escena 3D, para separar el renderizado 3D del renderizado de UI y eliminar la recursión visual.

### Criterios de Aceptación

- ✅ Render target offscreen creado con `D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET`
- ✅ RTV descriptor creado en heap existente (offset después de back buffers)
- ✅ SRV descriptor creado en ImGui heap (slot 1, reemplazando SRV actual)
- ✅ Resolución: 1920x1080 (configurable en el futuro)
- ✅ Formato: `DXGI_FORMAT_R8G8B8A8_UNORM` (match swap chain)
- ✅ Estado inicial: `D3D12_RESOURCE_STATE_RENDER_TARGET`
- ✅ Clear value optimizado: dark blue (0.0, 0.2, 0.4, 1.0)

### Tareas Técnicas

- H1.1: Añadir miembros a DX12Renderer (m_sceneRenderTarget, m_sceneRTV, m_sceneSRV)
- H1.2: Implementar CreateSceneRenderTarget() en Initialize()
- H1.3: Crear RTV descriptor (offset después de BACK_BUFFER_COUNT)
- H1.4: Crear SRV descriptor (slot 1 en ImGui heap)
- H1.5: Validar creación exitosa (logs + error handling)

### Notas Técnicas

```cpp
// Heap layout RTV:
// [0] Back buffer 0 RTV
// [1] Back buffer 1 RTV
// [2] Scene RT RTV  ← Nuevo

// Heap layout ImGui SRV:
// [0] Font atlas SRV
// [1] Scene RT SRV  ← Reemplaza swap chain SRV
```

---

## Historia 2: Refactorizar OpaquePass

**ID**: H2  
**Título**: OpaquePass renderiza a scene RT (no swap chain)  
**Prioridad**: 🔴 Crítica  
**Estimación**: 1 hora

### Descripción

Como desarrollador del engine, quiero que OpaquePass renderice geometría 3D solo al scene RT offscreen, para que el contenido 3D esté completamente aislado de la UI del editor.

### Criterios de Aceptación

- ✅ OpaquePass usa `m_sceneRTV` (no `m_renderTargets[m_frameIndex]`)
- ✅ Clear scene RT con dark blue (0.0, 0.2, 0.4, 1.0)
- ✅ Renderizar quad 3D a scene RT
- ✅ NO transicionar swap chain (dejar en PRESENT)
- ✅ Transición scene RT: RENDER_TARGET → PIXEL_SHADER_RESOURCE
- ✅ No llamar Present() (eso es responsabilidad de RenderForwardPass)

### Tareas Técnicas

- H2.1: Cambiar OMSetRenderTargets() a usar m_sceneRTV
- H2.2: Cambiar ClearRenderTargetView() a scene RTV
- H2.3: Actualizar TransitionResource() a usar m_sceneRenderTarget
- H2.4: Eliminar transición del swap chain (ya no toca OpaquePass)
- H2.5: Validar que geometría se renderiza correctamente a scene RT

### Notas Técnicas

```cpp
// ANTES (v1.5.0):
// OpaquePass renderiza a m_renderTargets[m_frameIndex] (swap chain)
// Transición: PRESENT → RENDER_TARGET → PIXEL_SHADER_RESOURCE

// DESPUÉS (v1.6.0):
// OpaquePass renderiza a m_sceneRenderTarget (offscreen)
// Transición: RENDER_TARGET → PIXEL_SHADER_RESOURCE
// Swap chain no se toca en OpaquePass
```

---

## Historia 3: Refactorizar UIPass

**ID**: H3  
**Título**: UIPass renderiza UI al swap chain con scene texture  
**Prioridad**: 🔴 Crítica  
**Estimación**: 1 hora

### Descripción

Como desarrollador del engine, quiero que UIPass renderice la UI del editor al swap chain back buffer, usando el scene RT como textura en el Viewport, para eliminar la recursión visual.

### Criterios de Aceptación

- ✅ UIPass transiciona swap chain: PRESENT → RENDER_TARGET
- ✅ Clear swap chain con color del editor (dark gray 0.1, 0.1, 0.1, 1.0)
- ✅ Viewport::SetRenderTargetSRV() recibe `m_sceneSRV_GPU` (no swap chain SRV)
- ✅ ImGui::Image() muestra scene RT (solo geometría 3D, sin UI)
- ✅ ImGui renderiza UI sobre swap chain
- ✅ Transición: RENDER_TARGET → PRESENT
- ✅ Present() al final de RenderForwardPass

### Tareas Técnicas

- H3.1: Actualizar transición inicial: PIXEL_SHADER_RESOURCE → RENDER_TARGET (cambiar a PRESENT → RT)
- H3.2: Añadir ClearRenderTargetView() para swap chain (dark gray)
- H3.3: Actualizar main.cpp: pasar m_sceneSRV_GPU a Viewport (no swap chain SRV)
- H3.4: Validar que UI se renderiza sobre fondo limpio
- H3.5: Verificar que ImGui::Image() muestra scene RT sin recursión

### Notas Técnicas

```cpp
// ANTES (v1.5.0):
// UIPass lee swap chain SRV → recursión infinita
// (frame N muestra frame N-1 que contiene UI con frame N-2...)

// DESPUÉS (v1.6.0):
// UIPass lee scene RT SRV → sin recursión
// Scene RT solo contiene geometría 3D (quad)
// Swap chain contiene UI limpia sobre fondo dark gray
```

---

## Historia 4: Validación y Testing

**ID**: H4  
**Título**: Validar Viewport sin recursión visual  
**Prioridad**: 🟡 Alta  
**Estimación**: 0.5 horas

### Descripción

Como usuario del editor, quiero ver el quad 3D en el Viewport sin recursión visual, para trabajar con una vista limpia y profesional de la escena.

### Criterios de Aceptación

- ✅ Viewport muestra SOLO quad 3D con colores interpolados
- ✅ NO hay UI recursiva dentro del Viewport
- ✅ Otros panels (Hierarchy, Inspector, Console) visibles y funcionales
- ✅ Controles de cámara (orbit/pan/zoom) funcionan solo con Viewport hover
- ✅ Resize del Viewport actualiza aspect ratio sin distorsión
- ✅ 60 FPS estables con VSync
- ✅ Memoria VRAM: +8MB para scene RT (aceptable)

### Tareas Técnicas

- H4.1: Testing manual - Validar vista limpia del Viewport
- H4.2: Testing manual - Verificar controles de cámara
- H4.3: Testing manual - Resize y aspect ratio dinámico
- H4.4: Profiling - Medir frame time antes/después (no debe empeorar)
- H4.5: Marcar BACK-008 como resuelto en backlog

### Checklist de Validación

```markdown
## Validación Visual

- [ ] Viewport muestra quad 3D limpio (rojo, azul, verde, amarillo)
- [ ] NO hay panels dentro del Viewport (Hierarchy, Inspector, etc.)
- [ ] NO hay efecto espejo/recursión infinita
- [ ] Fondo del Viewport: dark blue (color del scene RT clear)
- [ ] Fondo del editor: dark gray (color del swap chain clear)

## Validación Funcional

- [ ] Botón derecho en Viewport → Orbit funciona
- [ ] Botón derecho en Hierarchy → NO afecta cámara
- [ ] Teclas +/- con Viewport hover → Zoom funciona
- [ ] Resize Viewport → Aspect ratio se actualiza correctamente

## Validación Performance

- [ ] FPS: 60 estables con VSync
- [ ] Frame time: Similar o mejor que v1.5.0
- [ ] VRAM usage: +8MB (esperado)
```

---

## 📊 Resumen de Historias

| ID | Título | Prioridad | Estimación | Tareas |
|----|--------|-----------|------------|--------|
| H1 | Offscreen RT Creation | 🔴 Crítica | 1.5h | 5 |
| H2 | Refactorizar OpaquePass | 🔴 Crítica | 1h | 5 |
| H3 | Refactorizar UIPass | 🔴 Crítica | 1h | 5 |
| H4 | Validación & Testing | 🟡 Alta | 0.5h | 5 |

**Total**: 4 historias, 20 tareas, ~4 horas estimadas

---

## 🎯 Dependencias entre Historias

```
H1 (Offscreen RT) 
    ↓
H2 (OpaquePass) → H3 (UIPass)
    ↓
H4 (Validación)
```

**Orden de implementación**: H1 → H2 → H3 → H4

---

**Versión**: 1.0  
**Última actualización**: 2025-01-18  
**Sprint**: v1.6.0 (Viewport AAA)
