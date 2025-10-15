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
