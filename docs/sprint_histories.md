# Sprint v1.1.0 - Historias de usuario

> **Referencia**: Ver [`docs/MAIN.md`](MAIN.md) para los pilares fundamentales del proyecto.

Este fichero contiene las historias de usuario que forman el backlog del sprint activo. Cada historia se descompondr� en tareas en `docs/sprint_tasks.md` y se ejecutar� como iteraciones individuales (implementar, doble compilaci�n, commit, actualizar docs).

---

## Historias del Sprint v1.1.0

### **H1 - Inicializaci�n DX12** ?? (CR�TICO)
**ID**: H1  
**Prioridad**: CR�TICA  
**Descripci�n**: Implementar la inicializaci�n completa del contexto DirectX 12, incluyendo Device, Factory, SwapChain, descriptor heaps y command infrastructure (allocators, lists, queue, fence).

**Criterios de aceptaci�n**:
- ? `ID3D12Device` creado y funcional
- ? `IDXGIFactory4` y adapter seleccionado (GPU)
- ? `IDXGISwapChain3` con 2-3 back buffers (FLIP_DISCARD)
- ? Descriptor heaps (RTV, CBV/SRV/UAV) creados
- ? Command allocators, command list, command queue creados
- ? `ID3D12Fence` real con evento para sincronizaci�n

**Tareas derivadas**: T1.1, T1.2, T1.3, T1.4, T1.5

---

### **H2 - Pipeline State & Shaders** ?? (ALTO)
**ID**: H2  
**Prioridad**: ALTA  
**Descripci�n**: Crear el pipeline de renderizado con Root Signature, compilar shaders HLSL (Vertex y Pixel) y crear el Pipeline State Object (PSO) para renderizar geometr�a con colores.

**Criterios de aceptaci�n**:
- ? Root Signature creada con root constants (MVP matrix)
- ? Vertex Shader compilado (transforma posiciones, pasa colores)
- ? Pixel Shader compilado (retorna color interpolado)
- ? PSO creado con InputLayout (POSITION, COLOR)
- ? Estados configurados (Rasterizer, Blend, DepthStencil)

**Tareas derivadas**: T2.1, T2.2, T2.3

---

### **H3 - Geometry & Resources** ?? (MEDIO)
**ID**: H3  
**Prioridad**: MEDIA  
**Descripci�n**: Crear los recursos GPU necesarios para renderizar un quad: Vertex Buffer con 6 v�rtices (2 tri�ngulos) y Constant Buffer con MVP matrix (identity para el "Hola Mundo").

**Criterios de aceptaci�n**:
- ? Vertex Buffer creado con 6 v�rtices (posici�n + color)
- ? Upload heap para staging de datos
- ? Default heap con datos copiados
- ? Resource barrier (COPY_DEST ? VERTEX_AND_CONSTANT_BUFFER)
- ? Constant Buffer con MVP identity matrix
- ? Buffer mapeado para updates

**Tareas derivadas**: T3.1, T3.2, T3.3

---

### **H4 - Command Recording & Execution** ?? (B�SICO)
**ID**: H4  
**Prioridad**: B�SICA (pero esencial)  
**Descripci�n**: Implementar el loop de renderizado: grabar command list con clear + draw call, ejecutar en command queue, esperar fence, y hacer Present con VSync.

**Criterios de aceptaci�n**:
- ? Command list grabada con clear + draw call
- ? Resource barriers (PRESENT ? RENDER_TARGET ? PRESENT)
- ? `DrawInstanced(6, 1, 0, 0)` ejecutado
- ? Command queue ejecuta command lists
- ? Fence se�alizado y esperado correctamente
- ? `Present(1, 0)` con VSync
- ? 60 FPS estables

**Tareas derivadas**: T4.1, T4.2, T4.3, T4.4

---

## ?? Estado de Historias

| ID | T�tulo | Prioridad | Tareas | Estado |
|----|--------|-----------|--------|--------|
| H1 | Inicializaci�n DX12 | ?? CR�TICO | 5 | ? Pendiente |
| H2 | Pipeline State & Shaders | ?? ALTO | 3 | ? Pendiente |
| H3 | Geometry & Resources | ?? MEDIO | 3 | ? Pendiente |
| H4 | Command Recording & Execution | ?? B�SICO | 4 | ? Pendiente |

**Total**: 4 historias, 15 tareas

---

## ?? Notas

- Para consultar historias completadas, ver los archivos versionados `sprint_histories_v*.md`.
- Las historias se priorizan desde `docs/backlog.md` durante la planificaci�n del sprint.
- Cada historia debe completarse en orden (H1 ? H2 ? H3 ? H4) por dependencias t�cnicas.

