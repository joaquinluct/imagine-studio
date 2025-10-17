# Sprint v1.2.0 - Forward Rendering + Multi-Pass Architecture

> **Referencia**: Ver [`docs/MAIN.md`](MAIN.md) para los pilares fundamentales del proyecto.

Detalle ampliado: ver `docs/sprint_histories.md` y `docs/sprint_tasks.md` para historias y sub-tareas (estos son los archivos activos del sprint).

---

## ?? Objetivo del Sprint

**Meta**: Implementar un renderer Forward con arquitectura multi-pass (Opaque + UI) y sistema de input b�sico, siguiendo est�ndares AAA y dise�ado para evolucionar hacia Deferred/Hybrid rendering en futuros sprints.

**Resultado esperado**: Al finalizar el sprint, la ventana mostrar� geometr�a simple (tri�ngulos) renderizada en un pass de opacos, con un pass de UI overlay que se puede activar/desactivar con F1. El pipeline debe estar estructurado para permitir la adici�n de nuevos passes (Deferred, Post-processing) sin reescribir c�digo.

---

## ??? Estrategia de Implementaci�n: Forward First, Deferred Later

Este sprint implementa **Forward Rendering** (Opci�n A) como base s�lida que:
- ? Es m�s simple y debuggeable
- ? Permite aprender conceptos fundamentales (passes, render targets, input)
- ? **NO bloquea** la evoluci�n a Deferred (v1.3.0+) o Hybrid (v1.4.0+)
- ? Sigue el mismo patr�n que Unity/Unreal (Forward por defecto, Deferred opcional)

### **Roadmap de Rendering**

```
Sprint v1.2.0 (AHORA - Forward Rendering)
??? Forward Rendering + UI Pass
    ??? Opaque Pass (tri�ngulos simples)
    ??? UI Pass (overlay con toggle F1)
    ??? Base s�lida para evoluci�n

Sprint v1.3.0 (FUTURO - Deferred Rendering)
??? A�adir Deferred Rendering (opcional)
    ??? G-Buffer Pass (albedo, normal, depth)
    ??? Lighting Pass (compute lighting)
    ??? Reutiliza UI Pass de v1.2.0

Sprint v1.4.0 (AVANZADO - Hybrid Rendering)
??? Hybrid Rendering (lo mejor de ambos)
    ??? Forward para transparentes
    ??? Deferred para opacos
    ??? Igual que Unreal Engine
```

---

## ?? Hitos del Sprint

### **Hito 1: Arquitectura Multi-Pass** ???
- Refactorizar `DX12Renderer::RenderFrame()` para soportar m�ltiples passes
- Implementar sistema de render passes (BeginPass/EndPass)
- Separar command recording por pass (Opaque, UI)
- Preparar arquitectura para futura adici�n de Deferred

### **Hito 2: Input Manager & UI Toggle** ??
- Implementar `InputManager` b�sico (polling de teclado)
- Integrar con Win32 message loop (`WM_KEYDOWN`, `WM_KEYUP`)
- A�adir toggle UI con tecla F1
- Estado persistente de UI (visible/oculto)

### **Hito 3: Geometry Simple (Tri�ngulos)** ??
- Crear vertex buffer para tri�ngulos simples (sin quad)
- Implementar shader simple (color pass, sin textura)
- Renderizar en Opaque Pass
- Mantener pipeline DX12 funcional (PSO, root signature)

### **Hito 4: UI Pass Overlay** ??
- Implementar UI Pass separado (despu�s de Opaque Pass)
- Renderizar overlay simple (texto o rect�ngulo de debug)
- UI responde a toggle F1
- No usa depth buffer (overlay puro)

---

## ? Criterios de Aceptaci�n

1. ? Dos render passes separados y funcionales (Opaque Pass + UI Pass)
2. ? F1 toggle para mostrar/ocultar UI (estado persistente entre frames)
3. ? Tri�ngulos simples renderizados en Opaque Pass (color pass, sin textura)
4. ? UI overlay visible cuando est� activada (texto o rect�ngulo de debug)
5. ? Arquitectura multi-pass extensible (preparada para Deferred en v1.3.0+)
6. ? Compilaci�n limpia (CMake Debug + MSBuild Debug, 0 errores, 0 warnings)
7. ? 60 FPS estables con VSync

---

## ?? Estado del Sprint

- **Versi�n objetivo**: v1.2.0
- **Sprint iniciado**: [Fecha por definir]
- **Sprint finalizado**: [Fecha por definir]
- **Historias completadas**: 0/4 (0%)
- **Tareas completadas**: 0/[TBD] (0%)
- **Bugs resueltos**: 0
- **Estado actual**: ?? **EN PLANIFICACI�N**

---

## ?? Comparaci�n con v1.1.0

| Aspecto | v1.1.0 (Anterior) | v1.2.0 (Este Sprint) |
|---------|-------------------|----------------------|
| **Render Passes** | Single pass (todo junto) | Multi-pass (Opaque + UI) |
| **Geometry** | Quad simple | Tri�ngulos simples |
| **Input** | No hay input manager | InputManager con F1 toggle |
| **UI** | No hay UI | UI overlay con toggle |
| **Arquitectura** | Monol�tica | Multi-pass extensible |
| **Evoluci�n** | - | Preparado para Deferred (v1.3.0+) |

---

## ?? Notas

- **Forward vs Deferred**: Este sprint implementa Forward Rendering como base. Deferred se a�adir� en v1.3.0+ si es necesario (muchas luces, efectos avanzados). La arquitectura multi-pass permite ambos.
- **Hybrid Rendering**: Unity y Unreal usan Hybrid (Forward + Deferred seg�n el objeto). Esto se implementar� en v1.4.0+ si se requiere.
- **Extensibilidad**: El dise�o de este sprint permite a�adir nuevos passes (G-Buffer, Lighting, Post-processing) sin reescribir c�digo existente.
- Para consultar sprints completados, ver los archivos versionados `sprint_v*.md`.

---

## ?? Arquitectura Forward Rendering (v1.2.0)

```cpp
// DX12Renderer.h - v1.2.0 con ojo en v1.3.0+
class DX12Renderer {
public:
    void RenderFrame();
    
    // Forward rendering (v1.2.0 - IMPLEMENTAR AHORA)
    void RenderForwardPass();
    void OpaquePass();
    void UIPass();
    
    // Deferred rendering (v1.3.0+ - FUTURO, SOLO COMENTARIOS)
    // void RenderDeferredPass();  // v1.3.0+
    // void GBufferPass();         // v1.3.0+
    // void LightingPass();        // v1.3.0+
    
private:
    // Render targets
    ID3D12Resource* m_backBuffer;
    
    // Future G-Buffer textures (v1.3.0+)
    // ID3D12Resource* m_gBufferAlbedo;  // v1.3.0+
    // ID3D12Resource* m_gBufferNormal;  // v1.3.0+
    // ID3D12Resource* m_gBufferDepth;   // v1.3.0+
    
    // Input & UI state
    bool m_uiVisible = true;
};
```

### **Flujo de RenderFrame() v1.2.0**

```cpp
void DX12Renderer::RenderFrame() {
    // Forward Rendering (v1.2.0)
    RenderForwardPass();
}

void DX12Renderer::RenderForwardPass() {
    // Pass 1: Opaque
    BeginPass("Opaque Pass");
    SetRenderTarget(m_backBuffer);
    ClearRenderTarget(blue);
    DrawTriangles();
    EndPass();
    
    // Pass 2: UI (condicional)
    if (m_uiVisible) {
        BeginPass("UI Pass");
        DrawUI();
        EndPass();
    }
    
    Present();
}
```