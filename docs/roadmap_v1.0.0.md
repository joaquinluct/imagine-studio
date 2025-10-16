# Roadmap v1.0.0 - Snapshot of completed work

This document records the roadmap state and completed items for release v1.0.0.

Summary of completed high-level phases and notable work included in v1.0.0:

- Project skeleton, documentation and workflow (`docs/roadmap.md`, `docs/commits.md`, `.github/copilot-instructions.md`).
- Build system and IDE integration: `CMakeLists.txt`, solution generation and Visual Studio integration.
- Platform: Win32 `platform/Window` wrapper and `platform/Input` polling implemented and hardened.
- Renderer: DX12 renderer stubs added (`DX12Renderer`, `DX12Device`, `RenderTarget`, `Fence`), plus progressive improvements (swapchain stub, Present path, CPU fence sync).
- Command buffers and synchronization stubs: `CommandBuffer`, `CommandAllocator`, `Fence` and ring-buffer prototypes.
- Asset system: VFS, prioritized async loads, cancellation, cooperative preemption, instrumentation and persistence of metrics (`AssetManager`, `VFS`, metrics JSON).
- Multithreading & jobs: `ThreadPool`, `TaskGraph` prototype and benchmarks (`threadpool_bench`).
- UI: `SimpleUI` overlay, GDI text rendering and DirectWrite prototype for higher-quality text.
- Tools: minimal `Profiler` integrated with UI overlay (frame-times / FPS).
- AdapterFactory and test stubs for backend selection.
- Examples and tests: example runner and minimal scene, material system prototype and tests.

All code included in this snapshot compiled cleanly at the time of the release (CMake Debug and Visual Studio Build Solution verified).

For full history of commits and details, see `docs/commits.md` and `docs/roadmap_log.md`.

