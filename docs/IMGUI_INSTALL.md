# ImGui Installation Documentation

Este documento describe el proceso de instalación, actualización y reinstalación de ImGui en Imagine Studio.

---

## ?? Origen de ImGui

**Librería**: Dear ImGui (Immediate Mode Graphical User Interface)  
**Repositorio oficial**: https://github.com/ocornut/imgui  
**Licencia**: MIT License (open source, sin restricciones)  
**Versión recomendada**: v1.91.x o posterior (última versión estable)

**Descripción**: ImGui es el estándar de la industria AAA para UI de herramientas y editores (Unity Editor, Unreal Editor, Blender tools). Es un framework immediate mode optimizado para performance y fácil integración.

---

## ?? Instalación Inicial (Automática)

### Script Automático (Recomendado)

El proyecto incluye un script PowerShell que descarga e instala ImGui automáticamente:

```powershell
.\scripts\install-imgui.ps1
```

**Qué hace el script:**
1. Descarga la última versión estable de ImGui desde GitHub
2. Extrae los archivos necesarios:
   - Core: `imgui.h`, `imgui.cpp`, `imgui_draw.cpp`, `imgui_tables.cpp`, `imgui_widgets.cpp`, etc.
   - Backends: `imgui_impl_dx12.h/cpp`, `imgui_impl_win32.h/cpp`
3. Los copia a `external/imgui/`
4. Crea un archivo `VERSION.txt` con la versión instalada y fecha

**Opciones del script:**
- `-Version <version>`: Instalar versión específica (ej: `-Version v1.91.5`)
- `-Reinstall`: Forzar reinstalación (elimina ImGui existente)

**Ejemplos:**
```powershell
# Instalar última versión (default)
.\scripts\install-imgui.ps1

# Instalar versión específica
.\scripts\install-imgui.ps1 -Version v1.91.5

# Reinstalar (forzar descarga)
.\scripts\install-imgui.ps1 -Reinstall
```

---

## ?? Actualización de ImGui

Para actualizar ImGui a una versión más reciente:

**Opción 1: Actualizar a última versión**
```powershell
.\scripts\install-imgui.ps1 -Reinstall
```

**Opción 2: Actualizar a versión específica**
```powershell
.\scripts\install-imgui.ps1 -Version v1.92.0 -Reinstall
```

El script eliminará la versión anterior y descargará la nueva. **No perderás configuraciones** (el proyecto usa los archivos de ImGui como librería externa, sin modificaciones).

---

## ??? Reinstalación desde Cero

Si necesitas reinstalar ImGui completamente (por ejemplo, archivos corruptos o pruebas):

**Paso 1: Eliminar ImGui existente**
```powershell
Remove-Item external/imgui -Recurse -Force
```

**Paso 2: Reinstalar con script**
```powershell
.\scripts\install-imgui.ps1
```

**Alternativa: Reinstalar en un solo paso**
```powershell
.\scripts\install-imgui.ps1 -Reinstall
```

---

## ?? Estructura de Archivos Instalados

Después de la instalación, la estructura es:

```
external/imgui/
??? imgui.h                         # Header principal
??? imgui.cpp                       # Implementación core
??? imgui_draw.cpp                  # Rendering primitives
??? imgui_tables.cpp                # Tables API
??? imgui_widgets.cpp               # Widgets (buttons, sliders, etc.)
??? imgui_internal.h                # Internal API (no usar directamente)
??? imconfig.h                      # Configuration (customizable)
??? imstb_rectpack.h                # STB rect packer (font atlas)
??? imstb_textedit.h                # STB text editing (input text)
??? imstb_truetype.h                # STB TrueType font loading
??? backends/
?   ??? imgui_impl_dx12.h           # DirectX 12 backend header
?   ??? imgui_impl_dx12.cpp         # DirectX 12 backend implementation
?   ??? imgui_impl_win32.h          # Win32 backend header
?   ??? imgui_impl_win32.cpp        # Win32 backend implementation
??? VERSION.txt                     # Version tracking (generado por script)
```

---

## ?? Verificación de Instalación

Para verificar que ImGui está instalado correctamente:

**1. Verificar archivos existen**
```powershell
Test-Path external/imgui/imgui.h
Test-Path external/imgui/backends/imgui_impl_dx12.cpp
```
Ambos deben retornar `True`.

**2. Verificar versión instalada**
```powershell
Get-Content external/imgui/VERSION.txt
```
Ejemplo de salida:
```
ImGui Version: v1.91.5
Downloaded: 2025-01-18 12:34:56
Source: https://github.com/ocornut/imgui
```

**3. Compilar proyecto**
```powershell
cmake -S . -B build
cmake --build build --config Debug
```
No debe haber errores relacionados con ImGui.

---

## ?? Integración con Build System

ImGui se integra automáticamente en el build system de Imagine Studio:

### CMake (CMakeLists.txt)

El proyecto incluye configuración CMake para ImGui:

```cmake
# ImGui library
add_library(ImGui STATIC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_dx12.cpp
    external/imgui/backends/imgui_impl_win32.cpp
)
target_include_directories(ImGui PUBLIC 
    external/imgui 
    external/imgui/backends
)

# Link ImGui with main project
target_link_libraries(ImagineStudio PRIVATE ImGui d3d12 dxgi d3dcompiler)
```

**Regenerar CMake después de instalación:**
```powershell
cmake -S . -B build
```

### Visual Studio (Imagine Studio.vcxproj)

Los archivos ImGui se añaden manualmente al proyecto Visual Studio:
- Header Files / external / imgui
- Source Files / external / imgui
- Include directories: `$(SolutionDir)external\imgui;$(SolutionDir)external\imgui\backends`

**Esto se hace automáticamente en la tarea H1.3 del sprint.**

---

## ?? Troubleshooting

### Problema: Script no puede descargar ImGui
**Causa**: Firewall, proxy o problemas de red.  
**Solución**: Descargar manualmente desde GitHub y extraer a `external/imgui/`.

**Pasos manuales:**
1. Ir a https://github.com/ocornut/imgui/releases
2. Descargar última versión (ej: `imgui-1.91.5.zip`)
3. Extraer archivos core a `external/imgui/`
4. Extraer archivos `backends/imgui_impl_dx12.*` y `backends/imgui_impl_win32.*` a `external/imgui/backends/`

### Problema: Errores de compilación después de instalación
**Causa**: CMake no regenerado o archivos no encontrados.  
**Solución**:
```powershell
# Limpiar build
Remove-Item build -Recurse -Force

# Regenerar CMake
cmake -S . -B build

# Compilar
cmake --build build --config Debug
```

### Problema: Versión de ImGui desactualizada
**Causa**: Instalación antigua o versión específica requerida.  
**Solución**:
```powershell
# Reinstalar última versión
.\scripts\install-imgui.ps1 -Reinstall
```

---

## ?? Enlaces Útiles

- **Repositorio oficial**: https://github.com/ocornut/imgui
- **Documentación**: https://github.com/ocornut/imgui/wiki
- **Demo/Examples**: https://github.com/ocornut/imgui/tree/master/examples
- **DX12 Backend docs**: https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_dx12.cpp (header comments)
- **Win32 Backend docs**: https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_win32.cpp (header comments)

---

## ?? Notas Importantes

1. **No modificar archivos ImGui**: Los archivos en `external/imgui/` son parte de la librería oficial. Cualquier customización debe hacerse en `imconfig.h` (configuración) o en el código de integración (`main.cpp`, `EditorUI.cpp`, etc.).

2. **Actualizar regularmente**: ImGui recibe actualizaciones frecuentes con bugfixes y mejoras. Se recomienda actualizar cada 3-6 meses o cuando se necesite una feature nueva.

3. **Versión mínima**: ImGui v1.89+ es requerido para docking support (usado en Sprint v1.3.0).

4. **Backends oficiales**: Siempre usar backends oficiales de ImGui (`imgui_impl_dx12`, `imgui_impl_win32`). No usar backends de terceros o custom.

5. **Licencia MIT**: ImGui es open source con licencia MIT. No hay restricciones para uso comercial. Incluir créditos en About/Credits del editor.

---

## ?? Siguiente Paso

Después de instalar ImGui, continuar con:
- **Sprint v1.3.0 Task H1.2**: Añadir ImGui a CMakeLists.txt
- Ver `docs/sprint_tasks.md` para detalles de implementación

---

*Documento creado: 2025-01-18*  
*Última actualización: 2025-01-18*  
*Sprint: v1.3.0 - ImGui Integration*
