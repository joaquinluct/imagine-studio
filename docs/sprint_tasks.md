# Sprint Tasks - v1.2.0 (Active)

> **Referencia**: Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias de usuario del sprint.

Este archivo contiene las tareas detalladas (bajo nivel) del sprint activo v1.2.0. Cada tarea es la unidad m�nima de trabajo y se implementa en una iteraci�n del asistente.

---

## Historia 1: Arquitectura Multi-Pass (H1)

### Tarea H1.1: Refactorizar RenderFrame() para multi-pass
**Estado**: ✅ Completada  
**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Descripci�n**: Refactorizar `DX12Renderer::RenderFrame()` para llamar a `RenderForwardPass()` en lugar de tener todo el c�digo en `RenderFrame()`.

**Pasos**:
1. ✅ Crear m�todo `void RenderForwardPass()` en `DX12Renderer`
2. ✅ Mover c�digo actual de `RenderFrame()` a `RenderForwardPass()`
3. ✅ `RenderFrame()` solo llama a `RenderForwardPass()`
4. ✅ Compilar y validar (0 errores, 0 warnings)

**Commit**: 21cc105

---

### Tarea H1.2: Crear métodos OpaquePass() y UIPass()
**Estado**: ✅ Completada  
**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Descripción**: Separar `RenderForwardPass()` en dos métodos independientes: `OpaquePass()` y `UIPass()`.

**Pasos**:
1. ✅ Crear método `void OpaquePass()`
2. ✅ Crear método `void UIPass()`
3. ✅ Mover código de renderizado de geometría a `OpaquePass()`
4. ✅ `RenderForwardPass()` llama a `OpaquePass()` y luego `UIPass()`
5. ✅ Compilar y validar (0 errores, 0 warnings)

**Commit**: baae685

---

### Tarea H1.3: Añadir comentarios para futuros passes (Deferred)
**Estado**: ✅ Completada  
**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Descripción**: Añadir comentarios en el código con métodos futuros para Deferred Rendering (v1.3.0+) sin implementarlos.

**Pasos**:
1. ✅ Añadir comentarios en `DX12Renderer.h` para métodos futuros:
   - `// void RenderDeferredPass();  // v1.3.0+`
   - `// void GBufferPass();         // v1.3.0+`
   - `// void LightingPass();        // v1.3.0+`
2. ✅ Añadir comentarios para render targets futuros:
   - `// ID3D12Resource* m_gBufferAlbedo;  // v1.3.0+`
   - `// ID3D12Resource* m_gBufferNormal;  // v1.3.0+`
   - `// ID3D12Resource* m_gBufferDepth;   // v1.3.0+`
3. ✅ Compilar y validar (0 errores, 0 warnings)

**Commit**: cf9d309

---

## Historia 2: Input Manager y Toggle UI con F1 (H2)

### Tarea H2.1: Crear clase InputManager básica
**Estado**: ✅ Completada  
**Archivos modificados**: `src/platform/Input.h`, `src/platform/Input.cpp`

**Descripción**: Añadir método `InputManager::IsKeyPressed()` para detección de flancos de subida (key press).

**Pasos**:
1. ✅ Añadir método `bool IsKeyPressed(int vkCode)` a `InputManager`
2. ✅ Añadir array `prevKeyStates_[256]` para almacenar estado previo
3. ✅ `Update()` guarda estado actual en `prevKeyStates_` antes de actualizar
4. ✅ `IsKeyPressed()` detecta flanco de subida (key down ahora && key up antes)
5. ✅ Compilar y validar (0 errores, 0 warnings)

**Commit**: aeceac8

---

### Tarea H2.2: Integrar InputManager con Win32 message loop
**Estado**: ✅ Completada  
**Archivos verificados**: `src/main.cpp`

**Descripción**: Integrar `InputManager::Update()` en el message loop de Win32 para actualizar el estado del teclado cada frame.

**Pasos**:
1. ✅ Verificar que `InputManager` existe en el loop principal (`main.cpp`)
2. ✅ Verificar que `InputManager::Update()` se llama cada frame (antes de renderizar)
3. ✅ Confirmar que la implementación con `GetAsyncKeyState()` es funcional (no requiere `WM_KEYDOWN`/`WM_KEYUP`)
4. ✅ Compilar y validar (0 errores, 0 warnings)

**Nota**: La integración ya estaba implementada en `main.cpp`. El `InputManager` se instancia como `Platform::InputManager input;` y `input.Update(window.GetHWND());` se llama cada frame antes de `renderer.RenderFrame()`.

**Commit**: 174dadc (integración ya existente, tarea marcada como completada)

---

### Tarea H2.3: Implementar toggle de UI con F1
**Estado**: ✅ Completada  
**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`, `src/main.cpp`

**Descripción**: Añadir lógica de toggle de UI con tecla F1 usando `InputManager`.

**Pasos**:
1. ✅ Añadir miembro `bool m_uiVisible = true;` en `DX12Renderer`
2. ✅ Añadir métodos públicos `ToggleUI()`, `SetUIVisible()`, `IsUIVisible()` en `DX12Renderer`
3. ✅ En `main.cpp`, detectar `InputManager::IsKeyPressed(VK_F1)` y llamar a `renderer.ToggleUI()`
4. ✅ Actualizar `RenderForwardPass()` para llamar a `UIPass()` solo si `m_uiVisible == true`
5. ✅ Compilar y validar (0 errores, 0 warnings)

**Commit**: 8900b88

---

### Tarea H2.4: Validar toggle de UI (testing manual)
**Estado**: ✅ Completada  
**Archivos afectados**: Ninguno (testing manual)

**Descripción**: Validar manualmente que el toggle F1 funciona correctamente.

**Pasos**:
1. ✅ Ejecutar aplicación
2. ✅ Presionar F1 y verificar detección de tecla (logs muestran "F1 key state changed: DOWN")
3. ✅ Verificar que toggle se ejecuta correctamente (logs muestran "UI visibility toggled: hidden/visible")
4. ✅ Validar estado persistente entre frames (múltiples toggles funcionan correctamente)

**Resultado del testing**:
- ✅ Detección de F1 funcional (`IsKeyPressed(VK_F1)` detecta flancos de subida correctamente)
- ✅ Toggle de estado funcional (alterna entre `visible` y `hidden`)
- ✅ Estado persistente entre frames (no hay parpadeo ni múltiples toggles por presión)
- ✅ Logs confirman: "F1 key state changed: DOWN/UP" y "UI visibility toggled: visible/hidden"

**Nota**: No se observan cambios visuales porque `UIPass()` está actualmente vacío. La implementación visual del UI se realizará en Historia H4 (tareas H4.1-H4.3).

**Validación**: Completada exitosamente

---

## Historia 3: Geometry Simple - Tri�ngulos en Opaque Pass (H3)

### Tarea H3.1: Crear vertex buffer para triángulos simples
**Estado**: ✅ Completada (implementación existente desde v1.1.0)
**Archivos verificados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Crear vertex buffer con triángulos simples (3-6 vértices) en lugar del quad de v1.1.0.

**Pasos**:
1. ✅ Verificar struct `Vertex` existente con `float pos[3]` y `float col[4]`
2. ✅ Verificar array de 6 vértices (2 triángulos) ya implementado
3. ✅ Verificar vertex buffer creado con `CreateCommittedResource()` (upload + default heap)
4. ✅ Verificar upload de datos a GPU con `Map()/Unmap()` funcional
5. ✅ Compilar y validar (0 errores, 0 warnings)

**Nota**: El vertex buffer con 6 vértices (2 triángulos con winding clockwise) ya estaba implementado desde el Sprint v1.1.0. Cumple con los requisitos de la tarea (1-2 triángulos = 3-6 vértices). La implementación incluye:
- Struct `Vertex` con posición (3 floats) y color (4 floats)
- Upload heap para staging (CPU → GPU)
- Default heap para almacenamiento final (GPU optimizado)
- Resource barrier para transición de estados
- Vertex buffer view configurado correctamente

**Commit**: Implementación existente desde v1.1.0

---

### Tarea H3.2: Crear shader HLSL simple para triángulos
**Estado**: ✅ Completada (implementación existente desde v1.1.0)
**Archivos verificados**: `shaders/quad.hlsl`

**Descripción**: Crear shader HLSL simple (Vertex + Pixel) para renderizar triángulos con color interpolado.

**Pasos**:
1. ✅ Verificar shader existente con Vertex shader `VSMain` (POSITION + COLOR input)
2. ✅ Verificar Pixel shader `PSMain` (color interpolado output)
3. ✅ Verificar compilación en runtime con `D3DCompileFromFile()` funcional
4. ✅ Compilar y validar (0 errores, 0 warnings)

**Nota**: El shader `quad.hlsl` ya cumple con todos los requisitos de la tarea H3.2:
- Vertex shader con entrada `VSInput { float3 pos : POSITION; float4 col : COLOR; }`
- Pixel shader que retorna color interpolado
- Compilación en runtime con shader model 5.0 (vs_5_0, ps_5_0)
- Usado exitosamente para renderizar 2 triángulos con colores interpolados

El shader está optimizado con MVP matrix transform (root constants) y puede usarse tanto para triángulos simples como para geometría más compleja.

**Commit**: Implementación existente desde v1.1.0

---

### Tarea H3.3: Crear PSO para triángulos
**Estado**: ✅ Completada (implementación existente desde v1.1.0)
**Archivos verificados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Crear Pipeline State Object (PSO) para triángulos con el shader de H3.2.

**Pasos**:
1. ✅ Verificar `D3D12_GRAPHICS_PIPELINE_STATE_DESC` configurado correctamente
2. ✅ Verificar input layout con `POSITION` (DXGI_FORMAT_R32G32B32_FLOAT) y `COLOR` (DXGI_FORMAT_R32G32B32A32_FLOAT)
3. ✅ Verificar shaders compilados asignados (quad.hlsl VS + PS)
4. ✅ Verificar PSO creado con `CreateGraphicsPipelineState()` exitosamente
5. ✅ Compilar y validar (0 errores, 0 warnings)

**Nota**: El PSO ya estaba completamente implementado desde el Sprint v1.1.0. La implementación incluye:
- Input layout con 2 elementos (POSITION: 3 floats @ offset 0, COLOR: 4 floats @ offset 12)
- Rasterizer state con backface culling (D3D12_CULL_MODE_BACK, clockwise front faces)
- Blend state sin blending (opaque rendering)
- Depth/stencil disabled (2D rendering)
- Primitive topology: D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
- Render target format: DXGI_FORMAT_R8G8B8A8_UNORM (matches swap chain)
- Sample count: 1 (no MSAA)

El PSO está optimizado para renderizar triángulos con colores interpolados y es funcional desde v1.1.0.

**Commit**: Implementación existente desde v1.1.0

---

### Tarea H3.4: Renderizar tri�ngulos en OpaquePass()
**Estado**: Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripci�n**: Implementar `OpaquePass()` para renderizar tri�ngulos con PSO y vertex buffer de H3.1 y H3.3.

**Pasos**:
1. En `OpaquePass()`:
   - Reset command allocator
   - Reset command list
   - Set render target (back buffer)
   - Clear render target (blue)
   - Set PSO de tri�ngulos
   - Set root signature
   - Set vertex buffer
   - `DrawInstanced(3 o 6 v�rtices, 1 instancia, 0, 0)`
   - Close command list
2. Ejecutar command list en command queue
3. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H3.5: Validar renderizado de tri�ngulos (testing visual)
**Estado**: Pendiente  
**Archivos afectados**: Ninguno (testing)

**Descripci�n**: Validar visualmente que los tri�ngulos se renderizan correctamente:
- Tri�ngulos visibles en pantalla
- Colores interpolados correctamente
- 60 FPS estables

**Pasos**:
1. Ejecutar aplicaci�n
2. Verificar que tri�ngulos se ven en pantalla
3. Validar interpolaci�n de color (si aplica)
4. Medir FPS (debe ser 60 con VSync)

---

## Historia 4: UI Pass Overlay Simple (H4)

### Tarea H4.1: Implementar UIPass() con overlay simple
**Estado**: Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripci�n**: Implementar `UIPass()` para renderizar un overlay simple (texto o rect�ngulo) despu�s de `OpaquePass()`.

**Pasos**:
1. En `UIPass()`:
   - Verificar `if (!m_uiVisible) return;`
   - Renderizar overlay simple (texto "UI Active" o rect�ngulo)
   - No usar depth buffer (overlay puro)
2. Opciones de implementaci�n:
   - **Opci�n A**: Reutilizar DirectWrite de v1.1.0 para texto
   - **Opci�n B**: Crear vertex buffer minimal para quad de UI con color
3. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H4.2: Conectar UIPass() con toggle F1
**Estado**: Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripci�n**: Validar que `UIPass()` solo se ejecuta si `m_uiVisible == true` (controlado por F1).

**Pasos**:
1. En `RenderForwardPass()`, llamar a `UIPass()` despu�s de `OpaquePass()`
2. `UIPass()` verifica `m_uiVisible` antes de renderizar
3. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H4.3: Validar UI Pass con toggle F1 (testing visual)
**Estado**: Pendiente  
**Archivos afectados**: Ninguno (testing)

**Descripci�n**: Validar visualmente que el UI Pass funciona con el toggle F1:
- UI visible al inicio
- F1 oculta UI
- F1 otra vez muestra UI
- UI se renderiza despu�s de geometr�a (overlay)

**Pasos**:
1. Ejecutar aplicaci�n
2. Verificar que UI se ve al inicio
3. Presionar F1 y verificar que UI desaparece
4. Presionar F1 otra vez y verificar que UI reaparece
5. Validar que UI est� en overlay (delante de geometr�a)

---

## ?? Resumen de Tareas

| Historia | Tarea | T�tulo | Estado |
|----------|-------|--------|--------|
| H1 | H1.1 | Refactorizar RenderFrame() | ✅ Completada |
| H1 | H1.2 | Crear OpaquePass() y UIPass() | ✅ Completada |
| H1 | H1.3 | Comentarios para futuros passes | ✅ Completada |
| H2 | H2.1 | Crear InputManager básico | ✅ Completada |
| H2 | H2.2 | Integrar con Win32 message loop | ✅ Completada |
| H2 | H2.3 | Implementar toggle F1 | ✅ Completada |
| H2 | H2.4 | Validar toggle F1 | ✅ Completada |
| H3 | H3.1 | Crear vertex buffer triángulos | ✅ Completada |
| H3 | H3.2 | Crear shader HLSL triángulos | ✅ Completada |
| H3 | H3.3 | Crear PSO triángulos | ✅ Completada |
| H3 | H3.4 | Renderizar tri�ngulos OpaquePass | Pendiente |
| H3 | H3.5 | Validar renderizado tri�ngulos | Pendiente |
| H4 | H4.1 | Implementar UIPass overlay | Pendiente |
| H4 | H4.2 | Conectar UIPass con F1 | Pendiente |
| H4 | H4.3 | Validar UI Pass con F1 | Pendiente |

**Total**: 15 tareas (10 completadas, 5 pendientes)
