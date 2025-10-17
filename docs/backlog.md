# Backlog (ba�l)

> **Referencia**: Ver [`docs/MAIN.md`](MAIN.md) para los pilares fundamentales del proyecto.

Este fichero contendr�, de forma persistente, todos los �tems que NO est�n en el sprint activo y que quedan para futuras iteraciones.

Por ahora est� vac�o y se usar� como dep�sito para historias/tareas descartadas temporalmente o pendientes de priorizaci�n.

---

## Formato

Lista de elementos con:
- **ID**: Identificador �nico
- **Prioridad**: Baja/Media/Alta
- **Descripci�n**: Breve descripci�n del �tem
- **Referencia**: Enlace a historia/tarea en `docs/sprint_histories.md` o `docs/sprint_tasks.md` si procede

---

## Ítems en Backlog

### **BACK-001 - Deferred Rendering (G-Buffer + Lighting Pass)**
**ID**: BACK-001  
**Prioridad**: Media  
**Descripción**: Implementar Deferred Rendering pipeline como alternativa a Forward Rendering. Incluye G-Buffer Pass (multiple render targets para albedo, normal, depth) y Lighting Pass (combina G-Buffer con cálculos de iluminación). Permite renderizar escenas con muchas luces (100+) de forma eficiente.

**Beneficios**:
- Optimización para escenas complejas con muchas luces
- Base para efectos avanzados (SSAO, SSR, IBL)
- Usado en Unreal Engine, CRYENGINE, y otros motores AAA

**Prerrequisitos**:
- Forward Rendering funcional (Sprint v1.2.0)
- Multiple Render Targets (MRT) support
- Shaders de G-Buffer y Lighting

**Archivos afectados estimados**:
- `src/renderer/DX12Renderer.h` (añadir métodos RenderDeferred, GBufferPass, LightingPass)
- `src/renderer/DX12Renderer.cpp` (implementación de passes)
- `shaders/gbuffer.hlsl` (nuevo - vertex/pixel shaders para G-Buffer)
- `shaders/lighting.hlsl` (nuevo - compute/pixel shader para lighting)

**Referencia**: Esta feature se puede implementar después de Sprint v1.2.0 como evolución natural del renderer. Ver docs/TEMPLATE.md para comparación Forward vs Deferred.

---

*Nota*: El backlog se actualizará conforme se identifiquen ítems diferidos durante los sprints.

