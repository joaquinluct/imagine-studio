# Sprint Bug Attempts (v1.3.0)

**Prop�sito**: Este archivo registra TODOS los intentos de soluci�n para cada bug activo, incluyendo enfoques fallidos, parciales y exitosos. Esto permite trazabilidad completa del proceso de debugging y evita repetir soluciones ya probadas.

**Formato**: Cada bug tiene su propia secci�n con lista cronol�gica de intentos.

---

## BUG-002 - Click de rat�n no funciona en UI ImGui (RESUELTO)

### Intento 1: A�adir flag ImGuiBackendFlags_RendererHasTextures
**Fecha**: 2025-01-18
**Commit**: [pendiente]
**Enfoque**: Configurar `io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures` ANTES de `AddFontDefault()`
**Resultado**: ? **FALLO** - Assertion fail "Cannot use dynamic font size with locked ImFontAtlas!"
**Causa**: El atlas no estaba construido (built) antes de inicializar backends
**Lecci�n aprendida**: El flag solo indica soporte de texturas din�micas, pero el atlas debe estar construido primero

### Intento 2: Construir atlas expl�citamente
**Fecha**: 2025-01-18
**Commit**: [pendiente]
**Enfoque**: 
1. A�adir fuente por defecto: `io.Fonts->AddFontDefault()`
2. Construir atlas expl�citamente: `io.Fonts->Build()`
3. Configurar flag DESPU�S de Build: `io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures`

**Resultado**: ? **PARCIAL** - Se eliminaron los assertion fails, pero los clicks segu�an sin funcionar
**Causa**: El atlas estaba correctamente inicializado, pero los eventos de rat�n no se procesaban correctamente en WndProc
**Lecci�n aprendida**: Inicializaci�n correcta del atlas es necesaria pero no suficiente para eventos de rat�n

### Intento 3: Respetar valor de retorno de ImGui_ImplWin32_WndProcHandler
**Fecha**: 2025-01-18
**Commit**: [pendiente]
**Enfoque**: 
```cpp
// ANTES (INCORRECTO):
ImGui_ImplWin32_WndProcHandler(hwnd_, message, wParam, lParam); // Ignoraba retorno
// ... procesaba el evento de todos modos

// DESPU�S (CORRECTO):
if (ImGui_ImplWin32_WndProcHandler(hwnd_, message, wParam, lParam))
    return true; // ImGui captur� el evento, NO pasar a DefWindowProc
```

**Resultado**: ? **PENDIENTE VALIDACI�N USUARIO**
**Causa ra�z hipot�tica**: No se respetaba el valor de retorno del handler. Cuando ImGui capturaba un evento (retorna != 0), el evento se pasaba igualmente a DefWindowProc, lo que interfer�a con el procesamiento de ImGui.
**Archivos modificados**: `src/platform/Window.cpp`, `src/main.cpp`

**Lecci�n aprendida (hipot�tica)**: 
- El handler de eventos de ImGui **DEBE** retornar true cuando captura un evento
- Si retorna true, NO debemos pasar el evento a DefWindowProc
- El orden correcto de inicializaci�n del atlas es cr�tico pero ortogonal al procesamiento de eventos

**Estado actual**: Esperando validaci�n del usuario para confirmar que el fix funciona en ejecuci�n real.

**Soluci�n final (si se confirma)**:
1. Atlas construido correctamente (Intento 2)
2. Eventos respetados correctamente (Intento 3)

### Intento 4: Debugging con logs detallados de eventos de mouse
**Fecha**: 2025-01-18
**Commit**: [completado]
**Enfoque**: 
A�adir logs exhaustivos para diagnosticar exactamente d�nde se pierden los eventos de mouse

**Resultado**: ? **DIAGN�STICO EXITOSO**
**Hallazgos confirmados por logs del usuario**:
1. ? **NO hay logs de `[WndProc]`** ? Los mensajes WM_LBUTTONDOWN/WM_LBUTTONUP NO llegan a WndProc
2. ? `io.MouseDown[0]` siempre es 0 ? ImGui nunca recibe notificaci�n de clicks
3. ? `WantCaptureMouse=1` cuando est� sobre UI ? ImGui detecta hover correctamente
4. ? Error 1400 en CreateWindowEx ? Ventana creada con clase fallback `STATIC`
5. ? Fallback a clase STATIC exitoso ? `HWND=0000000000310B80` pero sin eventos de mouse

**Logs clave del usuario**:
```
[ERROR] Window::Window - CreateWindowEx failed, GetLastError=1400
[INFO] Window::Window - fallback using WC_OVERLAPPEDWINDOW succeeded, HWND=0000000000310B80
[INFO] [NewFrame] io.MouseDown[0]=0, io.MouseDown[1]=0, WantCaptureMouse=0
[INFO] [NewFrame] io.MouseDown[0]=0, io.MouseDown[1]=0, WantCaptureMouse=1  # ? Mouse sobre UI
# ?? NO hay ning�n log [WndProc] Mouse button DOWN/UP
```

**Causa ra�z identificada**: 
La ventana se est� creando con clase fallback `STATIC` en lugar de nuestra clase personalizada `ImagineStudioWindowClass`. La clase STATIC no tiene nuestro `WndProc` correctamente asociado, por lo que los eventos de mouse (WM_LBUTTONDOWN, WM_LBUTTONUP, etc.) **no llegan a WndProc**.

**�Por qu� falla CreateWindowExW?**
- Error 1400 = `ERROR_INVALID_WINDOW_HANDLE` sugiere problema con el registro de clase
- Posibles causas:
  1. `RegisterClassExW` falla silenciosamente (atom=0 no verificado)
  2. Mismatch entre `hInstance` usado en registro vs creaci�n
  3. Clase ya registrada pero con diferente `hInstance`

**Lecci�n aprendida**:
- ? Los logs fueron CR�TICOS para identificar que WndProc nunca recibi� eventos
- ? El error 1400 indica problema en el registro/creaci�n de la ventana personalizada
- ? El fallback a clase STATIC enmascara el problema y hace debugging m�s dif�cil
- ? La clase STATIC NO es adecuada para ventanas interactivas (no procesa eventos correctamente)

### Intento 5: Corregir registro de clase y eliminaci�n de fallback
**Fecha**: 2025-01-18
**Commit**: [completado]
**Enfoque**:
El problema identificado en Intento #4 es que `CreateWindowExW` falla con error 1400 y se usa fallback con clase `STATIC`, que no tiene nuestro WndProc correctamente asociado.

Soluci�n implementada:
1. ? Usar `GetModuleHandle(NULL)` **consistentemente** en registro y creaci�n de ventana
2. ? Verificar si la clase ya est� registrada antes de intentar registrarla de nuevo
3. ? Usar nombre de clase directamente (no atom) en `CreateWindowExW`
4. ? **ELIMINAR** fallback a clase STATIC (enmascara el problema real)
5. ? Si la creaci�n falla, abortar con error detallado en lugar de continuar con ventana defectuosa

**Cambios en c�digo**:
```cpp
// ANTES (problem�tico):
HINSTANCE regInstance = hInstance_ ? hInstance_ : GetModuleHandle(NULL);
// ... RegisterClassExW con regInstance
// ... CreateWindowExW falla ? usa fallback con clase STATIC

// DESPU�S (correcto):
HINSTANCE moduleInstance = GetModuleHandle(NULL);  // Siempre el mismo
BOOL classExists = GetClassInfoExW(moduleInstance, CLASS_NAME, &existingClass);
if (!classExists)
{
    // Registrar clase solo si no existe
    ATOM atom = RegisterClassExW(&wc);
    if (atom == 0) { /* ABORTAR con error */ }
}
hwnd_ = CreateWindowExW(0, CLASS_NAME, title, ..., moduleInstance, ...);
if (!hwnd_) { /* ABORTAR con error detallado - SIN FALLBACK */ }
```

**Archivos modificados**: `src/platform/Window.cpp`

**Resultado**: ? **COMPILACI�N LIMPIA**
- CMake Build (Debug): 0 errores, 0 warnings
- MSBuild "Imagine Studio.sln" (Debug): 0 errores, 0 warnings

**Estado**: ? **PENDIENTE VALIDACI�N USUARIO**

El usuario debe:
1. Ejecutar la aplicaci�n (`x64\Debug\Imagine Studio.exe`)
2. Verificar que:
   - ? La ventana se crea correctamente (sin error 1400)
   - ? Los logs muestran `[WndProc] Mouse button DOWN/UP` al hacer click
   - ? Los clicks funcionan en la UI de ImGui (abrir men�s, botones, etc.)

**Expectativa**: 
- Si el problema era el mismatch de `hInstance`, la ventana deber�a crearse correctamente ahora
- Si persiste error 1400, necesitamos investigar m�s (posible conflicto con sistema operativo o permisos)

### Intento 6: Eliminar uso de GWLP_USERDATA y usar map est�tico
**Fecha**: 2025-01-18
**Commit**: [completado]
**Enfoque**:
El error 1400 persiste en Intento #5 a pesar de usar `GetModuleHandle(NULL)` consistente y eliminar fallback. Nueva hip�tesis: el problema puede estar en el uso de `SetWindowLongPtr(GWLP_USERDATA)` en `WM_NCCREATE`, que intenta modificar datos de una ventana que a�n no est� completamente creada.

Soluci�n implementada:
1. **Eliminar** uso de `GWLP_USERDATA` en `WM_NCCREATE` completamente
2. **Usar** un `std::map<HWND, Window*>` est�tico para asociar `HWND` con instancia `Window*`
3. **No pasar** `this` en `lpCreateParams` de `CreateWindowExW`
4. **Registrar** la asociaci�n HWND ? Window* DESPU�S de creaci�n exitosa de ventana
5. **Simplificar** `WndProcStatic` para buscar en el map en lugar de `GetWindowLongPtr`

**Cambios en c�digo**:
```cpp
// Window.h - A�adir map est�tico
class Window {
    // ...existing code...
private:
    static std::map<HWND, Window*> s_windowMap;  // ??
};

// Window.cpp - Definir map fuera de clase
std::map<HWND, Window*> Window::s_windowMap;

// Window.cpp - Constructor: NO pasar 'this' en lpCreateParams
hwnd_ = CreateWindowExW(
    0, CLASS_NAME, title, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, width, height,
    NULL, NULL, moduleInstance,
    nullptr  // ?? NO pasar 'this' (evita problemas con WM_NCCREATE)
);

// Registrar asociaci�n DESPU�S de creaci�n exitosa
if (hwnd_) {
    s_windowMap[hwnd_] = this;  // ??
}

// WndProcStatic - Buscar en map en lugar de GWLP_USERDATA
LRESULT CALLBACK Window::WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    auto it = s_windowMap.find(hWnd);  // ??
    if (it != s_windowMap.end())
    {
        return it->second->WndProc(message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
```

**Archivos modificados**:
- `src/platform/Window.h`: A�adir `s_windowMap` est�tico
- `src/platform/Window.cpp`: Implementar map, eliminar `GWLP_USERDATA`, simplificar `WndProcStatic`

**Resultado**: ? **�XITO COMPLETO**

**Validaci�n del usuario (2025-01-18)**:
1. ? Ejecuci�n sin errores
2. ? F1/Toggle UI perfecta
3. ? Uso del rat�n sobre la UI perfecto (hover, click)
4. ? Todos los men�s y submen�s funcionan correctamente
5. ? Click en todas las opciones funciona
6. ? Cajas de texto funcionan (teclado tambi�n funciona)
7. ? Salida del programa (ESC) limpia y sin errores

**Causa ra�z confirmada**: 
El uso de `SetWindowLongPtr(GWLP_USERDATA)` en `WM_NCCREATE` causaba que Windows rechazara la creaci�n de la ventana con error 1400 (`ERROR_INVALID_WINDOW_HANDLE`). Al intentar modificar datos de una ventana que a�n no estaba completamente inicializada, Windows detectaba un problema y abortaba la creaci�n.

**Lecci�n aprendida CR�TICA**:
- ? **NUNCA** usar `SetWindowLongPtr(GWLP_USERDATA)` en `WM_NCCREATE`
- ? **SIEMPRE** usar un map/diccionario est�tico para asociar `HWND` con instancias de clase
- ? Registrar la asociaci�n **DESPU�S** de que `CreateWindowExW` retorne exitosamente
- ? El map est�tico es m�s robusto y no depende del timing interno de Windows

**Puntuaci�n del usuario**: 10 de 10 ??

---

## BUG-001 - [Ejemplo de formato para futuros bugs]

### Intento 1: [Descripci�n breve del enfoque]
**Fecha**: YYYY-MM-DD
**Commit**: [hash del commit o "sin commit"]
**Enfoque**: 
- Detalle t�cnico del cambio
- C�digo relevante si procede

**Resultado**: ? FALLO / ?? PARCIAL / ? �XITO
**Causa**: Explicaci�n de por qu� funcion� o no funcion�
**Lecci�n aprendida**: Qu� aprendimos de este intento

---

## Notas Generales

- **Cada intento debe tener su propia entrada**, incluso si fue fallido
- **Incluir c�digo relevante** en bloques de c�digo cuando sea �til para futuras referencias
- **Documentar SIEMPRE la causa ra�z** de fallos para evitar repetir errores
- **Al resolver un bug**, resumir la soluci�n final como combinaci�n de intentos exitosos
- Este archivo se archiva al finalizar el sprint como `sprint_bug_attempts_v<version>.md`
