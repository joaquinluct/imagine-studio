Imagine Studio - Renderer Framework (AAA style)

Descripción:
Esqueleto inicial para un framework renderer orientado a proyectos 3D AAA. Este repositorio contiene una estructura modular y ejemplos stub para iniciar el desarrollo.

- Documentación inicial y roadmap.
- Módulo `core` básico (logging, assert, types).
- Wrapper Win32 `platform/Window` y sistema de input `platform/Input`.
- Stub renderer `renderer/DX12Renderer` y `renderer/CommandBuffer` ring-buffer.
- Stub UI `ui/SimpleUI`.
- CMake support para builds multiplataforma.

Nota: este proyecto ahora usa C++23. Requisitos mínimos de compilador recomendados:
- MSVC: Visual Studio 2022 (MSVC toolset) con soporte C++23
- GCC: 11+ (recomendado 12+)
- Clang: 13+

Asegúrate de que tu entorno/CI suministre toolchains compatibles antes de cambiar la política de compatibilidad.
- Documentación inicial y roadmap.
- Módulo `core` básico (logging, assert, types).
- Wrapper Win32 `platform/Window` y sistema de input `platform/Input`.
- Stub renderer `renderer/DX12Renderer` y `renderer/CommandBuffer` ring-buffer.
- Stub UI `ui/SimpleUI`.
- CMake support para builds multiplataforma.

Siguientes pasos:
- Implementar `jobs/ThreadPool` (sistema de jobs) e integrar con el renderer y carga de assets.
