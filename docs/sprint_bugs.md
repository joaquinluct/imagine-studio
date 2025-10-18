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

**S�ntomas confirmados**:
- ? Hover de rat�n S� funciona (cambio de color enter/exit)
- ? F1 toggle S� funciona
- ? Clics de rat�n NO tienen efecto

**Archivos afectados**: `src/platform/Window.cpp`, `src/main.cpp`, `CMakeLists.txt`

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

**Pendiente**: Validaci�n del usuario ejecutando la aplicaci�n para confirmar que los clics de rat�n ahora funcionan correctamente.

---

(Sin bugs pendientes - BUG-002 resuelto y movido a sprint_fix.md)
