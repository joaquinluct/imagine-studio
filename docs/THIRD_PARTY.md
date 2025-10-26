# Third Party Libraries

Este documento registra todas las bibliotecas de terceros utilizadas en el proyecto y las reglas para su mantenimiento.

## Dear ImGui

- **Version**: v1.92.5 WIP (development branch)
- **License**: MIT
- **URL**: https://github.com/ocornut/imgui
- **Fecha de integraci�n**: 2025-01-17
- **Commit/Tag**: docking branch (latest as of 2025-01-17)
- **Ubicaci�n**: `external/imgui/`
- **Modificaciones**: **NINGUNA** - C�digo original intacto
- **Extensiones**: Todas las personalizaciones est�n en `src/editor/` y otros directorios del proyecto

### Archivos incluidos

```
external/imgui/
??? imgui.h
??? imgui.cpp
??? imgui_draw.cpp          ?? NUNCA MODIFICAR
??? imgui_internal.h
??? imgui_tables.cpp
??? imgui_widgets.cpp
??? imconfig.h
??? imstb_rectpack.h
??? imstb_textedit.h
??? imstb_truetype.h
??? backends/
    ??? imgui_impl_win32.h
    ??? imgui_impl_win32.cpp
    ??? imgui_impl_dx12.h
    ??? imgui_impl_dx12.cpp
```

### ?? REGLA CR�TICA: NO MODIFICAR C�DIGO DE IMGUI

**NUNCA** modificar directamente los archivos de `external/imgui/`. 

#### ? Pr�cticas INCORRECTAS

```cpp
// ? NO HACER: A�adir checks en imgui_draw.cpp
void ImFontAtlasUpdateNewFrame(ImFontAtlas* atlas, int frame_count, bool renderer_has_textures)
{
    // ? ESTO EST� MAL - tapa el s�ntoma, no arregla la causa
    if (atlas->Builder == NULL)
        ImFontAtlasBuildInit(atlas);
    
    // ... resto del c�digo
}
```

**Problemas de esta aproximaci�n:**
1. Imposible actualizar ImGui en el futuro (se perder�n los cambios)
2. Oculta bugs en nuestro c�digo
3. Rompe la reproducibilidad del proyecto
4. Dificulta el debugging

#### ? Pr�cticas CORRECTAS

**Opci�n 1: Wrappers en nuestro c�digo**
```cpp
// src/editor/ImGuiWrapper.h
namespace ImagineStudio {
    
    // Wrapper que inicializa correctamente antes de usar
    inline void SafeUpdateNewFrame(ImFontAtlas* atlas, int frame_count, bool has_textures)
    {
        // Asegurar estado correcto ANTES de llamar a ImGui
        if (atlas->Builder == NULL)
            atlas->Build(); // O la funci�n correcta de inicializaci�n
        
        ImFontAtlasUpdateNewFrame(atlas, frame_count, has_textures);
    }
    
}
```

**Opci�n 2: Inicializaci�n correcta en nuestro c�digo**
```cpp
// src/main.cpp o donde inicializamos ImGui
void InitializeImGui()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    // ? CORRECTO: Asegurar que el atlas est� inicializado
    io.Fonts->Build(); // O cualquier inicializaci�n necesaria
    
    // Inicializar backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX12_Init(...);
}
```

**Opci�n 3: Parches separados (solo si es absolutamente necesario)**
```
external/
  imgui/              ? C�digo original SIN TOCAR
  imgui_patches/      ? Nuestras modificaciones (si son realmente necesarias)
    001-fix-init-order.patch
```

### Actualizaci�n de ImGui

Para actualizar ImGui en el futuro:

```powershell
# 1. Descargar nueva versi�n
Invoke-WebRequest -Uri "https://github.com/ocornut/imgui/archive/docking.zip" -OutFile "imgui.zip"

# 2. Respaldar configuraci�n (si existe)
Copy-Item "external\imgui\imconfig.h" "imconfig.h.backup"

# 3. Reemplazar todo el directorio
Remove-Item -Recurse -Force "external\imgui"
Expand-Archive imgui.zip -DestinationPath "external"
Rename-Item "external\imgui-docking" "external\imgui"

# 4. Restaurar configuraci�n personalizada (si existe)
Copy-Item "imconfig.h.backup" "external\imgui\imconfig.h"

# 5. Compilar y probar
cmake --build build --config Debug
msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /m
```

### Configuraci�n permitida

El **�NICO** archivo que podemos modificar es `imconfig.h` para configuraci�n espec�fica del proyecto:

```cpp
// external/imgui/imconfig.h
// ? ESTO S� est� permitido - es el archivo de configuraci�n

#pragma once

// Definir tipos personalizados si es necesario
// #define ImTextureID MyTextureType*

// Habilitar caracter�sticas opcionales
#define IMGUI_ENABLE_FREETYPE
```

### Debugging de problemas con ImGui

Si encuentras un problema relacionado con ImGui:

1. **Verificar inicializaci�n**: �Estamos llamando a las funciones de ImGui en el orden correcto?
2. **Consultar documentaci�n**: Revisar `imgui.h` y ejemplos oficiales
3. **Buscar en issues**: https://github.com/ocornut/imgui/issues
4. **Hacer wrapper**: Crear funci�n de ayuda en `src/editor/ImGuiWrapper.h`
5. **Como �ltimo recurso**: Si es un bug real de ImGui, reportarlo upstream

### ?? Ejemplo del problema actual (BUG-002)

**S�ntoma**: `builder` es `nullptr` en `ImFontAtlasUpdateNewFrame`

**? Soluci�n INCORRECTA**: A�adir `if (builder == NULL)` en `imgui_draw.cpp`

**? Soluci�n CORRECTA**: Investigar por qu� nuestro c�digo est� llamando a `ImFontAtlasUpdateNewFrame` sin haber inicializado el atlas correctamente. El problema est� en **NUESTRO c�digo**, no en ImGui.

```cpp
// src/main.cpp
// ? Asegurar orden correcto de inicializaci�n
void InitializeRendering()
{
    // 1. Crear contexto
    ImGui::CreateContext();
    
    // 2. Configurar IO
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    // 3. ? CR�TICO: Construir atlas de fuentes ANTES de usar
    io.Fonts->Build();
    
    // 4. Inicializar backends
    ImGui_ImplWin32_Init(g_hwnd);
    ImGui_ImplDX12_Init(...);
}
```

---

## Otras bibliotecas de terceros

(A�adir aqu� seg�n se integren)

## Pol�tica general para bibliotecas externas

1. **NUNCA** modificar c�digo en `external/`
2. **SIEMPRE** usar wrappers en `src/`
3. **DOCUMENTAR** versi�n y fecha de integraci�n
4. **VERIFICAR** licencia compatible con el proyecto
5. **MANTENER** historial de actualizaciones en este documento
