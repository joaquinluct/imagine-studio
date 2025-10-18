# Sprint Bugs (v1.3.0)

Bugs reportados durante el sprint activo pendientes de resolución.

---

### BUG-001 - Clics de ratón sin efecto en UI de ImGui

**ID**: BUG-001
**Prioridad**: Alta
**Estado**: Pendiente validación usuario
**Fecha de entrada**: 2025-01-18

**Descripción**: 

Los clics de ratón no tenían efecto en la UI de ImGui:
- No cambia de menú al hacer clic
- No abre/cierra desplegables
- No responde a interacción con controles

La tecla F1 (toggle UI) sí funcionaba correctamente, indicando que el problema era específico de eventos de ratón.

**Archivos afectados**: `src/platform/Window.cpp`, `CMakeLists.txt`

**Fix implementado** (commit 80b7b7e):
- Eliminada verificación del retorno de `ImGui_ImplWin32_WndProcHandler()`
- Handler ahora se llama SIEMPRE sin bloquear procesamiento de mensajes
- Compilación limpia: CMake + MSBuild (0 errores, 0 warnings)

**Pendiente**: Validación del usuario ejecutando la aplicación para confirmar que los clics de ratón ahora funcionan correctamente.

---

(Sin bugs pendientes - BUG-002 resuelto y movido a sprint_fix.md)
