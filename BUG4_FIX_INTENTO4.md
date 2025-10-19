# BUG-4 FIX INTENTO #4

## Problema Detectado

El log revela que en el **PRIMER FRAME**, el scene RT ya est� en estado `RENDER_TARGET` (como se especifica en `CreateSceneRenderTarget`), pero el c�digo intenta hacer una transici�n desde `PIXEL_SHADER_RESOURCE` ? `RENDER_TARGET`, lo cual causa un error de barrier mismatch.

## Soluci�n

Condicionar el barrier del scene RT `PSR ? RT` solo para frames DESPU�S del primer frame.

## Cambio en `src/renderer/DX12Renderer.cpp`

### L�NEA ~492 (reemplazar el bloque existente):

**ANTES** (l�neas 492-511):
```cpp
    // === RESOURCE STATE TRANSITION: Scene RT ? RENDER_TARGET (if needed) ===
    // BUG-4 FIX INTENTO #3: Transition scene RT back to RENDER_TARGET before rendering
    m_commandContext->BeginFrame(nullptr);
    ID3D12GraphicsCommandList* commandList = m_commandContext->GetCommandList();
    
    if (commandList && m_sceneRenderTarget)
    {
        // Check if this is not the first frame (scene RT might be in PIXEL_SHADER_RESOURCE from previous frame)
        // Transition: PIXEL_SHADER_RESOURCE ? RENDER_TARGET
        D3D12_RESOURCE_BARRIER sceneRTBarrier = {};
        sceneRTBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        sceneRTBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        sceneRTBarrier.Transition.pResource = m_sceneRenderTarget;
        sceneRTBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        sceneRTBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        sceneRTBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        
        commandList->ResourceBarrier(1, &sceneRTBarrier);
    }
    
    m_commandContext->EndFrame();
    m_commandContext->Execute();
    m_commandContext->WaitForGPU();
```

**DESPU�S** (a�adir condicional `isFirstFrame`):
```cpp
    // === RESOURCE STATE TRANSITION: Scene RT ? RENDER_TARGET (only after first frame) ===
    // BUG-4 FIX INTENTO #4: Scene RT starts in RENDER_TARGET state on creation
    // Only transition from PIXEL_SHADER_RESOURCE on subsequent frames
    static bool isFirstFrame = true;
    
    if (!isFirstFrame)
    {
        m_commandContext->BeginFrame(nullptr);
        ID3D12GraphicsCommandList* commandList = m_commandContext->GetCommandList();
        
        if (commandList && m_sceneRenderTarget)
        {
            // Transition: PIXEL_SHADER_RESOURCE ? RENDER_TARGET (frame N+1)
            D3D12_RESOURCE_BARRIER sceneRTBarrier = {};
            sceneRTBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            sceneRTBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            sceneRTBarrier.Transition.pResource = m_sceneRenderTarget;
            sceneRTBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
            sceneRTBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
            sceneRTBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            
            commandList->ResourceBarrier(1, &sceneRTBarrier);
        }
        
        m_commandContext->EndFrame();
        m_commandContext->Execute();
        m_commandContext->WaitForGPU();
    }
```

### L�NEA ~542 (DESPU�S del segundo barrier, a�adir):

**A�adir al final del segundo barrier (despu�s de WaitForGPU())**:
```cpp
    m_commandContext->EndFrame();
    m_commandContext->Execute();
    m_commandContext->WaitForGPU();
    
    // Mark that first frame has been processed
    isFirstFrame = false;
```

## Resultado Esperado

- ? Primer frame: NO hace transici�n PSR?RT (scene RT ya est� en RT)
- ? Frames siguientes: S� hace transici�n PSR?RT (scene RT viene de frame anterior en PSR)
- ? Sin errores de barrier mismatch
- ? Quad visible en Viewport

## Compilar y Probar

```powershell
cmake --build build --config Debug
msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /m
```

Ejecutar y verificar que NO aparece el error:
```
ERROR: Before state (0x80: PIXEL_SHADER_RESOURCE) does not match with state (0x4: RENDER_TARGET)
```
