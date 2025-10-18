# Sprint Commits - v1.5.0 (Viewport Real)

> **Referencia**: Ver [`docs/sprint.md`](sprint.md) para objetivo y progreso del sprint.

Este archivo registra todos los commits realizados durante el **Sprint v1.5.0 - Viewport Real (Render Target Texture)**.

---

## Convenciones de Commits

Formato recomendado para los mensajes de commit:

```
<tipo>(<ámbito>): <mensaje corto>

<mensaje largo opcional>

Refs: <issue/track-id>
```

### Tipos comunes:
- **feat**: Nueva funcionalidad
- **fix**: Corrección de errores
- **docs**: Cambios en documentación
- **style**: Formato, estilo, sin cambios funcionales
- **refactor**: Refactorización sin cambios en comportamiento
- **perf**: Mejoras de rendimiento
- **test**: Añadir o corregir tests
- **chore**: Tareas de mantenimiento

### Ejemplo:
```
feat(renderer): implementar SRV descriptor heap

Crear descriptor heap para Shader Resource Views usado por ImGui.
Configurado como SHADER_VISIBLE para acceso desde GPU.

Refs: H1.1
```

---

## Historial de Commits (Sprint v1.5.0)

### 2025-01-18

#### `34b8d30` - feat renderer H1.3 Implementar helper TransitionResource para gestión de estados

**Tipo**: Feature (Renderer)  
**Ámbito**: Sprint v1.5.0 - H1.3  
**Descripción**: Implementar método helper `TransitionResource()` para simplificar gestión de transiciones de estado de recursos DX12

**Implementación**:
- Crear método `TransitionResource(resource, stateBefore, stateAfter)`:
  - Valida resource y command list antes de crear barrier
  - Optimización: Skip transition si estados son iguales
  - Simplifica código eliminando repetición de barriers
- Refactorizar `OpaquePass()` para usar helper method:
  - Transición `PRESENT` → `RENDER_TARGET` antes de renderizar
  - Transición `RENDER_TARGET` → `PRESENT` después de renderizar
  - Añadir comentario: En H3.1 cambiará a `PIXEL_SHADER_RESOURCE` para Viewport
- Refactorizar `UIPass()` para usar helper method:
  - Transición `PRESENT` → `RENDER_TARGET` antes de UI
  - Transición `RENDER_TARGET` → `PRESENT` después de UI

**Beneficios**:
- ✅ Código más limpio y mantenible (DRY)
- ✅ Consistencia en manejo de barriers
- ✅ Facilita futuras transiciones para SRV (H3.1)
- ✅ Optimización: evita transiciones innecesarias

**Preparación para H3.1**:
Cuando se implemente Viewport con `ImGui::Image()`, solo se necesitará cambiar:
```cpp
// En OpaquePass(), cambiar:
TransitionResource(rt, RENDER_TARGET, PRESENT);
// Por:
TransitionResource(rt, RENDER_TARGET, PIXEL_SHADER_RESOURCE);
```

**Archivos modificados**:
- `src/renderer/DX12Renderer.h` (declaración TransitionResource)
- `src/renderer/DX12Renderer.cpp` (implementación + refactor OpaquePass/UIPass)

**Compilación**: ✅ Limpia (CMake + MSBuild: 0 errores, 0 warnings)

**Próxima tarea**: H1.4 - Validación Visual SRV

**Referencia**: Sprint v1.5.0 - H1.3

---

#### `7afbbf8` - feat renderer H1.2 Actualizar SRV descriptor para render target dinámico

**Tipo**: Feature (Renderer)  
**Ámbito**: Sprint v1.5.0 - H1.2  
**Descripción**: Actualizar SRV descriptor dinámicamente cuando cambia el back buffer actual (m_frameIndex)

**Implementación**:
- Modificar `CreateRenderTargetSRV()` para regenerar SRV con `m_renderTargets[m_frameIndex]` actual
- Llamar a `CreateRenderTargetSRV()` después de `Present()` en `RenderForwardPass()`
- Asegurar que `GetRenderTargetSRV()` siempre retorna handle válido apuntando al back buffer correcto
- Añadir logging con número de frame index para debugging

**Razón**:
- El swap chain alterna entre 2 back buffers (double buffering)
- `m_frameIndex` cambia después de cada `Present()`
- El SRV debe actualizarse para apuntar al back buffer correcto

**Fix adicional**:
- Corregir typo (otra instancia): `D3D_PRIMITIVE_TOPOLOGY_TRIANGLE` → `D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST`

**Archivos modificados**:
- `src/renderer/DX12Renderer.cpp` (actualizar CreateRenderTargetSRV + llamada en RenderForwardPass)

**Compilación**: ✅ Limpia (CMake + MSBuild: 0 errores, 0 warnings)

**Próxima tarea**: H1.3 - Implementar Resource State Transitions

**Referencia**: Sprint v1.5.0 - H1.2

---

#### `49a39db` - feat renderer H1.1 Expandir descriptor heap ImGui SRV para viewport texture

**Tipo**: Feature (Renderer)  
**Ámbito**: Sprint v1.5.0 - H1.1  
**Descripción**: Expandir descriptor heap ImGui SRV de 1 a 2 descriptors para soportar render target texture del viewport

**Implementación**:
- Expandir `m_imguiSrvHeap` de 1 a 2 descriptors
  - Slot 0: ImGui font atlas SRV (existente)
  - Slot 1: Render target SRV (nuevo - v1.5.0)
- Implementar método `CreateRenderTargetSRV()`:
  - Generar SRV descriptor del back buffer actual
  - Handles CPU/GPU apuntando a slot 1
  - Formato: `DXGI_FORMAT_R8G8B8A8_UNORM` (coincide con swap chain)
  - ViewDimension: `D3D12_SRV_DIMENSION_TEXTURE2D`
- Añadir `m_imguiSrvDescriptorSize` para calcular offsets dentro del heap
- Añadir getter `GetRenderTargetSRV()` para acceso externo
- Llamar a `CreateRenderTargetSRV()` al final de `Initialize(HWND)`

**Fix adicional**:
- Corregir typo: `D3D_PRIMITIVE_TOPOLOGY_TRIANGLE` → `D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST`

**Archivos creados/modificados**:
- `src/renderer/DX12Renderer.h` (declaraciones + miembros nuevos)
- `src/renderer/DX12Renderer.cpp` (implementación + fix typo)

**Compilación**: ✅ Limpia (CMake + MSBuild: 0 errores, 0 warnings)

**Próxima tarea**: H1.2 - Crear SRV Descriptor para Render Target

**Referencia**: Sprint v1.5.0 - H1.1

---

#### `77d7473` - docs: Planificación Sprint v1.5.0 - Viewport Real

**Tipo**: Docs (Planificación)  
**Ámbito**: Sprint v1.5.0  
**Descripción**: Planificación completa del Sprint v1.5.0 - Viewport Real (Render Target Texture)

**Objetivo**: Implementar Viewport Real con render 3D en tiempo real dentro del panel del editor usando `ImGui::Image()`. Incluye controles de cámara (orbit/pan/zoom) y picking de objetos.

**Historias (4)**:
- **H1**: Render Target como Texture (4 tareas)
- **H2**: Camera System (4 tareas)
- **H3**: Viewport Integration (4 tareas)
- **H4**: Picking & Selection (3 tareas)

**Total**: 15 tareas  
**Duración estimada**: 4-6 horas  
**Prioridad**: Alta (complementa v1.4.0)

**Archivos creados/modificados**:
- `docs/sprint.md`
- `docs/sprint_histories.md`
- `docs/sprint_tasks.md`
- `docs/daily.md`

**Compilación**: ✅ Limpia (CMake + MSBuild: 0 errores, 0 warnings)

**Próxima tarea**: H1.1 - Crear Descriptor Heap para SRV

**Referencia**: Sprint v1.5.0 - Viewport Real

---

#### `65b39e5` - refactor(docs): Renombrar commits.md a sprint_commits.md

**Tipo**: Refactor (Documentación)  
**Ámbito**: Docs  
**Descripción**: Renombrar commits.md a sprint_commits.md para versionado por sprint

**Cambios**:
- Renombrar `commits.md` → `sprint_commits.md`
- Historizar commits previos (v1.0.0-1.4.0) en `docs/sprints/sprint_commits_v1.0.0-1.4.0.md`
- Limpiar `sprint_commits.md` para Sprint v1.5.0
- Actualizar `close-sprint.ps1` para incluir `sprint_commits.md` en archivado
- Actualizar referencias en `.github/copilot-instructions.md`
- Actualizar referencias en `docs/TEMPLATE.md`

**Beneficios**:
- ✅ Commits versionados por sprint (como otros archivos)
- ✅ `sprint_commits.md` se archiva al cerrar sprint
- ✅ Historial limpio y organizado en `docs/sprints/`
- ✅ Coherencia con metodología (`sprint_*.md`)

**Archivos creados**:
- `docs/sprint_commits.md` (limpio para v1.5.0)
- `docs/sprints/sprint_commits_v1.0.0-1.4.0.md` (histórico)

**Archivos modificados**:
- `scripts/close-sprint.ps1` (incluir sprint_commits.md)
- `.github/copilot-instructions.md` (actualizar referencias)
- `docs/TEMPLATE.md` (actualizar referencias)

**Compilación**: ✅ Limpia (CMake + MSBuild: 0 errores, 0 warnings)

**Referencia**: Refactor metodología v1.5.0

---

## Commits por Historia

### H1 - Render Target como Texture
*Pendiente de commits*

### H2 - Camera System
*Pendiente de commits*

### H3 - Viewport Integration
*Pendiente de commits*

### H4 - Picking & Selection
*Pendiente de commits*

---

## Estadísticas del Sprint

**Commits totales**: 2  
**Features**: 0  
**Tests**: 0  
**Fixes**: 0  
**Docs**: 1  
**Refactor**: 1

**Progreso**: 0/15 tareas completadas (0.0%)

---

**Versión**: v1.0  
**Última actualización**: 2025-01-18  
**Sprint**: v1.5.0 (Viewport Real) - **EN PROGRESO**

---

## 📚 Histórico de Sprints Anteriores

Los commits de sprints anteriores (v1.0.0 - v1.4.0) están archivados en:
- `docs/sprints/sprint_commits_v1.0.0-1.4.0.md`