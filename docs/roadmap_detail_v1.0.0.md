# Roadmap Detail (extensi�n de `roadmap.md`)

Este documento enlaza los hitos principales del `docs/roadmap.md` con historias de menor granularidad (p. ej. 6.00, 6.01, ...), su estado y referencias a los commits relevantes.

�ndice y detalle de fases

Este fichero extiende `docs/roadmap.md` con historias de menor granularidad (X.YZ) que permiten seguir el avance por tareas concretas.

Enlace principal: ver tambi�n `docs/roadmap.md`.

1 Estructura y documentaci�n inicial (detalle)
------------------------------------------------

1.00 Documentaci�n inicial y estructura del repo
- Descripci�n: Crear `docs/roadmap.md`, `docs/roadmap_log.md`, `docs/commits.md` y las instrucciones de trabajo. Establecer normas de commits.
- Estado: completado
- Commits: iniciales (historial en `docs/commits.md`)

1.01 Creaci�n de skeletons de m�dulos
- Descripci�n: A�adir carpetas/skeletons para `core`, `platform`, `renderer`, `ui`, `assets`, `jobs`.
- Estado: completado

2 Sistema de build y proyecto (detalle)
------------------------------------------------

2.00 CMake y soluci�n multiplataforma
- Descripci�n: A�adir `CMakeLists.txt`, configurar C++ standard y salida binaria.
- Estado: completado (migraci�n a C++23 en commit `52dbaa8`).

2.01 Integraci�n con Visual Studio
- Descripci�n: Generar y usar la soluci�n en `build/` para evitar desincronizaciones con `.vcxproj`.
- Estado: completado (workflow documentado en `.github/copilot-instructions.md`).

3 Sistema de plataforma/abstracci�n (Windowing + input)
------------------------------------------------

3.00 Win32 Window wrapper
- Descripci�n: Implementar `platform/Window` que cree la ventana y gestione mensajes.
- Estado: completado (implementaci�n en `src/platform/Window.*`).

3.01 Input polling
- Descripci�n: Implementar `platform/Input` para teclado y rat�n (polling + eventos).
- Estado: completado (commit `24d0709` - `InputManager`).

4 Backend de render inicial - DirectX12 minimal
------------------------------------------------

4.00 DX12 renderer stub
- Descripci�n: Crear `renderer/DX12Renderer` con `Initialize/Shutdown/RenderFrame`.
- Estado: completado (stubs, commit `377ced4`).

4.01 DX12 device/RTV stubs
- Descripci�n: A�adir `DX12Device` y `RenderTarget` stubs y estructuras para composici�n UI.
- Estado: completado (commit `377ced4`, `f6e13bf`).

5 Command buffers y sincronizaci�n
------------------------------------------------

5.00 CommandBuffer ring-buffer
- Descripci�n: Implementar `CommandBuffer` ring-buffer stub para preparaci�n de comandos GPU.
- Estado: completado (commit `0167a93`).

5.01 Fence y CommandAllocator stubs
- Descripci�n: Implementar `Fence`, `CommandAllocator` y flujo de submit/espera (stubs).
- Estado: completado (commit `ffcfa80`).

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
- Commits relevantes: `24d0709`, `3d168dc`

6.03 Tests de streaming: prioridad y cancelaci�n
- Descripci�n: A�adir pruebas de integraci�n en `main` que encolan m�ltiples cargas con diferentes prioridades, cancelan una de ellas y verifican la llegada/consumo por el renderer.
- Estado: completado (prueba manual en main)
- Commit: `3b8f28b` - test(assets): streaming priority and cancellation test in main

6.04 Preempci�n cooperativa
- Descripci�n: Prototipo de preempci�n cooperativa: las tareas en ejecuci�n exponen un flag de cancelaci�n; cuando llega una tarea de mayor prioridad, se marcan flags para que las tareas de menor prioridad se cancelen cooperativamente durante el streaming.
- Estado: completado (prototipo)
- Commit: `0fe3cf1` - feat(assets): cooperative preemption for running low-priority loads when high-priority tasks arrive

6.05 Instrumentaci�n y m�tricas
- Descripci�n: A�adir contadores y m�tricas para medir tiempos de carga, n�mero de cancelaciones, colas por prioridad y latencias; exponer logs estructurados o puntos de muestreo para el profiler.
- Estado: completado (commit `3f24c69`)

6.07 Persistencia de m�tricas
- Descripci�n: Guardar m�tricas en disco en formato JSON para an�lisis posteriores.
- Estado: completado (implementado `SaveMetrics` y escritura `metrics_asset_manager.json` en shutdown)

7 Multithreading y job system (detalle)
------------------------------------------------

7.00 ThreadPool
- Descripci�n: Implementar `jobs/ThreadPool` para ejecutar tareas as�ncronas.
- Estado: completado (commit `9b4e8c5`).

7.01 Task graph (pr�ximo)
- Descripci�n: Dise�ar y soportar dependencias entre tareas (future work).
7.01 Task graph
- Descripci�n: Implementar TaskGraph para expresar dependencias entre tareas y ejecutarlas sobre ThreadPool.
- Estado: completado (prototipo) - `src/jobs/TaskGraph.*` (commit `755a2a4`).

7.02 ThreadPool/TaskGraph benchmarks
- Descripci�n: Micro-bench para medir throughput de `ThreadPool` y ejecuci�n de grafos en `TaskGraph`. Test parametrizable sobre n�mero de hilos.
- Estado: completado (bench executable `threadpool_bench` en `build/Debug`) 

8 Simple UI layer + text rendering
------------------------------------------------

8.00 Simple UI panel + GDI text
- Descripci�n: Panel semi-transparente pintado con GDI y texto "Hola Mundo" como prueba de concepto.
- Estado: completado (commit `44d4ceb`).

8.01 DirectWrite/Direct2D text renderer
- Descripci�n: A�adir `DirectWriteTextRenderer` como opci�n para mejor calidad y futuro Direct2D integration.
- Estado: implementado (prueba) `DirectWriteTextRenderer.cpp`.

9 Shading/material system y pipeline state
------------------------------------------------

9.00 Dise�o de sistema de materiales (pendiente)
- Descripci�n: Definir estructuras de materiales, binding de resources y pipeline states.
- Estado: pendiente

10 Tools: profiler, debug overlays, capture
------------------------------------------------

10.00 Profiler y overlays (pendiente)
- Descripci�n: Integrar herramientas para medir tiempos de CPU/GPU y mostrar overlays en pantalla.
- Estado: pendiente

11 Adapter layers para DX11/Vulkan/OpenGL
------------------------------------------------

11.00 Abstracci�n de backend y adaptadores (pendiente)
- Descripci�n: Dise�ar la capa de abstracci�n del renderer para permitir backends alternativos.
- Estado: pendiente

12 Polishing, examples y tests
------------------------------------------------

12.00 Ejemplos, samples y tests (pendiente)
- Descripci�n: Crear escenas de ejemplo y tests autom�ticos para garantizar estabilidad.
- Estado: pendiente

C�mo usar este fichero
- Mantener las entradas 1 por l�nea con estado y commit cuando se complete.
- A�adir sub-historias nuevas bajo la fase correspondiente (por ejemplo 6.06, 6.07...).

```