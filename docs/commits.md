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

