# Sprint Tasks - v1.2.0 (Active)

> **Referencia**: Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias de usuario del sprint.

Este archivo contiene las tareas detalladas (bajo nivel) del sprint activo v1.2.0. Cada tarea es la unidad mínima de trabajo y se implementa en una iteración del asistente.

---

## Historia 1: Arquitectura Multi-Pass (H1)

### Tarea H1.1: Refactorizar RenderFrame() para multi-pass
**Estado**: Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Descripción**: Refactorizar `DX12Renderer::RenderFrame()` para llamar a `RenderForwardPass()` en lugar de tener todo el código en `RenderFrame()`.

**Pasos**:
1. Crear método `void RenderForwardPass()` en `DX12Renderer`
2. Mover código actual de `RenderFrame()` a `RenderForwardPass()`
3. `RenderFrame()` solo llama a `RenderForwardPass()`
4. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H1.2: Crear métodos OpaquePass() y UIPass()
**Estado**: Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Descripción**: Separar `RenderForwardPass()` en dos métodos independientes: `OpaquePass()` y `UIPass()`.

**Pasos**:
1. Crear método `void OpaquePass()`
2. Crear método `void UIPass()`
3. Mover código de renderizado de geometría a `OpaquePass()`
4. `RenderForwardPass()` llama a `OpaquePass()` y luego `UIPass()`
5. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H1.3: Añadir comentarios para futuros passes (Deferred)
**Estado**: Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Descripción**: Añadir comentarios en el código con métodos futuros para Deferred Rendering (v1.3.0+) sin implementarlos.

**Pasos**:
1. Añadir comentarios en `DX12Renderer.h` para métodos futuros:
   - `// void RenderDeferredPass();  // v1.3.0+`
   - `// void GBufferPass();         // v1.3.0+`
   - `// void LightingPass();        // v1.3.0+`
2. Añadir comentarios para render targets futuros:
   - `// ID3D12Resource* m_gBufferAlbedo;  // v1.3.0+`
   - `// ID3D12Resource* m_gBufferNormal;  // v1.3.0+`
   - `// ID3D12Resource* m_gBufferDepth;   // v1.3.0+`
3. Compilar y validar (0 errores, 0 warnings)

---

## Historia 2: Input Manager y Toggle UI con F1 (H2)

### Tarea H2.1: Crear clase InputManager básica
**Estado**: Pendiente  
**Archivos nuevos**: `src/platform/InputManager.h`, `src/platform/InputManager.cpp`  
**Archivos modificados**: `CMakeLists.txt`

**Descripción**: Crear clase `InputManager` con polling de teclado básico usando `GetAsyncKeyState()` (Win32).

**Pasos**:
1. Crear `InputManager.h` con:
   - `void Update()` (polling de teclado)
   - `bool IsKeyPressed(int vkCode)` (detecta flanco de subida)
   - `bool IsKeyDown(int vkCode)` (detecta estado actual)
2. Crear `InputManager.cpp` con implementación Win32
3. Añadir archivos a `CMakeLists.txt`
4. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H2.2: Integrar InputManager con Win32 message loop
**Estado**: Pendiente  
**Archivos afectados**: `src/platform/Win32Window.cpp`, `src/core/Application.cpp`

**Descripción**: Integrar `InputManager::Update()` en el message loop de Win32 para actualizar el estado del teclado cada frame.

**Pasos**:
1. Añadir `InputManager` como miembro de `Application`
2. Llamar a `InputManager::Update()` en el loop principal (antes de renderizar)
3. Procesar mensajes `WM_KEYDOWN` y `WM_KEYUP` si es necesario
4. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H2.3: Implementar toggle de UI con F1
**Estado**: Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`, `src/core/Application.cpp`

**Descripción**: Añadir lógica de toggle de UI con tecla F1 usando `InputManager`.

**Pasos**:
1. Añadir miembro `bool m_uiVisible = true;` en `DX12Renderer`
2. En `Application::Run()`, detectar `InputManager::IsKeyPressed(VK_F1)`
3. Si F1 es presionado, alternar `m_uiVisible` en `DX12Renderer`
4. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H2.4: Validar toggle de UI (testing manual)
**Estado**: Pendiente  
**Archivos afectados**: Ninguno (testing)

**Descripción**: Validar manualmente que el toggle F1 funciona correctamente:
- UI visible al inicio
- F1 oculta UI
- F1 otra vez muestra UI
- Estado persiste entre frames

**Pasos**:
1. Ejecutar aplicación
2. Presionar F1 y verificar que UI se oculta
3. Presionar F1 otra vez y verificar que UI reaparece
4. Validar que no hay parpadeo

---

## Historia 3: Geometry Simple - Triángulos en Opaque Pass (H3)

### Tarea H3.1: Crear vertex buffer para triángulos simples
**Estado**: Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Crear vertex buffer con triángulos simples (3-6 vértices) en lugar del quad de v1.1.0.

**Pasos**:
1. Definir struct `TriangleVertex` con `float3 position` y `float3 color`
2. Crear array de vértices para 1-2 triángulos
3. Crear vertex buffer con `CreateCommittedResource()`
4. Upload de datos a GPU con `Map()/Unmap()`
5. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H3.2: Crear shader HLSL simple para triángulos
**Estado**: Pendiente  
**Archivos nuevos**: `shaders/triangle.hlsl`

**Descripción**: Crear shader HLSL simple (Vertex + Pixel) para renderizar triángulos con color interpolado.

**Pasos**:
1. Crear `triangle.hlsl` con:
   - Vertex shader: `VSMain(float3 pos : POSITION, float3 color : COLOR) : SV_POSITION`
   - Pixel shader: `PSMain(float4 pos : SV_POSITION, float3 color : COLOR) : SV_TARGET`
2. Compilar shader con `dxc` o `fxc`
3. Añadir shader compilado al build system
4. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H3.3: Crear PSO para triángulos
**Estado**: Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Crear Pipeline State Object (PSO) para triángulos con el shader de H3.2.

**Pasos**:
1. Crear `D3D12_GRAPHICS_PIPELINE_STATE_DESC` para triángulos
2. Configurar input layout con `POSITION` y `COLOR`
3. Asignar shaders compilados de `triangle.hlsl`
4. Crear PSO con `CreateGraphicsPipelineState()`
5. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H3.4: Renderizar triángulos en OpaquePass()
**Estado**: Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Implementar `OpaquePass()` para renderizar triángulos con PSO y vertex buffer de H3.1 y H3.3.

**Pasos**:
1. En `OpaquePass()`:
   - Reset command allocator
   - Reset command list
   - Set render target (back buffer)
   - Clear render target (blue)
   - Set PSO de triángulos
   - Set root signature
   - Set vertex buffer
   - `DrawInstanced(3 o 6 vértices, 1 instancia, 0, 0)`
   - Close command list
2. Ejecutar command list en command queue
3. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H3.5: Validar renderizado de triángulos (testing visual)
**Estado**: Pendiente  
**Archivos afectados**: Ninguno (testing)

**Descripción**: Validar visualmente que los triángulos se renderizan correctamente:
- Triángulos visibles en pantalla
- Colores interpolados correctamente
- 60 FPS estables

**Pasos**:
1. Ejecutar aplicación
2. Verificar que triángulos se ven en pantalla
3. Validar interpolación de color (si aplica)
4. Medir FPS (debe ser 60 con VSync)

---

## Historia 4: UI Pass Overlay Simple (H4)

### Tarea H4.1: Implementar UIPass() con overlay simple
**Estado**: Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Implementar `UIPass()` para renderizar un overlay simple (texto o rectángulo) después de `OpaquePass()`.

**Pasos**:
1. En `UIPass()`:
   - Verificar `if (!m_uiVisible) return;`
   - Renderizar overlay simple (texto "UI Active" o rectángulo)
   - No usar depth buffer (overlay puro)
2. Opciones de implementación:
   - **Opción A**: Reutilizar DirectWrite de v1.1.0 para texto
   - **Opción B**: Crear vertex buffer minimal para quad de UI con color
3. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H4.2: Conectar UIPass() con toggle F1
**Estado**: Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Validar que `UIPass()` solo se ejecuta si `m_uiVisible == true` (controlado por F1).

**Pasos**:
1. En `RenderForwardPass()`, llamar a `UIPass()` después de `OpaquePass()`
2. `UIPass()` verifica `m_uiVisible` antes de renderizar
3. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H4.3: Validar UI Pass con toggle F1 (testing visual)
**Estado**: Pendiente  
**Archivos afectados**: Ninguno (testing)

**Descripción**: Validar visualmente que el UI Pass funciona con el toggle F1:
- UI visible al inicio
- F1 oculta UI
- F1 otra vez muestra UI
- UI se renderiza después de geometría (overlay)

**Pasos**:
1. Ejecutar aplicación
2. Verificar que UI se ve al inicio
3. Presionar F1 y verificar que UI desaparece
4. Presionar F1 otra vez y verificar que UI reaparece
5. Validar que UI está en overlay (delante de geometría)

---

## ?? Resumen de Tareas

| Historia | Tarea | Título | Estado |
|----------|-------|--------|--------|
| H1 | H1.1 | Refactorizar RenderFrame() | Pendiente |
| H1 | H1.2 | Crear OpaquePass() y UIPass() | Pendiente |
| H1 | H1.3 | Comentarios para futuros passes | Pendiente |
| H2 | H2.1 | Crear InputManager básico | Pendiente |
| H2 | H2.2 | Integrar con Win32 message loop | Pendiente |
| H2 | H2.3 | Implementar toggle F1 | Pendiente |
| H2 | H2.4 | Validar toggle F1 | Pendiente |
| H3 | H3.1 | Crear vertex buffer triángulos | Pendiente |
| H3 | H3.2 | Crear shader HLSL triángulos | Pendiente |
| H3 | H3.3 | Crear PSO triángulos | Pendiente |
| H3 | H3.4 | Renderizar triángulos OpaquePass | Pendiente |
| H3 | H3.5 | Validar renderizado triángulos | Pendiente |
| H4 | H4.1 | Implementar UIPass overlay | Pendiente |
| H4 | H4.2 | Conectar UIPass con F1 | Pendiente |
| H4 | H4.3 | Validar UI Pass con F1 | Pendiente |

**Total**: 15 tareas (0 completadas, 15 pendientes)
