# Sprint Fix (v1.3.0)

Bugs resueltos durante el sprint activo.

---

### FIX-001 - Clics de ratón sin efecto en UI de ImGui

**ID Original**: BUG-001
**Prioridad**: Alta
**Fecha de entrada**: 2025-01-18
**Fecha de resolución**: 2025-01-18

**Descripción del problema**: 

Los clics de ratón no tenían efecto en la UI de ImGui:
- No cambia de menú al hacer clic
- No abre/cierra desplegables
- No responde a interacción con controles

La tecla F1 (toggle UI) sí funcionaba correctamente, indicando que el problema era específico de eventos de ratón.

**Causa raíz identificada**:

La lógica en `Window::WndProc()` estaba **retornando inmediatamente** si `ImGui_ImplWin32_WndProcHandler()` retornaba un valor distinto de 0:

```cpp
// ? INCORRECTO (código anterior)
LRESULT imgui_result = ImGui_ImplWin32_WndProcHandler(hwnd_, message, wParam, lParam);
if (imgui_result != 0)
    return imgui_result; // Retornar inmediatamente bloquea eventos
```

Según la documentación oficial de ImGui, **el handler retorna 0 en la mayoría de casos** PERO sigue capturando y procesando eventos internamente. El código estaba bloqueando el procesamiento normal de mensajes cuando NO debía hacerlo.

ImGui gestiona eventos de ratón/teclado actualizando internamente `io.WantCaptureMouse` e `io.WantCaptureKeyboard`, pero **el handler DEBE ser llamado siempre** sin importar su valor de retorno.

**Solución implementada**:

Eliminar la verificación del retorno de `ImGui_ImplWin32_WndProcHandler()` y permitir que el procesamiento normal de mensajes continúe:

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

- `src/platform/Window.cpp` - Eliminar condición `if (imgui_result != 0) return imgui_result`
- `CMakeLists.txt` - Añadir linkado de ImGui a `material_test` (compilación auxiliar)

**Commit de resolución**: 80b7b7e

**Validación**:
- ? Compilación 1 (CMake): 0 errores, 0 warnings
- ? Compilación 2 (MSBuild): 0 errores, 0 warnings

**Notas importantes**:

- ?? **Lección aprendida**: `ImGui_ImplWin32_WndProcHandler()` debe llamarse **SIEMPRE** en el WndProc, sin verificar su retorno
- ?? El backend Win32 de ImGui gestiona eventos de forma pasiva: actualiza estado interno (`io.WantCaptureMouse`, etc.) pero NO bloquea mensajes de Windows
- ?? Verificar siempre la documentación oficial de backends antes de asumir comportamientos

**Referencias**:

- ImGui Win32 backend: `external/imgui/backends/imgui_impl_win32.h`
- Documentación oficial: https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_win32.cpp

---

### FIX-002 - ImFontAtlasBuilder nulo al llamar ImFontAtlasUpdateNewFrame

**ID Original**: BUG-002
**Prioridad**: Crítica
**Fecha de entrada**: 2025-01-18
**Fecha de resolución**: 2025-01-18

**Descripción del problema**: 

Al ejecutar la aplicación, se producían DOS crashes consecutivos en ImGui:

1. **Primer crash**: `atlas->Builder` era NULL en `ImFontAtlasUpdateNewFrame()` (`external/imgui/imgui_draw.cpp:2764`)
2. **Segundo crash**: `g.FontBaked` era NULL en `UpdateCurrentFontSize()` (`external/imgui/imgui.cpp`)

**Causa raíz identificada**:

El problema tenía dos partes:

1. **Builder NULL**: El `Builder` del atlas de fuentes se inicializa **SOLO** cuando se añade el primer font con `io.Fonts->AddFontDefault()`. Sin añadir ningún font, el Builder permanecía NULL cuando `ImGui_ImplDX12_NewFrame()` llamaba internamente a `ImFontAtlasUpdateNewFrame()`.

2. **FontBaked NULL**: `g.FontBaked` se inicializa **SOLO** en el primer `ImGui::NewFrame()`. Llamar a `ImGui_ImplDX12_Init()` ANTES del primer NewFrame/EndFrame causaba que funciones internas intentaran acceder a `g.FontBaked` antes de que existiera.

**Solución implementada**:

Dos cambios en `src/main.cpp`:

1. **Añadir font por defecto ANTES de backends**:
```cpp
// ? CRÍTICO: Añadir font por defecto ANTES de inicializar backends
io.Fonts->AddFontDefault();
ImGui_ImplWin32_Init(hwnd);
```

2. **Llamar a NewFrame/EndFrame ANTES de DX12 Init para bakear fonts**:
```cpp
// ? CRÍTICO: Llamar a NewFrame() + EndFrame() para inicializar g.FontBaked
// antes de inicializar el backend DX12 (soluciona crash en ImGui_ImplDX12_Init)
ImGui::NewFrame();
ImGui::EndFrame();
```

**Archivos afectados**: 

- `src/main.cpp` - Añadidas líneas `io.Fonts->AddFontDefault()` y `ImGui::NewFrame()/EndFrame()` antes de `ImGui_ImplDX12_Init()`
- `docs/sprint_bugs.md` - Limpiado (bug resuelto)
- `docs/sprint_fix.md` - Documentada resolución completa de BUG-002

**Commit de resolución**: (pendiente de crear)

**Notas importantes**:

- ?? **NUNCA se modificó código en `external/imgui/`** - Se siguió correctamente la política de `docs/THIRD_PARTY.md`
- ? La solución fue arreglar NUESTRO código de inicialización, no tapar el síntoma en la biblioteca
- ?? Esto demuestra la importancia de **no añadir checks defensivos en bibliotecas externas**: el error era NUESTRO, no de ImGui
- ?? **Lección aprendida 1**: ImGui requiere que se añada al menos un font ANTES de que los backends llamen a `NewFrame()`
- ?? **Lección aprendida 2**: `ImGui_ImplDX12_Init()` internamente accede a `g.FontBaked`, por lo que **DEBE** haber un `NewFrame()/EndFrame()` previo para inicializar el estado interno de ImGui
- ?? **Lección aprendida 3**: El orden correcto de inicialización de ImGui con múltiples backends es:
  1. `ImGui::CreateContext()`
  2. Configurar `io.ConfigFlags`, estilos, etc.
  3. `io.Fonts->AddFontDefault()` (o AddFont...)
  4. `ImGui_ImplWin32_Init(hwnd)` (backend de plataforma primero)
  5. **`ImGui::NewFrame()` + `ImGui::EndFrame()`** (bakear fonts y estado)
  6. `ImGui_ImplDX12_Init(...)` (backend de render después)
  7. Loop principal con NewFrame/Render/EndFrame

**Referencias**:

- `docs/THIRD_PARTY.md` - Política sobre bibliotecas externas
- ImGui documentation: https://github.com/ocornut/imgui/wiki/Getting-Started
