# Sprint Histories - v1.3.0 (Active)

> **Referencia**: Ver [`docs/sprint.md`](sprint.md) para el sprint activo y objetivos de alto nivel.

Este archivo contiene las historias de usuario del sprint activo v1.3.0 (ImGui Integration). Las historias se descomponen en tareas detalladas en `docs/sprint_tasks.md`.

---

## Historia 1: ImGui Core Integration (H1)

**Como** desarrollador de Imagine Studio  
**Quiero** integrar ImGui como framework de UI  
**Para** tener una base s�lida de Editor UI siguiendo est�ndares AAA

**Prioridad**: Alta  
**Estimaci�n**: 5 Story Points

**Descripci�n detallada**:
Esta historia cubre la integraci�n inicial de ImGui en el proyecto: descargar la librer�a, a�adirla al sistema de build (CMake + Visual Studio), crear el context de ImGui, y configurar estilo/theme por defecto. Al finalizar, ImGui debe compilar sin errores y estar listo para renderizar UI.

**Criterios de Aceptaci�n**:
- [ ] ImGui descargado e integrado en el proyecto (carpeta `external/imgui/`)
- [ ] CMakeLists.txt actualizado con ImGui como library est�tica
- [ ] Visual Studio project incluye archivos ImGui
- [ ] ImGui context creado en `main.cpp` (`ImGui::CreateContext()`)
- [ ] Estilo por defecto configurado (ImGui::StyleColorsDark o custom)
- [ ] Compilaci�n limpia (CMake Debug + MSBuild Debug, 0 errores, 0 warnings)

**Tareas asociadas**: Ver `docs/sprint_tasks.md` (H1.1, H1.2, H1.3, H1.4)

---

## Historia 2: ImGui DX12 Rendering Backend (H2)

**Como** desarrollador de Imagine Studio  
**Quiero** implementar el backend de rendering DirectX 12 de ImGui  
**Para** poder renderizar UI panels con text rendering y geometr�a ImGui

**Prioridad**: Alta  
**Estimaci�n**: 8 Story Points

**Descripci�n detallada**:
Esta historia cubre la integraci�n del backend oficial `imgui_impl_dx12.cpp` para renderizar ImGui con DirectX 12. Incluye crear descriptor heap para ImGui (SRV para font atlas), configurar command list, y renderizar ImGui draw data en `UIPass()`. Al finalizar, ImGui debe renderizar text y widgets correctamente.

**Criterios de Aceptaci�n**:
- [ ] `imgui_impl_dx12.cpp/h` integrado en el proyecto
- [ ] Descriptor heap SRV creado para ImGui (1 descriptor para font atlas)
- [ ] `ImGui_ImplDX12_Init()` llamado correctamente con device, num frames, format, heap
- [ ] Font atlas creada y cargada como SRV texture
- [ ] `ImGui_ImplDX12_NewFrame()` llamado antes de `ImGui::NewFrame()`
- [ ] `ImGui_ImplDX12_RenderDrawData()` llamado en `UIPass()`
- [ ] Text rendering funcional (letras visibles en pantalla)
- [ ] Compilaci�n limpia (CMake Debug + MSBuild Debug, 0 errores, 0 warnings)

**Tareas asociadas**: Ver `docs/sprint_tasks.md` (H2.1, H2.2, H2.3, H2.4)

---

## Historia 3: ImGui Win32 Input Backend (H3)

**Como** desarrollador de Imagine Studio  
**Quiero** implementar el backend de input Win32 de ImGui  
**Para** poder interactuar con UI panels usando mouse y teclado

**Prioridad**: Alta  
**Estimaci�n**: 5 Story Points

**Descripci�n detallada**:
Esta historia cubre la integraci�n del backend oficial `imgui_impl_win32.cpp` para procesar input Win32 (mouse, keyboard, scroll). Incluye procesar mensajes Win32 (`WM_MOUSEMOVE`, `WM_LBUTTONDOWN`, `WM_KEYDOWN`, etc.), integrar con InputManager existente, y manejar focus/hover de ventanas ImGui. Al finalizar, los panels deben responder a clicks, drag, scroll, y keyboard input.

**Criterios de Aceptaci�n**:
- [ ] `imgui_impl_win32.cpp/h` integrado en el proyecto
- [ ] `ImGui_ImplWin32_Init()` llamado correctamente con HWND
- [ ] Message loop procesando eventos Win32 para ImGui (`ImGui_ImplWin32_WndProcHandler`)
- [ ] `ImGui_ImplWin32_NewFrame()` llamado antes de `ImGui::NewFrame()`
- [ ] Mouse input funcional (click, drag, hover)
- [ ] Keyboard input funcional (text input, shortcuts)
- [ ] Scroll input funcional (mouse wheel)
- [ ] Compilaci�n limpia (CMake Debug + MSBuild Debug, 0 errores, 0 warnings)

**Tareas asociadas**: Ver `docs/sprint_tasks.md` (H3.1, H3.2, H3.3)

---

## Historia 4: Editor Panels & Docking (H4)

**Como** desarrollador de Imagine Studio  
**Quiero** crear panels de Editor funcionales con docking  
**Para** tener una UI de herramientas profesional estilo Unity/Unreal

**Prioridad**: Alta  
**Estimaci�n**: 8 Story Points

**Descripci�n detallada**:
Esta historia cubre la creaci�n de 4 panels principales de Editor (Hierarchy, Inspector, Console, Viewport) y habilitar docking/multi-window. Incluye crear funciones de rendering para cada panel, habilitar `ImGuiConfigFlags_DockingEnable`, y renderizar viewport con render target del renderer. Al finalizar, el Editor debe tener 4 panels flotantes con docking funcional.

**Criterios de Aceptaci�n**:
- [ ] Panel "Hierarchy" creado (�rbol de objetos placeholder)
- [ ] Panel "Inspector" creado (propiedades placeholder)
- [ ] Panel "Console" creado (logs funcionales con CORE_LOG_*)
- [ ] Panel "Viewport" creado (render target del renderer visible)
- [ ] Docking habilitado (`ImGuiConfigFlags_DockingEnable`)
- [ ] Panels pueden arrastrarse, redimensionarse y dockear
- [ ] Toggle F1 muestra/oculta todos los panels
- [ ] Compilaci�n limpia (CMake Debug + MSBuild Debug, 0 errores, 0 warnings)

**Tareas asociadas**: Ver `docs/sprint_tasks.md` (H4.1, H4.2, H4.3, H4.4, H4.5)

---

## ?? Resumen de Historias

| ID | T�tulo | Estado | Tareas |
|----|--------|--------|--------|
| H1 | ImGui Core Integration | ?? Pendiente | 0/4 |
| H2 | ImGui DX12 Rendering Backend | ?? Pendiente | 0/4 |
| H3 | ImGui Win32 Input Backend | ?? Pendiente | 0/3 |
| H4 | Editor Panels & Docking | ?? Pendiente | 0/5 |

**Total**: 4 historias (0 completadas, 4 pendientes)
