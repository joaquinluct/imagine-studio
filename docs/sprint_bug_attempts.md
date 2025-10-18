# Sprint Bug Attempts - v1.5.0 (Active)

> **Referencia**: Ver [docs/sprint_bugs.md](sprint_bugs.md) para bugs reportados actualmente.

Este archivo contiene el registro detallado de TODOS los intentos de solución para cada bug reportado durante el sprint activo v1.5.0.

---

## Intentos de Solución

### BUG-001 - Quad no visible con matriz Projection de Camera

**Intento #1**
- **Descripción**: Cambiar shader de row-major a column-major (mul(mvp, vec) en lugar de mul(vec, mvp))
- **Resultado**: Fallido
- **Detalles**: El quad seguía sin aparecer. Cambiar el orden de multiplicación en el shader no resolvió el problema.
- **Fecha y hora**: 2025-01-18 (hora actual de sesión)
- **Archivos modificados**: `shaders/quad.hlsl`

**Intento #2**
- **Descripción**: Usar solo matriz Projection (sin View) con column-major
- **Resultado**: Fallido
- **Detalles**: El quad seguía sin aparecer incluso usando solo Projection sin transformación View.
- **Fecha y hora**: 2025-01-18
- **Archivos modificados**: `src/renderer/DX12Renderer.cpp`

**Intento #3**
- **Descripción**: Revertir shader a row-major y transponer matriz Projection antes de enviar
- **Resultado**: Fallido
- **Detalles**: El quad seguía sin aparecer. La transposición no resolvió el problema, indicando que el issue no es solo el layout de memoria.
- **Fecha y hora**: 2025-01-18
- **Archivos modificados**: `shaders/quad.hlsl`, `src/renderer/DX12Renderer.cpp`

**Intento #4**
- **Descripción**: Usar matriz ortográfica simple en lugar de Camera::GetProjectionMatrix()
- **Resultado**: Éxito ✅
- **Detalles**: El quad aparece correctamente con una matriz ortográfica simple hardcodeada. Esto confirma que el problema está en la implementación de `Camera::UpdateProjectionMatrix()`.
- **Fecha y hora**: 2025-01-18
- **Archivos modificados**: `src/renderer/DX12Renderer.cpp`

**Intento #5**
- **Descripción**: Reescribir Camera::UpdateViewMatrix() y UpdateProjectionMatrix() para row-major + actualizar MultiplyMatrix4x4
- **Resultado**: Fallido ❌
- **Detalles**: El quad volvió a desaparecer. Cambiar ambas matrices (View + Projection) simultáneamente a row-major no funcionó. Indica que el problema es más sutil o que la multiplicación de matrices también necesita ajustes.
- **Fecha y hora**: 2025-01-18
- **Archivos modificados**: `src/renderer/Camera.cpp`, `src/renderer/Camera.h`, `src/renderer/DX12Renderer.cpp`

**Intento #6**
- **Descripción**: Usar DirectXMath estándar (XMMatrixPerspectiveFovLH y XMMatrixLookAtLH) en lugar de implementación manual
- **Resultado**: Éxito ✅
- **Detalles**: El quad aparece correctamente con perspectiva 3D usando las funciones estándar de DirectXMath de Microsoft. Las matrices generadas son correctas y compatibles con el shader row-major.
- **Fecha y hora**: 2025-01-18
- **Archivos modificados**: `src/renderer/Camera.cpp`

**Conclusión final**: El problema era la implementación manual incorrecta de las matrices View y Projection. Usar las funciones estándar de DirectXMath (XMMatrixPerspectiveFovLH y XMMatrixLookAtLH) resuelve el problema completamente.

---

**Nota**: Este fichero se actualiza automáticamente conforme se trabajan los bugs en sprint_bugs.md.
