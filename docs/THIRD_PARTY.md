# Third Party Libraries

Este documento registra todas las bibliotecas de terceros utilizadas en el proyecto y las reglas para su mantenimiento.

## Dear ImGui

- **Version**: v1.92.5 WIP (development branch)
- **License**: MIT
- **URL**: https://github.com/ocornut/imgui
- **Fecha de integración**: 2025-01-17
- **Commit/Tag**: docking branch (latest as of 2025-01-17)
- **Ubicación**: `external/imgui/`
- **Modificaciones**: **NINGUNA** - Código original intacto
- **Extensiones**: Todas las personalizaciones están en `src/editor/` y otros directorios del proyecto

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

### ?? REGLA CRÍTICA: NO MODIFICAR CÓDIGO DE IMGUI

**NUNCA** modificar directamente los archivos de `external/imgui/`. 

#### ? Prácticas INCORRECTAS

```cpp
// ? NO HACER: Añadir checks en imgui_draw.cpp
void ImFontAtlasUpdateNewFrame(ImFontAtlas* atlas, int frame_count, bool renderer_has_textures)
{
    // ? ESTO ESTÁ MAL - tapa el síntoma, no arregla la causa
    if (atlas->Builder == NULL)
        ImFontAtlasBuildInit(atlas);
    
    // ... resto del código
}
```

**Problemas de esta aproximación:**
1. Imposible actualizar ImGui en el futuro (se perderán los cambios)
2. Oculta bugs en nuestro código
3. Rompe la reproducibilidad del proyecto
4. Dificulta el debugging

#### ? Prácticas CORRECTAS

**Opción 1: Wrappers en nuestro código**
```cpp
// src/editor/ImGuiWrapper.h
namespace ImagineStudio {
    
    // Wrapper que inicializa correctamente antes de usar
    inline void SafeUpdateNewFrame(ImFontAtlas* atlas, int frame_count, bool has_textures)
    {
        // Asegurar estado correcto ANTES de llamar a ImGui
        if (atlas->Builder == NULL)
            atlas->Build(); // O la función correcta de inicialización
        
        ImFontAtlasUpdateNewFrame(atlas, frame_count, has_textures);
    }
    
}
```

**Opción 2: Inicialización correcta en nuestro código**
```cpp
// src/main.cpp o donde inicializamos ImGui
void InitializeImGui()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    // ? CORRECTO: Asegurar que el atlas está inicializado
    io.Fonts->Build(); // O cualquier inicialización necesaria
    
    // Inicializar backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX12_Init(...);
}
```

**Opción 3: Parches separados (solo si es absolutamente necesario)**
```
external/
  imgui/              ? Código original SIN TOCAR
  imgui_patches/      ? Nuestras modificaciones (si son realmente necesarias)
    001-fix-init-order.patch
```

### Actualización de ImGui

Para actualizar ImGui en el futuro:

```powershell
# 1. Descargar nueva versión
Invoke-WebRequest -Uri "https://github.com/ocornut/imgui/archive/docking.zip" -OutFile "imgui.zip"

# 2. Respaldar configuración (si existe)
Copy-Item "external\imgui\imconfig.h" "imconfig.h.backup"

# 3. Reemplazar todo el directorio
Remove-Item -Recurse -Force "external\imgui"
Expand-Archive imgui.zip -DestinationPath "external"
Rename-Item "external\imgui-docking" "external\imgui"

# 4. Restaurar configuración personalizada (si existe)
Copy-Item "imconfig.h.backup" "external\imgui\imconfig.h"

# 5. Compilar y probar
cmake --build build --config Debug
msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /m
```

### Configuración permitida

El **ÚNICO** archivo que podemos modificar es `imconfig.h` para configuración específica del proyecto:

```cpp
// external/imgui/imconfig.h
// ? ESTO SÍ está permitido - es el archivo de configuración

#pragma once

// Definir tipos personalizados si es necesario
// #define ImTextureID MyTextureType*

// Habilitar características opcionales
#define IMGUI_ENABLE_FREETYPE
```

### Debugging de problemas con ImGui

Si encuentras un problema relacionado con ImGui:

1. **Verificar inicialización**: ¿Estamos llamando a las funciones de ImGui en el orden correcto?
2. **Consultar documentación**: Revisar `imgui.h` y ejemplos oficiales
3. **Buscar en issues**: https://github.com/ocornut/imgui/issues
4. **Hacer wrapper**: Crear función de ayuda en `src/editor/ImGuiWrapper.h`
5. **Como último recurso**: Si es un bug real de ImGui, reportarlo upstream

### ?? Ejemplo del problema actual (BUG-002)

**Síntoma**: `builder` es `nullptr` en `ImFontAtlasUpdateNewFrame`

**? Solución INCORRECTA**: Añadir `if (builder == NULL)` en `imgui_draw.cpp`

**? Solución CORRECTA**: Investigar por qué nuestro código está llamando a `ImFontAtlasUpdateNewFrame` sin haber inicializado el atlas correctamente. El problema está en **NUESTRO código**, no en ImGui.

```cpp
// src/main.cpp
// ? Asegurar orden correcto de inicialización
void InitializeRendering()
{
    // 1. Crear contexto
    ImGui::CreateContext();
    
    // 2. Configurar IO
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    // 3. ? CRÍTICO: Construir atlas de fuentes ANTES de usar
    io.Fonts->Build();
    
    // 4. Inicializar backends
    ImGui_ImplWin32_Init(g_hwnd);
    ImGui_ImplDX12_Init(...);
}
```

---

## Otras bibliotecas de terceros

(Añadir aquí según se integren)

## Política general para bibliotecas externas

1. **NUNCA** modificar código en `external/`
2. **SIEMPRE** usar wrappers en `src/`
3. **DOCUMENTAR** versión y fecha de integración
4. **VERIFICAR** licencia compatible con el proyecto
5. **MANTENER** historial de actualizaciones en este documento
