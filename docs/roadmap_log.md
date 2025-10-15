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

Estado de compilaci�n: build Debug x64 correcta (0 errores, 0 warnings) tras las iteraciones anteriores.

Pr�ximo paso propuesto:
- Implementar el sistema de jobs/thread pool en `src/jobs` (crear `ThreadPool.cpp` y completar `ThreadPool.h`) para soportar tareas as�ncronas y pruebas de integraci�n con el renderer.
