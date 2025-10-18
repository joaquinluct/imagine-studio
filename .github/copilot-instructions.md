Este repositorio usa las siguientes instrucciones de trabajo para sesiones con el asistente (GitHub Copilot).

## ?? LEER PRIMERO

**OBLIGATORIO**: Antes de comenzar cualquier sesión, leer:  
?? **[`docs/MAIN.md`](../docs/MAIN.md)** - Decálogo fundamental del proyecto (pilares, arquitectura, estándares AAA)

Este documento define los 4 pilares fundamentales y principios del proyecto Imagine Studio.  
**Todas las decisiones arquitectónicas y de implementación DEBEN alinearse con estos pilares.**

---

Archivos principales de trabajo:
- `docs/MAIN.md` - **[LEER PRIMERO]** Decálogo y pilares fundamentales del proyecto.
- `docs/sprint.md` - Sprint actual con hitos y objetivos.
- `docs/daily.md` - Última tarea completada y tarea actual en progreso (o "Sprint vX.Y.Z cerrado. Sin sprint activo." si no hay sprint activo).
- `docs/commits.md` - Convenciones de commits y mensajes.
- `docs/sprint_bugs.md` - Tracking de bugs reportados (pendientes de resolución).
- `docs/sprint_fix.md` - Historial de bugs resueltos durante el sprint.

Flujo de trabajo por sesión:
1) Leer estas instrucciones.
2) Consultar `docs/daily.md` para ver la última tarea completada y la tarea actual.
3) Comparar con el código fuente.
4) Proponer el siguiente paso (por parte del asistente).
5) Si se confirma, el asistente implementa el paso.
   - Antes de crear el commit se debe compilar la solución/proyecto.
     - **Requisito obligatorio: DOS compilaciones limpias (0 errores, 0 warnings)**:
       
       **Compilación 1 - CMake Build (Debug)**:
       ```powershell
       cmake --build build --config Debug
       ```
       
       **Compilación 2 - Visual Studio Build Solution (Ctrl+Mayús+B equivalente)**:
       ```powershell
       msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /m
       ```
       
       **IMPORTANTE**: La compilación 2 debe ejecutarse **EXACTAMENTE** con el comando `msbuild` mostrado arriba.
       Este comando replica **exactamente** lo que hace Visual Studio 2022 cuando pulsas **Ctrl+Mayús+B** ("Build Solution") en la solución principal `Imagine Studio.sln` de la raíz.
       
       **NOTA**: Existen DOS sistemas de build en este proyecto:
       - **Sistema Principal (desarrollo)**: `Imagine Studio.sln` + `Imagine Studio.vcxproj` en la raíz → Usado para desarrollo diario con F5/Ctrl+Mayús+B
       - **Sistema CMake (CI/automatización)**: `build/ImagineStudio.sln` generado por CMake → Usado para builds automatizados
       
       Ambos sistemas deben compilar sin errores. La Compilación 1 usa CMake, la Compilación 2 usa el proyecto principal de Visual Studio.
       
       **NO usar** variaciones como:
       - ❌ `cmake --build` (eso es compilación 1, no compilación 2)
       - ❌ `msbuild "build/ImagineStudio.sln"` (esa es la solución de CMake, no la principal)
       - ❌ `devenv /build`
       - ❌ Otros métodos alternativos
       
       **SÍ usar**:
       - ✅ `msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /m`
       
     - Si alguna de las dos compilaciones produce errores o warnings, el asistente corregirá los errores cuando sea posible y volverá a ejecutar ambas compilaciones hasta obtener builds limpias.
     - El asistente ejecutará automáticamente ambas compilaciones (CMake + MSBuild) antes de crear commits durante sus iteraciones.
   - Una vez ambas compilaciones estén limpias, crear el commit y actualizar los archivos Markdown pertinentes (`docs/daily.md`, `docs/commits.md`, `README.md` cuando proceda).
6) Repetir desde 1.

Nota: Los archivos anteriores son la fuente de la verdad para la organización del proyecto y la comunicación con el asistente.

Preferencias del propietario del repositorio:
- Prefiero hacer bien las cosas desde el principio: evitar partes intermedias, temporales, incompletes o no funcionales. Cada módulo debe diseñarse para cumplir los estándares AAA desde su concepción.
- En cada iteración, antes del commit, siempre se debe compilar y corregir errores/warnings para asegurar commits limpios y funcionales.
- Estilo de includes: las directivas `#include` deben ordenarse siempre con las cabeceras del proyecto (entre comillas `"..."`) primero y después las cabeceras del sistema (`<...>`), y dentro de cada grupo deben aparecer en orden alfabético. Esta regla será verificada por el asistente y aplicada o reportada como warning al preparar commits.
- **Codificación de archivos**: TODOS los archivos de texto (`.md`, `.cpp`, `.h`, `.hlsl`, etc.) DEBEN usar **UTF-8 con BOM** y **line endings CRLF** (Windows). Esto es CRÍTICO para evitar problemas de codificación con caracteres especiales (emojis, caracteres no-ASCII). El asistente DEBE:
  - Al crear/editar archivos, asegurarse de que se guarden con UTF-8 con BOM
  - Verificar que no haya line endings mixtos (CRLF + LF)
  - Si detecta problemas de codificación, ejecutar `.\scripts\check-encoding.ps1 -Fix` antes del commit
  - Nunca usar UTF-8 sin BOM para archivos con caracteres no-ASCII
  - Los warnings de Git "LF will be replaced by CRLF" son normales y esperados (Git normaliza automáticamente)

Sección de resolución de problemas (Resolución de problemas)
Si en algún momento la herramienta no puede compilar la solución o no encuentra la `.sln`, o aparece un error de enlace tipo "unresolved external main", seguir estos pasos:

1) Comprobar la existencia y ruta exacta de la solución desde PowerShell:
   - Ejecutar `Get-ChildItem -Force` o `ls` en el directorio raíz del repositorio para localizar `*.sln` y obtener la ruta exacta.
2) Compilar explícitamente usando PowerShell y `msbuild` con la ruta entre comillas (importante si el nombre contiene espacios):
   - `msbuild "C:\ruta\a\Imagine Studio.sln" /p:Configuration=Debug /m`
3) Si la herramienta reporta que no puede encontrar archivos fuente nuevos, confirmar que los ficheros están incluidos en el `.vcxproj` o en el `CMakeLists.txt` de la solución. Añadirlos si es necesario.
4) Si aparece `LNK2019: unresolved external main` o similar:
   - Causa frecuente: discrepancia entre la entrada esperada por el subsistema (CONSOLE vs WINDOWS) y las funciones `main`/`WinMain` definidas en el código.
   - Solución: proporcionar entradas portables (por ejemplo, una función `RunApp(HINSTANCE)` y definir tanto `int main()` como `WinMain` que llamen a `RunApp`), o ajustar la configuración del proyecto al subsistema correcto y tener la firma adecuada.
5) Si la compilación muestra errores/warnings: corregirlos hasta obtener una build limpia antes de proceder al commit (esta regla ya está en el flujo de trabajo).
6) Alternativa: si no hay `.sln` o se desea portabilidad, crear un `CMakeLists.txt` básico y usar `cmake` + `cmake --build`.

Registrar en la bitácora (`docs/daily.md`) cualquier incidencia relevante y su resolución para futuras referencias.

⚠️ REGLA CRÍTICA: Bibliotecas de terceros (external/)
--------------------------------------------------------
**NUNCA MODIFICAR** código en el directorio `external/`. 

**Razones:**
1. Imposibilita actualizar la biblioteca en el futuro
2. Oculta bugs en nuestro código (tapa síntomas, no arregla causas)
3. Rompe la reproducibilidad del proyecto
4. Dificulta el debugging y mantenimiento

**Política:**
- TODO el código de bibliotecas externas (Dear ImGui, etc.) debe permanecer **INTACTO**
- Si necesitas personalizar comportamiento: crear **wrappers** en `src/` (ej: `src/editor/ImGuiWrapper.h`)
- Si encuentras un bug relacionado con una biblioteca externa:
  1. **NO** añadir checks defensivos en la biblioteca
  2. **SÍ** investigar por qué nuestro código está llamando incorrectamente a la API
  3. **SÍ** corregir el orden de inicialización o uso en nuestro código
  4. **SÍ** consultar `docs/THIRD_PARTY.md` para ver versiones y políticas específicas

**Ejemplo de error común:**
```cpp
// ❌ NO HACER: Añadir if(builder == NULL) en imgui_draw.cpp para evitar crash
// ✅ CORRECTO: Arreglar nuestro código para asegurar que builder esté inicializado ANTES de llamar

// src/main.cpp
void InitializeRendering()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    // ✅ CRÍTICO: Build debe llamarse ANTES de usar el atlas
    io.Fonts->Build();
    
    // Ahora sí podemos usar backends
    ImGui_ImplWin32_Init(g_hwnd);
    ImGui_ImplDX12_Init(...);
}
```

**Consultar siempre:** `docs/THIRD_PARTY.md` para políticas detalladas sobre cada biblioteca externa.

Iteración controlada (proceso obligatorio por iteración)
1) El asistente consultará `docs/daily.md` y propondrá el siguiente punto a ejecutar, acompañado de una breve explicación (una frase) sobre en qué consiste.

Ejecución automática (sin necesidad de confirmación previa):
2) Tras proponer el punto, el asistente implementará el cambio, compilará la solución y corregirá los errores/warnings hasta lograr una compilación limpia.
3) Si la compilación queda limpia, el asistente realizará directamente el commit local siguiendo `docs/commits.md` y actualizará los ficheros necesarios (`docs/daily.md`, `docs/commits.md`, `README.md` y `.github/copilot-instructions.md`) sin pedir permiso adicional.
4) El asistente informará de lo realizado y del siguiente punto propuesto y continuará el ciclo.

Excepciones: Si aparece un bloqueo técnico, decisión arquitectónica crítica o dependencia que impida avanzar, el asistente pausará y solicitará instrucciones al propietario.

Regla estricta sobre commits y documentación:
- Requisito: Siempre que el asistente realice un commit local como resultado de una iteración (es decir, la compilación queda limpia), actualizará automáticamente `docs/daily.md` y `docs/commits.md` para reflejar el cambio sin pedir confirmación adicional. El push al repositorio remoto no se realizará automáticamente salvo instrucción explícita del propietario.

Versionado de los ficheros del Sprint (snapshots de sprint):
- Los ficheros activos del sprint actual son:
  - `docs/sprint.md` (sprint de alto nivel con hitos y objetivos)
  - `docs/sprint_histories.md` (historias de usuario para el sprint)
  - `docs/sprint_tasks.md` (tareas detalladas por historia; unidad mínima de trabajo e iteración)
  - `docs/sprint_bugs.md` (tracking de bugs reportados y pendientes de resolución)
  - `docs/sprint_fix.md` (historial de bugs resueltos durante el sprint)
- Al final de un sprint (release), el asistente archivará los ficheros de trabajo renombrándolos con la versión, por ejemplo:
  - `docs/sprint_v<version>.md`
  - `docs/sprint_histories_v<version>.md`
  - `docs/sprint_tasks_v<version>.md`
  - `docs/sprint_bugs_v<version>.md` (bugs que quedaron pendientes al cerrar sprint)
  - `docs/sprint_fix_v<version>.md` (bugs resueltos durante el sprint)
- A continuación el asistente creará ficheros nuevos y vacíos con los nombres activos para el siguiente sprint.
- Esta política de versionado garantiza trazabilidad de los sprints completados y mantiene los ficheros activos pequeños y enfocados.

Fichero Backlog (`docs/backlog.md`)
-----------------------------------
- Propósito: `docs/backlog.md` es el repositorio a largo plazo para los ítems que NO forman parte del sprint activo. Úsalo como "baúl" persistente para ideas, historias diferidas, tareas de baja prioridad y deuda técnica que deben conservarse para priorizar en el futuro.
- Flujo y uso:
  - Cuando se identifique una historia o tarea pero no se seleccione para el sprint actual, añade una entrada breve en `docs/backlog.md` con una descripción corta, etiqueta de prioridad (Baja/Media/Alta) y, opcionalmente, referencia al ID de historia/tarea en `docs/sprint_histories.md` o `docs/sprint_tasks.md`.
  - El backlog es la fuente para la planificación del sprint: durante la planificación los ítems pueden moverse de `docs/backlog.md` a `docs/sprint_histories.md` (como historia) y descomponerse en tareas en `docs/sprint_tasks.md` para el sprint activo.
  - Las entradas en `docs/backlog.md` deben ser concisas y enlazables (incluir una ruta o ancla al código relacionado si procede).
  - El asistente no implementará ítems directamente desde el backlog a menos que se muevan a los ficheros activos del sprint.

Fichero Sprint Bugs (`docs/sprint_bugs.md`)
----------------------------------------------
- Propósito: `docs/sprint_bugs.md` es el **contenedor inicial** de bugs reportados durante el sprint activo. Permite mantener trazabilidad de defectos encontrados y su estado actual.
- Contenido: Cada entrada de bug debe incluir:
  - **ID**: Identificador único del bug con prefijo BUG- (ej: BUG-001, BUG-002)
  - **Título**: Descripción breve del error
  - **Descripción**: Detalle del error, pasos para reproducir, comportamiento esperado vs observado
  - **Prioridad**: Crítica/Alta/Media/Baja
  - **Estado**: Reportado/En progreso
  - **Fecha de entrada**: Fecha en que se reportó el bug
  - **Archivos afectados**: Lista de archivos relacionados con el bug
- Formato ejemplo:
  ```markdown
  ### BUG-001 - Crash al renderizar quad sin shader
  **ID**: BUG-001
  **Prioridad**: Crítica
  **Estado**: Reportado
  **Fecha de entrada**: 2025-01-15
  
  **Descripción**: La aplicación crashea al intentar renderizar el quad si no se compila correctamente el shader HLSL.
  
  **Archivos afectados**: `src/renderer/DX12Renderer.cpp`, `shaders/quad.hlsl`
  ```
- Flujo de trabajo:
  - Cuando el usuario reporte un bug durante el sprint, el asistente añadirá una entrada en `docs/sprint_bugs.md` con ID **BUG-XXX** y estado "Reportado"
  - Al comenzar a trabajar en el bug, actualizar estado a "En progreso"
  - Al resolver el bug (commit exitoso), el asistente **moverá automáticamente** la entrada a `docs/sprint_fix.md` con:
    * ID cambiado a **FIX-XXX**
    * Estado "Resuelto"
    * Fecha de resolución
    * Hash del commit de resolución
    * Descripción de la solución implementada
  - El bug se elimina de `docs/sprint_bugs.md` tras moverlo a `docs/sprint_fix.md`
  - Los bugs pendientes se archivan como `docs/sprint_bugs_v<version>.md` al finalizar el sprint
- El asistente NO implementará bugs directamente a menos que se indique explícitamente; primero los registrará en `docs/sprint_bugs.md` para priorización.

Fichero Sprint Fix (`docs/sprint_fix.md`)
------------------------------------------
- Propósito: `docs/sprint_fix.md` es el **historial de bugs resueltos** durante el sprint activo. Contiene bugs que fueron reportados en `docs/sprint_bugs.md` y posteriormente solucionados.
- Contenido: Cada entrada de bug resuelto debe incluir:
  - **ID Original**: Identificador original del bug (BUG-XXX del sprint_bugs.md)
  - **ID Resolución**: Identificador con prefijo FIX- (ej: FIX-001, FIX-002)
  - **Título**: Descripción breve del error
  - **Descripción del problema**: Detalle original del error
  - **Solución implementada**: Descripción de cómo se resolvió el bug
  - **Prioridad**: Crítica/Alta/Media/Baja
  - **Fecha de entrada**: Fecha en que se reportó originalmente
  - **Fecha de resolución**: Fecha en que se resolvió
  - **Archivos afectados**: Lista de archivos relacionados con el bug y su resolución
  - **Commit de resolución**: Hash del commit que resolvió el bug
- Formato ejemplo:
  ```markdown
  ### FIX-001 - Crash al renderizar quad sin shader
  **ID Original**: BUG-001
  **Prioridad**: Crítica
  **Fecha de entrada**: 2025-01-15
  **Fecha de resolución**: 2025-01-15
  
  **Descripción del problema**: La aplicación crasheaba al intentar renderizar el quad si no se compilaba correctamente el shader HLSL.
  
  **Solución implementada**: Se añadió validación del shader compilado antes de crear el PSO y mensaje de error descriptivo.
  
  **Archivos afectados**: `src/renderer/DX12Renderer.cpp`, `shaders/quad.hlsl`
  
  **Commit de resolución**: abc123def
  ```
- Flujo de trabajo:
  - Los bugs resueltos se mueven automáticamente desde `docs/sprint_bugs.md`
  - El asistente actualiza el ID (BUG-XXX → FIX-XXX), añade fecha de resolución, commit hash y descripción de la solución
  - Los bugs resueltos se archivan como `docs/sprint_fix_v<version>.md` al finalizar el sprint

Sincronización con TEMPLATE.md (`docs/TEMPLATE.md`)
----------------------------------------------------
- Propósito: `docs/TEMPLATE.md` es el documento maestro que define la metodología genérica de trabajo con asistentes IA, aplicable a cualquier proyecto.
- Regla de sincronización: **Siempre que se modifique la metodología en `.github/copilot-instructions.md`** (añadir/eliminar secciones, cambiar flujo de trabajo, actualizar proceso de versionado, etc.), el asistente debe actualizar también `docs/TEMPLATE.md` para reflejar los cambios de forma genérica.
- Ejemplo: Si se añade una nueva sección como "Fichero Sprint Fix", debe añadirse también a `TEMPLATE.md` con placeholders genéricos `[PLACEHOLDER]` para que sea aplicable a otros proyectos.
- El asistente ejecutará esta sincronización automáticamente antes de crear commits que afecten a la metodología.
- Esta regla asegura que `TEMPLATE.md` esté siempre actualizado y pueda ser reutilizado en otros proyectos.


Formato de la explicación final de cada iteración:
- Requisito: Al final de cada iteración (cuando se informa lo realizado y el siguiente punto), la explicación debe contener obligatoriamente:
  
  1. **Dos títulos numerados** siguiendo el esquema del Sprint:
     - "Hecho: <número> <título>" (por ejemplo, "Hecho: 9.18 Shading/material...") que describe en breve lo completado.
     - "Siguiente: <número> <título>" que describe el siguiente punto propuesto.
  
  2. **Barra de progreso visual del sprint** mostrando el avance de tareas completadas:
     - Formato: Barra horizontal con fondo negro (█), progreso verde (🟩), bordes blancos (┃), y porcentaje centrado
     - Ejemplo con 2 de 15 tareas (13.3%):
       ```
       ┌────────────────────────────────────────────────────────────────────┐
       │🟩🟩⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛ 13.3%│
       └────────────────────────────────────────────────────────────────────┘
       ```
     - La barra debe ocupar todo el ancho disponible del prompt (aprox. 70-80 caracteres)
     - El porcentaje se calcula como: (tareas completadas / total tareas sprint) × 100
     - Usar emojis/caracteres Unicode: 🟩 (verde completado), ⬛ (negro pendiente)
     - Incluir el porcentaje en formato "XX.X%" al final de la barra
  
  Estos elementos deben estar presentes en la explicación final que acompaña al commit/documentación. El asistente rellenará los números y calculará el progreso guiándose por `docs/sprint.md`, `docs/sprint_tasks.md` y `docs/daily.md`.

Fichero Daily (`docs/daily.md`)
--------------------------------
- Propósito: `docs/daily.md` es el fichero simplificado de seguimiento diario.
- Contenido: El fichero solo contiene DOS cosas:
  1. **Última tarea realizada**: Número y descripción de la tarea completada (formato: "Hecho: <n> <descripción>")
  2. **Tarea actual**: Número y descripción de la siguiente tarea a realizar (formato: "Siguiente: <n> <descripción>")
- Formato ejemplo (con sprint activo):
  ```
  # Daily Log

  Hecho: 3.01 Plataforma - robustez en creación de ventana y prueba WM_PAINT
  Siguiente: 4.00 Backend de render inicial - DirectX12 minimal
  ```
- Formato ejemplo (sin sprint activo):
  ```
  # Daily Log

  Sprint v1.1.0 cerrado. Sin sprint activo.
  ```
- El asistente actualiza este fichero automáticamente tras cada commit exitoso.
- Al finalizar un sprint (release), el contenido de `daily.md` se actualiza a "Sprint vX.Y.Z cerrado. Sin sprint activo." y se archiva en los ficheros versionados del sprint. Se crea un nuevo `daily.md` para el siguiente sprint cuando este comienza.

Modificación de archivos `.vcxproj` (Visual Studio Project Files)
-------------------------------------------------------------------
- **Problema**: Los archivos `.vcxproj` están bloqueados por Visual Studio cuando la solución está abierta. Intentar modificarlos directamente con `replace_string_in_file` fallará con error "El documento ya está abierto como proyecto o solución".
- **Solución**: Usar comandos PowerShell con manipulación XML para modificar el `.vcxproj` sin necesidad de cerrarlo:

**Método 1: Añadir archivo fuente al proyecto (.cpp)**
```powershell
# Cargar XML del proyecto
[xml]$proj = Get-Content "Imagine Studio.vcxproj"

# Encontrar el ItemGroup que contiene archivos .cpp
$compileGroup = $proj.Project.ItemGroup | Where-Object { $_.ClCompile -ne $null } | Select-Object -First 1

# Crear nuevo elemento ClCompile
$newCompile = $proj.CreateElement("ClCompile", $proj.Project.NamespaceURI)
$newCompile.SetAttribute("Include", "external\imgui\imgui_demo.cpp")

# Añadir al grupo
$compileGroup.AppendChild($newCompile) | Out-Null

# Guardar cambios
$proj.Save("Imagine Studio.vcxproj")
```

**Método 2: Añadir archivo header al proyecto (.h)**
```powershell
[xml]$proj = Get-Content "Imagine Studio.vcxproj"
$includeGroup = $proj.Project.ItemGroup | Where-Object { $_.ClInclude -ne $null } | Select-Object -First 1
$newInclude = $proj.CreateElement("ClInclude", $proj.Project.NamespaceURI)
$newInclude.SetAttribute("Include", "src\editor\EditorUI.h")
$includeGroup.AppendChild($newInclude) | Out-Null
$proj.Save("Imagine Studio.vcxproj")
```

**Método 3: Modificar configuración del proyecto (ejemplo: cambiar standard C++)**
```powershell
[xml]$proj = Get-Content "Imagine Studio.vcxproj"
$propertyGroups = $proj.Project.PropertyGroup | Where-Object { $_.LanguageStandard -ne $null }
foreach ($pg in $propertyGroups) {
    $pg.LanguageStandard = "stdcpp14"
}
$proj.Save("Imagine Studio.vcxproj")
```

- **IMPORTANTE**: Después de modificar el `.vcxproj` con PowerShell, Visual Studio detectará el cambio y mostrará un diálogo de recarga. El usuario debe hacer clic en "Reload" para que los cambios se reflejen.
- **Alternativa CMake**: Si el proyecto usa CMake como sistema principal (como este proyecto), es preferible modificar `CMakeLists.txt` y regenerar el proyecto con `cmake -S . -B build` en lugar de modificar manualmente el `.vcxproj`. El `.vcxproj` se regenerará automáticamente desde CMake.
- **Regla del asistente**: Cuando se necesite añadir archivos al proyecto:
  1. **Primero** añadirlos a `CMakeLists.txt` (si existe)
  2. **Después** regenerar con `cmake -S . -B build`
  3. **Solo si CMake no se usa o hay conflictos**, modificar directamente el `.vcxproj` con PowerShell XML

Nota sobre estándar C++:
- Este repositorio usa C++14 como estándar de compilación en `CMakeLists.txt`. Asegúrate de que tu entorno local/CI tenga toolchains compatibles (MSVC/Clang/GCC) antes de compilar
