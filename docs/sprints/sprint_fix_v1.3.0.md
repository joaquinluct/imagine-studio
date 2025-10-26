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

### FIX-002 - Click de rat�n no funciona en UI ImGui (CreateWindowExW error 1400)

**ID Original**: BUG-002
**Prioridad**: Cr�tica
**Fecha de entrada**: 2025-01-18
**Fecha de resoluci�n**: 2025-01-18

**Descripci�n del problema**: 

Los clics de rat�n no ten�an efecto en la UI de ImGui:
- No cambia de men� al hacer clic
- No abre/cierra desplegables
- No responde a interacci�n con controles
- Hover S� funciona (cambio de color enter/exit)
- F1 toggle UI funciona correctamente

**S�ntomas confirmados por logs**:
- ? Error 1400 (`ERROR_INVALID_WINDOW_HANDLE`) en `CreateWindowExW`
- ? Fallback a clase `STATIC` permite arranque pero sin eventos de mouse
- ? `WndProc` **NUNCA** recibe mensajes `WM_LBUTTONDOWN`/`WM_LBUTTONUP`
- ? `WantCaptureMouse=1` cuando mouse sobre UI (ImGui detecta hover)
- ? `io.MouseDown[0]=0` siempre (ImGui nunca recibe clicks)

**Causa ra�z identificada**:

El uso de `SetWindowLongPtr(GWLP_USERDATA)` en `WM_NCCREATE` causaba que Windows rechazara la creaci�n de la ventana con error 1400. 

**�Por qu� fallaba?**

En `WndProcStatic`, el c�digo intentaba guardar el puntero `Window*` en `GWLP_USERDATA` durante el mensaje `WM_NCCREATE`:

```cpp
// ? PROBLEM�TICO (c�digo anterior)
LRESULT CALLBACK Window::WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_NCCREATE)
    {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        Window* win = reinterpret_cast<Window*>(cs->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(win)); // ? FALLA
    }
    // ...
}
```

**El problema**: `WM_NCCREATE` ocurre **ANTES** de que la ventana est� completamente inicializada. Al intentar modificar `GWLP_USERDATA` prematuramente, Windows detectaba un problema y rechazaba la creaci�n con error 1400.

**Soluci�n implementada (Intento #6)**:

Eliminar completamente el uso de `GWLP_USERDATA` y reemplazar con un `std::map<HWND, Window*>` est�tico:

**1. En `Window.h` - A�adir map est�tico:**
```cpp
class Window {
    // ...existing code...
private:
    static std::map<HWND, Window*> s_windowMap;  // ??
};
```

**2. En `Window.cpp` - Definir map fuera de clase:**
```cpp
std::map<HWND, Window*> Window::s_windowMap;
```

**3. En Constructor - NO pasar `this` en `lpCreateParams`:**
```cpp
hwnd_ = CreateWindowExW(
    0, CLASS_NAME, title, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, width, height,
    NULL, NULL, moduleInstance,
    nullptr  // ?? NO pasar 'this' (evita problemas con WM_NCCREATE)
);

// Registrar asociaci�n DESPU�S de creaci�n exitosa
if (hwnd_) {
    s_windowMap[hwnd_] = this;  // ?
}
```

**4. En `WndProcStatic` - Buscar en map en lugar de `GWLP_USERDATA`:**
```cpp
LRESULT CALLBACK Window::WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    auto it = s_windowMap.find(hWnd);  // ?
    if (it != s_windowMap.end())
    {
        return it->second->WndProc(message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
```

**Archivos afectados**: 

- `src/platform/Window.h` - A�adir `s_windowMap` est�tico
- `src/platform/Window.cpp` - Implementar map, eliminar `GWLP_USERDATA`, simplificar `WndProcStatic`
- `docs/sprint_bug_attempts.md` - **Nuevo archivo** con tracking completo de todos los intentos
- `docs/sprint_bugs.md` - Estado actualizado

**Intentos de soluci�n previos**:

1. **Intento #1-3**: Corregir inicializaci�n de ImGui y atlas de fuentes (parcial)
2. **Intento #4**: A�adir logs detallados ? Diagn�stico exitoso (identific� que eventos no llegaban a WndProc)
3. **Intento #5**: Corregir registro de clase y eliminar fallback STATIC (error 1400 persisti�)
4. **Intento #6**: Eliminar `GWLP_USERDATA` y usar map est�tico ? ? **�XITO**

**Commits de resoluci�n**: 
- `07fc72c` - Intento #5 (eliminaci�n de fallback)
- `9cd5a85` - Intento #6 (soluci�n final)

**Validaci�n del usuario (2025-01-18)**:

Usuario report�: **"Ole ole ole!! (Aplausos). Puntuaci�n sobre 10: 10 de 10!! ole!"**

1. ? Ejecuci�n sin errores
2. ? F1/Toggle UI perfecta
3. ? Uso del rat�n sobre la UI perfecto (hover, click)
4. ? Todos los men�s y submen�s funcionan correctamente
5. ? Click en todas las opciones funciona
6. ? Cajas de texto funcionan (teclado tambi�n funciona)
7. ? Salida del programa (ESC) limpia y sin errores

**Notas importantes**:

- ?? **NUNCA se modific� c�digo en `external/imgui/`** - Se sigui� la pol�tica de `docs/THIRD_PARTY.md`
- ?? **Lecci�n aprendida CR�TICA**: **NUNCA** usar `SetWindowLongPtr(GWLP_USERDATA)` en `WM_NCCREATE`
- ? **SIEMPRE** usar un map/diccionario est�tico para asociar `HWND` con instancias de clase
- ? Registrar la asociaci�n **DESPU�S** de que `CreateWindowExW` retorne exitosamente
- ?? El map est�tico es m�s robusto y no depende del timing interno de Windows
- ?? El proceso de debugging sistem�tico con logs fue CR�TICO para identificar la causa ra�z

**Referencias**:

- `docs/THIRD_PARTY.md` - Pol�tica sobre bibliotecas externas
- `docs/sprint_bug_attempts.md` - Tracking completo de todos los intentos (nuevo archivo)
- Win32 API Documentation: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
