# Sprint v1.3.0 - ImGui Integration (Editor UI Framework)

> **Referencia**: Ver [`docs/MAIN.md`](MAIN.md) para los pilares fundamentales del proyecto.

Detalle ampliado: ver `docs/sprint_histories.md` y `docs/sprint_tasks.md` para historias y sub-tareas (estos son los archivos activos del sprint).

---

## 🎯 Objetivo del Sprint

**Meta**: Integrar ImGui como framework de UI para **Editor/Studio Mode** (Pilar #1), con text rendering, panels flotantes, input handling y docking system. Seguir estándares AAA de la industria (Unity Editor, Unreal Editor, Blender tools).

**Resultado esperado**: Al finalizar el sprint, la ventana mostrará UI de Editor con panels funcionales (Hierarchy, Inspector, Console, Viewport), docking habilitado, y toggle F1 para mostrar/ocultar toda la UI de Editor. El sistema debe estar preparado para añadir custom widgets y herramientas de desarrollo.

---

## 🎨 Estrategia de Implementación: ImGui como UI de Editor

Este sprint implementa **ImGui (Immediate Mode GUI)** como solución para UI de Editor:
- ✅ **Estándar de la industria AAA** para herramientas (Unity, Unreal, Blender)
- ✅ **Performance excelente** (immediate mode optimizado)
- ✅ **Code-driven** (declarativo, fácil de mantener)
- ✅ **Open source** (MIT license, sin restricciones)
- ✅ **DX12 backend oficial** mantenido por ImGui
- ✅ **Alineado con Pilar #1** (Editor/Studio Mode)

### **Arquitectura Dual de UI (Imagine Studio)**

```
Editor UI (v1.3.0 - AHORA)
└── ImGui (Immediate Mode)
    ├── Hierarchy panel (árbol de objetos)
    ├── Inspector panel (propiedades)
    ├── Console panel (logs)
    ├── Viewport panel (render target)
    └── Docking/multi-window support

Runtime UI (v1.4.0 - FUTURO)
└── Custom Retained-Mode UI
    ├── Layout system (XML/JSON)
    ├── Widget library (Button, Text, Image)
    ├── Event system (onClick, onHover)
    └── Rendering con DX12 pipeline
```

---

## 🏁 Hitos del Sprint

### **Hito 1: ImGui Core Integration** 🎯
- Descargar e integrar ImGui (última versión estable)
- Añadir ImGui al sistema de build (CMake + Visual Studio)
- Crear ImGui context e inicializar framework
- Configurar estilo y theme por defecto

### **Hito 2: ImGui DX12 Rendering Backend** 🎨
- Implementar backend DirectX 12 oficial (`imgui_impl_dx12`)
- Crear descriptor heap para ImGui (SRV para font atlas)
- Configurar command list para ImGui rendering
- Integrar ImGui render loop en `UIPass()`

### **Hito 3: ImGui Win32 Input Backend** ⌨️
- Implementar backend Win32 oficial (`imgui_impl_win32`)
- Procesar eventos Win32 (mouse, keyboard, scroll)
- Integrar con InputManager existente
- Manejar focus/hover de ventanas ImGui

### **Hito 4: Editor Panels & Docking** 🪟
- Crear panel "Hierarchy" (árbol de objetos)
- Crear panel "Inspector" (propiedades)
- Crear panel "Console" (logs)
- Crear panel "Viewport" (render target)
- Habilitar docking/multi-window

---

## 🏁 Criterios de Aceptación

1. ✅ ImGui integrado y compilando (0 errores, 0 warnings)
2. ✅ Text rendering funcional (font atlas cargada, letras visibles)
3. ✅ Panels flotantes con docking habilitado
4. ✅ Input mouse/keyboard funcional en UI (click, drag, scroll)
5. ✅ Toggle F1 para mostrar/ocultar UI de Editor
6. ✅ 4 panels funcionales (Hierarchy, Inspector, Console, Viewport)
7. ✅ 60 FPS estables con ImGui activo
8. ✅ Arquitectura extensible para custom widgets
9. ✅ Compilación limpia (CMake Debug + MSBuild Debug, 0 errores, 0 warnings)

---

## 📊 Estado del Sprint

- **Versión objetivo**: v1.3.0
- **Sprint iniciado**: [Pendiente]
- **Sprint finalizado**: [Pendiente]
- **Historias completadas**: 0/4 (0%)
- **Tareas completadas**: 0/[TBD] (0%)
- **Bugs resueltos**: 0
- **Estado actual**: 🚀 **EN PLANIFICACIÓN**

---

## 🔄 Comparación con v1.2.0

| Aspecto | v1.2.0 (Anterior) | v1.3.0 (Este Sprint) |
|---------|-------------------|----------------------|
| **UI Framework** | Simple overlay (rectángulo blanco) | ImGui (Editor UI completo) |
| **Text Rendering** | No disponible | Font atlas + letras visibles |
| **Panels** | No disponible | 4 panels (Hierarchy, Inspector, Console, Viewport) |
| **Input** | Toggle F1 básico | Input completo (mouse, keyboard, scroll) |
| **Docking** | No disponible | Docking/multi-window habilitado |
| **Propósito** | Demostración de multi-pass | Editor/Studio Mode funcional |

---

## 📝 Notas

- **ImGui vs Runtime UI**: ImGui es solo para **Editor/Tools** (no para gameplay). Runtime UI (HUD, menús) se implementará en Sprint v1.4.0 con custom framework retained-mode.
- **Versión de ImGui**: Usar última versión estable (v1.91.x o posterior) desde [GitHub oficial](https://github.com/ocornut/imgui).
- **Backends oficiales**: `imgui_impl_dx12.cpp` y `imgui_impl_win32.cpp` son mantenidos por el equipo de ImGui y siguen best practices DX12.
- **Extensibilidad**: El diseño permite añadir nuevos panels (Profiler, Asset Browser, Scene Editor) sin modificar arquitectura base.
- Para consultar sprints completados, ver los archivos versionados `sprint_v*.md`.

---

## 🎨 Arquitectura ImGui (v1.3.0)

```cpp
// Imagine Studio - ImGui Integration Architecture

// Main.cpp - Application Loop
int main() {
    // ... Window/Renderer initialization ...
    
    // ImGui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    // Setup backends
    ImGui_ImplWin32_Init(window.GetHWND());
    ImGui_ImplDX12_Init(device, NUM_FRAMES_IN_FLIGHT, 
                        DXGI_FORMAT_R8G8B8A8_UNORM, 
                        cbvSrvHeap, cpuHandle, gpuHandle);
    
    // Main loop
    while (running) {
        // Input
        ImGui_ImplDX12_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        
        // Render Editor UI
        EditorUI::RenderHierarchy();
        EditorUI::RenderInspector();
        EditorUI::RenderConsole();
        EditorUI::RenderViewport();
        
        // ImGui rendering
        ImGui::Render();
        renderer.RenderFrame(); // includes UIPass with ImGui
        
        // Present
        swapChain->Present(1, 0);
    }
    
    // Cleanup
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

// DX12Renderer.cpp - UIPass Integration
void DX12Renderer::UIPass() {
    if (!m_uiVisible) return;
    
    // ImGui rendering (v1.3.0)
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_commandList);
}
```

### **Flujo de Rendering con ImGui**

```cpp
void DX12Renderer::RenderFrame() {
    // Forward Rendering (v1.2.0)
    RenderForwardPass();
}

void DX12Renderer::RenderForwardPass() {
    // Pass 1: Opaque (geometría 3D)
    OpaquePass();
    
    // Pass 2: UI (ImGui Editor - v1.3.0)
    if (m_uiVisible) {
        UIPass(); // renders ImGui panels
    }
    
    // Present
    Present();
}
```

---

## 🚀 Roadmap de UI (3 Sprints)

```
Sprint v1.3.0 (AHORA - ImGui Editor UI)
└── ImGui para Editor/Tools
    ├── Text rendering (font atlas) ✅
    ├── Panels (Hierarchy, Inspector, Console, Viewport) ✅
    ├── DX12 backend oficial ✅
    ├── Win32 input handling ✅
    └── Docking/multi-window ✅

Sprint v1.4.0 (FUTURO - Custom Runtime UI)
└── Retained-Mode UI para Gameplay
    ├── Layout system (XML/JSON)
    ├── Widget library (Button, Text, Image)
    ├── Event system (onClick, onHover)
    └── Renderizado con DX12 pipeline

Sprint v1.5.0 (AVANZADO - UI Theming & Styling)
└── Styling system AAA
    ├── CSS-like styling
    ├── Themes (dark/light)
    ├── Custom fonts (TTF loading)
    └── Vector rendering (icons, shapes)
```