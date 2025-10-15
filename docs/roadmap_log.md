# Roadmap Log

Último punto realizado: Inicialización del repositorio y creación de la documentación base.
Punto actual: Estructura inicial del proyecto y archivos skeleton.

Historial:
- [1] Documentación inicial creada (roadmap, commits, copilot-instructions) - estado: completado

Próximo paso propuesto:
- Crear los archivos skeleton para los módulos (`core`, `renderer`, `ui`, `assets`, `jobs`).
- Compilar la solución y corregir cualquier error/warning antes de hacer commit.

Registro de la iteración reciente:
- Se añadieron los skeletons de los módulos `core`, `renderer`, `ui`, `assets`, `jobs`.
- Compilación: build Debug x64 correcta (0 errores, 0 warnings).
- Commit: realizado por el propietario y sincronizado con el repositorio remoto.

Iteración actual completa:
- Implementado módulo `core` básico (`core/Assert.h`, `core/Log.h` con niveles y macros).
- Se usó logging en `main.cpp`.
- Compilación: build Debug x64 correcta (0 errores, 0 warnings).
- Commit: pendiente de realizar por el asistente (si confirma el propietario).  

Historial de iteraciones adicionales (reciente):
- Añadido soporte CMake y archivos de build (CMakeLists.txt). Compilación con CMake: OK.
- Implementado `platform/Input` (keyboard/mouse polling) e integrado en el loop principal.
- Añido stub `renderer/DX12Renderer` y se integró en el ciclo de inicialización/render/shutdown.
- Añadido `ui/SimpleUI` (stub) e integrado en el loop principal.
- Añadido `renderer/CommandBuffer` (ring-buffer stub) para preparación de comandos GPU.

- Implementado UI panel y render de texto con GDI/AlphaBlend y `Win32TextRenderer` ("Hola Mundo" en panel transparente).

Estado de compilación: build Debug x64 correcta (0 errores, 0 warnings) tras las iteraciones anteriores.

Próximo paso propuesto:
- Preparar la integración del backend DX12 para composición UI y render targets (crear stubs y estructura para presentar la UI vía renderer).

Iteración completada:
- Preparados stubs DX12: `DX12Device`, `RenderTarget` y ampliado `DX12Renderer` con `ComposeUI()` para soportar composición UI (stub). Compilación: OK.

Iteraciones recientes completadas (assets):
- 6.05 Instrumentación y métricas: contadores y muestreo añadidos en `AssetManager` con `DumpMetrics()` (commit `3f24c69`).
- 6.07 Persistencia de métricas: `SaveMetrics()` guarda métricas en `metrics_asset_manager.json` en shutdown.

- 7.01 TaskGraph: prototipo de TaskGraph añadido para ejecución de tareas con dependencias sobre `ThreadPool` (commit `755a2a4`).
-
- Estilo: aplicada política de orden de `#include` en los fuentes del proyecto (reordenado alfabéticamente y separando includes de proyecto/sistema). Compromiso: `style(include): reorder includes across src per project policy` (commit `336c993`).
- 7.02 Bench: micro-bench `threadpool_bench` added to test ThreadPool throughput across multiple thread counts.
