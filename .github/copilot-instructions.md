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
- `docs/daily.md` - Última tarea completada y tarea actual en progreso.
- `docs/commits.md` - Convenciones de commits y mensajes.
- `docs/sprint_fix.md` - Tracking de bugs/errores reportados durante el sprint.

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
- Prefiero hacer bien las cosas desde el principio: evitar partes intermedias, temporales, incompletas o no funcionales. Cada módulo debe diseñarse para cumplir los estándares AAA desde su concepción.
- En cada iteración, antes del commit, siempre se debe compilar y corregir errores/warnings para asegurar commits limpios y funcionales.
 - Estilo de includes: las directivas `#include` deben ordenarse siempre con las cabeceras del proyecto (entre comillas `"..."`) primero y después las cabeceras del sistema (`<...>`), y dentro de cada grupo deben aparecer en orden alfabético. Esta regla será verificada por el asistente y aplicada o reportada como warning al preparar commits.

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
  - `docs/sprint_fix.md` (tracking de bugs/errores reportados durante el sprint)
- Al final de un sprint (release), el asistente archivará los ficheros de trabajo renombrándolos con la versión, por ejemplo:
  - `docs/sprint_v<version>.md`
  - `docs/sprint_histories_v<version>.md`
  - `docs/sprint_tasks_v<version>.md`
  - `docs/sprint_fix_v<version>.md`
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

Fichero Sprint Fix (`docs/sprint_fix.md`)
------------------------------------------
- Propósito: `docs/sprint_fix.md` es el fichero de tracking de bugs y errores reportados durante el sprint activo. Permite mantener trazabilidad de defectos encontrados, su estado y resolución.
- Contenido: Cada entrada de bug/error debe incluir:
  - **ID**: Identificador único del bug (ej: FIX-001, FIX-002)
  - **Título**: Descripción breve del error
  - **Descripción**: Detalle del error, pasos para reproducir, comportamiento esperado vs observado
  - **Prioridad**: Crítica/Alta/Media/Baja
  - **Estado**: Reportado/En progreso/Resuelto/Verificado
  - **Fecha de entrada**: Fecha en que se reportó el bug
  - **Fecha de resolución**: Fecha en que se resolvió (si aplica)
  - **Archivos afectados**: Lista de archivos relacionados con el bug
  - **Commit de resolución**: Hash del commit que resolvió el bug (si aplica)
- Formato ejemplo:
  ```markdown
  ### FIX-001 - Crash al renderizar quad sin shader
  **ID**: FIX-001
  **Prioridad**: Crítica
  **Estado**: Resuelto
  **Fecha de entrada**: 2025-01-15
  **Fecha de resolución**: 2025-01-15
  
  **Descripción**: La aplicación crashea al intentar renderizar el quad si no se compila correctamente el shader HLSL.
  
  **Archivos afectados**: `src/renderer/DX12Renderer.cpp`, `shaders/quad.hlsl`
  
  **Commit de resolución**: abc123def
  ```
- Flujo de trabajo:
  - Cuando el usuario reporte un bug durante el sprint, el asistente añadirá una entrada en `docs/sprint_fix.md` con estado "Reportado"
  - Al comenzar a trabajar en el bug, actualizar estado a "En progreso"
  - Al resolver el bug (commit exitoso), actualizar estado a "Resuelto" y añadir fecha de resolución y hash del commit
  - Los bugs se archivan junto con los demás ficheros del sprint al finalizar (como `docs/sprint_fix_v<version>.md`)
- El asistente NO implementará bugs directamente a menos que se indique explícitamente; primero los registrará en `docs/sprint_fix.md` para priorización.

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
- Formato ejemplo:
  ```
  # Daily Log

  Hecho: 3.01 Plataforma - robustez en creación de ventana y prueba WM_PAINT
  Siguiente: 4.00 Backend de render inicial - DirectX12 minimal
  ```
- El asistente actualiza este fichero automáticamente tras cada commit exitoso.
- Al finalizar un sprint (release), el contenido de `daily.md` se archiva en los ficheros versionados del sprint y se crea un nuevo `daily.md` para el siguiente sprint.

Nota sobre estándar C++:
- Este repositorio usa C++14 como estándar de compilación en `CMakeLists.txt`. Asegúrate de que tu entorno local/CI tenga toolchains compatibles (MSVC/Clang/GCC) antes de compilar.
