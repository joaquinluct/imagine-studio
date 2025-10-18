# Sprint Fix (v1.3.0)

Bugs resueltos durante el sprint activo.

---

### FIX-002 - ImFontAtlasBuilder nulo al llamar ImFontAtlasUpdateNewFrame

**ID Original**: BUG-002
**Prioridad**: Crítica
**Fecha de entrada**: 2025-01-18
**Fecha de resolución**: 2025-01-18

**Descripción del problema**: 

Al ejecutar la aplicación, se producían DOS crashes consecutivos en ImGui:

1. **Primer crash**: `atlas->Builder` era NULL en `ImFontAtlasUpdateNewFrame()` (`external/imgui/imgui_draw.cpp:2764`)
2. **Segundo crash**: `g.FontBaked` era NULL en `UpdateCurrentFontSize()` (`external/imgui/imgui.cpp`)

**Causa raíz identificada**:

El problema tenía dos partes:

1. **Builder NULL**: El `Builder` del atlas de fuentes se inicializa **SOLO** cuando se añade el primer font con `io.Fonts->AddFontDefault()`. Sin añadir ningún font, el Builder permanecía NULL cuando `ImGui_ImplDX12_NewFrame()` llamaba internamente a `ImFontAtlasUpdateNewFrame()`.

2. **FontBaked NULL**: `g.FontBaked` se inicializa **SOLO** en el primer `ImGui::NewFrame()`. Llamar a `ImGui_ImplDX12_Init()` ANTES del primer NewFrame/EndFrame causaba que funciones internas intentaran acceder a `g.FontBaked` antes de que existiera.

**Solución implementada**:

Dos cambios en `src/main.cpp`:

1. **Añadir font por defecto ANTES de backends**:
```cpp
// ? CRÍTICO: Añadir font por defecto ANTES de inicializar backends
io.Fonts->AddFontDefault();
ImGui_ImplWin32_Init(hwnd);
```

2. **Llamar a NewFrame/EndFrame ANTES de DX12 Init para bakear fonts**:
```cpp
// ? CRÍTICO: Llamar a NewFrame() + EndFrame() para inicializar g.FontBaked
// antes de inicializar el backend DX12 (soluciona crash en ImGui_ImplDX12_Init)
ImGui::NewFrame();
ImGui::EndFrame();
```

**Archivos afectados**: 

- `src/main.cpp` - Añadidas líneas `io.Fonts->AddFontDefault()` y `ImGui::NewFrame()/EndFrame()` antes de `ImGui_ImplDX12_Init()`
- `docs/sprint_bugs.md` - Limpiado (bug resuelto)
- `docs/sprint_fix.md` - Documentada resolución completa de BUG-002

**Commit de resolución**: (pendiente de crear)

**Notas importantes**:

- ?? **NUNCA se modificó código en `external/imgui/`** - Se siguió correctamente la política de `docs/THIRD_PARTY.md`
- ? La solución fue arreglar NUESTRO código de inicialización, no tapar el síntoma en la biblioteca
- ?? Esto demuestra la importancia de **no añadir checks defensivos en bibliotecas externas**: el error era NUESTRO, no de ImGui
- ?? **Lección aprendida 1**: ImGui requiere que se añada al menos un font ANTES de que los backends llamen a `NewFrame()`
- ?? **Lección aprendida 2**: `ImGui_ImplDX12_Init()` internamente accede a `g.FontBaked`, por lo que **DEBE** haber un `NewFrame()/EndFrame()` previo para inicializar el estado interno de ImGui
- ?? **Lección aprendida 3**: El orden correcto de inicialización de ImGui con múltiples backends es:
  1. `ImGui::CreateContext()`
  2. Configurar `io.ConfigFlags`, estilos, etc.
  3. `io.Fonts->AddFontDefault()` (o AddFont...)
  4. `ImGui_ImplWin32_Init(hwnd)` (backend de plataforma primero)
  5. **`ImGui::NewFrame()` + `ImGui::EndFrame()`** (bakear fonts y estado)
  6. `ImGui_ImplDX12_Init(...)` (backend de render después)
  7. Loop principal con NewFrame/Render/EndFrame

**Referencias**:

- `docs/THIRD_PARTY.md` - Política sobre bibliotecas externas
- ImGui documentation: https://github.com/ocornut/imgui/wiki/Getting-Started
