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
**Estado**: Pendiente validaci�n usuario (Intento #5)
**Fecha de entrada**: 2025-01-18

**Descripci�n**: 

Los clics de rat�n no tienen efecto en la UI de ImGui:
- No cambia de men� al hacer clic
- No abre/cierra desplegables
- No responde a interacci�n con controles

**S�ntomas confirmados por logs usuario (Intento #4)**:
- ? Ejecuci�n sin errores
- ? Ventana se muestra correctamente
- ? Error 1400 en CreateWindowExW ? Fallback a clase STATIC
- ? Sin logs `[WndProc]` ? Eventos de mouse NO llegan a WndProc
- ? `WantCaptureMouse=1` cuando mouse sobre UI ? ImGui detecta hover
- ? `io.MouseDown[0]=0` siempre ? ImGui nunca recibe clicks

**Causa ra�z identificada**:
La ventana se crea con clase fallback `STATIC` debido a error 1400 en `CreateWindowExW`. La clase STATIC no tiene nuestro `WndProc` correctamente asociado, por lo que los eventos de mouse (WM_LBUTTONDOWN, WM_LBUTTONUP) **no llegan a WndProc**.

**Archivos afectados**: `src/platform/Window.cpp`, `src/main.cpp`

**Fixes implementados**:

1. **Fix v1 (Intento #2)** - Construir atlas expl�citamente:
   - A�adir `io.Fonts->AddFontDefault()` + `io.Fonts->Build()`
   - Resultado: ? PARCIAL - Atlas correcto pero clicks siguen sin funcionar

2. **Fix v2 (Intento #3)** - Respetar valor de retorno de handler ImGui:
   - Verificar retorno de `ImGui_ImplWin32_WndProcHandler()` antes de procesar
   - Resultado: ?? HIPOT�TICO - No validado porque eventos no llegan a WndProc

3. **Fix v3 (Intento #4)** - Debugging con logs detallados:
   - A�adir logs exhaustivos para diagnosticar p�rdida de eventos
   - Resultado: ? DIAGN�STICO EXITOSO - Identific� causa ra�z (clase STATIC)

4. **Fix v4 (Intento #5)** - Corregir registro de clase y eliminar fallback:
   - Usar `GetModuleHandle(NULL)` consistentemente
   - Verificar si clase ya existe antes de registrar
   - **ELIMINAR** fallback a clase STATIC
   - Abortar con error claro si creaci�n falla
   - Compilaci�n limpia: CMake + MSBuild (0 errores, 0 warnings)

**? ESTADO ACTUAL: PENDIENTE VALIDACI�N USUARIO**

El usuario debe:
1. Ejecutar la aplicaci�n (`x64\Debug\Imagine Studio.exe`)
2. Verificar que:
   - ? Ventana se crea sin error 1400
   - ? Logs muestran `[WndProc] Mouse button DOWN/UP` al hacer click
   - ? Clicks funcionan en UI de ImGui

**Expectativa**:
- Si el mismatch de `hInstance` era el problema ? ventana se crear� correctamente
- Si persiste error 1400 ? investigar permisos o conflictos de sistema operativo

---

(Sin otros bugs pendientes)
