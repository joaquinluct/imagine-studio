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

Estado de compilación: build Debug x64 correcta (0 errores, 0 warnings) tras las iteraciones anteriores.

Próximo paso propuesto:
- Implementar el sistema de jobs/thread pool en `src/jobs` (crear `ThreadPool.cpp` y completar `ThreadPool.h`) para soportar tareas asíncronas y pruebas de integración con el renderer.
