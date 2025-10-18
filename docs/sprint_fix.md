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

### FIX-002 - Click de ratón no funciona en UI ImGui (CreateWindowExW error 1400)

**ID Original**: BUG-002
**Prioridad**: Crítica
**Fecha de entrada**: 2025-01-18
**Fecha de resolución**: 2025-01-18

**Descripción del problema**: 

Los clics de ratón no tenían efecto en la UI de ImGui:
- No cambia de menú al hacer clic
- No abre/cierra desplegables
- No responde a interacción con controles
- Hover SÍ funciona (cambio de color enter/exit)
- F1 toggle UI funciona correctamente

**Síntomas confirmados por logs**:
- ? Error 1400 (`ERROR_INVALID_WINDOW_HANDLE`) en `CreateWindowExW`
- ? Fallback a clase `STATIC` permite arranque pero sin eventos de mouse
- ? `WndProc` **NUNCA** recibe mensajes `WM_LBUTTONDOWN`/`WM_LBUTTONUP`
- ? `WantCaptureMouse=1` cuando mouse sobre UI (ImGui detecta hover)
- ? `io.MouseDown[0]=0` siempre (ImGui nunca recibe clicks)

**Causa raíz identificada**:

El uso de `SetWindowLongPtr(GWLP_USERDATA)` en `WM_NCCREATE` causaba que Windows rechazara la creación de la ventana con error 1400. 

**¿Por qué fallaba?**

En `WndProcStatic`, el código intentaba guardar el puntero `Window*` en `GWLP_USERDATA` durante el mensaje `WM_NCCREATE`:

```cpp
// ? PROBLEMÁTICO (código anterior)
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

**El problema**: `WM_NCCREATE` ocurre **ANTES** de que la ventana esté completamente inicializada. Al intentar modificar `GWLP_USERDATA` prematuramente, Windows detectaba un problema y rechazaba la creación con error 1400.

**Solución implementada (Intento #6)**:

Eliminar completamente el uso de `GWLP_USERDATA` y reemplazar con un `std::map<HWND, Window*>` estático:

**1. En `Window.h` - Añadir map estático:**
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

// Registrar asociación DESPUÉS de creación exitosa
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

- `src/platform/Window.h` - Añadir `s_windowMap` estático
- `src/platform/Window.cpp` - Implementar map, eliminar `GWLP_USERDATA`, simplificar `WndProcStatic`
- `docs/sprint_bug_attempts.md` - **Nuevo archivo** con tracking completo de todos los intentos
- `docs/sprint_bugs.md` - Estado actualizado

**Intentos de solución previos**:

1. **Intento #1-3**: Corregir inicialización de ImGui y atlas de fuentes (parcial)
2. **Intento #4**: Añadir logs detallados ? Diagnóstico exitoso (identificó que eventos no llegaban a WndProc)
3. **Intento #5**: Corregir registro de clase y eliminar fallback STATIC (error 1400 persistió)
4. **Intento #6**: Eliminar `GWLP_USERDATA` y usar map estático ? ? **ÉXITO**

**Commits de resolución**: 
- `07fc72c` - Intento #5 (eliminación de fallback)
- `9cd5a85` - Intento #6 (solución final)

**Validación del usuario (2025-01-18)**:

Usuario reportó: **"Ole ole ole!! (Aplausos). Puntuación sobre 10: 10 de 10!! ole!"**

1. ? Ejecución sin errores
2. ? F1/Toggle UI perfecta
3. ? Uso del ratón sobre la UI perfecto (hover, click)
4. ? Todos los menús y submenús funcionan correctamente
5. ? Click en todas las opciones funciona
6. ? Cajas de texto funcionan (teclado también funciona)
7. ? Salida del programa (ESC) limpia y sin errores

**Notas importantes**:

- ?? **NUNCA se modificó código en `external/imgui/`** - Se siguió la política de `docs/THIRD_PARTY.md`
- ?? **Lección aprendida CRÍTICA**: **NUNCA** usar `SetWindowLongPtr(GWLP_USERDATA)` en `WM_NCCREATE`
- ? **SIEMPRE** usar un map/diccionario estático para asociar `HWND` con instancias de clase
- ? Registrar la asociación **DESPUÉS** de que `CreateWindowExW` retorne exitosamente
- ?? El map estático es más robusto y no depende del timing interno de Windows
- ?? El proceso de debugging sistemático con logs fue CRÍTICO para identificar la causa raíz

**Referencias**:

- `docs/THIRD_PARTY.md` - Política sobre bibliotecas externas
- `docs/sprint_bug_attempts.md` - Tracking completo de todos los intentos (nuevo archivo)
- Win32 API Documentation: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
