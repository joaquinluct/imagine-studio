# Sprint Bugs (v1.3.0)

Bugs reportados durante el sprint activo pendientes de resoluci�n.

---

### BUG-001 - Clics de rat�n sin efecto en UI de ImGui

**ID**: BUG-001
**Prioridad**: Alta
**Estado**: Pendiente validaci�n logs usuario (Intento #4)
**Fecha de entrada**: 2025-01-18

**Descripci�n**: 

Los clics de rat�n no tienen efecto en la UI de ImGui:
- No cambia de men� al hacer clic
- No abre/cierra desplegables
- No responde a interacci�n con controles

**S�ntomas confirmados por usuario**:
- ? Ejecuci�n sin errores
- ? Ventana se muestra correctamente
- ? F1 toggle funciona correctamente
- ? Hover de rat�n S� funciona (cambio de color enter/exit)
- ? Clics de rat�n NO tienen efecto

**Archivos afectados**: `src/platform/Window.cpp`, `src/main.cpp`

**Fixes implementados**:

1. **Fix v1** (commit 80b7b7e):
   - Eliminada verificaci�n del retorno de `ImGui_ImplWin32_WndProcHandler()`
   - Handler ahora se llama SIEMPRE sin bloquear procesamiento de mensajes
   - Resultado: **Fix parcial** (el handler se llama, pero atlas bloqueado impide procesamiento de clics)

2. **Fix v2** (commit pendiente):
   - Eliminada llamada prematura a `io.Fonts->GetTexDataAsRGBA32()` en `main.cpp`
   - Causa ra�z: atlas bloqueado imped�a procesamiento de eventos de botones
   - ImGui ahora maneja el atlas autom�ticamente durante `NewFrame()`
   - Compilaci�n limpia: MSBuild (0 errores, 0 warnings)
   
3. **Intento #4** (commit actual):
   - A�adidos logs detallados de debugging en `Window.cpp` y `main.cpp`
   - Logs permitir�n diagnosticar exactamente d�nde se pierden los eventos de mouse
   - Compilaci�n limpia: CMake (0 errores, 0 warnings) + MSBuild (0 errores, 0 warnings)

**? ESTADO ACTUAL: PENDIENTE VALIDACI�N LOGS USUARIO**

El usuario debe:
1. Ejecutar la aplicaci�n (`x64\Debug\Imagine Studio.exe`)
2. Hacer click en la UI de ImGui
3. Reportar los logs generados para an�lisis

**Logs esperados:**
- `[WndProc] Mouse button DOWN message received: ...`
- `[WndProc] ImGui handler returned: ...`
- `[NewFrame] io.MouseDown[0]=...`

---

### BUG-002 - Click de rat�n no funciona en UI ImGui

**ID**: BUG-002
**Prioridad**: Cr�tica
**Estado**: Pendiente validaci�n usuario (Intento #6)
**Fecha de entrada**: 2025-01-18

**Descripci�n**: 

Los clics de rat�n no tienen efecto en la UI de ImGui:
- No cambia de men� al hacer clic
- No abre/cierra desplegables
- No responde a interacci�n con controles

**S�ntomas confirmados por logs usuario**:
- ? Ejecuci�n sin errores
- ? Ventana se muestra correctamente
- ? Error 1400 en CreateWindowExW ? **Intento #5: Eliminado fallback, muestra error y aborta**
- ? Sin logs `[WndProc]` ? Eventos de mouse NO llegan a WndProc
- ? `WantCaptureMouse=1` cuando mouse sobre UI ? ImGui detecta hover
- ? `io.MouseDown[0]=0` siempre ? ImGui nunca recibe clicks

**Causa ra�z identificada**:
Error 1400 (`ERROR_INVALID_WINDOW_HANDLE`) en `CreateWindowExW`. Hip�tesis del **Intento #6**: el uso de `SetWindowLongPtr(GWLP_USERDATA)` en `WM_NCCREATE` puede causar que Windows rechace la creaci�n de ventana al intentar modificar datos de una ventana que a�n no est� completamente inicializada.

**Archivos afectados**: `src/platform/Window.h`, `src/platform/Window.cpp`, `src/main.cpp`

**Fixes implementados**:

1. **Fix v1 (Intento #2)** - Construir atlas expl�citamente ?
2. **Fix v2 (Intento #3)** - Respetar valor de retorno de handler ImGui ??
3. **Fix v3 (Intento #4)** - Debugging con logs detallados ? (diagn�stico exitoso)
4. **Fix v4 (Intento #5)** - Corregir registro de clase y eliminar fallback ?? (error 1400 persiste)
5. **Fix v5 (Intento #6)** - Eliminar `GWLP_USERDATA`, usar map est�tico:
   - **NO** pasar `this` en `lpCreateParams` de `CreateWindowExW`
   - Usar `std::map<HWND, Window*>` est�tico para asociar HWND con instancia
   - Registrar asociaci�n DESPU�S de creaci�n exitosa de ventana
   - Simplificar `WndProcStatic` para buscar en map en lugar de `GetWindowLongPtr`
   - Compilaci�n limpia: CMake + MSBuild (0 errores, 0 warnings)

**? ESTADO ACTUAL: PENDIENTE VALIDACI�N USUARIO**

El usuario debe:
1. Ejecutar la aplicaci�n (`x64\Debug\Imagine Studio.exe`)
2. Verificar que:
   - ? Ventana se crea sin error 1400
   - ? Logs muestran `[WndProc] Mouse button DOWN/UP` al hacer click
   - ? Clicks funcionan en UI de ImGui

**Expectativa**:
- Si el uso de `GWLP_USERDATA` en `WM_NCCREATE` era el problema ? ventana se crear� correctamente
- Si persiste error 1400 ? puede ser restricci�n de Windows o permisos de sistema (investigar m�s)

---

(Sin otros bugs pendientes)
