# Sprint Bugs (v1.3.0)

Bugs reportados durante el sprint activo pendientes de resolución.

---

### BUG-002 - ImFontAtlasBuilder nulo al llamar ImFontAtlasUpdateNewFrame

**ID**: BUG-002
**Prioridad**: Crítica
**Estado**: Reportado
**Fecha de entrada**: 2025-01-18

**Descripción**: 

Al ejecutar la aplicación, se produce un crash en `external/imgui/imgui_draw.cpp` línea 2764:

```cpp
ImFontAtlasBuilder* builder = atlas->Builder;
builder->FrameCount = frame_count;  // ? CRASH: builder es 0x0000 (nullptr)
```

El debugger muestra que `atlas->Builder` es `nullptr` cuando se llama a `ImFontAtlasUpdateNewFrame()`.

**Causa raíz sospechada**:

El problema NO está en ImGui (biblioteca externa). El problema está en **nuestro código de inicialización**:

1. Falta llamar a `ImGui_ImplWin32_Init(hwnd)` ANTES de inicializar el backend DX12
2. Falta llamar a `ImGui_ImplWin32_NewFrame()` ANTES de `ImGui_ImplDX12_NewFrame()` en cada frame
3. Posiblemente el orden de inicialización del atlas de fuentes no es correcto

**Comportamiento esperado**:

ImGui debe inicializarse correctamente con:
1. `ImGui::CreateContext()`
2. `ImGui_ImplWin32_Init(hwnd)` ? **ESTO FALTABA**
3. Configurar fuentes (opcional)
4. `ImGui_ImplDX12_Init(...)`

**Archivos afectados**: 

- `src/main.cpp` (código de inicialización de ImGui)
- `external/imgui/imgui_draw.cpp` (línea 2764, pero **NO TOCAR** - es biblioteca externa)

**Solución propuesta**:

? Añadir inicialización correcta del backend Win32 en `src/main.cpp`:

```cpp
// Setup Dear ImGui context
IMGUI_CHECKVERSION();
ImGui::CreateContext();
ImGuiIO& io = ImGui::GetIO();
io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
ImGui::StyleColorsDark();

// ? AÑADIR: Inicializar backend Win32
ImGui_ImplWin32_Init(hwnd);

// Luego inicializar backend DX12
ImGui_ImplDX12_Init(...);
```

Y en el loop de renderizado:

```cpp
// ? AÑADIR: Win32 NewFrame PRIMERO
ImGui_ImplWin32_NewFrame();
ImGui_ImplDX12_NewFrame();
ImGui::NewFrame();
```

**Notas importantes**:

- ?? **NUNCA modificar código en `external/imgui/`** - Consultar `docs/THIRD_PARTY.md`
- ? La solución correcta es arreglar nuestro código de inicialización en `src/main.cpp`
- ?? Este es un problema típico de orden de inicialización de ImGui con múltiples backends

**Referencias**:

- `docs/THIRD_PARTY.md` - Política sobre bibliotecas externas
- ImGui documentation: https://github.com/ocornut/imgui/wiki/Getting-Started
