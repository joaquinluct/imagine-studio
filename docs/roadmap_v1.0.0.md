# Roadmap v1.0.0 - Instantánea del trabajo completado

Este documento registra el estado del roadmap y los ítems completados en la release v1.0.0.

Resumen de fases y trabajo notable completado en v1.0.0:

- Estructura del proyecto, documentación y flujo de trabajo (`docs/roadmap.md`, `docs/commits.md`, `.github/copilot-instructions.md`).
- Sistema de build e integración con el IDE: `CMakeLists.txt`, generación de la solución e integración con Visual Studio.
- Plataforma: wrapper Win32 `platform/Window` y `platform/Input` implementados y endurecidos.
- Renderer: stubs de DX12 añadidos (`DX12Renderer`, `DX12Device`, `RenderTarget`, `Fence`), además de mejoras progresivas (swapchain, camino de Present, sincronización de fence en CPU).
- Buffers de comandos y sincronización (stubs): `CommandBuffer`, `CommandAllocator`, `Fence` y prototipos de ring-buffer.
- Sistema de assets: VFS, cargas asíncronas priorizadas, cancelación, preempción cooperativa, instrumentación y persistencia de métricas (`AssetManager`, `VFS`, JSON de métricas).
- Multihilo y jobs: `ThreadPool`, prototipo de `TaskGraph` y benchmarks (`threadpool_bench`).
- UI: overlay `SimpleUI`, render de texto con GDI y prototipo con DirectWrite para mayor calidad.
- Herramientas: `Profiler` mínimo integrado con overlay UI (tiempos por frame / FPS).
- `AdapterFactory` y stubs de tests para selección de backend.
- Ejemplos y tests: example runner y escena mínima, prototipo de sistema de materiales y tests.

Todo el código incluido en esta instantánea compiló correctamente en el momento del release (verificado con CMake Debug y Build Solution de Visual Studio).

Para el historial completo de commits y detalles, ver `docs/commits.md` y `docs/roadmap_log.md`.

