# Sprint Bug Attempts - v1.6.0 (Active)

> **Referencia**: Ver [docs/sprint_bugs.md](sprint_bugs.md) para bugs reportados actualmente.

Este archivo contiene el registro detallado de TODOS los intentos de solución para cada bug reportado durante el sprint activo v1.6.0.

---

## Intentos de Solución

### BUG-4 - Quad no visible en Viewport (Scene RT SRV issue)

**Intento #1**
- **Descripción**: Añadir resource state transition `RENDER_TARGET → PIXEL_SHADER_RESOURCE` después de OpaquePass
- **Resultado**: Fallido
- **Detalles**: Barrier correcto pero faltaban otras transiciones
- **Fecha y hora**: 2025-01-18
- **Archivos modificados**: `src/renderer/DX12Renderer.cpp`

---

**Intento #2**
- **Descripción**: Añadir logging detallado para diagnosticar estado de resources
- **Resultado**: Exitoso (diagnóstico completo)
- **Detalles**: 
  - Log reveló 3 errores críticos de resource state transitions:
    1. **Back buffer**: UIPass intenta `ClearRenderTargetView()` con back buffer en estado `PRESENT` (esperado: `RENDER_TARGET`)
    2. **Back buffer (Present)**: `Present()` se llama con back buffer en estado `RENDER_TARGET` (esperado: `PRESENT`)
    3. **Scene RT**: OpaquePass (frame N+1) intenta usar scene RT en estado `PIXEL_SHADER_RESOURCE` (esperado: `RENDER_TARGET`)
  - Compilación: ✅ LIMPIA
  - Validación usuario: Log completo obtenido
- **Fecha y hora**: 2025-01-18
- **Archivos modificados**: `src/renderer/DX12Renderer.cpp`

---

**Intento #3**
- **Descripción**: Añadir 3 resource barriers faltantes para corregir estados de back buffer y scene RT
- **Resultado**: Fallido (quad invisible, error upload buffer)
- **Detalles**: 
  - **Barrier 1 (NUEVO)**: Scene RT `PIXEL_SHADER_RESOURCE → RENDER_TARGET` antes de OpaquePass (frame N+1)
  - **Barrier 2 (EXISTENTE)**: Scene RT `RENDER_TARGET → PIXEL_SHADER_RESOURCE` después de OpaquePass
  - **Barrier 3 (NUEVO)**: Back buffer `PRESENT → RENDER_TARGET` antes de UIPass
  - **Barrier 4 (NUEVO)**: Back buffer `RENDER_TARGET → PRESENT` después de UIPass
  - Compilación 1 (CMake): ✅ LIMPIA
  - Compilación 2 (MSBuild): ✅ LIMPIA
  - **Problema**: Error de barrier mismatch en primer frame + upload buffer liberado prematuramente
- **Fecha y hora**: 2025-01-18
- **Archivos modificados**: `src/renderer/DX12Renderer.cpp`

---

**Intento #4**
- **Descripción**: Condicionar barrier scene RT solo después del primer frame (scene RT inicia en RENDER_TARGET)
- **Resultado**: Parcial (eliminó error barrier, quad aún invisible)
- **Detalles**: 
  - Añadido flag `isFirstFrame` para evitar transición incorrecta en frame 1
  - **Barrier condicional**: Scene RT `PSR → RT` solo si `!isFirstFrame`
  - Compilación 1 (CMake): ✅ LIMPIA
  - Compilación 2 (MSBuild): ✅ LIMPIA
  - **Progreso**: ✅ Eliminó error "Before state does not match" en D3D12 debug layer
  - **Problema pendiente**: Upload buffer liberado antes de que GPU termine copia (ERROR #921: OBJECT_DELETED_WHILE_STILL_IN_USE)
- **Fecha y hora**: 2025-01-18
- **Archivos modificados**: `src/renderer/DX12Renderer.cpp`

---

**Intento #5** ✅ **ÉXITO**
- **Descripción**: Mantener upload buffer vivo hasta después de `WaitForGPU()`
- **Resultado**: ✅ **EXITOSO - QUAD VISIBLE CON COLORES CORRECTOS**
- **Detalles**: 
  - **Cambio 1**: Actualizar firma `DX12ResourceManager::CreateVertexBuffer()` para retornar upload buffer como parámetro de salida (`ID3D12Resource** outUploadBuffer`)
  - **Cambio 2**: En `DX12Renderer::Initialize()`, capturar upload buffer y liberarlo DESPUÉS de `WaitForGPU()`
  - **Flujo correcto**:
    1. `CreateVertexBuffer()` crea upload buffer y graba comando de copia
    2. `Execute()` envía comando a GPU
    3. `WaitForGPU()` espera a que GPU termine copia ✅
    4. **AHORA** es seguro liberar upload buffer
  - Compilación 1 (CMake): ✅ LIMPIA
  - Compilación 2 (MSBuild): ✅ LIMPIA
  - **Validación usuario**: ✅ **100% EXITOSA**
    - ✅ Quad visible en Viewport
    - ✅ Interpolación de colores correcta (rojo, azul, verde, amarillo)
    - ✅ Fondo dark blue (0.0, 0.2, 0.4)
    - ✅ Sin errores D3D12 debug layer
    - ✅ Performance estable (220 FPS sin VSync)
- **Fecha y hora**: 2025-01-18
- **Archivos modificados**: 
  - `src/renderer/DX12ResourceManager.h`
  - `src/renderer/DX12ResourceManager.cpp`
  - `src/renderer/DX12Renderer.cpp`

**Lección aprendida crítica**: En DirectX 12, los upload buffers (D3D12_HEAP_TYPE_UPLOAD) usados para copiar datos a GPU DEBEN mantenerse vivos hasta que la GPU termine de leer de ellos. La secuencia correcta es:
1. Crear upload buffer
2. Grabar comando de copia en command list
3. Ejecutar command list (`Execute()`)
4. **Esperar a GPU** (`WaitForGPU()`)
5. **SOLO ENTONCES** liberar upload buffer

**Flujo de estados final (correcto)**:
```
Frame 1:
- Scene RT: RENDER_TARGET (estado inicial, NO transition)
- OpaquePass: Render to scene RT
- Scene RT: RT → PSR (barrier después OpaquePass)
- Back buffer: PRESENT → RT (barrier antes UIPass)
- UIPass: Clear + ImGui::Image(scene RT)
- Back buffer: RT → PRESENT (barrier después UIPass)
- Present()

Frame N (N>1):
- Scene RT: PSR → RT (barrier antes OpaquePass) ← CONDICIONAL
- OpaquePass: Render to scene RT
- Scene RT: RT → PSR (barrier después OpaquePass)
- Back buffer: PRESENT → RT (barrier antes UIPass)
- UIPass: Clear + ImGui::Image(scene RT)
- Back buffer: RT → PRESENT (barrier después UIPass)
- Present()
```

---

**Nota**: Este fichero se actualiza automáticamente conforme se trabajan los bugs en sprint_bugs.md.
