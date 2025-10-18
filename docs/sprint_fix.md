# Sprint Fix (v1.3.0)

Bugs resueltos durante el sprint activo.

---

### FIX-002 - ImFontAtlasBuilder nulo al llamar ImFontAtlasUpdateNewFrame

**ID Original**: BUG-002
**Prioridad**: Crítica
**Fecha de entrada**: 2025-01-18
**Fecha de resolución**: 2025-01-18

**Descripción del problema**: 

Al ejecutar la aplicación, se producía un crash en `external/imgui/imgui_draw.cpp` línea 2764:

```cpp
ImFontAtlasBuilder* builder = atlas->Builder;
builder->FrameCount = frame_count;  // ? CRASH: builder era 0x0000 (nullptr)
```

El debugger mostraba que `atlas->Builder` era `nullptr` cuando se llamaba a `ImFontAtlasUpdateNewFrame()`.

**Causa raíz identificada**:

El `Builder` del atlas de fuentes de ImGui se inicializa **SOLO** cuando se añade el primer font. En nuestro código:

1. ? `ImGui::CreateContext()` estaba presente
2. ? `ImGui_ImplWin32_Init(hwnd)` estaba presente
3. ? **FALTABA**: `io.Fonts->AddFontDefault()` para inicializar el atlas ANTES de los backends
4. ? El orden Win32 ? DX12 estaba correcto

Sin añadir ningún font, el `Builder` permanecía NULL cuando `ImGui_ImplDX12_NewFrame()` llamaba internamente a `ImFontAtlasUpdateNewFrame()`.

**Solución implementada**:

Añadir `io.Fonts->AddFontDefault()` DESPUÉS de crear el contexto y ANTES de inicializar backends:

```cpp
// Setup Dear ImGui context
IMGUI_CHECKVERSION();
ImGui::CreateContext();
ImGuiIO& io = ImGui::GetIO();
io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
ImGui::StyleColorsDark();

// ? CRÍTICO: Añadir font por defecto ANTES de inicializar backends
// Esto inicializa el Builder del atlas (soluciona BUG-002)
io.Fonts->AddFontDefault();

// Initialize Win32 backend (REQUERIDO - debe estar ANTES de DX12 backend)
ImGui_ImplWin32_Init(hwnd);
```

**Archivos afectados**: 

- `src/main.cpp` - Añadida línea `io.Fonts->AddFontDefault()`
- `docs/THIRD_PARTY.md` - Documentada política de NO modificar bibliotecas externas
- `docs/sprint_bugs.md` - Registrado y movido a sprint_fix.md

**Commit de resolución**: (pendiente de crear)

**Notas importantes**:

- ?? **NUNCA se modificó código en `external/imgui/`** - Se siguió correctamente la política de `docs/THIRD_PARTY.md`
- ? La solución fue arreglar NUESTRO código de inicialización, no tapar el síntoma en la biblioteca
- ?? Esto demuestra la importancia de **no añadir checks defensivos en bibliotecas externas**: el error era NUESTRO, no de ImGui
- ?? Lección aprendida: ImGui requiere que se añada al menos un font antes de que los backends llamen a `NewFrame()`

**Referencias**:

- `docs/THIRD_PARTY.md` - Política sobre bibliotecas externas
- ImGui documentation: https://github.com/ocornut/imgui/wiki/Getting-Started
