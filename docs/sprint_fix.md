# Sprint v1.1.0 - Bugs Resueltos

> **Referencia**: Ver [`docs/MAIN.md`](MAIN.md) para los pilares fundamentales del proyecto.

Este fichero contiene el **historial de bugs resueltos** durante el sprint activo.

**Prop�sito**: Mantener trazabilidad de bugs que fueron reportados en `docs/sprint_bugs.md` y posteriormente resueltos.

---

## ✅ Bugs Resueltos

### FIX-001 - Quad no visible en pantalla (solo fondo azul)
**ID Original**: BUG-001
**Prioridad**: Crítica
**Fecha de entrada**: 2025-01-17
**Fecha de resolución**: 2025-01-17

**Descripción del problema**: Después de completar el Sprint v1.1.0 (DX12 Minimal Renderer), la aplicación renderizaba correctamente el fondo azul y liberaba recursos al pulsar ESC, pero **no se veía el quad coloreado** que debería renderizarse como "Hola Mundo" visual. Todos los logs indicaban que el renderizado se ejecutaba correctamente (sin errores), pero no había rastro del quad en pantalla.

**Comportamiento observado**:
- Solo fondo azul, sin quad visible
- Sin errores en logs
- Aplicación funcionaba correctamente (ventana, input, cierre limpio)

**Causa identificada**: **Backface culling con winding order incorrecto**. Los triángulos estaban definidos con winding order counter-clockwise (CCW), pero DirectX con `FrontCounterClockwise = FALSE` considera que los triángulos clockwise (CW) miran hacia adelante. Por lo tanto, los triángulos se descartaban por backface culling antes de renderizar.

**Solución implementada**:
1. Invertido el winding order de los vértices a clockwise (CW):
   - Triangle 1: bottom-left → top-left → bottom-right (clockwise)
   - Triangle 2: bottom-right → top-left → top-right (clockwise)
2. Restaurado backface culling (`D3D12_CULL_MODE_BACK`) para optimización (se había deshabilitado temporalmente para debugging)
3. Reducido logging excesivo en `RenderFrame()` (eliminados `CORE_LOG_INFO` del render loop, solo errores críticos)

**Resultado**: Quad ahora visible correctamente con colores interpolados (rojo, verde, azul, amarillo) y backface culling activo para optimización.

**Archivos afectados**: 
- `src/renderer/DX12Renderer.cpp` (winding order de vértices corregido, culling restaurado, logging reducido)

**Commit de resolución**: [PENDIENTE]


## 📊 Resumen

| ID | Título | Prioridad | Fecha Entrada | Fecha Resolución | Commit |
|----|--------|-----------|---------------|------------------|--------|
| FIX-001 | Quad no visible en pantalla (solo fondo azul) | Crítica | 2025-01-17 | 2025-01-17 | [PENDIENTE] |

**Total bugs resueltos**: 1

---

## ?? Formato de Entrada

Cada bug resuelto debe seguir este formato:

```markdown
### FIX-XXX - T�tulo del bug
**ID Original**: BUG-XXX (del sprint_bugs.md)
**Prioridad**: Cr�tica/Alta/Media/Baja
**Fecha de entrada**: YYYY-MM-DD
**Fecha de resoluci�n**: YYYY-MM-DD

**Descripci�n del problema**: Descripci�n detallada del error, pasos para reproducir, comportamiento esperado vs observado.

**Soluci�n implementada**: Descripci�n de c�mo se resolvi� el bug.

**Archivos afectados**: Lista de archivos relacionados con el bug y su resoluci�n

**Commit de resoluci�n**: Hash del commit que resolvi� el bug
```

---

## ?? Flujo de Trabajo

1. Bug reportado ? Se registra en `docs/sprint_bugs.md` con ID **BUG-XXX**
2. Bug en progreso ? Estado actualizado en `docs/sprint_bugs.md`
3. Bug resuelto ? Se **mueve autom�ticamente** aqu� con:
   - ID cambiado a **FIX-XXX**
   - Estado "Resuelto"
   - Fecha de resoluci�n
   - Hash del commit de resoluci�n
   - Descripci�n de la soluci�n implementada
4. Bug eliminado de `docs/sprint_bugs.md`

---

## ?? Notas

- Para ver bugs pendientes del sprint actual, consultar `docs/sprint_bugs.md`
- Para ver bugs de sprints completados, ver los archivos versionados:
  - `docs/sprint_bugs_v<version>.md` (bugs que quedaron pendientes al cerrar sprint)
  - `docs/sprint_fix_v<version>.md` (bugs resueltos durante el sprint)
- Los bugs se archivan junto con los dem�s ficheros del sprint al finalizar.

