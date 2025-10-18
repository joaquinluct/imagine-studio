# Sprint Fix (v1.3.0)

Bugs resueltos durante el sprint activo.

---

### FIX-002 - ImFontAtlasBuilder nulo al llamar ImFontAtlasUpdateNewFrame

**ID Original**: BUG-002
**Prioridad**: Cr�tica
**Fecha de entrada**: 2025-01-18
**Fecha de resoluci�n**: 2025-01-18

**Descripci�n del problema**: 

Al ejecutar la aplicaci�n, se produc�a un crash en `external/imgui/imgui_draw.cpp` l�nea 2764:

```cpp
ImFontAtlasBuilder* builder = atlas->Builder;
builder->FrameCount = frame_count;  // ? CRASH: builder era 0x0000 (nullptr)
```

El debugger mostraba que `atlas->Builder` era `nullptr` cuando se llamaba a `ImFontAtlasUpdateNewFrame()`.

**Causa ra�z identificada**:

El `Builder` del atlas de fuentes de ImGui se inicializa **SOLO** cuando se a�ade el primer font. En nuestro c�digo:

1. ? `ImGui::CreateContext()` estaba presente
2. ? `ImGui_ImplWin32_Init(hwnd)` estaba presente
3. ? **FALTABA**: `io.Fonts->AddFontDefault()` para inicializar el atlas ANTES de los backends
4. ? El orden Win32 ? DX12 estaba correcto

Sin a�adir ning�n font, el `Builder` permanec�a NULL cuando `ImGui_ImplDX12_NewFrame()` llamaba internamente a `ImFontAtlasUpdateNewFrame()`.

**Soluci�n implementada**:

A�adir `io.Fonts->AddFontDefault()` DESPU�S de crear el contexto y ANTES de inicializar backends:

```cpp
// Setup Dear ImGui context
IMGUI_CHECKVERSION();
ImGui::CreateContext();
ImGuiIO& io = ImGui::GetIO();
io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
ImGui::StyleColorsDark();

// ? CR�TICO: A�adir font por defecto ANTES de inicializar backends
// Esto inicializa el Builder del atlas (soluciona BUG-002)
io.Fonts->AddFontDefault();

// Initialize Win32 backend (REQUERIDO - debe estar ANTES de DX12 backend)
ImGui_ImplWin32_Init(hwnd);
```

**Archivos afectados**: 

- `src/main.cpp` - A�adida l�nea `io.Fonts->AddFontDefault()`
- `docs/THIRD_PARTY.md` - Documentada pol�tica de NO modificar bibliotecas externas
- `docs/sprint_bugs.md` - Registrado y movido a sprint_fix.md

**Commit de resoluci�n**: (pendiente de crear)

**Notas importantes**:

- ?? **NUNCA se modific� c�digo en `external/imgui/`** - Se sigui� correctamente la pol�tica de `docs/THIRD_PARTY.md`
- ? La soluci�n fue arreglar NUESTRO c�digo de inicializaci�n, no tapar el s�ntoma en la biblioteca
- ?? Esto demuestra la importancia de **no a�adir checks defensivos en bibliotecas externas**: el error era NUESTRO, no de ImGui
- ?? Lecci�n aprendida: ImGui requiere que se a�ada al menos un font antes de que los backends llamen a `NewFrame()`

**Referencias**:

- `docs/THIRD_PARTY.md` - Pol�tica sobre bibliotecas externas
- ImGui documentation: https://github.com/ocornut/imgui/wiki/Getting-Started
