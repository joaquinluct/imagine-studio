# Roadmap Log

�ltimo punto realizado: Inicializaci�n del repositorio y creaci�n de la documentaci�n base.
Punto actual: Estructura inicial del proyecto y archivos skeleton.

Historial:
- [1] Documentaci�n inicial creada (roadmap, commits, copilot-instructions) - estado: completado

Pr�ximo paso propuesto:
- Crear los archivos skeleton para los m�dulos (`core`, `renderer`, `ui`, `assets`, `jobs`).
- Compilar la soluci�n y corregir cualquier error/warning antes de hacer commit.

Registro de la iteraci�n reciente:
- Se a�adieron los skeletons de los m�dulos `core`, `renderer`, `ui`, `assets`, `jobs`.
- Compilaci�n: build Debug x64 correcta (0 errores, 0 warnings).
- Commit: realizado por el propietario y sincronizado con el repositorio remoto.

Iteraci�n actual completa:
- Implementado m�dulo `core` b�sico (`core/Assert.h`, `core/Log.h` con niveles y macros).
- Se us� logging en `main.cpp`.
- Compilaci�n: build Debug x64 correcta (0 errores, 0 warnings).
- Commit: pendiente de realizar por el asistente (si confirma el propietario).  

Historial de iteraciones adicionales (reciente):
- A�adido soporte CMake y archivos de build (CMakeLists.txt). Compilaci�n con CMake: OK.
- Implementado `platform/Input` (keyboard/mouse polling) e integrado en el loop principal.
- A�ido stub `renderer/DX12Renderer` y se integr� en el ciclo de inicializaci�n/render/shutdown.
- A�adido `ui/SimpleUI` (stub) e integrado en el loop principal.
- A�adido `renderer/CommandBuffer` (ring-buffer stub) para preparaci�n de comandos GPU.

- Implementado UI panel y render de texto con GDI/AlphaBlend y `Win32TextRenderer` ("Hola Mundo" en panel transparente).

Estado de compilaci�n: build Debug x64 correcta (0 errores, 0 warnings) tras las iteraciones anteriores.

Pr�ximo paso propuesto:
- Preparar la integraci�n del backend DX12 para composici�n UI y render targets (crear stubs y estructura para presentar la UI v�a renderer).

Iteraci�n completada:
- Preparados stubs DX12: `DX12Device`, `RenderTarget` y ampliado `DX12Renderer` con `ComposeUI()` para soportar composici�n UI (stub). Compilaci�n: OK.

Iteraciones recientes completadas (assets):
- 6.05 Instrumentaci�n y m�tricas: contadores y muestreo a�adidos en `AssetManager` con `DumpMetrics()` (commit `3f24c69`).
- 6.07 Persistencia de m�tricas: `SaveMetrics()` guarda m�tricas en `metrics_asset_manager.json` en shutdown.

- 7.01 TaskGraph: prototipo de TaskGraph a�adido para ejecuci�n de tareas con dependencias sobre `ThreadPool` (commit `755a2a4`).
-
- Estilo: aplicada pol�tica de orden de `#include` en los fuentes del proyecto (reordenado alfab�ticamente y separando includes de proyecto/sistema). Compromiso: `style(include): reorder includes across src per project policy` (commit `336c993`).
- 7.02 Bench: micro-bench `threadpool_bench` added to test ThreadPool throughput across multiple thread counts.

- 9.00 Material system: prototipo `Material` y `MaterialSystem` a�adido con test `material_test` (archivos `src/renderer/Material.*`, `src/tests/material_test.cpp`).

- 10.00 Profiler & overlays: a�adido `Profiler` m�nimo y overlay en `SimpleUI` para mostrar FPS (archivos `src/tools/Profiler.*`, integraci�n en `src/ui/SimpleUI.*` y `src/main.cpp`).

- 11.00 Adapter factory: a�adida factor�a `AdapterFactory` con stub adapter para tests y wiring para DX12 adapter (archivos `src/renderer/AdapterFactory.*`, `src/tests/adapter_test.cpp`).

- 12.00 Polishing & examples: iniciado trabajo en estructura de ejemplos y tests autom�ticos; pr�ximos pasos: a�adir `examples/` y tests de integraci�n.

- 12.00 Examples & tests: a�adido `examples/scene1` and `src/examples/ExampleRunner.cpp`. Example runner reads the sample scene and prints it.

Registro de la acci�n reciente:
- Se cambi� la entrada de la aplicaci�n a GUI-only: la aplicaci�n arranca mediante `WinMain` y el ejecutable usa el subsistema Windows, ocultando la consola en ejecuci�n normal.
- Se forz� el inicio de la ventana principal en modo maximizado (`SW_SHOWMAXIMIZED`).
- Se fij� la versi�n del proyecto en `1.0.0` en `CMakeLists.txt` y se a�adi� opci�n CMake `IMAGINE_START_MAXIMIZED`.
- Compilaci�n tras el cambio: CMake build Debug OK, msbuild Debug OK (0 errores, 0 warnings).

Hecho: 3. Sistema de plataforma/abstracci�n (Windowing + input)
Siguiente: 4. Backend de render inicial - DirectX12 minimal
