# Roadmap Detail (extensión de `roadmap.md`)

Este documento enlaza los hitos principales del `docs/roadmap.md` con historias de menor granularidad (p. ej. 6.00, 6.01, ...), su estado y referencias a los commits relevantes.

Índice y detalle de fases

Este fichero extiende `docs/roadmap.md` con historias de menor granularidad (X.YZ) que permiten seguir el avance por tareas concretas.

Enlace principal: ver también `docs/roadmap.md`.

1 Estructura y documentación inicial (detalle)
------------------------------------------------

1.00 Documentación inicial y estructura del repo
- Descripción: Crear `docs/roadmap.md`, `docs/roadmap_log.md`, `docs/commits.md` y las instrucciones de trabajo. Establecer normas de commits.
- Estado: completado
- Commits: iniciales (historial en `docs/commits.md`)

1.01 Creación de skeletons de módulos
- Descripción: Añadir carpetas/skeletons para `core`, `platform`, `renderer`, `ui`, `assets`, `jobs`.
- Estado: completado

2 Sistema de build y proyecto (detalle)
------------------------------------------------

2.00 CMake y solución multiplataforma
- Descripción: Añadir `CMakeLists.txt`, configurar C++ standard y salida binaria.
- Estado: completado (migración a C++23 en commit `52dbaa8`).

2.01 Integración con Visual Studio
- Descripción: Generar y usar la solución en `build/` para evitar desincronizaciones con `.vcxproj`.
- Estado: completado (workflow documentado en `.github/copilot-instructions.md`).

3 Sistema de plataforma/abstracción (Windowing + input)
------------------------------------------------

3.00 Win32 Window wrapper
- Descripción: Implementar `platform/Window` que cree la ventana y gestione mensajes.
- Estado: completado (implementación en `src/platform/Window.*`).

3.01 Input polling
- Descripción: Implementar `platform/Input` para teclado y ratón (polling + eventos).
- Estado: completado (commit `24d0709` - `InputManager`).

4 Backend de render inicial - DirectX12 minimal
------------------------------------------------

4.00 DX12 renderer stub
- Descripción: Crear `renderer/DX12Renderer` con `Initialize/Shutdown/RenderFrame`.
- Estado: completado (stubs, commit `377ced4`).

4.01 DX12 device/RTV stubs
- Descripción: Añadir `DX12Device` y `RenderTarget` stubs y estructuras para composición UI.
- Estado: completado (commit `377ced4`, `f6e13bf`).

5 Command buffers y sincronización
------------------------------------------------

5.00 CommandBuffer ring-buffer
- Descripción: Implementar `CommandBuffer` ring-buffer stub para preparación de comandos GPU.
- Estado: completado (commit `0167a93`).

5.01 Fence y CommandAllocator stubs
- Descripción: Implementar `Fence`, `CommandAllocator` y flujo de submit/espera (stubs).
- Estado: completado (commit `ffcfa80`).

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
- Commits relevantes: `24d0709`, `3d168dc`

6.03 Tests de streaming: prioridad y cancelación
- Descripción: Añadir pruebas de integración en `main` que encolan múltiples cargas con diferentes prioridades, cancelan una de ellas y verifican la llegada/consumo por el renderer.
- Estado: completado (prueba manual en main)
- Commit: `3b8f28b` - test(assets): streaming priority and cancellation test in main

6.04 Preempción cooperativa
- Descripción: Prototipo de preempción cooperativa: las tareas en ejecución exponen un flag de cancelación; cuando llega una tarea de mayor prioridad, se marcan flags para que las tareas de menor prioridad se cancelen cooperativamente durante el streaming.
- Estado: completado (prototipo)
- Commit: `0fe3cf1` - feat(assets): cooperative preemption for running low-priority loads when high-priority tasks arrive

6.05 Instrumentación y métricas
- Descripción: Añadir contadores y métricas para medir tiempos de carga, número de cancelaciones, colas por prioridad y latencias; exponer logs estructurados o puntos de muestreo para el profiler.
- Estado: completado (commit `3f24c69`)

6.07 Persistencia de métricas
- Descripción: Guardar métricas en disco en formato JSON para análisis posteriores.
- Estado: completado (implementado `SaveMetrics` y escritura `metrics_asset_manager.json` en shutdown)

7 Multithreading y job system (detalle)
------------------------------------------------

7.00 ThreadPool
- Descripción: Implementar `jobs/ThreadPool` para ejecutar tareas asíncronas.
- Estado: completado (commit `9b4e8c5`).

7.01 Task graph (próximo)
- Descripción: Diseñar y soportar dependencias entre tareas (future work).
7.01 Task graph
- Descripción: Implementar TaskGraph para expresar dependencias entre tareas y ejecutarlas sobre ThreadPool.
- Estado: completado (prototipo) - `src/jobs/TaskGraph.*` (commit `755a2a4`).

7.02 ThreadPool/TaskGraph benchmarks
- Descripción: Micro-bench para medir throughput de `ThreadPool` y ejecución de grafos en `TaskGraph`. Test parametrizable sobre número de hilos.
- Estado: completado (bench executable `threadpool_bench` en `build/Debug`) 

8 Simple UI layer + text rendering
------------------------------------------------

8.00 Simple UI panel + GDI text
- Descripción: Panel semi-transparente pintado con GDI y texto "Hola Mundo" como prueba de concepto.
- Estado: completado (commit `44d4ceb`).

8.01 DirectWrite/Direct2D text renderer
- Descripción: Añadir `DirectWriteTextRenderer` como opción para mejor calidad y futuro Direct2D integration.
- Estado: implementado (prueba) `DirectWriteTextRenderer.cpp`.

9 Shading/material system y pipeline state
------------------------------------------------

9.00 Diseño de sistema de materiales (pendiente)
- Descripción: Definir estructuras de materiales, binding de resources y pipeline states.
- Estado: pendiente

10 Tools: profiler, debug overlays, capture
------------------------------------------------

10.00 Profiler y overlays (pendiente)
- Descripción: Integrar herramientas para medir tiempos de CPU/GPU y mostrar overlays en pantalla.
- Estado: pendiente

11 Adapter layers para DX11/Vulkan/OpenGL
------------------------------------------------

11.00 Abstracción de backend y adaptadores (pendiente)
- Descripción: Diseñar la capa de abstracción del renderer para permitir backends alternativos.
- Estado: pendiente

12 Polishing, examples y tests
------------------------------------------------

12.00 Ejemplos, samples y tests (pendiente)
- Descripción: Crear escenas de ejemplo y tests automáticos para garantizar estabilidad.
- Estado: pendiente

Cómo usar este fichero
- Mantener las entradas 1 por línea con estado y commit cuando se complete.
- Añadir sub-historias nuevas bajo la fase correspondiente (por ejemplo 6.06, 6.07...).

```