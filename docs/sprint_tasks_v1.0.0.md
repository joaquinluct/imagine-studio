# Roadmap Tasks - v1.0.0 (completed tasks)

This document lists main tasks completed in release v1.0.0 (high level summary). For full commit references see `docs/commits.md` and `docs/roadmap_log.md`.

- Create project skeletons for modules: `core`, `platform`, `renderer`, `ui`, `assets`, `jobs`.
- Add CMake and Visual Studio solution integration; ensure Debug builds pass.
- Implement `platform/Window` (Win32 wrapper) and `platform/Input` polling.
- Implement `DX12Renderer` stub and basic `DX12Device`, `RenderTarget`, `Fence` stubs.
- Add `CommandBuffer` ring-buffer, `CommandAllocator` and simple `Fence` stub.
- Implement `VFS`, `AssetManager` with prioritized async loads, cancelation and cooperative preemption.
- Add instrumentation and `SaveMetrics()` to persist metrics to `metrics_asset_manager.json`.
- Implement `ThreadPool`, `TaskGraph` prototype and micro-bench `threadpool_bench`.
- Implement `SimpleUI` and text rendering (GDI + DirectWrite prototype).
- Add `Profiler` minimal and integrate with UI overlays.

