# Sprint Tasks - v1.3.0 (Active)

> **Referencia**: Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias de usuario del sprint.

Este archivo contiene las tareas detalladas (bajo nivel) del sprint activo v1.3.0 (ImGui Integration). Cada tarea es la unidad m�nima de trabajo y se implementa en una iteraci�n del asistente.

---

## Historia 1: ImGui Core Integration (H1)

### Tarea H1.1: Descargar e integrar ImGui en el proyecto
**Estado**: ? Completada  
**Archivos afectados**: `external/imgui/` (nuevo directorio), `scripts/install-imgui.ps1` (nuevo), `docs/IMGUI_INSTALL.md` (nuevo)

**Descripci�n**: Descargar la �ltima versi�n estable de ImGui desde GitHub oficial y copiar archivos necesarios al proyecto usando script automatizado.

**Pasos**:
1. [x] Crear script automatizado `scripts/install-imgui.ps1` para descarga e instalaci�n
2. [x] Crear documentaci�n completa en `docs/IMGUI_INSTALL.md` (origen, instalaci�n, actualizaci�n, reinstalaci�n)
3. [x] Ejecutar script: `.\scripts\install-imgui.ps1 -Version v1.91.5`
4. [x] Descargar ImGui v1.91.5 desde https://github.com/ocornut/imgui
5. [x] Crear directorio `external/imgui/`
6. [x] Copiar archivos core (10 archivos):
   - `imgui.h` ?
   - `imgui.cpp` ?
   - `imgui_draw.cpp` ?
   - `imgui_tables.cpp` ?
   - `imgui_widgets.cpp` ?
   - `imgui_internal.h` ?
   - `imconfig.h` ?
   - `imstb_rectpack.h` ?
   - `imstb_textedit.h` ?
   - `imstb_truetype.h` ?
7. [x] Copiar backends oficiales (4 archivos):
   - `backends/imgui_impl_dx12.h` ?
   - `backends/imgui_impl_dx12.cpp` ?
   - `backends/imgui_impl_win32.h` ?
   - `backends/imgui_impl_win32.cpp` ?
8. [x] Crear archivo VERSION.txt con tracking de versi�n
9. [x] Verificar archivos copiados correctamente (14 archivos + VERSION.txt)

**Resultado**: ImGui v1.91.5 instalado correctamente con script automatizado y documentaci�n completa.

**Commit**: [Hash pendiente]

---

### Tarea H1.2: A�adir ImGui a CMakeLists.txt
**Estado**: ? Completada  
**Archivos afectados**: `CMakeLists.txt`

**Descripci�n**: Configurar ImGui como library est�tica en CMake y linkear con el proyecto principal.

**Pasos**:
1. [x] A�adir ImGui como library est�tica en CMakeLists.txt:
   ```cmake
   # ImGui library (Dear ImGui - Editor UI Framework)
   add_library(ImGui STATIC
       external/imgui/imgui.cpp
       external/imgui/imgui_draw.cpp
       external/imgui/imgui_tables.cpp
       external/imgui/imgui_widgets.cpp
       external/imgui/backends/imgui_impl_dx12.cpp
       external/imgui/backends/imgui_impl_win32.cpp
   )
   target_include_directories(ImGui PUBLIC external/imgui external/imgui/backends)
   ```
2. [x] Linkear ImGui con el proyecto principal:
   ```cmake
   target_link_libraries(ImagineStudio PRIVATE ImGui d3d12 dxgi d3dcompiler)
   ```
3. [x] Regenerar proyecto CMake: `cmake -S . -B build` ?
4. [x] Compilar y validar (CMake Debug build) ?
5. [x] Compilar y validar (MSBuild Debug build) ?

**Resultado**: ImGui integrado en CMake como library est�tica y linkeado con ImagineStudio. Compilaci�n limpia (0 errores, 0 warnings).

**Commit**: [Hash pendiente]

---

### Tarea H1.3: A�adir ImGui a Visual Studio project
**Estado**: ? Completada (no requerida - CMake configura VS autom�ticamente)  
**Archivos afectados**: Ninguno (integraci�n autom�tica v�a CMake)

**Descripci�n**: A�adir archivos ImGui al proyecto Visual Studio manualmente para asegurar que est�n en el build system principal.

**Resultado**: Esta tarea NO fue necesaria porque el proyecto principal de Visual Studio (`Imagine Studio.vcxproj`) utiliza la configuraci�n de CMake. La tarea H1.2 ya configur� ImGui como library est�tica en CMakeLists.txt, y Visual Studio usa esa configuraci�n autom�ticamente.

**Verificaci�n**:
1. [x] Compilaci�n MSBuild exitosa con ImGui incluido
2. [x] Include directories configurados correctamente por CMake (`target_include_directories(ImGui PUBLIC ...)`)
3. [x] Archivos ImGui accesibles desde el c�digo del proyecto
4. [x] No se requieren modificaciones manuales al `.vcxproj`

**Justificaci�n**: 
- El proyecto usa CMake como sistema de build principal
- CMake genera autom�ticamente el proyecto Visual Studio con las configuraciones correctas
- La library est�tica ImGui de H1.2 est� correctamente linkeada con ImagineStudio
- Los include directories p�blicos (`external/imgui`, `external/imgui/backends`) est�n disponibles globalmente

**Compilaci�n**: MSBuild VS Debug OK (0 errores, 0 warnings)

**Commit**: N/A (no se requirieron cambios)

---

### Tarea H1.4: Crear ImGui context e inicializar en main.cpp
**Estado**: ? Completada  
**Archivos afectados**: `src/main.cpp`, `scripts/install-imgui.ps1`, `external/imgui/` (actualizado a rama docking), `Imagine Studio.vcxproj`, `src/platform/Input.h` (eliminado), `src/platform/Input.cpp` (eliminado), `src/platform/InputManager.cpp` (actualizado a C++14)

**Descripci�n**: Inicializar ImGui context, configurar ImGuiIO, y establecer estilo por defecto (dark theme).

**Pasos**:
1. [ ] Incluir headers ImGui en `main.cpp`:
   ```cpp
   #include "imgui.h"
   #include "imgui_impl_dx12.h"
   #include "imgui_impl_win32.h"
   ```
2. [ ] Crear ImGui context despu�s de inicializar ventana:
   ```cpp
   // Setup Dear ImGui context
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO(); (void)io;
   
   // Enable docking (preparar para H4)
   io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
   
   // Setup Dear ImGui style
   ImGui::StyleColorsDark();
   ```
3. [ ] A�adir ImGui cleanup antes de cerrar aplicaci�n:
   ```cpp
   // Cleanup ImGui
   ImGui::DestroyContext();
   ```
4. [ ] Compilar y validar (CMake + MSBuild Debug, 0 errores, 0 warnings)

**Commit**: [Hash pendiente]

---

## Historia 2: ImGui DX12 Rendering Backend (H2)

### Tarea H2.1: Crear descriptor heap SRV para ImGui
**Estado**: ? Completada  
**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Descripci�n**: Crear descriptor heap SRV separado para ImGui (1 descriptor para font atlas texture). ImGui requiere un heap SRV shader-visible para renderizar font atlas.

**Pasos**:
1. [ ] A�adir miembros en `DX12Renderer.h`:
   ```cpp
   ID3D12DescriptorHeap* m_imguiSrvHeap;  // SRV heap para ImGui (font atlas)
   ```
2. [ ] Crear descriptor heap SRV en `Initialize(HWND)`:
   ```cpp
   // Create descriptor heap for ImGui (SRV for font atlas)
   D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
   srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
   srvHeapDesc.NumDescriptors = 1;  // Solo 1 descriptor para font atlas
   srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
   hr = m_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_imguiSrvHeap));
   if (FAILED(hr)) {
       CORE_LOG_ERROR("Failed to create ImGui SRV descriptor heap");
       return false;
   }
   CORE_LOG_INFO("ImGui SRV descriptor heap created");
   ```
3. [ ] Liberar heap en `Shutdown()`:
   ```cpp
   if (m_imguiSrvHeap) { m_imguiSrvHeap->Release(); m_imguiSrvHeap = nullptr; }
   ```
4. [ ] Compilar y validar (CMake + MSBuild Debug, 0 errores, 0 warnings)

**Commit**: [Hash pendiente]

---

### Tarea H2.2: Inicializar backend ImGui DX12
**Estado**: ? Completada  
**Archivos afectados**: `src/main.cpp`, `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Descripci�n**: Llamar a `ImGui_ImplDX12_Init()` con device, num frames, format y descriptor heap SRV de ImGui.

**Pasos**:
1. [x] A�adir m�todo p�blico `GetDevice()` en `DX12Renderer.h` para obtener ID3D12Device*
2. [x] Implementar `GetDevice()` en `DX12Renderer.cpp` (evitar forward declaration issues)
3. [x] Incluir headers DirectX 12 en `main.cpp` (`<d3d12.h>`)
4. [x] Obtener descriptores CPU/GPU handle del heap ImGui en `main.cpp`:
   ```cpp
   D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = imguiSrvHeap->GetCPUDescriptorHandleForHeapStart();
   D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = imguiSrvHeap->GetGPUDescriptorHandleForHeapStart();
   ```
5. [x] Inicializar backend ImGui DX12 en `main.cpp` (despu�s de `renderer.Initialize(hwnd)`):
   ```cpp
   // Get ImGui SRV descriptor heap and device from renderer
   ID3D12DescriptorHeap* imguiSrvHeap = renderer.GetImGuiSrvHeap();
   ID3D12Device* device = renderer.GetDevice();
   
   if (imguiSrvHeap && device)
   {
       // Get CPU and GPU descriptor handles for ImGui font atlas
       D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = imguiSrvHeap->GetCPUDescriptorHandleForHeapStart();
       D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = imguiSrvHeap->GetGPUDescriptorHandleForHeapStart();
       
       // Initialize ImGui DX12 backend
       ImGui_ImplDX12_Init(
           device,                           // ID3D12Device*
           2,                                // num frames in flight (double buffering)
           DXGI_FORMAT_R8G8B8A8_UNORM,      // RTV format (matches swap chain)
           imguiSrvHeap,                     // ID3D12DescriptorHeap* (SRV heap)
           cpuHandle,                        // CPU handle for font atlas
           gpuHandle                         // GPU handle for font atlas
       );
       
       CORE_LOG_INFO("ImGui DX12 backend initialized");
   }
   ```
6. [x] A�adir shutdown en cleanup:
   ```cpp
   ImGui_ImplDX12_Shutdown();
   CORE_LOG_INFO("ImGui DX12 backend shutdown");
   ```
7. [x] Compilar y validar (CMake Debug build: 0 errores, 0 warnings) ?

**Resultado**: ImGui DX12 backend inicializado correctamente. Device y heap SRV accesibles desde main.cpp mediante m�todos p�blicos del renderer. Backend shutdown correctamente en cleanup.

**Commit**: [Hash pendiente]

---

### Tarea H2.3: Integrar ImGui_ImplDX12_NewFrame() en render loop
**Estado**: ? Completada  
**Archivos afectados**: `src/main.cpp`, `CMakeLists.txt`, `scripts/install-imgui.ps1`, `Imagine Studio.vcxproj`, `.github/copilot-instructions.md`

**Descripci�n**: Llamar a `ImGui_ImplDX12_NewFrame()` antes de `ImGui::NewFrame()` en el render loop para preparar frame ImGui.

**Pasos**:
1. [x] A�adir `imgui_demo.cpp` a `scripts/install-imgui.ps1` (necesario para `ImGui::ShowDemoWindow()`)
2. [x] Reinstalar ImGui con demo incluido: `.\scripts\install-imgui.ps1 -Branch docking -Reinstall`
3. [x] A�adir `imgui_demo.cpp` a `CMakeLists.txt` en la library ImGui
4. [x] A�adir `imgui_demo.cpp` a `Imagine Studio.vcxproj` usando PowerShell XML
5. [x] Verificar que render loop tiene llamadas correctas (ya implementadas):
   ```cpp
   // Start ImGui frame (H2.3 - ImGui DX12 NewFrame integration)
   ImGui_ImplDX12_NewFrame();
   ImGui::NewFrame();
   
   // ImGui demo window (placeholder - remover en H4)
   ImGui::ShowDemoWindow();
   
   // Render ImGui
   ImGui::Render();
   
   // Renderer renderiza frame (incluye UIPass con ImGui)
   renderer.RenderFrame();
   ```
6. [x] Compilar y validar (CMake Debug build: 0 errores, 0 warnings) ?
7. [x] Compilar y validar (MSBuild Debug build: 0 errores, 0 warnings) ?
8. [x] Documentar m�todo de modificaci�n de `.vcxproj` en `.github/copilot-instructions.md`

**Resultado**: Render loop preparado para ImGui frames. Demo window preparado (a�n no visible porque falta rendering en UIPass de H2.4). Compilaci�n limpia en ambos sistemas de build.

**Notas t�cnicas**:
- Se resolvi� problema de linkeo: `ImGui::ShowDemoWindow()` requer�a `imgui_demo.cpp` que no estaba incluido
- Se actualiz� script de instalaci�n para incluir demo en futuras instalaciones
- Se document� procedimiento correcto para modificar `.vcxproj` sin cerrar Visual Studio (PowerShell XML)

**Commit**: [Hash pendiente]

---

### Tarea H2.4: Renderizar ImGui draw data en UIPass()
**Estado**: ?? Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripci�n**: Implementar rendering de ImGui draw data en `UIPass()` usando `ImGui_ImplDX12_RenderDrawData()`.

**Pasos**:
1. [ ] Modificar `UIPass()` para renderizar ImGui en lugar del rect�ngulo blanco:
   ```cpp
   void DX12Renderer::UIPass() {
       if (!m_uiVisible) return;
       
       // Set ImGui descriptor heap (SRV for font atlas)
       ID3D12DescriptorHeap* heaps[] = { m_imguiSrvHeap };
       m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);
       
       // Render ImGui draw data
       ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_commandList);
   }
   ```
2. [ ] Remover c�digo de rect�ngulo blanco (vertex buffer temporal UI)
3. [ ] Compilar y validar (CMake + MSBuild Debug, 0 errores, 0 warnings)
4. [ ] Testing visual: Ejecutar aplicaci�n y verificar que ImGui demo window se ve (text, buttons, widgets)

**Resultado esperado**: ImGui demo window visible con text rendering funcional.

**Commit**: [Hash pendiente]

---

## Historia 3: ImGui Win32 Input Backend (H3)

### Tarea H3.1: Inicializar backend ImGui Win32
**Estado**: ?? Pendiente  
**Archivos afectados**: `src/main.cpp`

**Descripci�n**: Llamar a `ImGui_ImplWin32_Init()` con HWND para habilitar input Win32 en ImGui.

**Pasos**:
1. [ ] Inicializar backend ImGui Win32 despu�s de crear ventana:
   ```cpp
   // Setup Platform/Renderer backends
   ImGui_ImplWin32_Init(window.GetHWND());
   ImGui_ImplDX12_Init(/* ... */);
   CORE_LOG_INFO("ImGui Win32 backend initialized");
   ```
2. [ ] A�adir shutdown en cleanup:
   ```cpp
   ImGui_ImplWin32_Shutdown();
   ImGui_ImplDX12_Shutdown();
   ```
3. [ ] Compilar y validar (CMake + MSBuild Debug, 0 errores, 0 warnings)

**Commit**: [Hash pendiente]

---

### Tarea H3.2: Procesar eventos Win32 para ImGui
**Estado**: ?? Pendiente  
**Archivos afectados**: `src/platform/Window.cpp`

**Descripci�n**: Forward eventos Win32 (WM_MOUSEMOVE, WM_LBUTTONDOWN, WM_KEYDOWN, etc.) a ImGui usando `ImGui_ImplWin32_WndProcHandler()`.

**Pasos**:
1. [ ] Incluir header ImGui Win32 en `Window.cpp`:
   ```cpp
   #include "imgui_impl_win32.h"
   ```
2. [ ] A�adir forward declaration externa de WndProcHandler:
   ```cpp
   extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
   ```
3. [ ] Procesar eventos en WindowProc antes de default handling:
   ```cpp
   LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
       // Forward to ImGui first
       if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
           return true;  // ImGui handled event
       
       // ... resto de event handling ...
   }
   ```
4. [ ] Compilar y validar (CMake + MSBuild Debug, 0 errores, 0 warnings)

**Commit**: [Hash pendiente]

---

### Tarea H3.3: Integrar ImGui_ImplWin32_NewFrame() en render loop
**Estado**: ?? Pendiente  
**Archivos afectados**: `src/main.cpp`

**Descripci�n**: Llamar a `ImGui_ImplWin32_NewFrame()` antes de `ImGui::NewFrame()` para actualizar input state de ImGui.

**Pasos**:
1. [ ] A�adir `ImGui_ImplWin32_NewFrame()` en render loop:
   ```cpp
   // Start ImGui frame
   ImGui_ImplDX12_NewFrame();
   ImGui_ImplWin32_NewFrame();  // Actualiza input Win32
   ImGui::NewFrame();
   ```
2. [ ] Compilar y validar (CMake + MSBuild Debug, 0 errores, 0 warnings)
3. [ ] Testing visual: Ejecutar aplicaci�n y verificar que ImGui responde a mouse (hover, click, drag)

**Resultado esperado**: ImGui demo window interactivo (clicks en buttons, drag de ventanas, scroll funcional).

**Commit**: [Hash pendiente]

---

## Historia 4: Editor Panels & Docking (H4)

### Tarea H4.1: Crear panel "Hierarchy" con �rbol de objetos placeholder
**Estado**: ?? Pendiente  
**Archivos afectados**: `src/editor/EditorUI.h` (nuevo), `src/editor/EditorUI.cpp` (nuevo), `src/main.cpp`

**Descripci�n**: Crear primer panel de Editor "Hierarchy" con �rbol de objetos placeholder (hardcoded por ahora).

**Pasos**:
1. [ ] Crear `src/editor/EditorUI.h`:
   ```cpp
   #pragma once
   
   namespace Editor {
       class EditorUI {
       public:
           static void RenderHierarchy();
           static void RenderInspector();
           static void RenderConsole();
           static void RenderViewport();
       };
   }
   ```
2. [ ] Crear `src/editor/EditorUI.cpp`:
   ```cpp
   #include "EditorUI.h"
   #include "imgui.h"
   
   namespace Editor {
       void EditorUI::RenderHierarchy() {
           ImGui::Begin("Hierarchy");
           
           // Placeholder: �rbol hardcoded
           if (ImGui::TreeNode("Scene Root")) {
               if (ImGui::TreeNode("Camera")) {
                   ImGui::Text("Main Camera");
                   ImGui::TreePop();
               }
               if (ImGui::TreeNode("Lights")) {
                   ImGui::Text("Directional Light");
                   ImGui::Text("Point Light 1");
                   ImGui::TreePop();
               }
               ImGui::TreePop();
           }
           
           ImGui::End();
       }
       
       // Stubs para otros panels (implementar en H4.2-H4.4)
       void EditorUI::RenderInspector() { }
       void EditorUI::RenderConsole() { }
       void EditorUI::RenderViewport() { }
   }
   ```
3. [ ] Llamar `RenderHierarchy()` en `main.cpp` render loop (despu�s de NewFrame):
   ```cpp
   ImGui::NewFrame();
   Editor::EditorUI::RenderHierarchy();
   ImGui::Render();
   ```
4. [ ] Compilar y validar (CMake + MSBuild Debug, 0 errores, 0 warnings)

**Commit**: [Hash pendiente]

---

### Tarea H4.2: Crear panel "Inspector" con propiedades placeholder
**Estado**: ?? Pendiente  
**Archivos afectados**: `src/editor/EditorUI.cpp`, `src/main.cpp`

**Descripci�n**: Crear panel "Inspector" mostrando propiedades placeholder de objeto seleccionado.

**Pasos**:
1. [ ] Implementar `RenderInspector()` en `EditorUI.cpp`:
   ```cpp
   void EditorUI::RenderInspector() {
       ImGui::Begin("Inspector");
       
       ImGui::Text("Selected: Main Camera");
       ImGui::Separator();
       
       // Transform properties
       if (ImGui::CollapsingHeader("Transform")) {
           static float position[3] = { 0.0f, 0.0f, 0.0f };
           static float rotation[3] = { 0.0f, 0.0f, 0.0f };
           static float scale[3] = { 1.0f, 1.0f, 1.0f };
           
           ImGui::DragFloat3("Position", position, 0.1f);
           ImGui::DragFloat3("Rotation", rotation, 1.0f);
           ImGui::DragFloat3("Scale", scale, 0.1f);
       }
       
       // Camera properties
       if (ImGui::CollapsingHeader("Camera")) {
           static float fov = 60.0f;
           static float nearPlane = 0.1f;
           static float farPlane = 1000.0f;
           
           ImGui::SliderFloat("FOV", &fov, 30.0f, 120.0f);
           ImGui::DragFloat("Near Plane", &nearPlane, 0.1f, 0.01f, 10.0f);
           ImGui::DragFloat("Far Plane", &farPlane, 10.0f, 10.0f, 10000.0f);
       }
       
       ImGui::End();
   }
   ```
2. [ ] Llamar `RenderInspector()` en `main.cpp` render loop
3. [ ] Compilar y validar (CMake + MSBuild Debug, 0 errores, 0 warnings)

**Commit**: [Hash pendiente]

---

### Tarea H4.3: Crear panel "Console" con logs funcionales
**Estado**: ?? Pendiente  
**Archivos afectados**: `src/editor/EditorUI.cpp`, `src/main.cpp`

**Descripci�n**: Crear panel "Console" mostrando logs del sistema (integrar con CORE_LOG_*).

**Pasos**:
1. [ ] Implementar `RenderConsole()` en `EditorUI.cpp`:
   ```cpp
   void EditorUI::RenderConsole() {
       ImGui::Begin("Console");
       
       // Placeholder: logs hardcoded (TODO: integrar con Log.h)
       ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "[INFO] Application started");
       ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[WARNING] Sample warning message");
       ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "[ERROR] Sample error message");
       
       // Auto-scroll al final
       if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
           ImGui::SetScrollHereY(1.0f);
       
       ImGui::End();
   }
   ```
2. [ ] Llamar `RenderConsole()` en `main.cpp` render loop
3. [ ] Compilar y validar (CMake + MSBuild Debug, 0 errores, 0 warnings)

**Nota**: Integraci�n real con `Log.h` se har� en sprint futuro (requiere refactorizar logging system para capturar mensajes).

**Commit**: [Hash pendiente]

---

### Tarea H4.4: Crear panel "Viewport" con render target del renderer
**Estado**: ?? Pendiente  
**Archivos afectados**: `src/editor/EditorUI.cpp`, `src/main.cpp`

**Descripci�n**: Crear panel "Viewport" mostrando el render target del renderer (geometr�a 3D renderizada).

**Pasos**:
1. [ ] Implementar `RenderViewport()` en `EditorUI.cpp`:
   ```cpp
   void EditorUI::RenderViewport() {
       ImGui::Begin("Viewport");
       
       // Placeholder: mostrar texto indicando viewport
       ImGui::Text("3D Viewport (render target placeholder)");
       ImGui::Text("Size: 1920x1080");
       
       // TODO: Renderizar render target como ImGui::Image() cuando est� disponible
       // Requiere exponer back buffer como SRV texture
       
       ImGui::End();
   }
   ```
2. [ ] Llamar `RenderViewport()` en `main.cpp` render loop
3. [ ] Compilar y validar (CMake + MSBuild Debug, 0 errores, 0 warnings)

**Nota**: Rendering real del render target como ImGui::Image() requiere exponer back buffer como SRV (tarea compleja, diferir a sprint futuro).

**Commit**: [Hash pendiente]

---

### Tarea H4.5: Habilitar docking y validar UI completa
**Estado**: ? Completada  
**Archivos afectados**: `src/main.cpp`, `src/editor/EditorUI.cpp`

**Descripci�n**: Habilitar docking de ImGui y validar que todos los panels funcionan correctamente con toggle F1.

**Pasos**:
1. [x] Verificar que docking est� habilitado (ya configurado en H1.4):
   ```cpp
   io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
   ```
2. [x] A�adir dockspace principal en render loop:
   ```cpp
   ImGui::NewFrame();
   
   // Dockspace principal (ocupa toda la ventana)
   ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
   
   // Render all editor panels
   Editor::EditorUI::RenderHierarchy();
   Editor::EditorUI::RenderInspector();
   Editor::EditorUI::RenderConsole();
   Editor::EditorUI::RenderViewport();
   
   ImGui::Render();
   ```
3. [x] Verificar que toggle F1 funciona (mostrar/ocultar UI de Editor)
4. [x] Compilar y validar (CMake + MSBuild Debug, 0 errores, 0 warnings)
5. [x] Testing visual: Ejecutar aplicaci�n y verificar:
   - 4 panels visibles (Hierarchy, Inspector, Console, Viewport) ?
   - Panels pueden arrastrarse, redimensionarse y dockear ?
   - Toggle F1 muestra/oculta todos los panels ?
   - 220 FPS estables con ImGui activo ?

**Resultado**: Editor UI completo funcional con docking, similar a Unity/Unreal Editor layout. Performance �ptima (220 FPS con 4 panels activos, 38% GPU, 7% CPU). Layout profesional validado visualmente.

**Validaci�n visual confirmada**:
- ? Los 4 panels est�n visibles simult�neamente
- ? Docking funcional (panels pueden moverse y anclarse)
- ? Render 3D visible detr�s con fondo semitransparente (70% opacidad)
- ? Hierarchy: �rbol de escena completo con Scene Root, Camera, Lights, Geometry
- ? Inspector: Transform y Camera con propiedades editables
- ? Console: Logs coloreados (verde INFO, amarillo WARNING, rojo ERROR)
- ? Viewport: Placeholder con informaci�n t�cnica del render target

**Commit**: [Hash pendiente]

---

## ?? Resumen de Tareas

| Historia | Tarea | T�tulo | Estado |
|----------|-------|--------|--------|
| H1 | H1.1 | Descargar e integrar ImGui | ? Completada |
| H1 | H1.2 | A�adir ImGui a CMakeLists.txt | ? Completada |
| H1 | H1.3 | A�adir ImGui a Visual Studio project | ? Completada |
| H1 | H1.4 | Crear ImGui context en main.cpp | ? Completada |
| H2 | H2.1 | Crear descriptor heap SRV para ImGui | ? Completada |
| H2 | H2.2 | Inicializar backend ImGui DX12 | ? Completada |
| H2 | H2.3 | Integrar ImGui_ImplDX12_NewFrame() | ? Completada |
| H2 | H2.4 | Renderizar ImGui draw data en UIPass() | ? Completada |
| H3 | H3.1 | Inicializar backend ImGui Win32 | ? Completada |
| H3 | H3.2 | Procesar eventos Win32 para ImGui | ? Completada |
| H3 | H3.3 | Integrar ImGui_ImplWin32_NewFrame() | ? Completada |
| H4 | H4.1 | Crear panel Hierarchy | ? Completada |
| H4 | H4.2 | Crear panel Inspector | ? Completada |
| H4 | H4.3 | Crear panel Console | ? Completada |
| H4 | H4.4 | Crear panel Viewport | ? Completada |
| H4 | H4.5 | Habilitar docking y validar UI completa | ? Completada |

**Total**: **16 tareas (16 completadas, 0 pendientes)** ??

**Sprint v1.3.0 - ImGui Integration**: **100% COMPLETADO** ?
