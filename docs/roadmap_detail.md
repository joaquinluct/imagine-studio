# Roadmap Detail (extensión de `roadmap.md`)

Este documento enlaza los hitos principales del `docs/roadmap.md` con historias de menor granularidad (p. ej. 6.00, 6.01, ...), su estado y referencias a los commits relevantes.

6 Resource management y asset streaming (detalle)
------------------------------------------------

6.00 VFS y pool básico
- Descripción: Implementar un VFS mínimo para montar rutas y lectura básica de ficheros; añadir un pool simple de buffers donde almacenar datos cargados asíncronamente.
- Estado: completado
- Commit: `6b1458a` - feat(assets): add VFS stub and simple buffer pool for async loads

6.01 Cargas asíncronas priorizadas y cancelación
- Descripción: Dispatcher que acepta tareas con prioridad (Low, Normal, High), encola en un ThreadPool y devuelve un handle para posible cancelación.
- Estado: completado
- Commit: `ce3f633` - feat(assets): prioritized async loads with dispatcher and cancelation support

6.02 Integración streaming <-> renderer
- Descripción: Cola de assets cargados (loadedQueue) consumida desde el loop principal; notificar al renderer con `OnAssetLoaded(path)` para que procese recursos al llegar.
- Estado: completado
- Commits relevantes: `24d0709` (asset manager + async load integration), `3d168dc` (signal fence on async load)

6.03 Tests de streaming: prioridad y cancelación
- Descripción: Añadir pruebas de integración en `main` que encolan múltiples cargas con diferentes prioridades, cancelan una de ellas y verifican la llegada/consumo por el renderer.
- Estado: completado (prueba manual en main)
- Commit: `3b8f28b` - test(assets): streaming priority and cancellation test in main

6.04 Preempción cooperativa
- Descripción: Prototipo de preempción cooperativa: las tareas en ejecución exponen un flag de cancelación; cuando llega una tarea de mayor prioridad, se marcan flags para que las tareas de menor prioridad se cancelen cooperativamente durante el streaming.
- Estado: completado (prototipo)
- Commit: `0fe3cf1` - feat(assets): cooperative preemption for running low-priority loads when high-priority tasks arrive

6.05 (próximo) Instrumentación y métricas
- Descripción: Añadir contadores y métricas para medir tiempos de carga, número de cancelaciones, colas por prioridad y latencias; exponer logs estructurados o puntos de muestreo para el profiler.
- Estado: pendiente

Notas sobre numeración y rastreabilidad
- Las entradas X.YZ son una convención interna usada en la bitácora para romper las fases principales en tareas manejables. Este fichero es la fuente de referencia para las historias detalladas de cada fase.
- Cuando se complete una historia, el asistente registrará en `docs/roadmap_log.md` la actualización y añadirá la referencia al commit en `docs/commits.md` (regla aplicada automáticamente por el asistente durante las iteraciones).

Otras fases relevantes (resumen rápido)
- 2 Sistema de build: CMake y soporte multi-toolchain (commit `52dbaa8` - actualización a C++23)
- 3 Platform: `platform/Window` y `platform/Input` (commits previos)
- 4-5 Render & Sync: stubs DX12, CommandBuffer, Fence, CommandAllocator (commits `377ced4`, `ffcfa80` y siguientes)

Cómo usar este fichero
- Mantener las entradas 1 por línea con estado y commit cuando se complete.
- Añadir sub-historias nuevas bajo la fase correspondiente (por ejemplo 6.06, 6.07...).
