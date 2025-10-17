Imagine Studio - Renderer Framework (AAA style)

Descripci�n:
Esqueleto inicial para un framework renderer orientado a proyectos 3D AAA. Este repositorio contiene una estructura modular y ejemplos stub para iniciar el desarrollo.

- Documentaci�n inicial y roadmap.
- M�dulo `core` b�sico (logging, assert, types).
- Wrapper Win32 `platform/Window` y sistema de input `platform/Input`.
- Stub renderer `renderer/DX12Renderer` y `renderer/CommandBuffer` ring-buffer.
- Stub UI `ui/SimpleUI`.
- CMake support para builds multiplataforma.

Nota: este proyecto ahora usa C++23. Requisitos m�nimos de compilador recomendados:
- MSVC: Visual Studio 2022 (MSVC toolset) con soporte C++23
- GCC: 11+ (recomendado 12+)
- Clang: 13+

Aseg�rate de que tu entorno/CI suministre toolchains compatibles antes de cambiar la pol�tica de compatibilidad.
- Documentaci�n inicial y roadmap.
- M�dulo `core` b�sico (logging, assert, types).
- Wrapper Win32 `platform/Window` y sistema de input `platform/Input`.
- Stub renderer `renderer/DX12Renderer` y `renderer/CommandBuffer` ring-buffer.
- Stub UI `ui/SimpleUI`.
- CMake support para builds multiplataforma.

Siguientes pasos:
- Implementar `jobs/ThreadPool` (sistema de jobs) e integrar con el renderer y carga de assets.
