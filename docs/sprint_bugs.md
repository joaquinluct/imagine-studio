# Sprint Bugs (v1.3.0)

Bugs reportados durante el sprint activo pendientes de resoluci�n.

---

### BUG-002 - ImFontAtlasBuilder nulo al llamar ImFontAtlasUpdateNewFrame

**ID**: BUG-002
**Prioridad**: Cr�tica
**Estado**: Reportado
**Fecha de entrada**: 2025-01-18

**Descripci�n**: 

Al ejecutar la aplicaci�n, se produce un crash en `external/imgui/imgui_draw.cpp` l�nea 2764:

```cpp
ImFontAtlasBuilder* builder = atlas->Builder;
builder->FrameCount = frame_count;  // ? CRASH: builder es 0x0000 (nullptr)
```

El debugger muestra que `atlas->Builder` es `nullptr` cuando se llama a `ImFontAtlasUpdateNewFrame()`.

**Causa ra�z sospechada**:

El problema NO est� en ImGui (biblioteca externa). El problema est� en **nuestro c�digo de inicializaci�n**:

1. Falta llamar a `ImGui_ImplWin32_Init(hwnd)` ANTES de inicializar el backend DX12
2. Falta llamar a `ImGui_ImplWin32_NewFrame()` ANTES de `ImGui_ImplDX12_NewFrame()` en cada frame
3. Posiblemente el orden de inicializaci�n del atlas de fuentes no es correcto

**Comportamiento esperado**:

ImGui debe inicializarse correctamente con:
1. `ImGui::CreateContext()`
2. `ImGui_ImplWin32_Init(hwnd)` ? **ESTO FALTABA**
3. Configurar fuentes (opcional)
4. `ImGui_ImplDX12_Init(...)`

**Archivos afectados**: 

- `src/main.cpp` (c�digo de inicializaci�n de ImGui)
- `external/imgui/imgui_draw.cpp` (l�nea 2764, pero **NO TOCAR** - es biblioteca externa)

**Soluci�n propuesta**:

? A�adir inicializaci�n correcta del backend Win32 en `src/main.cpp`:

```cpp
// Setup Dear ImGui context
IMGUI_CHECKVERSION();
ImGui::CreateContext();
ImGuiIO& io = ImGui::GetIO();
io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
ImGui::StyleColorsDark();

// ? A�ADIR: Inicializar backend Win32
ImGui_ImplWin32_Init(hwnd);

// Luego inicializar backend DX12
ImGui_ImplDX12_Init(...);
```

Y en el loop de renderizado:

```cpp
// ? A�ADIR: Win32 NewFrame PRIMERO
ImGui_ImplWin32_NewFrame();
ImGui_ImplDX12_NewFrame();
ImGui::NewFrame();
```

**Notas importantes**:

- ?? **NUNCA modificar c�digo en `external/imgui/`** - Consultar `docs/THIRD_PARTY.md`
- ? La soluci�n correcta es arreglar nuestro c�digo de inicializaci�n en `src/main.cpp`
- ?? Este es un problema t�pico de orden de inicializaci�n de ImGui con m�ltiples backends

**Referencias**:

- `docs/THIRD_PARTY.md` - Pol�tica sobre bibliotecas externas
- ImGui documentation: https://github.com/ocornut/imgui/wiki/Getting-Started
