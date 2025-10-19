# Sprint Fix - v1.6.0 (Active)

> **Referencia**: Ver [docs/sprint_bugs.md](sprint_bugs.md) para bugs actualmente pendientes.

Este archivo contiene el historial de bugs resueltos durante el sprint activo v1.6.0.

---

## Bugs Resueltos

### BUG-4 - Quad no visible en Viewport (Scene RT SRV issue) ✅ RESUELTO

**ID**: BUG-4  
**Prioridad**: Crítica  
**Estado**: ✅ **RESUELTO**  
**Fecha de entrada**: 2025-01-18  
**Fecha de resolución**: 2025-01-18  
**Intentos necesarios**: 5

---

#### **Descripción del Problema**

El quad 3D no se veía en el Viewport panel después de la refactorización AAA (DEV-002). El Viewport mostraba una textura negra/basura en lugar del quad con colores (rojo, azul, verde, amarillo).

---

#### **Causa Raíz Identificada**

**Dos problemas críticos simultáneos**:

1. **Resource State Transitions Incompletas** (Intento #2 - Diagnóstico):
   - Back buffer sin transiciones `PRESENT ↔ RENDER_TARGET`
   - Scene RT sin transición `PIXEL_SHADER_RESOURCE → RENDER_TARGET` en frames subsecuentes
   - Scene RT intentaba transicionar desde `PSR` en frame 1, pero iniciaba en `RENDER_TARGET`

2. **Upload Buffer Liberado Prematuramente** (Intento #5 - Solución final):
   - El upload buffer temporal usado para copiar vertex data a GPU se liberaba ANTES de que GPU terminara la copia
   - Error D3D12: `OBJECT_DELETED_WHILE_STILL_IN_USE` (ERROR #921)
   - GPU intentaba leer memoria ya liberada, causando comportamiento indefinido

---

#### **Solución Implementada**

**Intento #4** (Parcial - Resource Barriers):
- ✅ Añadir flag `isFirstFrame` para condicionar barrier scene RT
- ✅ Scene RT: `PSR → RT` solo si `!isFirstFrame` (evita barrier mismatch en frame 1)
- ✅ Back buffer: `PRESENT → RT` antes de UIPass
- ✅ Back buffer: `RT → PRESENT` después de UIPass

**Intento #5** (Final - Upload Buffer Lifetime):
- ✅ Actualizar `DX12ResourceManager::CreateVertexBuffer()` para retornar upload buffer como parámetro de salida
- ✅ Capturar upload buffer en caller (`DX12Renderer::Initialize()`)
- ✅ Liberar upload buffer **DESPUÉS** de `WaitForGPU()` (cuando GPU ha terminado de copiar)

---

#### **Archivos Modificados**

| Archivo | Cambio | Intento |
|---------|--------|---------|
| `src/renderer/DX12Renderer.cpp` | Añadir 4 resource barriers + condicional `isFirstFrame` | #3, #4 |
| `src/renderer/DX12Renderer.cpp` | Capturar upload buffer y liberarlo después de `WaitForGPU()` | #5 |
| `src/renderer/DX12ResourceManager.h` | Añadir parámetro `outUploadBuffer` a `CreateVertexBuffer()` | #5 |
| `src/renderer/DX12ResourceManager.cpp` | Retornar upload buffer al caller en lugar de liberarlo inmediatamente | #5 |

---

#### **Validación**

**Compilación**:
- ✅ CMake Build (Debug): 0 errores, 0 warnings
- ✅ MSBuild "Imagine Studio.sln": 0 errores, 0 warnings

**Ejecución** (validación usuario):
- ✅ **Quad visible** en Viewport con interpolación de colores correcta
  - 🔴 Rojo (bottom-left vertex)
  - 🔵 Azul (top-left vertex)
  - 🟢 Verde (bottom-right vertex)
  - 🟡 Amarillo (top-right vertex)
- ✅ **Fondo Viewport**: Dark blue (0.0, 0.2, 0.4) según clear color del scene RT
- ✅ **Sin errores D3D12**: Debug layer limpio (no barrier errors, no upload buffer errors)
- ✅ **Performance estable**: 220 FPS (sin VSync)

---

#### **Lección Aprendida**

**DirectX 12 - Gestión de Upload Buffers**:

En D3D12, los upload buffers (`D3D12_HEAP_TYPE_UPLOAD`) usados para staging data DEBEN mantenerse vivos hasta que la GPU termine de leer de ellos.

**Secuencia correcta**:
1. Crear upload buffer en CPU-visible memory
2. Copiar datos del CPU al upload buffer (Map/memcpy/Unmap)
3. Grabar comando `CopyBufferRegion()` en command list
4. Ejecutar command list (`CommandQueue::ExecuteCommandLists()`)
5. **Esperar a GPU** (`WaitForGPU()` o usar fence manual)
6. **SOLO ENTONCES** liberar upload buffer

**Antipatrón (causa del bug)**:
```cpp
// ❌ INCORRECTO
ID3D12Resource* uploadBuffer = CreateUploadBuffer(...);
commandList->CopyBufferRegion(destBuffer, uploadBuffer, ...);
uploadBuffer->Release();  // ← ERROR: GPU aún no ha copiado!
ExecuteCommandList();
```

**Patrón correcto (solución)**:
```cpp
// ✅ CORRECTO
ID3D12Resource* uploadBuffer = CreateUploadBuffer(...);
commandList->CopyBufferRegion(destBuffer, uploadBuffer, ...);
ExecuteCommandList();
WaitForGPU();  // ← GPU termina de copiar
uploadBuffer->Release();  // ← Ahora es seguro
```

**Alternativa AAA (deferred release)**:
- Mantener lista de recursos pendientes con fence values
- Liberar resources cuando fence value sea alcanzado por GPU
- Usado en engines como Unreal (FD3D12DeferredDeletionQueue) y Unity

---

#### **Referencias**

- **Sprint Bug Attempts**: [docs/sprint_bug_attempts.md](sprint_bug_attempts.md) - 5 intentos documentados
- **Commit**: [pendiente] - Fix BUG-4: Resource barriers + upload buffer lifetime

---

**Total bugs resueltos**: 1
