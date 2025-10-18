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

#### `2e44685` - feat renderer H2.2 Integrar Camera en DX12Renderer con matriz MVP

**Tipo**: Feature (Renderer)  
**Ámbito**: Sprint v1.5.0 - H2.2  
**Descripción**: Integrar instancia `Camera` en `DX12Renderer` y calcular matriz MVP para transformación 3D del quad

**Implementación**:
- Añadir `Camera* m_camera` a `DX12Renderer.h`
- Inicializar cámara en `Initialize(HWND)`:
  - Posición: (0, 2, -5) - ligeramente elevada y alejada
  - Target: (0, 0, 0) - mirando al origen
  - FOV: 45°, Aspect: 16:9 (1920/1080)
- Calcular matriz MVP en `OpaquePass()` antes de draw call:
  - Model matrix: identidad (quad en origen)
  - MVP = Model × View × Projection
  - Usar helper `MultiplyMatrix4x4()` para multiplicación column-major
- Actualizar root constants con MVP calculada
- Liberar cámara en `Shutdown()`
- Añadir `Camera.cpp` al proyecto VS `.vcxproj` para compilación MSBuild

**Helper implementado**:
- `MultiplyMatrix4x4(a, b, result)` - multiplicación de matrices 4x4 column-major
- Namespace anónimo para evitar contaminación de símbolo global

**Resultado**:
- El quad ahora se renderiza con transformación 3D completa
- Perspectiva correcta aplicada (objetos lejanos más pequeños)
- Base preparada para controles de cámara interactivos (H2.3)

**Archivos modificados**:
- `src/renderer/DX12Renderer.h` (añadir Camera* member)
- `src/renderer/DX12Renderer.cpp` (integración completa + cálculo MVP)
- `Imagine Studio.vcxproj` (añadir Camera.cpp)

**Compilación**: ✅ Limpia (CMake + MSBuild: 0 errores, 0 warnings)

**Próxima tarea**: H2.3 - Implementar controles básicos de cámara (orbit/pan/zoom)

**Referencia**: Sprint v1.5.0 - H2.2

---

#### `8563934` - feat renderer H2.1 Crear clase Camera con matrices View y Projection

**Tipo**: Feature (Renderer)  
**Ámbito**: Sprint v1.5.0 - H2.1  
**Descripción**: Crear clase `Camera` con soporte para matrices View (LookAt) y Projection (Perspective)

**Implementación**:
- Crear `src/renderer/Camera.h` y `Camera.cpp`
- Soporte para vectores de posición, target y up
- Matriz View: LookAt transformation (column-major DX12)
  - Forward vector: normalized(target - position)
  - Right vector: cross(forward, up)
  - Recomputed up: cross(right, forward)
- Matriz Projection: Perspective (column-major DX12)
  - FOV en grados (default: 45°)
  - Aspect ratio (default: 16:9)
  - Near/far planes (default: 0.1 - 1000)
- Helper functions inline para math vectorial:
  - `Normalize()`, `Cross()`, `Dot()`
- Getters para matrices y posición/target
- Métodos `UpdateViewMatrix()` y `UpdateProjectionMatrix()`

**Valores por defecto**:
- Posición: (0, 0, -5) - cámara alejada 5 unidades en -Z
- Target: (0, 0, 0) - mirando al origen
- Up: (0, 1, 0) - vector arriba en +Y
- FOV: 45°, Aspect: 16:9, Near: 0.1, Far: 1000

**Archivos creados**:
- `src/renderer/Camera.h` (clase con matrices 4x4 column-major)
- `src/renderer/Camera.cpp` (implementación LookAt + Perspective)

**Compilación**: ✅ Limpia (CMake + MSBuild: 0 errores, 0 warnings)

**Próxima tarea**: H2.2 - Integrar Camera en DX12Renderer

**Referencia**: Sprint v1.5.0 - H2.1

---

#### `5dfedd1` - feat renderer H1.3+H1.4 Implementar TransitionResource helper y validación SRV

**Tipo**: Feature (Renderer)  
**Ámbito**: Sprint v1.5.0 - H1.3 + H1.4  
**Descripción**: Implementar helper `TransitionResource()` para gestión de estados + validación SRV readiness

**H1.3 - TransitionResource Helper**:
- Implementar método `TransitionResource(resource, stateBefore, stateAfter)`:
  - Validación de resource y command list
  - Optimización: Skip transition si estados son iguales
  - Simplifica creación de barriers DX12
- Refactorizar `OpaquePass()` y `UIPass()` para usar helper method
- Eliminar duplicación de código (DRY)

**H1.4 - Validación SRV**:
- Añadir validaciones en `CreateRenderTargetSRV()`:
  - Validar render target no es null
  - Validar ImGui SRV heap existe
- Añadir logging detallado de SRV:
  - Frame index actual
  - GPU handle ptr
  - Formato DXGI (28 = R8G8B8A8_UNORM)
  - Confirmación readiness para H3.1
- Validación en getter `GetRenderTargetSRV()` (handle ptr != 0)

**Beneficios**:
- ✅ Código más limpio y mantenible (principio DRY)
- ✅ Consistencia en manejo de barriers
- ✅ Validación robusta de recursos DX12
- ✅ Logging completo para debugging
- ✅ Preparado para transiciones SRV en H3.1

**Preparación para H3.1**:
Cuando se implemente Viewport con `ImGui::Image()`, solo se necesitará cambiar:
```cpp
// En OpaquePass(), cambiar:
TransitionResource(rt, RENDER_TARGET, PRESENT);
// Por:
TransitionResource(rt, RENDER_TARGET, PIXEL_SHADER_RESOURCE);
```

**Archivos modificados**:
- `src/renderer/DX12Renderer.h` (declaración TransitionResource + validación getter)
- `src/renderer/DX12Renderer.cpp` (implementación completa H1.3 + H1.4)

**Compilación**: ✅ Limpia (CMake + MSBuild: 0 errores, 0 warnings)

**Próxima tarea**: H2.1 - Crear clase Camera

**Referencia**: Sprint v1.5.0 - H1.3 + H1.4

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