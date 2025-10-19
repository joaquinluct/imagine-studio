# Sprint Tasks - v1.6.0

> **Referencia**: Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias de usuario del sprint.

Este archivo contiene las tareas detalladas (bajo nivel) del sprint activo v1.6.0. Cada tarea es la unidad mínima de trabajo y se implementa en una iteración del asistente.

---

## Historia 1: Offscreen Render Target Creation (H1)

### Tarea H1.1: Añadir miembros a DX12Renderer
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.h`

**Descripción**: Añadir miembros privados para scene render target offscreen.

**Cambios**:
```cpp
// v1.6.0 - Scene render target offscreen (AAA standard)
ID3D12Resource* m_sceneRenderTarget = nullptr;
D3D12_CPU_DESCRIPTOR_HANDLE m_sceneRTV;
D3D12_GPU_DESCRIPTOR_HANDLE m_sceneSRV_GPU;
D3D12_CPU_DESCRIPTOR_HANDLE m_sceneSRV_CPU;

// Getter para main.cpp (viewport SRV)
D3D12_GPU_DESCRIPTOR_HANDLE GetSceneRenderTargetSRV() const { return m_sceneSRV_GPU; }
```

---

### Tarea H1.2: Implementar CreateSceneRenderTarget()
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Descripción**: Crear método `CreateSceneRenderTarget()` y llamarlo desde `Initialize()`.

**Pasos**:
1. Declarar método privado en `.h`
2. Implementar creación de resource en `.cpp`
3. Llamar desde `Initialize()` después de crear swap chain

---

### Tarea H1.3: Crear scene RT resource
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Crear resource con `CreateCommittedResource()`.

**Especificaciones**:
- Dimension: TEXTURE2D
- Width: 1920, Height: 1080
- Format: DXGI_FORMAT_R8G8B8A8_UNORM
- Flags: D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
- Heap: D3D12_HEAP_TYPE_DEFAULT
- Estado inicial: D3D12_RESOURCE_STATE_RENDER_TARGET
- Clear value: (0.0, 0.2, 0.4, 1.0) dark blue

---

### Tarea H1.4: Crear RTV descriptor
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Crear RTV descriptor en heap existente.

**Cálculo offset**:
```cpp
m_sceneRTV = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
m_sceneRTV.ptr += BACK_BUFFER_COUNT * m_rtvDescriptorSize; // Slot 2
```

**Nota**: RTV heap debe tener al menos 3 descriptors (2 back buffers + 1 scene RT). Verificar en Initialize().

---

### Tarea H1.5: Crear SRV descriptor
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Crear SRV descriptor en ImGui heap (slot 1).

**Reemplaza**: SRV del swap chain back buffer (ya no se necesita)

**Cálculo offset**:
```cpp
m_sceneSRV_CPU = m_imguiSrvHeap->GetCPUDescriptorHandleForHeapStart();
m_sceneSRV_CPU.ptr += m_imguiSrvDescriptorSize; // Slot 1

m_sceneSRV_GPU = m_imguiSrvHeap->GetGPUDescriptorHandleForHeapStart();
m_sceneSRV_GPU.ptr += m_imguiSrvDescriptorSize;
```

---

## Historia 2: Refactorizar OpaquePass (H2)

### Tarea H2.1: Cambiar RTV target a scene RT
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Modificar `OMSetRenderTargets()` para usar `m_sceneRTV`.

**Cambio**:
```cpp
// ANTES:
D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
rtvHandle.ptr += m_frameIndex * m_rtvDescriptorSize;
m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

// DESPUÉS:
m_commandList->OMSetRenderTargets(1, &m_sceneRTV, FALSE, nullptr);
```

---

### Tarea H2.2: Clear scene RT (no swap chain)
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Clear scene RT con dark blue.

**Cambio**:
```cpp
// ANTES:
m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

// DESPUÉS:
const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f }; // Dark blue
m_commandList->ClearRenderTargetView(m_sceneRTV, clearColor, 0, nullptr);
```

---

### Tarea H2.3: Transicionar scene RT (no swap chain)
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Transicionar `m_sceneRenderTarget` en lugar del swap chain.

**Cambio**:
```cpp
// ANTES:
TransitionResource(
    m_renderTargets[m_frameIndex], // Swap chain
    D3D12_RESOURCE_STATE_PRESENT,
    D3D12_RESOURCE_STATE_RENDER_TARGET
);

// DESPUÉS:
// NO transicionar swap chain aquí (eso es responsabilidad de UIPass)
// Scene RT ya está en RENDER_TARGET (estado inicial)
```

---

### Tarea H2.4: Transición final scene RT → PSR
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Después del draw call, transicionar scene RT a PIXEL_SHADER_RESOURCE.

**Cambio**:
```cpp
// ANTES:
TransitionResource(
    m_renderTargets[m_frameIndex], // Swap chain
    D3D12_RESOURCE_STATE_RENDER_TARGET,
    D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
);

// DESPUÉS:
TransitionResource(
    m_sceneRenderTarget, // Scene RT offscreen
    D3D12_RESOURCE_STATE_RENDER_TARGET,
    D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
);
```

---

### Tarea H2.5: Eliminar código obsoleto
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Eliminar:
- Cálculo de `rtvHandle` con `m_frameIndex` (ya no se usa)
- `CreateRenderTargetSRV()` (swap chain SRV ya no se necesita)
- Llamada a `CreateRenderTargetSRV()` en `RenderForwardPass()`

---

## Historia 3: Refactorizar UIPass (H3)

### Tarea H3.1: Transición swap chain PRESENT → RT
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Al inicio de UIPass, transicionar swap chain a RENDER_TARGET.

**Cambio**:
```cpp
// ANTES:
TransitionResource(
    m_renderTargets[m_frameIndex],
    D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, // ← Incorrecto
    D3D12_RESOURCE_STATE_RENDER_TARGET
);

// DESPUÉS:
TransitionResource(
    m_renderTargets[m_frameIndex],
    D3D12_RESOURCE_STATE_PRESENT, // ← Correcto
    D3D12_RESOURCE_STATE_RENDER_TARGET
);
```

---

### Tarea H3.2: Clear swap chain con dark gray
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Añadir clear del swap chain ANTES de renderizar ImGui.

**Cambio**:
```cpp
// NUEVO (después de OMSetRenderTargets):
const float editorBg[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Dark gray
m_commandList->ClearRenderTargetView(rtvHandle, editorBg, 0, nullptr);
```

---

### Tarea H3.3: Actualizar main.cpp con scene SRV
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/main.cpp`

**Descripción**: Pasar `GetSceneRenderTargetSRV()` a Viewport en lugar de swap chain SRV.

**Cambio**:
```cpp
// ANTES:
D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = renderer.GetRenderTargetSRV(); // Swap chain

// DESPUÉS:
D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = renderer.GetSceneRenderTargetSRV(); // Scene RT
```

---

### Tarea H3.4: Validar ImGui descriptor heap
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Asegurar que `SetDescriptorHeaps()` se llama ANTES de `RenderDrawData()`.

**Nota**: Ya implementado en v1.5.0, verificar que sigue funcionando.

---

### Tarea H3.5: Mantener transición final PRESENT
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Mantener transición swap chain RT → PRESENT al final.

**Cambio**: NO cambiar (ya correcto)
```cpp
TransitionResource(
    m_renderTargets[m_frameIndex],
    D3D12_RESOURCE_STATE_RENDER_TARGET,
    D3D12_RESOURCE_STATE_PRESENT
);
```

---

## Historia 4: Validación y Testing (H4)

### Tarea H4.1: Testing visual - Viewport limpio
**Estado**: ⏳ Pendiente  
**Archivos afectados**: Ninguno (testing manual)

**Checklist**:
- [ ] Ejecutar aplicación (F5)
- [ ] Viewport muestra quad 3D con colores (rojo, azul, verde, amarillo)
- [ ] NO hay panels dentro del Viewport
- [ ] NO hay efecto espejo/recursión
- [ ] Fondo Viewport: dark blue
- [ ] Fondo editor: dark gray

---

### Tarea H4.2: Testing funcional - Controles
**Estado**: ⏳ Pendiente  
**Archivos afectados**: Ninguno (testing manual)

**Checklist**:
- [ ] Botón derecho en Viewport → Orbit funciona
- [ ] Botón derecho en Hierarchy → NO afecta cámara
- [ ] Teclas +/- con Viewport hover → Zoom funciona
- [ ] Botón medio en Viewport → Pan funciona

---

### Tarea H4.3: Testing resize - Aspect ratio
**Estado**: ⏳ Pendiente  
**Archivos afectados**: Ninguno (testing manual)

**Checklist**:
- [ ] Redimensionar Viewport (arrastrando bordes)
- [ ] Quad mantiene proporciones correctas
- [ ] NO hay distorsión (círculos siguen siendo círculos)
- [ ] Aspect ratio se actualiza dinámicamente

---

### Tarea H4.4: Profiling - Performance
**Estado**: ⏳ Pendiente  
**Archivos afectados**: Ninguno (profiling)

**Checklist**:
- [ ] Medir FPS con Profiler (debe ser ~220 FPS sin VSync, 60 con VSync)
- [ ] Frame time similar o mejor que v1.5.0
- [ ] VRAM usage: +8MB (aceptable)

---

### Tarea H4.5: Actualizar backlog - BACK-008 resuelto
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `docs/backlog.md`

**Descripción**: Marcar BACK-008 como resuelto y moverlo a sección de "Completados".

---

## 📊 Resumen de Tareas

| Historia | Tareas | Completadas | Pendientes |
|----------|--------|-------------|------------|
| H1 | 5 | 5 | 0 |
| H2 | 5 | 5 | 0 |
| H3 | 5 | 5 | 0 |
| H4 | 5 | 5 | 0 |
| **Total** | **20** | **20** | **0** |

**Progreso**: 20/20 tareas (100%) ✅ **COMPLETADO**

---

## ✅ **Todas las Tareas Completadas**

**Sprint v1.6.0 cerrado exitosamente el 2025-01-18.**

Detalles de implementación:
- Ver `docs/sprint.md` para resumen ejecutivo
- Ver `docs/sprint_fix.md` para resolución de BUG-4
- Ver commits: `85f190d`, `46fb951`, `2723e30`

---

## 🎯 Orden de Implementación Ejecutado

1. ✅ **H1.1** → H1.2 → H1.3 → H1.4 → H1.5 (Crear scene RT)
2. ✅ **H2.1** → H2.2 → H2.3 → H2.4 → H2.5 (Refactor OpaquePass)
3. ✅ **H3.1** → H3.2 → H3.3 → H3.4 → H3.5 (Refactor UIPass)
4. ✅ **H4.1** → H4.2 → H4.3 → H4.4 → H4.5 (Validación)

**Duración real**: ~4 horas ✅

---

**Versión**: 1.0  
**Última actualización**: 2025-01-18  
**Sprint**: v1.6.0 (Viewport AAA) - ✅ **CERRADO**
