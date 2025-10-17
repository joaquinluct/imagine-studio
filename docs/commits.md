# Convenciones de commits

Formato recomendado para los mensajes de commit:

```

- chore(app): switch to GUI-only entry (WinMain) and remove console main to hide console window during normal execution

<tipo>(<ámbito>): <mensaje corto>

<mensaje largo opcional>

Refs: <issue/track-id>
```

Tipos comunes:
- feat: Nueva funcionalidad
- fix: Corrección de errores
- docs: Cambios en documentación
- style: Formato, estilo, sin cambios funcionales
- refactor: Refactorización sin cambios en comportamiento
- perf: Mejoras de rendimiento
- test: Añadir o corregir tests
- chore: Tareas de mantenimiento

Ejemplo:
```
feat(renderer): inicializa estructura base del renderer

Añade clases básicas y estructura de carpetas para el renderer.
```

Historial de commits relevantes:

- feat(core): implementar módulo core (logging, asserts, types) - incluye `core/Log.h`, `core/Assert.h`, `core/Types.h`. (Iteración inicial: estructura y compilación limpia)

- chore(build): add CMake support and core module; ensure clean build
- feat(platform): add InputManager (keyboard/mouse polling) and integrate in main loop
- feat(ui): add SimpleUI stub and integrate in main loop
- feat(renderer): add CommandBuffer ring-buffer stub for GPU commands

- fix(assets): respect legacy callback semantics in AssetManager

Adjust `AssetManager` to provide a backwards-compatible overload that accepts
the old `void(const std::string&)` callback. The wrapper now forwards calls
only on successful loads and logs failures instead of invoking the legacy
callback, preventing clients from receiving false-positive load notifications.

Files changed: `src/assets/AssetManager.cpp`, `src/assets/AssetManager.h`

- feat(assets): VFS search mounts and add default assets mount

Implement a simple mount list in `VFS` and search mounted directories when
reading files. Add a default mount of `assets` and include a test asset
`assets/example_asset.dat` so the async loader can find example files.

Files changed: `src/assets/VFS.cpp`, `src/assets/VFS.h`, `assets/example_asset.dat`

- fix(platform): Window creation fallback, WM_PAINT test and fence wait safety in DX12Renderer

Describe: Improve robustness of `CreateWindowExW` in `Window.cpp` by adding fallback creation using a built-in class and detailed error logging. Add `WM_PAINT` simple rendering test string. Avoid waiting on a stubbed `Fence` in `DX12Renderer::RenderFrame()` to prevent deadlocks in the current stub GPU flow. Add test asset files `assets/asset_high.dat`, `assets/asset_normal.dat` and `metrics_asset_manager.json` created by AssetManager tests.

Files changed: `src/platform/Window.cpp`, `src/renderer/DX12Renderer.cpp`, `assets/asset_high.dat`, `assets/asset_normal.dat`, `metrics_asset_manager.json`

- docs: refactor instructions and tracking - add project pillars and daily.md

Added 4 fundamental pillars of Imagine Studio to copilot-instructions.md:
  1. Modo Edición/Studio (rich interface for 2D/3D world creation)
  2. Multiplataforma (publish to Windows, Mac, Consoles, etc)
  3. Extensibilidad (programming in editor to customize engine)
  4. Performance y Calidad AAA (AAA-level code, performance and visual quality PRIMORDIAL)

Replaced roadmap_log.md with simplified daily.md (only tracks last completed task and current task).
Updated all references in copilot-instructions.md from roadmap_log.md to daily.md.
Fixed C++ standard reference: uses C++14 (not C++23).
Added Daily.md section explaining the simplified tracking format.

Files changed: `.github/copilot-instructions.md`, `docs/daily.md` (created), `docs/roadmap_log.md` (deleted)

- docs: refactor documentation structure - create MAIN.md and rename roadmap to sprint

Created docs/MAIN.md with project fundamentals (4 pillars, AAA standards, architecture).
Updated copilot-instructions.md to reference MAIN.md as LEER PRIMERO (read first).
Renamed all roadmap* files to sprint* (better naming convention):
  - roadmap.md -> sprint.md
  - roadmap_histories.md -> sprint_histories.md
  - roadmap_tasks.md -> sprint_tasks.md
  - roadmap_v1.0.0.md -> sprint_v1.0.0.md
  - roadmap_histories_v1.0.0.md -> sprint_histories_v1.0.0.md
  - roadmap_tasks_v1.0.0.md -> sprint_tasks_v1.0.0.md

Cleared content of active sprint files (sprint.md, sprint_histories.md, sprint_tasks.md).
Updated all documentation references from roadmap to sprint.
Updated backlog.md with reference to MAIN.md.

Files changed: `.github/copilot-instructions.md`, `docs/MAIN.md` (created), `docs/backlog.md`, `docs/sprint*.md` (renamed and cleared)

- chore: add .gitattributes to normalize line endings (CRLF for Windows)

Created .gitattributes with explicit CRLF line endings for all text files.
Configured text=auto for automatic detection.
Set CRLF for C++, markdown, CMake, Visual Studio files.
Set LF for shell scripts (.sh).
Marked binary files (images, executables, libs) as binary.
Normalized existing files with git add --renormalize.
This prevents Visual Studio line ending dialogs from appearing during file edits.

Files changed: `.gitattributes` (created), `assets/*.dat` (normalized)

- docs: update copilot-instructions.md to use main VS solution for build verification

Updated Compilation 2 to use the main Visual Studio solution (`Imagine Studio.sln` in root) instead of the CMake-generated solution (`build/ImagineStudio.sln`).

Key changes:
- Changed `msbuild` command from `"build/ImagineStudio.sln"` to `"Imagine Studio.sln"` with `/p:Platform=x64`
- Added **NOTA section** explaining the **two build systems**:
  * **Main VS project** (`Imagine Studio.sln` in root) ? For daily development with F5/Ctrl+Shift+B
  * **CMake project** (`build/ImagineStudio.sln`) ? For CI/automation
- Added clear ? WARNING showing what **NOT to use** (`msbuild "build/ImagineStudio.sln"`)
- Added clear ? indicator showing what **TO use** (`msbuild "Imagine Studio.sln"` with Platform=x64)

This ensures compilation verification matches what developers see when pressing Ctrl+Shift+B in Visual Studio, preventing the issue where AI verification showed 0 errors but manual compilation in VS showed errors (missing include directories).

Files changed: `.github/copilot-instructions.md`

- feat(renderer): implement Command Queue creation (T1.2)

Implemented Task T1.2 - Create `ID3D12CommandQueue` following AAA standards.

Key changes:
- Created Command Queue immediately after Device creation in `Initialize()`
- Used `D3D12_COMMAND_LIST_TYPE_DIRECT` for graphics commands
- Added error handling with fallback to next adapter if queue creation fails
- Added logging for successful Command Queue creation using `CORE_LOG_INFO`
- Stored queue in `m_commandQueue` member variable (already declared in T1.1)
- Command Queue is released properly in `Shutdown()` (release order: Queue ? Device ? Adapter ? Factory)

Implementation details:
```cpp
D3D12_COMMAND_QUEUE_DESC queueDesc = {};
queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
```

Files changed: `src/renderer/DX12Device.cpp` (added Command Queue creation after Device creation)

Compilation: CMake Debug OK + MSBuild VS Debug OK (0 errors, 0 warnings)

**Hecho: T1.2 Crear Command Queue**  
**Siguiente: T1.3 Crear SwapChain con Back Buffers**

- docs: add visual progress bar to iteration completion format

Added visual sprint progress bar requirement to `copilot-instructions.md` for end-of-iteration reports.

Progress bar format:
- Horizontal bar with green squares (??) for completed tasks, black squares (?) for pending tasks
- White borders using box-drawing characters (???????)
- Percentage display at the end of the bar
- Bar width approximately 70-80 characters to fit prompt
- Percentage calculated as: (completed tasks / total sprint tasks) × 100

Example format for 2 of 15 tasks (13.3%):
```
??????????????????????????????????????????????????????????????????????
????????????????????????????????????????????????????? 13.3%?
??????????????????????????????????????????????????????????????????????
```

This provides instant visual feedback on sprint advancement at each iteration completion, making progress tracking more intuitive and engaging.

Files changed: `.github/copilot-instructions.md`

- feat(renderer): implement SwapChain and RTV creation (T1.3)

Implemented Task T1.3 - Create `IDXGISwapChain3` with back buffers and RTVs following AAA standards.

Key changes:
- Created **RTV descriptor heap** (`D3D12_DESCRIPTOR_HEAP_TYPE_RTV`) with 2 descriptors
- Created **SwapChain** with `CreateSwapChainForHwnd`:
  * Format: `DXGI_FORMAT_R8G8B8A8_UNORM`
  * Swap effect: `DXGI_SWAP_EFFECT_FLIP_DISCARD`
  * Buffer count: 2 (double buffering)
- Queried `IDXGISwapChain3` interface from `IDXGISwapChain1`
- Obtained back buffers with `GetBuffer()` for each buffer (index 0 and 1)
- Created **RTV** for each back buffer using `CreateRenderTargetView()`
- Stored frame index with `GetCurrentBackBufferIndex()`
- Added proper cleanup in `Shutdown()`:
  * Release render targets (in order)
  * Release RTV descriptor heap
  * Release SwapChain

Implementation follows DX12 best practices:
- Proper resource management with COM AddRef/Release
- Error handling with HRESULT checks and logging
- Correct release order during shutdown
- Resources obtained from DX12Device (Factory, Device, CommandQueue)

Files changed:
- `src/renderer/DX12Renderer.h`: Added SwapChain, render targets array, RTV heap, descriptor size, frame index members
- `src/renderer/DX12Renderer.cpp`: Implemented SwapChain creation in `Initialize(HWND)`, updated `Shutdown()` with proper cleanup

Compilation: CMake Debug OK + MSBuild VS Debug OK (0 errors, 0 warnings)

**Hecho: T1.3 Crear SwapChain con Back Buffers**  
**Siguiente: T1.4 Crear Descriptor Heaps (RTV, CBV/SRV/UAV)**

- docs: create methodology template (TEMPLATE.md)

Created generic methodology template abstracted from specific technologies.

Key features:
- **Agile-like sprint methodology** adapted for AI-assisted development
- **Technology-agnostic**: uses `[PLACEHOLDER]` markers for language/framework/build system
- Includes all core concepts:
  * Sprints, histories, tasks
  * Daily tracking (daily.md)
  * Visual progress bar (??? with percentage)
  * Commits conventions
  * Backlog management
  * Versioning/archiving of completed sprints

**Reusable for any project type**:
- Web (React, Vue, Angular)
- Mobile (iOS, Android, Flutter)
- Backend (Node, Python, Go, Rust)
- Desktop (C++, C#, Electron)
- Embedded, games, etc.

**Validation process abstracted**:
- Compile/execute/test depending on stack
- Examples provided for:
  * React + TypeScript ? `npm run lint && npm test && npm run build`
  * Python FastAPI ? `pytest && mypy . && black --check .`
  * Rust ? `cargo clippy && cargo test && cargo build`
  * Unity C# ? Compilation + unit tests

Template sections:
1. Core workflow (read docs ? propose ? implement ? validate ? commit ? repeat)
2. Sprint structure (sprint.md, histories.md, tasks.md)
3. Daily tracking (daily.md with "Hecho/Siguiente")
4. Visual progress bar (green/black squares with percentage)
5. Backlog management
6. Commit conventions
7. Versioning/archiving of completed sprints
8. Adaptation guide with technology-specific examples

**Purpose**: This template can be used as base for `copilot-instructions.md` in any future project by replacing `[PLACEHOLDER]` markers with project-specific details.

Files changed: `docs/TEMPLATE.md` (created - 436 lines)

- docs: plan sprint v1.1.0 - DX12 Minimal Renderer

Created complete sprint planning for v1.1.0 with AAA-level DX12 renderer implementation.

Sprint Goal: Implement minimal DirectX 12 renderer showing a colored quad rendered via GPU.

Sprint files populated:
- `docs/sprint.md`: Sprint overview, milestones (H1-H4), acceptance criteria, comparison with v1.0.0
- `docs/sprint_histories.md`: 4 user stories with detailed descriptions
  * H1: DX12 Initialization (Device, SwapChain, Heaps, Commands)
  * H2: Pipeline State & Shaders (Root Signature, PSO, HLSL)
  * H3: Geometry & Resources (Vertex Buffer, Constant Buffer)
  * H4: Command Recording & Execution (Draw Call, Fence, Present)
- `docs/sprint_tasks.md`: 15 detailed tasks (T1.1-T4.4) with implementation code snippets

Each task is a small, actionable iteration following AAA standards:
- Code implementation details included (DirectX 12 API calls)
- Files affected listed
- Success criteria defined
- Follows DX12 best practices (no shortcuts or temporary solutions)

Updated `docs/daily.md`:
- Hecho: 0.02 Planning - Sprint v1.1.0 defined (DX12 Minimal Renderer)
- Siguiente: T1.1 Create DX12 Device and DXGI Factory

Files changed: `docs/sprint.md`, `docs/sprint_histories.md`, `docs/sprint_tasks.md`, `docs/daily.md`

- feat(renderer): implement DX12 Device and DXGI Factory creation (T1.1)

Implemented Task T1.1 - Create DX12 Device and DXGI Factory following AAA standards.

Key changes:
- Use `CreateDXGIFactory2` with `DXGI_CREATE_FACTORY_DEBUG` flag in debug builds
- Enable D3D12 debug layer in debug builds for validation (`D3D12GetDebugInterface`)
- Enumerate GPU adapters with `EnumAdapters1` and skip software adapters
- Create `ID3D12Device` with `D3D_FEATURE_LEVEL_11_0`
- Log selected GPU name (converted from wide string) and device creation status
- Store Factory, Adapter, Device as member variables for later use
- Add `NativeFactory()` and `NativeCommandQueue()` accessors (prep for T1.2)
- Proper resource cleanup in `Shutdown()` with correct release order

Files changed:
- `src/renderer/DX12Device.h`: Added forward declarations for DX12 types, member variables (`m_factory`, `m_adapter`, `m_device`, `m_commandQueue`), new accessors
- `src/renderer/DX12Device.cpp`: Full DX12 initialization with `CORE_LOG_*` logging, GPU enumeration, debug layer setup

Compilation: CMake Debug OK + MSBuild Debug OK (0 errors, 0 warnings)

**Hecho: T1.1 Crear DX12 Device y DXGI Factory**  
**Siguiente: T1.2 Crear Command Queue**

- docs: clarify msbuild compilation step in copilot-instructions.md

Improved explanation of compilation step 2 to prevent confusion in new AI sessions.

Key changes:
- Added explicit **WARNING section** showing what **NOT to use** (`cmake --build` for step 2) and what **TO use** (`msbuild` command)
- Emphasized that `msbuild` command replicates **exactly** Visual Studio 2022 **Ctrl+Shift+B** behavior
- Removed ambiguous language that could lead to using `cmake` for step 2
- Formatted with clear ?/? indicators for correct/incorrect commands

This ensures new AI sessions will always use the correct `msbuild` command for step 2 compilation, preventing the recurring issue where new sessions would incorrectly use `cmake --build` for both steps.

Files changed: `.github/copilot-instructions.md`

