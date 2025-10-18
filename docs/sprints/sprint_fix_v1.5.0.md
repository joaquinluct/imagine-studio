# Sprint Fix - v1.5.0 (Active)

> **Referencia**: Ver [docs/sprint_bugs.md](sprint_bugs.md) para bugs actualmente pendientes.

Este archivo contiene el historial de bugs resueltos durante el sprint activo v1.5.0.

---

## Bugs Resueltos

### BUG-001 - Quad no visible con matriz Projection de Camera ✅

**ID**: BUG-001  
**Prioridad**: Crítica  
**Estado**: ✅ RESUELTO  
**Fecha de entrada**: 2025-01-18  
**Fecha de resolución**: 2025-01-18

**Descripción**: El quad desaparecía al usar la matriz Projection generada por la clase `Camera`. El quad se renderizaba correctamente con matriz identidad o matriz ortográfica simple, pero no con la perspectiva de la cámara.

**Causa raíz**: Implementación manual incorrecta de las matrices View y Projection en `Camera::UpdateViewMatrix()` y `Camera::UpdateProjectionMatrix()`. Las matrices generadas no coincidían con el formato esperado por el shader row-major.

**Solución aplicada**: Reemplazar implementación manual con funciones estándar de DirectXMath:
- `XMMatrixPerspectiveFovLH()` para matriz Projection
- `XMMatrixLookAtLH()` para matriz View

**Intentos de solución**: 6 (ver `sprint_bug_attempts.md` para detalles completos)
- Intento #1-5: Fallidos (ajustes manuales, transposiciones, cambios column-major/row-major)
- Intento #6: ✅ Exitoso (usar DirectXMath estándar)

**Archivos modificados**: 
- `src/renderer/Camera.cpp` (reemplazar UpdateViewMatrix y UpdateProjectionMatrix)
- `src/renderer/Camera.h` (actualizar comentarios)

**Validación**: ✅ Confirmado por usuario - El quad se renderiza correctamente con perspectiva 3D

---

**Total bugs resueltos**: 1
