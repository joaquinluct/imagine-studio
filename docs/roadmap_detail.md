# Roadmap Detail (extensi�n de `roadmap.md`)

Este documento enlaza los hitos principales del `docs/roadmap.md` con historias de menor granularidad (p. ej. 6.00, 6.01, ...), su estado y referencias a los commits relevantes.

6 Resource management y asset streaming (detalle)
------------------------------------------------

6.00 VFS y pool b�sico
- Descripci�n: Implementar un VFS m�nimo para montar rutas y lectura b�sica de ficheros; a�adir un pool simple de buffers donde almacenar datos cargados as�ncronamente.
- Estado: completado
- Commit: `6b1458a` - feat(assets): add VFS stub and simple buffer pool for async loads

6.01 Cargas as�ncronas priorizadas y cancelaci�n
- Descripci�n: Dispatcher que acepta tareas con prioridad (Low, Normal, High), encola en un ThreadPool y devuelve un handle para posible cancelaci�n.
- Estado: completado
- Commit: `ce3f633` - feat(assets): prioritized async loads with dispatcher and cancelation support

6.02 Integraci�n streaming <-> renderer
- Descripci�n: Cola de assets cargados (loadedQueue) consumida desde el loop principal; notificar al renderer con `OnAssetLoaded(path)` para que procese recursos al llegar.
- Estado: completado
- Commits relevantes: `24d0709` (asset manager + async load integration), `3d168dc` (signal fence on async load)

6.03 Tests de streaming: prioridad y cancelaci�n
- Descripci�n: A�adir pruebas de integraci�n en `main` que encolan m�ltiples cargas con diferentes prioridades, cancelan una de ellas y verifican la llegada/consumo por el renderer.
- Estado: completado (prueba manual en main)
- Commit: `3b8f28b` - test(assets): streaming priority and cancellation test in main

6.04 Preempci�n cooperativa
- Descripci�n: Prototipo de preempci�n cooperativa: las tareas en ejecuci�n exponen un flag de cancelaci�n; cuando llega una tarea de mayor prioridad, se marcan flags para que las tareas de menor prioridad se cancelen cooperativamente durante el streaming.
- Estado: completado (prototipo)
- Commit: `0fe3cf1` - feat(assets): cooperative preemption for running low-priority loads when high-priority tasks arrive

6.05 (pr�ximo) Instrumentaci�n y m�tricas
- Descripci�n: A�adir contadores y m�tricas para medir tiempos de carga, n�mero de cancelaciones, colas por prioridad y latencias; exponer logs estructurados o puntos de muestreo para el profiler.
- Estado: pendiente

Notas sobre numeraci�n y rastreabilidad
- Las entradas X.YZ son una convenci�n interna usada en la bit�cora para romper las fases principales en tareas manejables. Este fichero es la fuente de referencia para las historias detalladas de cada fase.
- Cuando se complete una historia, el asistente registrar� en `docs/roadmap_log.md` la actualizaci�n y a�adir� la referencia al commit en `docs/commits.md` (regla aplicada autom�ticamente por el asistente durante las iteraciones).

Otras fases relevantes (resumen r�pido)
- 2 Sistema de build: CMake y soporte multi-toolchain (commit `52dbaa8` - actualizaci�n a C++23)
- 3 Platform: `platform/Window` y `platform/Input` (commits previos)
- 4-5 Render & Sync: stubs DX12, CommandBuffer, Fence, CommandAllocator (commits `377ced4`, `ffcfa80` y siguientes)

C�mo usar este fichero
- Mantener las entradas 1 por l�nea con estado y commit cuando se complete.
- A�adir sub-historias nuevas bajo la fase correspondiente (por ejemplo 6.06, 6.07...).
