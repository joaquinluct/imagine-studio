# ImGui Installation Documentation

Este documento describe el proceso de instalaci�n, actualizaci�n y reinstalaci�n de ImGui en Imagine Studio.

---

## ?? Origen de ImGui

**Librer�a**: Dear ImGui (Immediate Mode Graphical User Interface)  
**Repositorio oficial**: https://github.com/ocornut/imgui  
**Licencia**: MIT License (open source, sin restricciones)  
**Versi�n recomendada**: v1.91.x o posterior (�ltima versi�n estable)

**Descripci�n**: ImGui es el est�ndar de la industria AAA para UI de herramientas y editores (Unity Editor, Unreal Editor, Blender tools). Es un framework immediate mode optimizado para performance y f�cil integraci�n.

---

## ?? Instalaci�n Inicial (Autom�tica)

### Script Autom�tico (Recomendado)

El proyecto incluye un script PowerShell que descarga e instala ImGui autom�ticamente:

```powershell
.\scripts\install-imgui.ps1
```

**Qu� hace el script:**
1. Descarga la �ltima versi�n estable de ImGui desde GitHub
2. Extrae los archivos necesarios:
   - Core: `imgui.h`, `imgui.cpp`, `imgui_draw.cpp`, `imgui_tables.cpp`, `imgui_widgets.cpp`, etc.
   - Backends: `imgui_impl_dx12.h/cpp`, `imgui_impl_win32.h/cpp`
3. Los copia a `external/imgui/`
4. Crea un archivo `VERSION.txt` con la versi�n instalada y fecha

**Opciones del script:**
- `-Version <version>`: Instalar versi�n espec�fica (ej: `-Version v1.91.5`)
- `-Reinstall`: Forzar reinstalaci�n (elimina ImGui existente)

**Ejemplos:**
```powershell
# Instalar �ltima versi�n (default)
.\scripts\install-imgui.ps1

# Instalar versi�n espec�fica
.\scripts\install-imgui.ps1 -Version v1.91.5

# Reinstalar (forzar descarga)
.\scripts\install-imgui.ps1 -Reinstall
```

---

## ?? Actualizaci�n de ImGui

Para actualizar ImGui a una versi�n m�s reciente:

**Opci�n 1: Actualizar a �ltima versi�n**
```powershell
.\scripts\install-imgui.ps1 -Reinstall
```

**Opci�n 2: Actualizar a versi�n espec�fica**
```powershell
.\scripts\install-imgui.ps1 -Version v1.92.0 -Reinstall
```

El script eliminar� la versi�n anterior y descargar� la nueva. **No perder�s configuraciones** (el proyecto usa los archivos de ImGui como librer�a externa, sin modificaciones).

---

## ??? Reinstalaci�n desde Cero

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

Despu�s de la instalaci�n, la estructura es:

```
external/imgui/
??? imgui.h                         # Header principal
??? imgui.cpp                       # Implementaci�n core
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

## ?? Verificaci�n de Instalaci�n

Para verificar que ImGui est� instalado correctamente:

**1. Verificar archivos existen**
```powershell
Test-Path external/imgui/imgui.h
Test-Path external/imgui/backends/imgui_impl_dx12.cpp
```
Ambos deben retornar `True`.

**2. Verificar versi�n instalada**
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

## ?? Integraci�n con Build System

ImGui se integra autom�ticamente en el build system de Imagine Studio:

### CMake (CMakeLists.txt)

El proyecto incluye configuraci�n CMake para ImGui:

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

**Regenerar CMake despu�s de instalaci�n:**
```powershell
cmake -S . -B build
```

### Visual Studio (Imagine Studio.vcxproj)

Los archivos ImGui se a�aden manualmente al proyecto Visual Studio:
- Header Files / external / imgui
- Source Files / external / imgui
- Include directories: `$(SolutionDir)external\imgui;$(SolutionDir)external\imgui\backends`

**Esto se hace autom�ticamente en la tarea H1.3 del sprint.**

---

## ?? Troubleshooting

### Problema: Script no puede descargar ImGui
**Causa**: Firewall, proxy o problemas de red.  
**Soluci�n**: Descargar manualmente desde GitHub y extraer a `external/imgui/`.

**Pasos manuales:**
1. Ir a https://github.com/ocornut/imgui/releases
2. Descargar �ltima versi�n (ej: `imgui-1.91.5.zip`)
3. Extraer archivos core a `external/imgui/`
4. Extraer archivos `backends/imgui_impl_dx12.*` y `backends/imgui_impl_win32.*` a `external/imgui/backends/`

### Problema: Errores de compilaci�n despu�s de instalaci�n
**Causa**: CMake no regenerado o archivos no encontrados.  
**Soluci�n**:
```powershell
# Limpiar build
Remove-Item build -Recurse -Force

# Regenerar CMake
cmake -S . -B build

# Compilar
cmake --build build --config Debug
```

### Problema: Versi�n de ImGui desactualizada
**Causa**: Instalaci�n antigua o versi�n espec�fica requerida.  
**Soluci�n**:
```powershell
# Reinstalar �ltima versi�n
.\scripts\install-imgui.ps1 -Reinstall
```

---

## ?? Enlaces �tiles

- **Repositorio oficial**: https://github.com/ocornut/imgui
- **Documentaci�n**: https://github.com/ocornut/imgui/wiki
- **Demo/Examples**: https://github.com/ocornut/imgui/tree/master/examples
- **DX12 Backend docs**: https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_dx12.cpp (header comments)
- **Win32 Backend docs**: https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_win32.cpp (header comments)

---

## ?? Notas Importantes

1. **No modificar archivos ImGui**: Los archivos en `external/imgui/` son parte de la librer�a oficial. Cualquier customizaci�n debe hacerse en `imconfig.h` (configuraci�n) o en el c�digo de integraci�n (`main.cpp`, `EditorUI.cpp`, etc.).

2. **Actualizar regularmente**: ImGui recibe actualizaciones frecuentes con bugfixes y mejoras. Se recomienda actualizar cada 3-6 meses o cuando se necesite una feature nueva.

3. **Versi�n m�nima**: ImGui v1.89+ es requerido para docking support (usado en Sprint v1.3.0).

4. **Backends oficiales**: Siempre usar backends oficiales de ImGui (`imgui_impl_dx12`, `imgui_impl_win32`). No usar backends de terceros o custom.

5. **Licencia MIT**: ImGui es open source con licencia MIT. No hay restricciones para uso comercial. Incluir cr�ditos en About/Credits del editor.

---

## ?? Siguiente Paso

Despu�s de instalar ImGui, continuar con:
- **Sprint v1.3.0 Task H1.2**: A�adir ImGui a CMakeLists.txt
- Ver `docs/sprint_tasks.md` para detalles de implementaci�n

---

*Documento creado: 2025-01-18*  
*�ltima actualizaci�n: 2025-01-18*  
*Sprint: v1.3.0 - ImGui Integration*
