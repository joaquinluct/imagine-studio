# Sprint Fix (v1.3.0)

Bugs resueltos durante el sprint activo.

---

### FIX-001 - Clics de rat�n sin efecto en UI de ImGui

**ID Original**: BUG-001
**Prioridad**: Alta
**Fecha de entrada**: 2025-01-18
**Fecha de resoluci�n**: 2025-01-18

**Descripci�n del problema**: 

Los clics de rat�n no ten�an efecto en la UI de ImGui:
- No cambia de men� al hacer clic
- No abre/cierra desplegables
- No responde a interacci�n con controles

La tecla F1 (toggle UI) s� funcionaba correctamente, indicando que el problema era espec�fico de eventos de rat�n.

**Causa ra�z identificada**:

La l�gica en `Window::WndProc()` estaba **retornando inmediatamente** si `ImGui_ImplWin32_WndProcHandler()` retornaba un valor distinto de 0:

```cpp
// ? INCORRECTO (c�digo anterior)
LRESULT imgui_result = ImGui_ImplWin32_WndProcHandler(hwnd_, message, wParam, lParam);
if (imgui_result != 0)
    return imgui_result; // Retornar inmediatamente bloquea eventos
```

Seg�n la documentaci�n oficial de ImGui, **el handler retorna 0 en la mayor�a de casos** PERO sigue capturando y procesando eventos internamente. El c�digo estaba bloqueando el procesamiento normal de mensajes cuando NO deb�a hacerlo.

ImGui gestiona eventos de rat�n/teclado actualizando internamente `io.WantCaptureMouse` e `io.WantCaptureKeyboard`, pero **el handler DEBE ser llamado siempre** sin importar su valor de retorno.

**Soluci�n implementada**:

Eliminar la verificaci�n del retorno de `ImGui_ImplWin32_WndProcHandler()` y permitir que el procesamiento normal de mensajes contin�e:

```cpp
// ? CORRECTO
// Llamar SIEMPRE al handler de ImGui para que capture eventos internamente
// NO retornar inmediatamente - continuar con el procesamiento normal
ImGui_ImplWin32_WndProcHandler(hwnd_, message, wParam, lParam);

switch (message)
{
    // Procesamiento normal de mensajes...
}
```

**Archivos afectados**: 

- `src/platform/Window.cpp` - Eliminar condici�n `if (imgui_result != 0) return imgui_result`
- `CMakeLists.txt` - A�adir linkado de ImGui a `material_test` (compilaci�n auxiliar)

**Commit de resoluci�n**: 80b7b7e

**Validaci�n**:
- ? Compilaci�n 1 (CMake): 0 errores, 0 warnings
- ? Compilaci�n 2 (MSBuild): 0 errores, 0 warnings

**Notas importantes**:

- ?? **Lecci�n aprendida**: `ImGui_ImplWin32_WndProcHandler()` debe llamarse **SIEMPRE** en el WndProc, sin verificar su retorno
- ?? El backend Win32 de ImGui gestiona eventos de forma pasiva: actualiza estado interno (`io.WantCaptureMouse`, etc.) pero NO bloquea mensajes de Windows
- ?? Verificar siempre la documentaci�n oficial de backends antes de asumir comportamientos

**Referencias**:

- ImGui Win32 backend: `external/imgui/backends/imgui_impl_win32.h`
- Documentaci�n oficial: https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_win32.cpp

---

### FIX-002 - ImFontAtlasBuilder nulo al llamar ImFontAtlasUpdateNewFrame

**ID Original**: BUG-002
**Prioridad**: Cr�tica
**Fecha de entrada**: 2025-01-18
**Fecha de resoluci�n**: 2025-01-18

**Descripci�n del problema**: 

Al ejecutar la aplicaci�n, se produc�an DOS crashes consecutivos en ImGui:

1. **Primer crash**: `atlas->Builder` era NULL en `ImFontAtlasUpdateNewFrame()` (`external/imgui/imgui_draw.cpp:2764`)
2. **Segundo crash**: `g.FontBaked` era NULL en `UpdateCurrentFontSize()` (`external/imgui/imgui.cpp`)

**Causa ra�z identificada**:

El problema ten�a dos partes:

1. **Builder NULL**: El `Builder` del atlas de fuentes se inicializa **SOLO** cuando se a�ade el primer font con `io.Fonts->AddFontDefault()`. Sin a�adir ning�n font, el Builder permanec�a NULL cuando `ImGui_ImplDX12_NewFrame()` llamaba internamente a `ImFontAtlasUpdateNewFrame()`.

2. **FontBaked NULL**: `g.FontBaked` se inicializa **SOLO** en el primer `ImGui::NewFrame()`. Llamar a `ImGui_ImplDX12_Init()` ANTES del primer NewFrame/EndFrame causaba que funciones internas intentaran acceder a `g.FontBaked` antes de que existiera.

**Soluci�n implementada**:

Dos cambios en `src/main.cpp`:

1. **A�adir font por defecto ANTES de backends**:
```cpp
// ? CR�TICO: A�adir font por defecto ANTES de inicializar backends
io.Fonts->AddFontDefault();
ImGui_ImplWin32_Init(hwnd);
```

2. **Llamar a NewFrame/EndFrame ANTES de DX12 Init para bakear fonts**:
```cpp
// ? CR�TICO: Llamar a NewFrame() + EndFrame() para inicializar g.FontBaked
// antes de inicializar el backend DX12 (soluciona crash en ImGui_ImplDX12_Init)
ImGui::NewFrame();
ImGui::EndFrame();
```

**Archivos afectados**: 

- `src/main.cpp` - A�adidas l�neas `io.Fonts->AddFontDefault()` y `ImGui::NewFrame()/EndFrame()` antes de `ImGui_ImplDX12_Init()`
- `docs/sprint_bugs.md` - Limpiado (bug resuelto)
- `docs/sprint_fix.md` - Documentada resoluci�n completa de BUG-002

**Commit de resoluci�n**: (pendiente de crear)

**Notas importantes**:

- ?? **NUNCA se modific� c�digo en `external/imgui/`** - Se sigui� correctamente la pol�tica de `docs/THIRD_PARTY.md`
- ? La soluci�n fue arreglar NUESTRO c�digo de inicializaci�n, no tapar el s�ntoma en la biblioteca
- ?? Esto demuestra la importancia de **no a�adir checks defensivos en bibliotecas externas**: el error era NUESTRO, no de ImGui
- ?? **Lecci�n aprendida 1**: ImGui requiere que se a�ada al menos un font ANTES de que los backends llamen a `NewFrame()`
- ?? **Lecci�n aprendida 2**: `ImGui_ImplDX12_Init()` internamente accede a `g.FontBaked`, por lo que **DEBE** haber un `NewFrame()/EndFrame()` previo para inicializar el estado interno de ImGui
- ?? **Lecci�n aprendida 3**: El orden correcto de inicializaci�n de ImGui con m�ltiples backends es:
  1. `ImGui::CreateContext()`
  2. Configurar `io.ConfigFlags`, estilos, etc.
  3. `io.Fonts->AddFontDefault()` (o AddFont...)
  4. `ImGui_ImplWin32_Init(hwnd)` (backend de plataforma primero)
  5. **`ImGui::NewFrame()` + `ImGui::EndFrame()`** (bakear fonts y estado)
  6. `ImGui_ImplDX12_Init(...)` (backend de render despu�s)
  7. Loop principal con NewFrame/Render/EndFrame

**Referencias**:

- `docs/THIRD_PARTY.md` - Pol�tica sobre bibliotecas externas
- ImGui documentation: https://github.com/ocornut/imgui/wiki/Getting-Started
