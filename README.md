Imagine Studio - Renderer Framework (AAA style)

Descripción:
Esqueleto inicial para un framework renderer orientado a proyectos 3D AAA. Este repositorio contiene una estructura modular y ejemplos stub para iniciar el desarrollo.

Estado actual:
- Documentación inicial y roadmap.
- Módulo `core` básico (logging, assert, types).
- Wrapper Win32 `platform/Window` y sistema de input `platform/Input`.
- Stub renderer `renderer/DX12Renderer` y `renderer/CommandBuffer` ring-buffer.
- Stub UI `ui/SimpleUI`.
- CMake support para builds multiplataforma.

Siguientes pasos:
- Implementar `jobs/ThreadPool` (sistema de jobs) e integrar con el renderer y carga de assets.
