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

### Tarea H1.2: Crear m�todos OpaquePass() y UIPass()
**Estado**: Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Descripci�n**: Separar `RenderForwardPass()` en dos m�todos independientes: `OpaquePass()` y `UIPass()`.

**Pasos**:
1. Crear m�todo `void OpaquePass()`
2. Crear m�todo `void UIPass()`
3. Mover c�digo de renderizado de geometr�a a `OpaquePass()`
4. `RenderForwardPass()` llama a `OpaquePass()` y luego `UIPass()`
5. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H1.3: A�adir comentarios para futuros passes (Deferred)
**Estado**: Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Descripci�n**: A�adir comentarios en el c�digo con m�todos futuros para Deferred Rendering (v1.3.0+) sin implementarlos.

**Pasos**:
1. A�adir comentarios en `DX12Renderer.h` para m�todos futuros:
   - `// void RenderDeferredPass();  // v1.3.0+`
   - `// void GBufferPass();         // v1.3.0+`
   - `// void LightingPass();        // v1.3.0+`
2. A�adir comentarios para render targets futuros:
   - `// ID3D12Resource* m_gBufferAlbedo;  // v1.3.0+`
   - `// ID3D12Resource* m_gBufferNormal;  // v1.3.0+`
   - `// ID3D12Resource* m_gBufferDepth;   // v1.3.0+`
3. Compilar y validar (0 errores, 0 warnings)

---

## Historia 2: Input Manager y Toggle UI con F1 (H2)

### Tarea H2.1: Crear clase InputManager b�sica
**Estado**: Pendiente  
**Archivos nuevos**: `src/platform/InputManager.h`, `src/platform/InputManager.cpp`  
**Archivos modificados**: `CMakeLists.txt`

**Descripci�n**: Crear clase `InputManager` con polling de teclado b�sico usando `GetAsyncKeyState()` (Win32).

**Pasos**:
1. Crear `InputManager.h` con:
   - `void Update()` (polling de teclado)
   - `bool IsKeyPressed(int vkCode)` (detecta flanco de subida)
   - `bool IsKeyDown(int vkCode)` (detecta estado actual)
2. Crear `InputManager.cpp` con implementaci�n Win32
3. A�adir archivos a `CMakeLists.txt`
4. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H2.2: Integrar InputManager con Win32 message loop
**Estado**: Pendiente  
**Archivos afectados**: `src/platform/Win32Window.cpp`, `src/core/Application.cpp`

**Descripci�n**: Integrar `InputManager::Update()` en el message loop de Win32 para actualizar el estado del teclado cada frame.

**Pasos**:
1. A�adir `InputManager` como miembro de `Application`
2. Llamar a `InputManager::Update()` en el loop principal (antes de renderizar)
3. Procesar mensajes `WM_KEYDOWN` y `WM_KEYUP` si es necesario
4. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H2.3: Implementar toggle de UI con F1
**Estado**: Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`, `src/core/Application.cpp`

**Descripci�n**: A�adir l�gica de toggle de UI con tecla F1 usando `InputManager`.

**Pasos**:
1. A�adir miembro `bool m_uiVisible = true;` en `DX12Renderer`
2. En `Application::Run()`, detectar `InputManager::IsKeyPressed(VK_F1)`
3. Si F1 es presionado, alternar `m_uiVisible` en `DX12Renderer`
4. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H2.4: Validar toggle de UI (testing manual)
**Estado**: Pendiente  
**Archivos afectados**: Ninguno (testing)

**Descripci�n**: Validar manualmente que el toggle F1 funciona correctamente:
- UI visible al inicio
- F1 oculta UI
- F1 otra vez muestra UI
- Estado persiste entre frames

**Pasos**:
1. Ejecutar aplicaci�n
2. Presionar F1 y verificar que UI se oculta
3. Presionar F1 otra vez y verificar que UI reaparece
4. Validar que no hay parpadeo

---

## Historia 3: Geometry Simple - Tri�ngulos en Opaque Pass (H3)

### Tarea H3.1: Crear vertex buffer para tri�ngulos simples
**Estado**: Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripci�n**: Crear vertex buffer con tri�ngulos simples (3-6 v�rtices) en lugar del quad de v1.1.0.

**Pasos**:
1. Definir struct `TriangleVertex` con `float3 position` y `float3 color`
2. Crear array de v�rtices para 1-2 tri�ngulos
3. Crear vertex buffer con `CreateCommittedResource()`
4. Upload de datos a GPU con `Map()/Unmap()`
5. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H3.2: Crear shader HLSL simple para tri�ngulos
**Estado**: Pendiente  
**Archivos nuevos**: `shaders/triangle.hlsl`

**Descripci�n**: Crear shader HLSL simple (Vertex + Pixel) para renderizar tri�ngulos con color interpolado.

**Pasos**:
1. Crear `triangle.hlsl` con:
   - Vertex shader: `VSMain(float3 pos : POSITION, float3 color : COLOR) : SV_POSITION`
   - Pixel shader: `PSMain(float4 pos : SV_POSITION, float3 color : COLOR) : SV_TARGET`
2. Compilar shader con `dxc` o `fxc`
3. A�adir shader compilado al build system
4. Compilar y validar (0 errores, 0 warnings)

---

### Tarea H3.3: Crear PSO para tri�ngulos
**Estado**: Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripci�n**: Crear Pipeline State Object (PSO) para tri�ngulos con el shader de H3.2.

**Pasos**:
1. Crear `D3D12_GRAPHICS_PIPELINE_STATE_DESC` para tri�ngulos
2. Configurar input layout con `POSITION` y `COLOR`
3. Asignar shaders compilados de `triangle.hlsl`
4. Crear PSO con `CreateGraphicsPipelineState()`
5. Compilar y validar (0 errores, 0 warnings)

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
| H1 | H1.2 | Crear OpaquePass() y UIPass() | Pendiente |
| H1 | H1.3 | Comentarios para futuros passes | Pendiente |
| H2 | H2.1 | Crear InputManager b�sico | Pendiente |
| H2 | H2.2 | Integrar con Win32 message loop | Pendiente |
| H2 | H2.3 | Implementar toggle F1 | Pendiente |
| H2 | H2.4 | Validar toggle F1 | Pendiente |
| H3 | H3.1 | Crear vertex buffer tri�ngulos | Pendiente |
| H3 | H3.2 | Crear shader HLSL tri�ngulos | Pendiente |
| H3 | H3.3 | Crear PSO tri�ngulos | Pendiente |
| H3 | H3.4 | Renderizar tri�ngulos OpaquePass | Pendiente |
| H3 | H3.5 | Validar renderizado tri�ngulos | Pendiente |
| H4 | H4.1 | Implementar UIPass overlay | Pendiente |
| H4 | H4.2 | Conectar UIPass con F1 | Pendiente |
| H4 | H4.3 | Validar UI Pass con F1 | Pendiente |

**Total**: 15 tareas (1 completada, 14 pendientes)
