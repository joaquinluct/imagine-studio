# Sprint Fix (v1.3.0)

Bugs resueltos durante el sprint activo.

---

### FIX-002 - ImFontAtlasBuilder nulo al llamar ImFontAtlasUpdateNewFrame

**ID Original**: BUG-002
**Prioridad**: Cr�tica
**Fecha de entrada**: 2025-01-18
**Fecha de resoluci�n**: 2025-01-18

**Descripci�n del problema**: 

Al ejecutar la aplicaci�n, se produc�an DOS crashes consecutivos en ImGui:

1. **Primer crash**: `atlas->Builder` era NULL en `ImFontAtlasUpdateNewFrame()` (`external/imgui/imgui_draw.cpp:2764`)
2. **Segundo crash**: `g.FontBaked` era NULL en `UpdateCurrentFontSize()` (`external/imgui/imgui.cpp`)

**Causa ra�z identificada**:

El problema ten�a dos partes:

1. **Builder NULL**: El `Builder` del atlas de fuentes se inicializa **SOLO** cuando se a�ade el primer font con `io.Fonts->AddFontDefault()`. Sin a�adir ning�n font, el Builder permanec�a NULL cuando `ImGui_ImplDX12_NewFrame()` llamaba internamente a `ImFontAtlasUpdateNewFrame()`.

2. **FontBaked NULL**: `g.FontBaked` se inicializa **SOLO** en el primer `ImGui::NewFrame()`. Llamar a `ImGui_ImplDX12_Init()` ANTES del primer NewFrame/EndFrame causaba que funciones internas intentaran acceder a `g.FontBaked` antes de que existiera.

**Soluci�n implementada**:

Dos cambios en `src/main.cpp`:

1. **A�adir font por defecto ANTES de backends**:
```cpp
// ? CR�TICO: A�adir font por defecto ANTES de inicializar backends
io.Fonts->AddFontDefault();
ImGui_ImplWin32_Init(hwnd);
```

2. **Llamar a NewFrame/EndFrame ANTES de DX12 Init para bakear fonts**:
```cpp
// ? CR�TICO: Llamar a NewFrame() + EndFrame() para inicializar g.FontBaked
// antes de inicializar el backend DX12 (soluciona crash en ImGui_ImplDX12_Init)
ImGui::NewFrame();
ImGui::EndFrame();
```

**Archivos afectados**: 

- `src/main.cpp` - A�adidas l�neas `io.Fonts->AddFontDefault()` y `ImGui::NewFrame()/EndFrame()` antes de `ImGui_ImplDX12_Init()`
- `docs/sprint_bugs.md` - Limpiado (bug resuelto)
- `docs/sprint_fix.md` - Documentada resoluci�n completa de BUG-002

**Commit de resoluci�n**: (pendiente de crear)

**Notas importantes**:

- ?? **NUNCA se modific� c�digo en `external/imgui/`** - Se sigui� correctamente la pol�tica de `docs/THIRD_PARTY.md`
- ? La soluci�n fue arreglar NUESTRO c�digo de inicializaci�n, no tapar el s�ntoma en la biblioteca
- ?? Esto demuestra la importancia de **no a�adir checks defensivos en bibliotecas externas**: el error era NUESTRO, no de ImGui
- ?? **Lecci�n aprendida 1**: ImGui requiere que se a�ada al menos un font ANTES de que los backends llamen a `NewFrame()`
- ?? **Lecci�n aprendida 2**: `ImGui_ImplDX12_Init()` internamente accede a `g.FontBaked`, por lo que **DEBE** haber un `NewFrame()/EndFrame()` previo para inicializar el estado interno de ImGui
- ?? **Lecci�n aprendida 3**: El orden correcto de inicializaci�n de ImGui con m�ltiples backends es:
  1. `ImGui::CreateContext()`
  2. Configurar `io.ConfigFlags`, estilos, etc.
  3. `io.Fonts->AddFontDefault()` (o AddFont...)
  4. `ImGui_ImplWin32_Init(hwnd)` (backend de plataforma primero)
  5. **`ImGui::NewFrame()` + `ImGui::EndFrame()`** (bakear fonts y estado)
  6. `ImGui_ImplDX12_Init(...)` (backend de render despu�s)
  7. Loop principal con NewFrame/Render/EndFrame

**Referencias**:

- `docs/THIRD_PARTY.md` - Pol�tica sobre bibliotecas externas
- ImGui documentation: https://github.com/ocornut/imgui/wiki/Getting-Started
