# Sprint v1.1.0 - DX12 Minimal Renderer (Hola Mundo Visual)

> **Referencia**: Ver [`docs/MAIN.md`](MAIN.md) para los pilares fundamentales del proyecto.

Detalle ampliado: ver `docs/sprint_histories.md` y `docs/sprint_tasks.md` para historias y sub-tareas (estas son los archivos activos del sprint).

---

## ?? Objetivo del Sprint

**Meta**: Implementar un renderer DirectX 12 minimal que muestre un **quad de colores renderizado vía GPU**, siguiendo estándares AAA (sin atajos ni soluciones temporales).

**Resultado esperado**: Al finalizar el sprint, la ventana mostrará geometría renderizada por la GPU con el mismo pipeline que un juego AAA (command lists, PSO, root signature, fences, present).

---

## ?? Hitos del Sprint

### **Hito 1: Inicialización DX12** ??
- Crear `ID3D12Device` y `IDXGIFactory`
- Crear SwapChain con back buffers
- Configurar descriptor heaps (RTV, CBV/SRV/UAV)
- Crear command infrastructure (allocators, lists, queue, fence)

### **Hito 2: Pipeline State & Shaders** ??
- Crear Root Signature
- Compilar shaders HLSL (Vertex & Pixel)
- Crear Pipeline State Object (PSO)

### **Hito 3: Geometry & Resources** ??
- Crear Vertex Buffer (quad de 6 vértices)
- Crear Constant Buffer (MVP matrix identity)
- Upload de datos a GPU

### **Hito 4: Command Recording & Execution** ??
- Grabar command list con draw call
- Ejecutar command queue
- Implementar fence GPU para sincronización
- Present con VSync

---

## ? Criterios de Aceptación

1. ? Ventana muestra un quad de colores renderizado vía GPU (DirectX 12)
2. ? No hay rendering software (sin GDI/Win32 para rendering)
3. ? Pipeline DX12 completo y funcional (Device, SwapChain, PSO, Commands)
4. ? Fence GPU sincronizando CPU/GPU correctamente
5. ? 60 FPS estables con VSync
6. ? Compilación limpia (CMake Debug + MSBuild Debug, 0 errores, 0 warnings)

---

## ?? Estado del Sprint

- **Versión objetivo**: v1.1.0
- **Sprint iniciado**: 2025-01-XX
- **Sprint finalizado**: TBD
- **Historias completadas**: 0/4
- **Tareas completadas**: 0/15

---

## ?? Comparación con v1.0.0

| Aspecto | v1.0.0 (Anterior) | v1.1.0 (Este Sprint) |
|---------|-------------------|----------------------|
| **Rendering** | Software (GDI/Win32) | Hardware (DirectX 12 GPU) |
| **Draw Calls** | Sin draw calls reales | DrawInstanced real |
| **Pipeline** | Stubs sin implementación | Pipeline DX12 completo |
| **Sincronización** | Fence stub (CPU-only) | ID3D12Fence (GPU real) |
| **Shaders** | No existen | HLSL compilados |
| **AAA-level** | Infraestructura | Renderer funcional |

---

## ?? Notas

- Para consultar sprints completados, ver los archivos versionados `sprint_v*.md`.
- Este sprint es el **primer paso hacia un renderer AAA real**, comparable a Unity/Unreal en su primer frame.
