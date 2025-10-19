# BUG-4 FIX: Quad no visible en Viewport

## ?? Problema

El quad no se ve en el Viewport panel después de la refactorización AAA.

## ?? Causa Raíz

**El Scene Render Target está en estado `RENDER_TARGET` después de OpaquePass, pero ImGui necesita que esté en `PIXEL_SHADER_RESOURCE` para leerlo como textura en el Viewport panel.**

## ? Solución

Añadir resource state transition **DESPUÉS** de OpaquePass y **ANTES** de UIPass.

---

## ?? Cambios en `src/renderer/DX12Renderer.cpp`

### **Ubicación**: Método `RenderForwardPass()`, línea ~535

### **ANTES**:
```cpp
    // === PASS 1: OPAQUE PASS (render 3D scene to scene RT) ===
    m_commandContext->BeginFrame(m_pipelineState);
    m_opaquePass->Execute(*m_commandContext);
    m_commandContext->EndFrame();
    m_commandContext->Execute();
    m_commandContext->WaitForGPU();
    
    // === PASS 2: UI PASS (render ImGui to back buffer) ===
    // IMPORTANT: Always execute UI pass, visibility is handled inside UIPass::Execute
    unsigned int backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
```

### **DESPUÉS**:
```cpp
    // === PASS 1: OPAQUE PASS (render 3D scene to scene RT) ===
    m_commandContext->BeginFrame(m_pipelineState);
    m_opaquePass->Execute(*m_commandContext);
    m_commandContext->EndFrame();
    m_commandContext->Execute();
    m_commandContext->WaitForGPU();
    
    // === RESOURCE STATE TRANSITION ===
    // CRITICAL: Transition scene RT from RENDER_TARGET to PIXEL_SHADER_RESOURCE
    // so ImGui can sample it as a texture in the Viewport panel
    m_commandContext->BeginFrame(nullptr);
    
    ID3D12GraphicsCommandList* commandList = m_commandContext->GetCommandList();
    if (commandList && m_sceneRenderTarget)
    {
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = m_sceneRenderTarget;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        
        commandList->ResourceBarrier(1, &barrier);
    }
    
    m_commandContext->EndFrame();
    m_commandContext->Execute();
    m_commandContext->WaitForGPU();
    
    // === PASS 2: UI PASS (render ImGui to back buffer) ===
    // IMPORTANT: Always execute UI pass, visibility is handled inside UIPass::Execute
    unsigned int backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
```

---

## ?? Validación

Después de aplicar el fix:

1. ? **Compilar**: `cmake --build build --config Debug`
2. ? **Ejecutar**: F5 en Visual Studio o `build\bin\Debug\ImagineStudio.exe`
3. ? **Verificar**: El quad debe aparecer en el Viewport panel con colores (rojo, azul, verde, amarillo)

---

## ?? Por qué funciona

### **Antes del fix**:
```
OpaquePass: Render quad ? Scene RT (estado: RENDER_TARGET)
UIPass: ImGui intenta leer Scene RT ? ? ERROR (estado incorrecto)
Resultado: Viewport muestra textura negra/basura
```

### **Después del fix**:
```
OpaquePass: Render quad ? Scene RT (estado: RENDER_TARGET)
Transition: Scene RT (RENDER_TARGET ? PIXEL_SHADER_RESOURCE)
UIPass: ImGui lee Scene RT ? ? SUCCESS (estado correcto)
Resultado: Viewport muestra quad correctamente
```

---

## ?? Estado esperado

- Scene RT debe estar en `RENDER_TARGET` durante OpaquePass (para escribir)
- Scene RT debe estar en `PIXEL_SHADER_RESOURCE` durante UIPass (para leer en ImGui)

Este es el comportamiento estándar AAA en DirectX 12.

---

**Commit sugerido**:
```bash
git add src/renderer/DX12Renderer.cpp BUG4_FIX.md
git commit -m "fix renderer BUG-4: Añadir resource state transition scene RT. RENDER_TARGET ? PIXEL_SHADER_RESOURCE después OpaquePass. ImGui puede leer scene RT como textura en Viewport. Quad visible. Compilación limpia CMake. Refs BUG-4"
```
