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
- `docs/sprint_bug_attempts.md` - **[DEBUGGING]** Log detallado de TODOS los intentos de solución para cada bug (fallidos, parciales, exitosos).
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
- Prefiero hacer bien las cosas desde el principio: evitar partes intermedas, temporales, incompletes o no funcionales. Cada módulo debe diseñarse para cumplir los estándares AAA desde su concepción.
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
  1. **NO** añadirs checks defensivos en la biblioteca
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
  - `docs/sprint_bug_attempts.md` (registro detallado de intentos de solución a bugs)
  - `docs/sprint_deviations.md` (ajustes arquitectónicos y desviaciones críticas)
  - `docs/sprint_ia_sessions.md` (registro de sesiones con asistentes IA)

- Al final de un sprint (release), el asistente archivará los ficheros de trabajo renombrándolos con la versión, por ejemplo:
  - `docs/sprint_v<version>.md`
  - `docs/sprint_histories_v<version>.md`
  - `docs/sprint_tasks_v<version>.md`
  - `docs/sprint_bugs_v<version>.md` (bugs que quedaron pendientes al cerrar sprint)
  - `docs/sprint_fix_v<version>.md` (bugs resueltos durante el sprint)
  - `docs/sprint_bug_attempts_v<version>.md` (intentos de solución a bugs)
  - `docs/sprint_deviations_v<version>.md` (ajustes y desviaciones registradas)
  - `docs/sprint_ia_sessions_v<version>.md` (sesiones con IA registradas)

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
  - **Estado**: Reportado/En progreso/Pendiente validación usuario
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
- **Flujo de trabajo con doble validación Agente-Usuario**:
  1. **Reporte**: Cuando el usuario reporte un bug, el asistente añadirá entrada en `docs/sprint_bugs.md` con ID **BUG-XXX** y estado "Reportado"
  2. **En progreso**: Al comenzar a trabajar en el bug, actualizar estado a "En progreso"
  3. **Implementación**: El asistente implementa el fix, compila (CMake + MSBuild limpias) y crea commit
  4. **Pendiente validación**: Actualizar estado a "Pendiente validación usuario" y **PAUSAR**
  5. **Validación usuario**: El usuario ejecuta la aplicación y verifica que el bug está realmente resuelto
  6. **Confirmación**: Si el usuario confirma que el fix funciona → mover a `docs/sprint_fix.md`
  7. **Rechazado**: Si el usuario reporta que el bug persiste → volver a estado "En progreso" y continuar debugging
- **CRÍTICO**: El asistente **NO debe mover automáticamente** bugs de `sprint_bugs.md` a `sprint_fix.md` solo porque la compilación sea limpia. La validación del usuario es **OBLIGATORIA** para confirmar que el fix es efectivo en ejecución real.
- **Excepciones**: Solo se permite mover automáticamente si:
  - El fix es trivial (typo, warning cosmético)
  - La prueba es determinista y verificable solo con compilación (ej: error de sintaxis)
  - El usuario da confirmación explícita sin necesidad de prueba
- Razón: **La doble compilación limpia NO garantiza que el bug esté resuelto en runtime**. Muchos bugs solo se manifiestan durante la ejecución (race conditions, comportamiento de UI, crashes bajo condiciones específicas, etc.).
- Los bugs pendientes se archivan como `docs/sprint_bugs_v<version>.md` al finalizar el sprint

Fichero Sprint Bug Attempts (`docs/sprint_bug_attempts.md`)
-------------------------------------------------------------
- **Propósito**: `docs/sprint_bug_attempts.md` es el registro detallado de TODOS los intentos de solución para cada bug reportado durante el sprint activo. Permite realizar un seguimiento exhaustivo de las acciones realizadas, incluyendo enfoques fallidos, parciales y exitosos.
- **Contenido**: Cada intento de solución debe incluir:
  - **ID de Bug**: Identificador del bug asociado (ej: BUG-001)
  - **Intento #**: Número secuencial del intento de solución
  - **Descripción del intento**: Breve descripción de lo que se intentó
  - **Resultado**: Éxito/Parcial/Fallido
  - **Detalles del resultado**: Explicación breve del resultado; incluir mensajes de error si los hubiese
  - **Fecha y hora**: Timestamp del intento
  - **Archivos modificados**: Lista de archivos que fueron cambiados en el intento
- **Formato ejemplo**:
  ```markdown
  ### BUG-001 - Crash al renderizar quad sin shader
  **Intento #1**
  - **Descripción**: Ajustar configuración de shaders
  - **Resultado**: Fallido
  - **Detalles**: El cambio en la configuración del shader no tuvo efecto. El error persiste.
  - **Fecha y hora**: 2025-01-15 10:00
  - **Archivos modificados**: `shaders/quad.hlsl`
  
  **Intento #2**
  - **Descripción**: Añadir comprobación de null en el builder
  - **Resultado**: Parcial
  - **Detalles**: Se evita el crash, pero el quad sigue sin renderizarse correctamente. Se muestra artefacto gráfico.
  - **Fecha y hora**: 2025-01-15 10:15
  - **Archivos modificados**: `src/renderer/DX12Renderer.cpp`
  
  **Intento #3**
  - **Descripción**: Reinicializar el contexto de ImGui en cada render
  - **Resultado**: Éxito
  - **Detalles**: El problema se resolvió al asegurar que el contexto de ImGui se reinicializara correctamente. Se puede renderizar el quad sin crashes.
  - **Fecha y hora**: 2025-01-15 10:30
  - **Archivos modificados**: `src/renderer/DX12Renderer.cpp`, `shaders/quad.hlsl`
  ```

**Flujo completo de resolución de bugs (OBLIGATORIO)**:
Cuando el asistente trabaje en la resolución de un bug, DEBE seguir este proceso:

1. **Contexto inicial**: Antes de comenzar cualquier intento de solución, el asistente DEBE:
   - Leer `docs/sprint_bugs.md` para conocer el bug actual
   - Leer `docs/sprint_bug_attempts.md` para revisar intentos previos (si existen)
   - Leer `.github/copilot-instructions.md` para seguir las reglas del proyecto
   - Leer `docs/sprint.md` y `docs/daily.md` para entender el contexto del sprint

2. **Registro de intento**: ANTES de modificar código, el asistente DEBE:
   - Añadir una nueva entrada en `docs/sprint_bug_attempts.md` con:
     - Intento # (secuencial)
     - Descripción clara del enfoque que se va a probar
     - Fecha y hora actual
   - Esta entrada debe crearse ANTES de tocar código

3. **Implementación**: El asistente implementa el cambio propuesto

4. **Compilación**: El asistente ejecuta las DOS compilaciones obligatorias:
   - CMake Build (Debug)
   - MSBuild "Imagine Studio.sln" (Debug)

5. **Registro de resultado**: DESPUÉS de compilar, el asistente DEBE:
   - Actualizar la entrada en `docs/sprint_bug_attempts.md` con:
     - Resultado (Éxito/Parcial/Fallido)
     - Detalles del resultado (errores de compilación, comportamiento observado, etc.)
     - Archivos modificados en este intento

6. **Validación usuario**: Si compilación es limpia:
   - Actualizar estado en `docs/sprint_bugs.md` a "Pendiente validación usuario"
   - **PAUSAR** y esperar confirmación del usuario
   - El asistente NO debe marcar el bug como resuelto automáticamente

7. **Iteración**: Si el intento falla o es parcial:
   - Volver al paso 2 con un nuevo intento
   - **IMPORTANTE**: NO repetir intentos ya probados (consultar `sprint_bug_attempts.md`)

8. **Resolución confirmada**: Solo cuando el usuario confirme que el fix funciona:
   - Mover bug de `docs/sprint_bugs.md` a `docs/sprint_fix.md`
   - Copiar el resumen de intentos exitosos en `docs/sprint_fix.md`
   - Archivar `docs/sprint_bug_attempts.md` para ese bug (mantener historial)

**REGLA CRÍTICA**: El asistente NO debe:
- Modificar código sin antes registrar el intento en `sprint_bug_attempts.md`
- Marcar un bug como resuelto solo porque la compilación sea limpia
- Repetir intentos de solución ya probados y registrados en `sprint_bug_attempts.md`
- Olvidar actualizar el resultado del intento después de compilar

**Beneficios de este flujo**:
- Evita repetir soluciones fallidas
- Proporciona contexto histórico invaluable para futuros bugs similares
- Facilita la colaboración (otro desarrollador puede ver qué se ha intentado)
- Permite análisis post-mortem de bugs complejos
- Detecta patrones en errores recurrentes

---

**IMPORTANTE**: Siempre revisar `docs/sprint_bugs.md`, `docs/sprint_bug_attempts.md` y el código relacionado **ANTES** de comenzar a implementar cualquier solución para un bug. Esto asegura que se comprende completamente el problema y se evita repetir intentos fallidos.

**Ejemplo de flujo de trabajo con un bug**:
1. Se reporta un bug y se añade a `sprint_bugs.md` como BUG-003.
2. El asistente revisa `sprint_bugs.md` y ve que es un problema de renderizado en el `DX12Renderer`.
3. Se consulta el historial de intentos en `sprint_bug_attempts.md` y se ve que ya hubo 2 intentos fallidos relacionados.
4. Se registra un nuevo intento:
   - **Intento #3**: Probar reinstalar el contexto de `ImGui` y verificar todas las dependencias de `DX12Renderer`.
5. Se implementa y registra el intento en `sprint_bug_attempts.md`:
   ```markdown
   ### BUG-003 - Error de renderizado en DX12Renderer
   **Intento #3**
   - **Descripción**: Reinstalar contexto de ImGui y verificar dependencias de DX12Renderer
   - **Resultado**: En Progreso
   - **Detalles**: Se está implementando una solución más robusta para la inicialización de ImGui y DX12.
   - **Fecha y hora**: 2025-01-15 11:00
   ```
6. Se espera a que el asistente complete el intento y valide con el usuario.

**NOTA**: Este fichero se archiva como `docs/sprint_bug_attempts_v<version>.md` al final del sprint.

---

## Gestión de Sesiones IA (`sprint_ia_sessions.md`)

### Propósito:
`docs/sprint_ia_sessions.md` registra todas las sesiones de trabajo con asistentes IA (GitHub Copilot, ChatGPT, Claude) que superan el **85% de consumo de tokens**.

Este límite asegura:
- ✅ **Evitar pérdida de contexto** por agotamiento de tokens
- ✅ **Trazabilidad completa** de prompts usados en el desarrollo
- ✅ **Auditoría de sesiones** para análisis posterior

### Formato de Registro:

```markdown
## Sesión IA #001 - Sprint v1.3.0

**LLM**: GitHub Copilot Chat (GPT-4o)
**Fecha/Hora**: 2025-01-18 15:48:20 UTC
**Usuario (Orquestador)**: joaquinluct
**Consumo de Tokens**: 892,000 / 1,000,000 (89.2%)
**Estado**: ⚠️ LÍMITE 85% ALCANZADO - Sesión cerrada

### Prompt Ejecutado:

```
[Prompt completo usado en la sesión]
Ejemplo:
"Implementa el sistema Sprint Deviations siguiendo TEMPLATE.md.
Debe incluir docs/sprint_deviations.md con formato AAA..."
```

### Contexto de la Sesión:

- **Sprint activo**: v1.3.0 (ImGui Integration)
- **Tareas completadas en sesión**:
  - DEV-001: Refactorización AAA
  - Sistema Sprint Deviations implementado
  - Documentación actualizada (TEMPLATE.md, copilot-instructions.md)
  
- **Tareas pendientes al cierre**:
  - H4.1: Panel Hierarchy
  - H4.2-H4.5: Resto de editor panels

### Próxima Sesión (Recomendaciones):

- Continuar desde: `docs/daily.md` (H4.1 - Panel Hierarchy)
- Leer: `docs/sprint_deviations.md` para contexto de DEV-001
- Validar: Refactorización AAA funcionando correctamente
```

### Flujo de Trabajo Automático:

**El asistente DEBE seguir este proceso al alcanzar 85% de tokens**:

1. **Detectar umbral**: Cuando consumo > 85% del límite de sesión

2. **Completar tarea actual**: 
   - Terminar implementación en progreso
   - Compilar (CMake + MSBuild)
   - Crear commit si build limpio
   - Actualizar `docs/daily.md`

3. **Registrar sesión en `sprint_ia_sessions.md`**:
   - Generar ID secuencial (Sesión #XXX)
   - Incluir prompt completo
   - Nombre y versión del LLM
   - Timestamp + usuario orquestador
   - Consumo de tokens (actual/límite)
   - Contexto de sesión (sprint, tareas completadas, pendientes)
   - Recomendaciones para próxima sesión

4. **Notificar al usuario**:
   ```
   ⚠️ LÍMITE DE SESIÓN ALCANZADO (>85%)
   
   ✅ Tarea actual completada: [nombre tarea]
   ✅ Commit creado: [hash]
   ✅ Sesión registrada en docs/sprint_ia_sessions.md
   
   📊 Consumo: XXX,XXX / 1,000,000 tokens (XX.X%)
   
   🔄 RECOMENDACIÓN: Cerrar esta sesión y abrir nuevo prompt
   
   Próxima tarea sugerida: [leer docs/daily.md]
   ```

5. **PAUSAR** y esperar que el usuario cierre la sesión

### Plantilla para Nuevas Sesiones:

```markdown
## Sesión IA #XXX - Sprint vX.Y.Z

**LLM**: [Nombre del LLM] ([Versión])
**Fecha/Hora**: [YYYY-MM-DD HH:MM:SS UTC]
**Usuario (Orquestador)**: [username]
**Consumo de Tokens**: [actual] / [límite] ([porcentaje]%)
**Estado**: ⚠️ LÍMITE 85% ALCANZADO - Sesión cerrada

### Prompt Ejecutado:

```
[Prompt completo]
```

### Contexto de la Sesión:

- **Sprint activo**: vX.Y.Z ([nombre sprint])
- **Tareas completadas en sesión**:
  - [Lista de tareas]
  
- **Tareas pendientes al cierre**:
  - [Lista de tareas]

### Próxima Sesión (Recomendaciones):

- Continuar desde: [referencia a daily.md o tarea específica]
- Leer: [archivos de contexto necesarios]
- Validar: [aspectos a verificar]
```

### Versionado:

- `sprint_ia_sessions.md` se archiva como `docs/sprints/sprint_ia_sessions_vX.Y.Z.md` al finalizar sprint
- **Nuevo**: Los históricos de sprint se mueven a `docs/sprints/` para mejor organización

---

## Gestión de Desviaciones del Sprint (`sprint_deviations.md`)

### ¿Cuándo usar `sprint_deviations.md`?

Usa este archivo cuando durante la **ejecución** del sprint surja:

1. **Ajuste Arquitectónico**: Cambio en diseño que afecta tareas futuras del sprint
2. **Tarea Emergente Bloqueante**: Descubierta durante implementación, bloquea progreso
3. **Deuda Técnica Crítica**: No puede diferirse al siguiente sprint
4. **Refactorización AAA**: Código no cumple estándares (`docs/AAA_STANDARDS.md` o `docs/MAIN.md`)

### Criterios para NO usar backlog:

- ✅ **Bloqueante**: Impide completar tareas del sprint actual
- ✅ **Crítico para pilares**: Viola `docs/MAIN.md` o `docs/AAA_STANDARDS.md`
- ✅ **Impacto inmediato**: Afecta a tareas en progreso o próximas inmediatas

### Diferencia con otros archivos:

| Archivo | Propósito | Cuándo usar |
|---------|-----------|-------------|
| `sprint_tasks.md` | Tareas **planificadas** del sprint | Al inicio del sprint |
| `sprint_deviations.md` | Tareas **emergentes/ajustes** durante sprint | Durante ejecución |
| `backlog.md` | Ítems **fuera** del sprint actual | Planificación futura |
| `sprint_bugs.md` | Bugs **reportados** durante sprint | Al detectar defecto |

### Flujo de Trabajo:

1. **Detectar desviación**: Durante implementación de HX.Y
2. **Evaluar criticidad**: ¿Bloquea sprint? ¿Viola pilares?
3. **Decidir**: 
   - Si **CRÍTICA** → `sprint_deviations.md` + pausar tarea actual
   - Si **NO CRÍTICA** → `backlog.md` + continuar sprint
4. **Documentar**: Crear entrada DEV-XXX en `sprint_deviations.md`
5. **Implementar**: Resolver desviación antes de continuar sprint
6. **Actualizar daily.md**: Reflejar trabajo realizado

### Formato de ID:

- **DEV-XXX**: Desviación general
- **DEV-XXX.Y**: Subtarea de desviación

### Ejemplo Real (DEV-001 - Refactorización AAA Sprint v1.3.0):

```markdown
## DEV-001: Refactorización AAA - Condicional de visibilidad UI

**Tipo**: Ajuste Arquitectónico
**Detectado en**: H2.3 (integración DX12 backend)
**Fecha**: 2025-01-18
**Prioridad**: CRÍTICA

### Contexto:
ImGui se procesaba SIEMPRE sin respetar m_uiVisible, violando estándares AAA.

### ¿Por qué NO backlog?
- Bloqueante para H4 (Editor Panels)
- Viola pilares AAA (docs/MAIN.md - "Hacer bien desde el principio")
- Impacto inmediato en arquitectura de 3 capas

### Decisión:
Pausar H4, implementar arquitectura AAA inmediatamente

### Tareas Derivadas:
- DEV-001.1: ✅ Condicional IsUIVisible() en main.cpp
- DEV-001.2: ✅ DockSpaceOverViewport()
- DEV-001.3: ✅ #ifdef _DEBUG para ShowDemoWindow()
- DEV-001.4: ✅ Crear docs/AAA_STANDARDS.md

### Resultado:
✅ Completado (commit: 011270b)
- Compilación limpia (CMake + MSBuild)
- Validación usuario: OK 100%

### Impacto en Sprint:
- Progreso: 62.5% → 75% (+12.5%)
- H4.1-H4.5 pueden continuar sobre base sólida

### Lecciones Aprendidas:
- Auditoría post-tarea crítica obligatoria
- No esperar a H4 para detectar problemas arquitectónicos
```

### Contenido de `sprint_deviations.md`:

Cada entrada debe incluir:
- **ID**: DEV-XXX
- **Tipo**: Ajuste Arquitectónico | Tarea Emergente | Deuda Técnica | Bloqueador
- **Detectado en**: HX.Y - nombre de tarea
- **Fecha**: YYYY-MM-DD
- **Prioridad**: CRÍTICA | ALTA | MEDIA | BAJA
- **Contexto**: Descripción del problema
- **¿Por qué NO backlog?**: Justificación de criticidad
- **Decisión**: Acción tomada
- **Tareas Derivadas**: DEV-XXX.1, DEV-XXX.2, etc.
- **Resultado**: Estado actual y validación
- **Impacto en Sprint**: Tareas afectadas y progreso
- **Lecciones Aprendidas**: Reflexión para futuros sprints

### Versionado:

- `sprint_deviations.md` se archiva como `docs/sprints/sprint_deviations_vX.Y.Z.md` al finalizar sprint
- Permite trazabilidad de decisiones arquitectónicas en sprints pasados
- Nuevo fichero vacío para próximo sprint

### Plantilla:

```markdown
## DEV-XXX: [Título de la desviación]

**Tipo**: [Tipo]
**Detectado en**: [HX.Y]
**Fecha**: [YYYY-MM-DD]
**Prioridad**: [PRIORIDAD]

### Contexto:
[Descripción del problema]

### ¿Por qué NO backlog?
- Razón 1
- Razón 2

### Decisión:
[Acción tomada]

### Tareas Derivadas:
- DEV-XXX.1: [Subtarea]

### Resultado:
[Estado y validación]

### Impacto en Sprint:
[Tareas afectadas]

### Lecciones Aprendidas:
[Reflexión]
```

---

## Versionado de Archivos del Sprint (ACTUALIZADO)

Los ficheros activos del sprint actual son:
- `docs/sprint.md`
- `docs/sprint_histories.md`
- `docs/sprint_tasks.md`
- `docs/sprint_bugs.md`
- `docs/sprint_bug_attempts.md`
- `docs/sprint_fix.md`
- `docs/sprint_deviations.md`
- **`docs/sprint_ia_sessions.md`** ← **NUEVO**

Al final de un sprint (release), el asistente archivará **TODOS** los ficheros a `docs/sprints/`:

```powershell
# Crear carpeta si no existe
New-Item -ItemType Directory -Force -Path "docs/sprints"

# Mover archivos versionados
Move-Item "docs/sprint_v<version>.md" "docs/sprints/"
Move-Item "docs/sprint_histories_v<version>.md" "docs/sprints/"
Move-Item "docs/sprint_tasks_v<version>.md" "docs/sprints/"
Move-Item "docs/sprint_bugs_v<version>.md" "docs/sprints/"
Move-Item "docs/sprint_bug_attempts_v<version>.md" "docs/sprints/"
Move-Item "docs/sprint_fix_v<version>.md" "docs/sprints/"
Move-Item "docs/sprint_deviations_v<version>.md" "docs/sprints/"
Move-Item "docs/sprint_ia_sessions_v<version>.md" "docs/sprints/"
```

**Estructura final de `docs/sprints/`**:

```
docs/
├── sprints/
│   ├── sprint_v1.0.0.md
│   ├── sprint_histories_v1.0.0.md
│   ├── sprint_tasks_v1.0.0.md
│   ├── sprint_bugs_v1.0.0.md
│   ├── sprint_bug_attempts_v1.0.0.md
│   ├── sprint_fix_v1.0.0.md
│   ├── sprint_deviations_v1.0.0.md (si aplica)
│   ├── sprint_ia_sessions_v1.0.0.md (si aplica)
│   ├── sprint_v1.1.0.md
│   ├── ...
│   └── sprint_ia_sessions_v1.3.0.md
```

**Regla**: Los históricos de sprints se mantienen en `docs/sprints/` para mejor organización y facilitar búsqueda de decisiones pasadas.
