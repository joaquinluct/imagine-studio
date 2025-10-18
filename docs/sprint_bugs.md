# Sprint Bugs (v1.3.0)

Bugs reportados durante el sprint activo pendientes de resoluci�n.

---

### BUG-001 - Clics de rat�n sin efecto en UI de ImGui

**ID**: BUG-001
**Prioridad**: Alta
**Estado**: Pendiente validaci�n usuario
**Fecha de entrada**: 2025-01-18

**Descripci�n**: 

Los clics de rat�n no ten�an efecto en la UI de ImGui:
- No cambia de men� al hacer clic
- No abre/cierra desplegables
- No responde a interacci�n con controles

La tecla F1 (toggle UI) s� funcionaba correctamente, indicando que el problema era espec�fico de eventos de rat�n.

**Archivos afectados**: `src/platform/Window.cpp`, `CMakeLists.txt`

**Fix implementado** (commit 80b7b7e):
- Eliminada verificaci�n del retorno de `ImGui_ImplWin32_WndProcHandler()`
- Handler ahora se llama SIEMPRE sin bloquear procesamiento de mensajes
- Compilaci�n limpia: CMake + MSBuild (0 errores, 0 warnings)

**Pendiente**: Validaci�n del usuario ejecutando la aplicaci�n para confirmar que los clics de rat�n ahora funcionan correctamente.

---

(Sin bugs pendientes - BUG-002 resuelto y movido a sprint_fix.md)
