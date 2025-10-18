# Sprint Bugs (v1.3.0)

Bugs reportados durante el sprint activo pendientes de resolución.

---

### BUG-001 - Clics de ratón sin efecto en UI de ImGui

**ID**: BUG-001
**Prioridad**: Alta
**Estado**: Pendiente validación logs usuario (Intento #4)
**Fecha de entrada**: 2025-01-18

**Descripción**: 

Los clics de ratón no tienen efecto en la UI de ImGui:
- No cambia de menú al hacer clic
- No abre/cierra desplegables
- No responde a interacción con controles

**Síntomas confirmados por usuario**:
- ? Ejecución sin errores
- ? Ventana se muestra correctamente
- ? F1 toggle funciona correctamente
- ? Hover de ratón SÍ funciona (cambio de color enter/exit)
- ? Clics de ratón NO tienen efecto

**Archivos afectados**: `src/platform/Window.cpp`, `src/main.cpp`

**Fixes implementados**:

1. **Fix v1** (commit 80b7b7e):
   - Eliminada verificación del retorno de `ImGui_ImplWin32_WndProcHandler()`
   - Handler ahora se llama SIEMPRE sin bloquear procesamiento de mensajes
   - Resultado: **Fix parcial** (el handler se llama, pero atlas bloqueado impide procesamiento de clics)

2. **Fix v2** (commit pendiente):
   - Eliminada llamada prematura a `io.Fonts->GetTexDataAsRGBA32()` en `main.cpp`
   - Causa raíz: atlas bloqueado impedía procesamiento de eventos de botones
   - ImGui ahora maneja el atlas automáticamente durante `NewFrame()`
   - Compilación limpia: MSBuild (0 errores, 0 warnings)
   
3. **Intento #4** (commit actual):
   - Añadidos logs detallados de debugging en `Window.cpp` y `main.cpp`
   - Logs permitirán diagnosticar exactamente dónde se pierden los eventos de mouse
   - Compilación limpia: CMake (0 errores, 0 warnings) + MSBuild (0 errores, 0 warnings)

**? ESTADO ACTUAL: PENDIENTE VALIDACIÓN LOGS USUARIO**

El usuario debe:
1. Ejecutar la aplicación (`x64\Debug\Imagine Studio.exe`)
2. Hacer click en la UI de ImGui
3. Reportar los logs generados para análisis

**Logs esperados:**
- `[WndProc] Mouse button DOWN message received: ...`
- `[WndProc] ImGui handler returned: ...`
- `[NewFrame] io.MouseDown[0]=...`

---

### BUG-002 - Click de ratón no funciona en UI ImGui

**ID**: BUG-002
**Prioridad**: Crítica
**Estado**: Pendiente validación usuario (Intento #6)
**Fecha de entrada**: 2025-01-18

**Descripción**: 

Los clics de ratón no tienen efecto en la UI de ImGui:
- No cambia de menú al hacer clic
- No abre/cierra desplegables
- No responde a interacción con controles

**Síntomas confirmados por logs usuario**:
- ? Ejecución sin errores
- ? Ventana se muestra correctamente
- ? Error 1400 en CreateWindowExW ? **Intento #5: Eliminado fallback, muestra error y aborta**
- ? Sin logs `[WndProc]` ? Eventos de mouse NO llegan a WndProc
- ? `WantCaptureMouse=1` cuando mouse sobre UI ? ImGui detecta hover
- ? `io.MouseDown[0]=0` siempre ? ImGui nunca recibe clicks

**Causa raíz identificada**:
Error 1400 (`ERROR_INVALID_WINDOW_HANDLE`) en `CreateWindowExW`. Hipótesis del **Intento #6**: el uso de `SetWindowLongPtr(GWLP_USERDATA)` en `WM_NCCREATE` puede causar que Windows rechace la creación de ventana al intentar modificar datos de una ventana que aún no está completamente inicializada.

**Archivos afectados**: `src/platform/Window.h`, `src/platform/Window.cpp`, `src/main.cpp`

**Fixes implementados**:

1. **Fix v1 (Intento #2)** - Construir atlas explícitamente ?
2. **Fix v2 (Intento #3)** - Respetar valor de retorno de handler ImGui ??
3. **Fix v3 (Intento #4)** - Debugging con logs detallados ? (diagnóstico exitoso)
4. **Fix v4 (Intento #5)** - Corregir registro de clase y eliminar fallback ?? (error 1400 persiste)
5. **Fix v5 (Intento #6)** - Eliminar `GWLP_USERDATA`, usar map estático:
   - **NO** pasar `this` en `lpCreateParams` de `CreateWindowExW`
   - Usar `std::map<HWND, Window*>` estático para asociar HWND con instancia
   - Registrar asociación DESPUÉS de creación exitosa de ventana
   - Simplificar `WndProcStatic` para buscar en map en lugar de `GetWindowLongPtr`
   - Compilación limpia: CMake + MSBuild (0 errores, 0 warnings)

**? ESTADO ACTUAL: PENDIENTE VALIDACIÓN USUARIO**

El usuario debe:
1. Ejecutar la aplicación (`x64\Debug\Imagine Studio.exe`)
2. Verificar que:
   - ? Ventana se crea sin error 1400
   - ? Logs muestran `[WndProc] Mouse button DOWN/UP` al hacer click
   - ? Clicks funcionan en UI de ImGui

**Expectativa**:
- Si el uso de `GWLP_USERDATA` en `WM_NCCREATE` era el problema ? ventana se creará correctamente
- Si persiste error 1400 ? puede ser restricción de Windows o permisos de sistema (investigar más)

---

(Sin otros bugs pendientes)
