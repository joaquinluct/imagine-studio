Este repositorio usa las siguientes instrucciones de trabajo para sesiones con el asistente (GitHub Copilot).

## ?? LEER PRIMERO

**OBLIGATORIO**: Antes de comenzar cualquier sesi�n, leer:  
?? **[`docs/MAIN.md`](../docs/MAIN.md)** - Dec�logo fundamental del proyecto (pilares, arquitectura, est�ndares AAA)

Este documento define los 4 pilares fundamentales y principios del proyecto Imagine Studio.  
**Todas las decisiones arquitect�nicas y de implementaci�n DEBEN alinearse con estos pilares.**

---

Archivos principales de trabajo:
- `docs/MAIN.md` - **[LEER PRIMERO]** Dec�logo y pilares fundamentales del proyecto.
- `docs/sprint.md` - Sprint actual con hitos y objetivos.
- `docs/daily.md` - �ltima tarea completada y tarea actual en progreso (o "Sprint vX.Y.Z cerrado. Sin sprint activo." si no hay sprint activo).
- `docs/sprint_commits.md` - Convenciones de commits y mensajes.
- `docs/sprint_bugs.md` - Tracking de bugs reportados (pendientes de resoluci�n).
- `docs/sprint_bug_attempts.md` - **[DEBUGGING]** Log detallado de TODOS los intentos de soluci�n para cada bug (fallidos, parciales, exitosos).
- `docs/sprint_fix.md` - Historial de bugs resueltos durante el sprint.

Flujo de trabajo por sesi�n:
1) Leer estas instrucciones.
2) Consultar `docs/daily.md` para ver la �ltima tarea completada y la tarea actual.
3) Comparar con el c�digo fuente.
4) Proponer el siguiente paso (por parte del asistente).
5) Si se confirma, el asistente implementa el paso.
   - Antes de crear el commit se debe compilar la soluci�n/proyecto.
     - **Requisito obligatorio: DOS compilaciones limpias (0 errores, 0 warnings)**:
       
       **Compilaci�n 1 - CMake Build (Debug)**:
       ```powershell
       cmake --build build --config Debug
       ```
       
       **Compilaci�n 2 - Visual Studio Build Solution (Ctrl+May�s+B equivalente)**:
       ```powershell
       msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /m
       ```
       
       **IMPORTANTE**: La compilaci�n 2 debe ejecutarse **EXACTAMENTE** con el comando `msbuild` mostrado arriba.
       Este comando replica **exactamente** lo que hace Visual Studio 2022 cuando pulsas **Ctrl+May�s+B** ("Build Solution") en la soluci�n principal `Imagine Studio.sln` de la ra�z.
       
       **NOTA**: Existen DOS sistemas de build en este proyecto:
       - **Sistema Principal (desarrollo)**: `Imagine Studio.sln` + `Imagine Studio.vcxproj` en la ra�z ? Usado para desarrollo diario con F5/Ctrl+May�s+B
       - **Sistema CMake (CI/automatizaci�n)**: `build/ImagineStudio.sln` generado por CMake ? Usado para builds automatizados
       
       Ambos sistemas deben compilar sin errores. La Compilaci�n 1 usa CMake, la Compilaci�n 2 usa el proyecto principal de Visual Studio.
       
       **NO usar** variaciones como:
       - ? `cmake --build` (eso es compilaci�n 1, no compilaci�n 2)
       - ? `msbuild "build/ImagineStudio.sln"` (esa es la soluci�n de CMake, no la principal)
       - ? `devenv /build`
       - ? Otros m�todos alternativos
       
       **S� usar**:
       - ? `msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /m`
       
     - Si alguna de las dos compilaciones produce errores o warnings, el asistente corregir� los errores cuando sea posible y volver� a ejecutar ambas compilaciones hasta obtener builds limpias.
     - El asistente ejecutar� autom�ticamente ambas compilaciones (CMake + MSBuild) antes de crear commits durante sus iteraciones.
   - Una vez ambas compilaciones est�n limpias, crear el commit y actualizar los archivos Markdown pertinentes (`docs/daily.md`, `docs/sprint_commits.md`, `README.md` cuando proceda).
6) Repetir desde 1.

Nota: Los archivos anteriores son la fuente de la verdad para la organizaci�n del proyecto y la comunicaci�n con el asistente.

Preferencias del propietario del repositorio:
- Prefiero hacer bien las cosas desde el principio: evitar partes intermedas, temporales, incompletes o no funcionales. Cada m�dulo debe dise�arse para cumplir los est�ndares AAA desde su concepci�n.
- En cada iteraci�n, antes del commit, siempre se debe compilar y corregir errores/warnings para asegurar commits limpios y funcionales.
- Estilo de includes: las directivas `#include` deben ordenarse siempre con las cabeceras del proyecto (entre comillas `"..."`) primero y despu�s las cabeceras del sistema (`<...>`), y dentro de cada grupo deben aparecer en orden alfab�tico. Esta regla ser� verificada por el asistente y aplicada o reportada como warning al preparar commits.
- **Codificaci�n de archivos**: TODOS los archivos de texto (`.md`, `.cpp`, `.h`, `.hlsl`, etc.) DEBEN usar **UTF-8 con BOM** y **line endings CRLF** (Windows). Esto es CR�TICO para evitar problemas de codificaci�n con caracteres especiales (emojis, caracteres no-ASCII). El asistente DEBE:
  - Al crear/editar archivos, asegurarse de que se guarden con UTF-8 con BOM
  - Verificar que no haya line endings mixtos (CRLF + LF)
  - Si detecta problemas de codificaci�n, ejecutar `.\scripts\check-encoding.ps1 -Fix` antes del commit
  - Nunca usar UTF-8 sin BOM para archivos con caracteres no-ASCII
  - Los warnings de Git "LF will be replaced by CRLF" son normales y esperados (Git normaliza autom�ticamente)

Secci�n de resoluci�n de problemas (Resoluci�n de problemas)
Si en alg�n momento la herramienta no puede compilar la soluci�n o no encuentra la `.sln`, o aparece un error de enlace tipo "unresolved external main", seguir estos pasos:

1) Comprobar la existencia y ruta exacta de la soluci�n desde PowerShell:
   - Ejecutar `Get-ChildItem -Force` o `ls` en el directorio ra�z del repositorio para localizar `*.sln` y obtener la ruta exacta.
2) Compilar expl�citamente usando PowerShell y `msbuild` con la ruta entre comillas (importante si el nombre contiene espacios):
   - `msbuild "C:\ruta\a\Imagine Studio.sln" /p:Configuration=Debug /m`
3) Si la herramienta reporta que no puede encontrar archivos fuente nuevos, confirmar que los ficheros est�n incluidos en el `.vcxproj` o en el `CMakeLists.txt` de la soluci�n. A�adirlos si es necesario.
4) Si aparece `LNK2019: unresolved external main` o similar:
   - Causa frecuente: discrepancia entre la entrada esperada por el subsistema (CONSOLE vs WINDOWS) y las funciones `main`/`WinMain` definidas en el c�digo.
   - Soluci�n: proporcionar entradas portables (por ejemplo, una funci�n `RunApp(HINSTANCE)` y definir tanto `int main()` como `WinMain` que llamen a `RunApp`), o ajustar la configuraci�n del proyecto al subsistema correcto y tener la firma adecuada.
5) Si la compilaci�n muestra errores/warnings: corregirlos hasta obtener una build limpia antes de proceder al commit (esta regla ya est� en el flujo de trabajo).
6) Alternativa: si no hay `.sln` o se desea portabilidad, crear un `CMakeLists.txt` b�sico y usar `cmake` + `cmake --build`.

Registrar en la bit�cora (`docs/daily.md`) cualquier incidencia relevante y su resoluci�n para futuras referencias.

?? REGLA CR�TICA: Bibliotecas de terceros (external/)
--------------------------------------------------------
**NUNCA MODIFICAR** c�digo en el directorio `external/`. 

**Razones:**
1. Imposibilita actualizar la biblioteca en el futuro
2. Oculta bugs en nuestro c�digo (tapa s�ntomas, no arregla causas)
3. Rompe la reproducibilidad del proyecto
4. Dificulta el debugging y mantenimiento

**Pol�tica:**
- TODO el c�digo de bibliotecas externas (Dear ImGui, etc.) debe permanecer **INTACTO**
- Si necesitas personalizar comportamiento: crear **wrappers** en `src/` (ej: `src/editor/ImGuiWrapper.h`)
- Si encuentras un bug relacionado con una biblioteca externa:
  1. **NO** a�adirs checks defensivos en la biblioteca
  2. **S�** investigar por qu� nuestro c�digo est� llamando incorrectamente a la API
  3. **S�** corregir el orden de inicializaci�n o uso en nuestro c�digo
  4. **S�** consultar `docs/THIRD_PARTY.md` para ver versiones y pol�ticas espec�ficas

**Ejemplo de error com�n:**
```cpp
// ? NO HACER: A�adir if(builder == NULL) en imgui_draw.cpp para evitar crash
// ? CORRECTO: Arreglar nuestro c�digo para asegurar que builder est� inicializado ANTES de llamar

// src/main.cpp
void InitializeRendering()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    // ? CR�TICO: Build debe llamarse ANTES de usar el atlas
    io.Fonts->Build();
    
    // Ahora s� podemos usar backends
    ImGui_ImplWin32_Init(g_hwnd);
    ImGui_ImplDX12_Init(...);
}
```

**Consultar siempre:** `docs/THIRD_PARTY.md` para pol�ticas detalladas sobre cada biblioteca externa.

Iteraci�n controlada (proceso obligatorio por iteraci�n)
1) El asistente consultar� `docs/daily.md` y propondr� el siguiente punto a ejecutar, acompa�ado de una breve explicaci�n (una frase) sobre en qu� consiste.

Ejecuci�n autom�tica (sin necesidad de confirmaci�n previa):
2) Tras proponer el punto, el asistente implementar� el cambio, compilar� la soluci�n y corregir� los errores/warnings hasta lograr una compilaci�n limpia.
3) Si la compilaci�n queda limpia, el asistente realizar� directamente el commit local siguiendo `docs/sprint_commits.md` y actualizar� los ficheros necesarios (`docs/daily.md`, `docs/sprint_commits.md`, `README.md` y `.github/copilot-instructions.md`) sin pedir permiso adicional.
4) El asistente informar� de lo realizado siguiendo el **Formato Obligatorio de Explicaci�n Final** (ver secci�n m�s abajo) y del siguiente punto propuesto y continuar� el ciclo.

Excepciones: Si aparece un bloqueo t�cnico, decisi�n arquitect�nica cr�tica o dependencia que impida avanzar, el asistente pausar� y solicitar� instrucciones al propietario.

## ?? Formato Obligatorio de Explicaci�n Final (IMPORTANTE)

Al final de cada iteraci�n (cuando se informa lo realizado y el siguiente punto), la explicaci�n debe contener **obligatoriamente** estas **3 secciones en orden**:

### 1. **Dos t�tulos numerados** siguiendo el esquema del Sprint:
- **"? Hecho: \<n�mero\> \<t�tulo\>"** (ej: "? H1.3 - EntityManager implementado") que describe en breve lo completado
- **"?? Siguiente: \<n�mero\> \<t�tulo\>"** (ej: "?? H1.4 - Tests unitarios Entity System") que describe el siguiente punto propuesto

### 2. **?? Visualizaci�n** - �Qu� cambia visualmente al ejecutar? (OBLIGATORIO)

**?? ESTA SECCI�N ES OBLIGATORIA Y DEBE APARECER ANTES DE LA BARRA DE PROGRESO**

**Formato estricto**:
```markdown
### ?? Visualizaci�n:

**�Algo nuevo que cambie la vista tras ejecuci�n?**: **S�** ? / **NO** ?

[SI ES "S� ?"]
**Qu� deber�as ver al ejecutar** (F5 en Visual Studio):
1. ? [Cambio visual espec�fico 1]
2. ? [Cambio visual espec�fico 2]
3. ?? [Nuevo componente UI visible]

**Qu� ha cambiado** (si aplica):
- ?? [Componente modificado]
- ?? [Comportamiento actualizado]

**Ausente** (si aplica):
- ? [Elemento que ya no se muestra]
- ? [Feature deshabilitada temporalmente]

[SI ES "NO ?"]
**Raz�n**: [Explicaci�n breve de por qu� no hay cambios visuales]
Ejemplo: "Esta tarea implementa clases internas sin efecto en UI"
```

**Especificaciones**:
- **OBLIGATORIO**: La secci�n debe aparecer en TODAS las iteraciones (incluso si la respuesta es "NO ?")
- Indicar claramente con **S�** ? o **NO** ? si hay cambios visuales
- Si es **S�** ?: Listar QU� se deber�a ver al ejecutar (F5 en Visual Studio, ejecutable, etc.)
- Si es **NO** ?: Explicar brevemente POR QU� (ej: "implementaci�n interna", "tests unitarios", "refactorizaci�n sin UI")
- Incluir emojis para claridad:
  - ? = Nuevo/Visible
  - ?? = Modificado/Actualizado
  - ? = Ausente/Deshabilitado
  - ?? = Nuevo componente a�adido
- **POSICI�N**: Esta secci�n DEBE aparecer DESPU�S de "Hecho/Siguiente" y ANTES de la barra de progreso

**Ejemplos seg�n tipo de tarea**:

**Ejemplo 1 - Tarea CON cambios visuales** (ej: H4.1 - Panel Hierarchy):
```markdown
### ?? Visualizaci�n:

**�Algo nuevo que cambie la vista tras ejecuci�n?**: **S�** ?

**Qu� deber�as ver al ejecutar** (F5 en Visual Studio):
1. ? Panel "Hierarchy" visible en la parte izquierda de la ventana
2. ? �rbol expandible con objetos: Scene Root ? Camera, Lights, Geometry
3. ?? Posibilidad de hacer dock/undock del panel arrastr�ndolo

**Qu� ha cambiado**:
- ?? El layout del editor ahora incluye un panel funcional (no solo ventana vac�a)

**Ausente**:
- ? Demo Window de ImGui ya no aparece (reemplazado por Hierarchy)
```

**Ejemplo 2 - Tarea SIN cambios visuales** (ej: H1.3 - EntityManager):
```markdown
### ?? Visualizaci�n:

**�Algo nuevo que cambie la vista tras ejecuci�n?**: **NO** ?

**Raz�n**: Esta tarea implementa clases internas (EntityManager, Entity) que no tienen representaci�n visual directa. Los cambios visuales aparecer�n en H4 cuando el panel Hierarchy muestre entities reales en lugar de placeholder.
```

### 3. **Barra de progreso visual del sprint** mostrando el avance de tareas completadas:

**Formato**: Barra horizontal con fondo negro (?), progreso verde (??), bordes blancos (�), y porcentaje centrado

**Ejemplo** con 3 de 16 tareas (18.8%):
```
+--------------------------------------------------------------------+
�????????????????????????????????????????????????? 18.8%�
+--------------------------------------------------------------------+
```

**Especificaciones**:
- La barra debe ocupar todo el ancho disponible del prompt (aprox. 70-80 caracteres)
- El porcentaje se calcula como: `(tareas completadas / total tareas sprint) � 100`
- Usar emojis/caracteres Unicode:
  - ?? (verde) = tarea completada
  - ? (negro) = tarea pendiente
- Incluir el porcentaje en formato `"XX.X%"` al final de la barra

**Datos para el c�lculo**:
El asistente rellenar� los n�meros y calcular� el progreso gui�ndose por:
- `docs/sprint.md`
- `docs/sprint_tasks.md`
- `docs/daily.md`

Regla estricta sobre commits y documentaci�n:
- Requisito: Siempre que el asistente realice un commit local como resultado de una iteraci�n (es decir, la compilaci�n queda limpia), actualizar� autom�ticamente `docs/daily.md` y `docs/sprint_commits.md` para reflejar el cambio sin pedir confirmaci�n adicional. El push al repositorio remoto no se realizar� autom�ticamente salvo instrucci�n expl�cita del propietario.

Versionado de los ficheros del Sprint (snapshots de sprint):
- Los ficheros activos del sprint actual son:
  - `docs/sprint.md` (sprint de alto nivel con hitos y objetivos)
  - `docs/sprint_histories.md` (historias de usuario para el sprint)
  - `docs/sprint_tasks.md` (tareas detalladas por historia; unidad m�nima de trabajo e iteraci�n)
  - `docs/sprint_bugs.md` (tracking de bugs reportados y pendientes de resoluci�n)
  - `docs/sprint_fix.md` (historial de bugs resueltos durante el sprint)
  - `docs/sprint_bug_attempts.md` (registro detallado de intentos de soluci�n a bugs)
  - `docs/sprint_deviations.md` (ajustes arquitect�nicos y desviaciones cr�ticas)
  - `docs/sprint_ia_sessions.md` (registro de sesiones con asistentes IA)

- Al final de un sprint (release), el asistente archivar� los ficheros de trabajo renombr�ndolos con la versi�n, por ejemplo:
  - `docs/sprint_v<version>.md`
  - `docs/sprint_histories_v<version>.md`
  - `docs/sprint_tasks_v<version>.md`
  - `docs/sprint_bugs_v<version>.md` (bugs que quedaron pendientes al cerrar sprint)
  - `docs/sprint_fix_v<version>.md` (bugs resueltos durante el sprint)
  - `docs/sprint_bug_attempts_v<version>.md` (intentos de soluci�n a bugs)
  - `docs/sprint_deviations_v<version>.md` (ajustes y desviaciones registradas)
  - `docs/sprint_ia_sessions_v<version>.md` (sesiones con IA registradas)

- A continuaci�n el asistente crear� ficheros nuevos y vac�os con los nombres activos para el siguiente sprint.
- Esta pol�tica de versionado garantiza trazabilidad de los sprints completados y mantiene los ficheros activos peque�os y enfocados.

Fichero Backlog (`docs/backlog.md`)
-----------------------------------
- Prop�sito: `docs/backlog.md` es el repositorio a largo plazo para los �tems que NO forman parte del sprint activo. �salo como "ba�l" persistente para ideas, historias diferidas, tareas de baja prioridad y deuda t�cnica que deben conservarse para priorizar en el futuro.
- Flujo y uso:
  - Cuando se identifique una historia o tarea pero no se seleccione para el sprint actual, a�ade una entrada breve en `docs/backlog.md` con una descripci�n corta, etiqueta de prioridad (Baja/Media/Alta) y, opcionalmente, referencia al ID de historia/tarea en `docs/sprint_histories.md` o `docs/sprint_tasks.md`.
  - El backlog es la fuente para la planificaci�n del sprint: durante la planificaci�n los �tems pueden moverse de `docs/backlog.md` a `docs/sprint_histories.md` (como historia) y descomponerse en tareas en `docs/sprint_tasks.md` para el sprint activo.
  - Las entradas en `docs/backlog.md` deben ser concisas y enlazables (incluir una ruta o ancla al c�digo relacionado si procede).
  - El asistente no implementar� �tems directamente desde el backlog a menos que se muevan a los ficheros activos del sprint.

Fichero Sprint Bugs (`docs/sprint_bugs.md`)
----------------------------------------------
- Prop�sito: `docs/sprint_bugs.md` es el **contenedor inicial** de bugs reportados durante el sprint activo. Permite mantener trazabilidad de defectos encontrados y su estado actual.
- Contenido: Cada entrada de bug debe incluir:
  - **ID**: Identificador �nico del bug con prefijo BUG- (ej: BUG-001, BUG-002)
  - **T�tulo**: Descripci�n breve del error
  - **Descripci�n**: Detalle del error, pasos para reproducir, comportamiento esperado vs observado
  - **Prioridad**: Cr�tica/Alta/Media/Baja
  - **Estado**: Reportado/En progreso/Pendiente validaci�n usuario
  - **Fecha de entrada**: Fecha en que se report� el bug
  - **Archivos afectados**: Lista de archivos relacionados con el bug
- Formato ejemplo:
  ```markdown
  ### BUG-001 - Crash al renderizar quad sin shader
  **ID**: BUG-001
  **Prioridad**: Cr�tica
  **Estado**: Reportado
  **Fecha de entrada**: 2025-01-15
  
  **Descripci�n**: La aplicaci�n crashea al intentar renderizar el quad si no se compila correctamente el shader HLSL.
  
  **Archivos afectados**: `src/renderer/DX12Renderer.cpp`, `shaders/quad.hlsl`
  ```
- **Flujo de trabajo con doble validaci�n Agente-Usuario**:
  1. **Reporte**: Cuando el usuario reporte un bug, el asistente a�adir� entrada en `docs/sprint_bugs.md` con ID **BUG-XXX** y estado "Reportado"
  2. **En progreso**: Al comenzar a trabajar en el bug, actualizar estado a "En progreso"
  3. **Implementaci�n**: El asistente implementa el fix, compila (CMake + MSBuild limpias) y crea commit
  4. **Pendiente validaci�n**: Actualizar estado a "Pendiente validaci�n usuario" y **PAUSAR**
  5. **Validaci�n usuario**: El usuario ejecuta la aplicaci�n y verifica que el bug est� realmente resuelto
  6. **Confirmaci�n**: Si el usuario confirma que el fix funciona ? mover a `docs/sprint_fix.md`
  7. **Rechazado**: Si el usuario reporta que el bug persiste ? volver a estado "En progreso" y continuar debugging
- **CR�TICO**: El asistente **NO debe mover autom�ticamente** bugs de `sprint_bugs.md` a `sprint_fix.md` solo porque la compilaci�n sea limpia. La validaci�n del usuario es **OBLIGATORIA** para confirmar que el fix es efectivo en ejecuci�n real.
- **Excepciones**: Solo se permite mover autom�ticamente si:
  - El fix es trivial (typo, warning cosm�tico)
  - La prueba es determinista y verificable solo con compilaci�n (ej: error de sintaxis)
  - El usuario da confirmaci�n expl�cita sin necesidad de prueba
- Raz�n: **La doble compilaci�n limpia NO garantiza que el bug est� resuelto en runtime**. Muchos bugs solo se manifiestan durante la ejecuci�n (race conditions, comportamiento de UI, crashes bajo condiciones espec�ficas, etc.).
- Los bugs pendientes se archivan como `docs/sprint_bugs_v<version>.md` al finalizar el sprint

Fichero Sprint Bug Attempts (`docs/sprint_bug_attempts.md`)
-------------------------------------------------------------
- **Prop�sito**: `docs/sprint_bug_attempts.md` es el registro detallado de TODOS los intentos de soluci�n para cada bug reportado durante el sprint activo. Permite realizar un seguimiento exhaustivo de las acciones realizadas, incluyendo enfoques fallidos, parciales y exitosos.
- **Contenido**: Cada intento de soluci�n debe incluir:
  - **ID de Bug**: Identificador del bug asociado (ej: BUG-001)
  - **Intento #**: N�mero secuencial del intento de soluci�n
  - **Descripci�n del intento**: Breve descripci�n de lo que se intent�
  - **Resultado**: �xito/Parcial/Fallido
  - **Detalles del resultado**: Explicaci�n breve del resultado; incluir mensajes de error si los hubiese
  - **Fecha y hora**: Timestamp del intento
  - **Archivos modificados**: Lista de archivos que fueron cambiados en el intento
- **Formato ejemplo**:
  ```markdown
  ### BUG-001 - Crash al renderizar quad sin shader
  **Intento #1**
  - **Descripci�n**: Ajustar configuraci�n de shaders
  - **Resultado**: Fallido
  - **Detalles**: El cambio en la configuraci�n del shader no tuvo efecto. El error persiste.
  - **Fecha y hora**: 2025-01-15 10:00
  - **Archivos modificados**: `shaders/quad.hlsl`
  
  **Intento #2**
  - **Descripci�n**: A�adir comprobaci�n de null en el builder
  - **Resultado**: Parcial
  - **Detalles**: Se evita el crash, pero el quad sigue sin renderizarse correctamente. Se muestra artefacto gr�fico.
  - **Fecha y hora**: 2025-01-15 10:15
  - **Archivos modificados**: `src/renderer/DX12Renderer.cpp`
  
  **Intento #3**
  - **Descripci�n**: Reinicializar el contexto de ImGui en cada render
  - **Resultado**: �xito
  - **Detalles**: El problema se resolvi� al asegurar que el contexto de ImGui se reinicializara correctamente. Se puede renderizar el quad sin crashes.
  - **Fecha y hora**: 2025-01-15 10:30
  - **Archivos modificados**: `src/renderer/DX12Renderer.cpp`, `shaders/quad.hlsl`
  ```

**Flujo completo de resoluci�n de bugs (OBLIGATORIO)**:
Cuando el asistente trabaje en la resoluci�n de un bug, DEBE seguir este proceso:

1. **Contexto inicial**: Antes de comenzar cualquier intento de soluci�n, el asistente DEBE:
   - Leer `docs/sprint_bugs.md` para conocer el bug actual
   - Leer `docs/sprint_bug_attempts.md` para revisar intentos previos (si existen)
   - Leer `.github/copilot-instructions.md` para seguir las reglas del proyecto
   - Leer `docs/sprint.md` y `docs/daily.md` para entender el contexto del sprint

2. **Registro de intento**: ANTES de modificar c�digo, el asistente DEBE:
   - A�adir una nueva entrada en `docs/sprint_bug_attempts.md` con:
     - Intento # (secuencial)
     - Descripci�n clara del enfoque que se va a probar
     - Fecha y hora actual
   - Esta entrada debe crearse ANTES de tocar c�digo

3. **Implementaci�n**: El asistente implementa el cambio propuesto

4. **Compilaci�n**: El asistente ejecuta las DOS compilaciones obligatorias:
   - CMake Build (Debug)
   - MSBuild "Imagine Studio.sln" (Debug)

5. **Registro de resultado**: DESPU�S de compilar, el asistente DEBE:
   - Actualizar la entrada en `docs/sprint_bug_attempts.md` con:
     - Resultado (�xito/Parcial/Fallido)
     - Detalles del resultado (errores de compilaci�n, comportamiento observado, etc.)
     - Archivos modificados en este intento

6. **Validaci�n usuario**: Si compilaci�n es limpia:
   - Actualizar estado en `docs/sprint_bugs.md` a "Pendiente validaci�n usuario"
   - **PAUSAR** y esperar confirmaci�n del usuario
   - El asistente NO debe marcar el bug como resuelto autom�ticamente

7. **Iteraci�n**: Si el intento falla o es parcial:
   - Volver al paso 2 con un nuevo intento
   - **IMPORTANTE**: NO repetir intentos ya probados (consultar `sprint_bug_attempts.md`)

8. **Resoluci�n confirmada**: Solo cuando el usuario confirme que el fix funciona:
   - Mover bug de `docs/sprint_bugs.md` a `docs/sprint_fix.md`
   - Copiar el resumen de intentos exitosos en `docs/sprint_fix.md`
   - Archivar `docs/sprint_bug_attempts.md` para ese bug (mantener historial)

**REGLA CR�TICA**: El asistente NO debe:
- Modificar c�digo sin antes registrar el intento en `sprint_bug_attempts.md`
- Marcar un bug como resuelto solo porque la compilaci�n sea limpia
- Repetir intentos de soluci�n ya probados y registrados en `sprint_bug_attempts.md`
- Olvidar actualizar el resultado del intento despu�s de compilar

**Beneficios de este flujo**:
- Evita repetir soluciones fallidas
- Proporciona contexto hist�rico invaluable para futuros bugs similares
- Facilita la colaboraci�n (otro desarrollador puede ver qu� se ha intentado)
- Permite an�lisis post-mortem de bugs complejos
- Detecta patrones en errores recurrentes

---

**IMPORTANTE**: Siempre revisar `docs/sprint_bugs.md`, `docs/sprint_bug_attempts.md` y el c�digo relacionado **ANTES** de comenzar a implementar cualquier soluci�n para un bug. Esto asegura que se comprende completamente el problema y se evita repetir intentos fallidos.

**Ejemplo de flujo de trabajo con un bug**:
1. Se reporta un bug y se a�ade a `sprint_bugs.md` como BUG-003.
2. El asistente revisa `sprint_bugs.md` y ve que es un problema de renderizado en el `DX12Renderer`.
3. Se consulta el historial de intentos en `sprint_bug_attempts.md` y se ve que ya hubo 2 intentos fallidos relacionados.
4. Se registra un nuevo intento:
   - **Intento #3**: Probar reinstalar el contexto de `ImGui` y verificar todas las dependencias de `DX12Renderer`.
5. Se implementa y registra el intento en `sprint_bug_attempts.md`:
   ```markdown
   ### BUG-003 - Error de renderizado en DX12Renderer
   **Intento #3**
   - **Descripci�n**: Reinstalar contexto de ImGui y verificar dependencias de DX12Renderer
   - **Resultado**: En Progreso
   - **Detalles**: Se est� implementando una soluci�n m�s robusta para la inicializaci�n de ImGui y DX12.
   - **Fecha y hora**: 2025-01-15 11:00
   ```
6. Se espera a que el asistente complete el intento y valide con el usuario.

**NOTA**: Este fichero se archiva como `docs/sprint_bug_attempts_v<version>.md` al final del sprint.

---

## Gesti�n de Sesiones IA (`sprint_ia_sessions.md`)

### Prop�sito:
`docs/sprint_ia_sessions.md` registra todas las sesiones de trabajo con asistentes IA (GitHub Copilot, ChatGPT, Claude) que superan el **85% de consumo de la ventana de contexto de la sesi�n actual**.

**?? ACLARACI�N IMPORTANTE**: El umbral del 85% se refiere a la **ventana de contexto activa de la sesi�n de chat actual** (t�picamente ~128k-200k tokens), **NO** al l�mite total del modelo LLM (1M tokens).

**�Por qu� este umbral?**
- ?? **Evitar p�rdida de contexto**: Al alcanzar el 85% de la ventana de contexto, el LLM empieza a "olvidar" mensajes antiguos
- ?? **Prevenir estado 'busy'**: La sesi�n se vuelve lenta y menos efectiva
- ?? **Garantizar coherencia**: Evita contradicciones con decisiones tomadas al inicio de la sesi�n
- ? **Trazabilidad completa** de prompts usados en el desarrollo
- ? **Auditor�a de sesiones** para an�lisis posterior

**Beneficios de cerrar al 85%**:
- ?? Abrir nuevo chat con contexto fresco (100% de ventana disponible)
- ?? Leer recomendaciones de la sesi�n anterior
- ?? Continuar con claridad desde donde se dej�
- ? Evitar errores por contexto truncado

### Formato de Registro:

```markdown
## Sesi�n IA #001 - Sprint vX.Y.Z

**LLM**: [PLACEHOLDER: Nombre del LLM] ([PLACEHOLDER: Versi�n])
**Fecha/Hora**: [YYYY-MM-DD HH:MM:SS UTC]
**Usuario (Orquestador)**: [PLACEHOLDER: username]
**Ventana de Contexto**: [actual] / [l�mite_ventana] ([porcentaje]%)
**Estado**: ?? L�MITE 85% ALCANZADO - Sesi�n cerrada

### Resumen de la Sesi�n:

Esta sesi�n ha sido muy productiva:

1. ? **[PLACEHOLDER: Logro principal 1]**
   - [Sub-logro 1]
   - [Sub-logro 2]

2. ? **[PLACEHOLDER: Logro principal 2]**
   - [Sub-logro 1]
   - [Sub-logro 2]

3. ? **Validaci�n completa**
   - Compilaci�n limpia (CMake + MSBuild)
   - Validaci�n usuario: [resultado]

### Prompt Ejecutado:

```
[PLACEHOLDER: Prompt completo usado en la sesi�n]
Ejemplo:
"Implementa el sistema X siguiendo TEMPLATE.md.
Debe incluir docs/Y con formato AAA..."
```

### Contexto de la Sesi�n:

- **Sprint activo**: vX.Y.Z ([PLACEHOLDER: nombre sprint])
- **Tareas completadas en sesi�n**:
  - [PLACEHOLDER: Lista de tareas]
  
- **Tareas pendientes al cierre**:
  - [PLACEHOLDER: Lista de tareas]

### Pr�xima Sesi�n (Recomendaciones):

- Continuar desde: [PLACEHOLDER: referencia a daily.md o tarea espec�fica]
- Leer: [PLACEHOLDER: archivos de contexto necesarios]
- Validar: [PLACEHOLDER: aspectos a verificar]
```

### Flujo de Trabajo Autom�tico:

**El asistente DEBE seguir este proceso al alcanzar 85% de la ventana de contexto de la sesi�n actual**:

1. **Detectar umbral**: Cuando consumo de **ventana de contexto de la sesi�n** > 85%
   - **NOTA**: NO se refiere al l�mite total del LLM (1M tokens)
   - Se refiere a la ventana activa del chat actual (~128k-200k tokens t�picamente)

2. **Completar tarea actual**: 
   - Terminar implementaci�n en progreso
   - Compilar (CMake + MSBuild)
   - Crear commit si build limpio
   - Actualizar `docs/daily.md`

3. **Registrar sesi�n en `sprint_ia_sessions.md`**:
   - Generar ID secuencial (Sesi�n #XXX)
   - Incluir prompt completo
   - Nombre y versi�n del LLM
   - Timestamp + usuario orquestador
   - Consumo de ventana de contexto (actual/l�mite/porcentaje)
   - Contexto de sesi�n (sprint, tareas completadas, pendientes)
   - Recomendaciones para pr�xima sesi�n

4. **Notificar al usuario**:
   ```
   ?? L�MITE DE VENTANA DE CONTEXTO ALCANZADO (>85%)
   
   ? Tarea actual completada: [nombre tarea]
   ? Commit creado: [hash]
   ? Sesi�n registrada en docs/sprint_ia_sessions.md
   
   ?? Ventana de contexto: XXX,XXX / YYY,YYY tokens (ZZ.Z%)
   
   ?? RECOMENDACI�N: Cerrar esta sesi�n y abrir nuevo prompt
   
   **Raz�n**: Al superar el 85% de la ventana de contexto, el LLM
   empieza a perder informaci�n de mensajes antiguos, lo que puede
   causar inconsistencias o contradicciones.
   
   Pr�xima tarea sugerida: [leer docs/daily.md]
   ```

5. **PAUSAR** y esperar que el usuario cierre la sesi�n

### Plantilla para Nuevas Sesiones:

```markdown
## Sesi�n IA #XXX - Sprint vX.Y.Z

**LLM**: [Nombre del LLM] ([Versi�n])
**Fecha/Hora**: [YYYY-MM-DD HH:MM:SS UTC]
**Usuario (Orquestador)**: [username]
**Ventana de Contexto**: [actual] / [l�mite_ventana] ([porcentaje]%)
**Estado**: ?? L�MITE 85% ALCANZADO - Sesi�n cerrada

### Resumen de la Sesi�n:

Esta sesi�n ha sido muy productiva:

1. ? **[Logro principal 1]**
   - [Sub-logro 1]
   - [Sub-logro 2]

2. ? **[Logro principal 2]**
   - [Sub-logro 1]
   - [Sub-logro 2]

3. ? **Validaci�n completa**
   - Compilaci�n limpia (CMake + MSBuild)
   - Validaci�n usuario: [resultado]

### Prompt Ejecutado:

```
[Prompt completo]
```

### Contexto de la Sesi�n:

- **Sprint activo**: vX.Y.Z ([nombre sprint])
- **Tareas completadas en sesi�n**:
  - [Lista de tareas]
  
- **Tareas pendientes al cierre**:
  - [Lista de tareas]

### Pr�xima Sesi�n (Recomendaciones):

- Continuar desde: [referencia a daily.md o tarea espec�fica]
- Leer: [archivos de contexto necesarios]
- Validar: [aspectos a verificar]
```

### Versionado:

- `sprint_ia_sessions.md` se archiva como `docs/sprints/sprint_ia_sessions_vX.Y.Z.md` al finalizar sprint
- **Nuevo**: Los hist�ricos de sprint se mueven a `docs/sprints/` para mejor organizaci�n

---

## Gesti�n de Desviaciones del Sprint (`sprint_deviations.md`)

### �Cu�ndo usar `sprint_deviations.md`?

Usa este archivo cuando durante la **ejecuci�n** del sprint surja:

1. **Ajuste Arquitect�nico**: Cambio en dise�o que afecta tareas futuras del sprint
2. **Tarea Emergente Bloqueante**: Descubierta durante implementaci�n, bloquea progreso
3. **Deuda T�cnica Cr�tica**: No puede diferirse al siguiente sprint
4. **Refactorizaci�n AAA**: C�digo no cumple est�ndares (`docs/AAA_STANDARDS.md` o `docs/MAIN.md`)

### Criterios para NO usar backlog:

- ? **Bloqueante**: Impide completar tareas del sprint actual
- ? **Cr�tico para pilares**: Viola `docs/MAIN.md` o `docs/AAA_STANDARDS.md`
- ? **Impacto inmediato**: Afecta a tareas en progreso o pr�ximas inmediatas

### Diferencia con otros archivos:

| Archivo | Prop�sito | Cu�ndo usar |
|---------|-----------|-------------|
| `sprint_tasks.md` | Tareas **planificadas** del sprint | Al inicio del sprint |
| `sprint_deviations.md` | Tareas **emergentes/ajustes** durante sprint | Durante ejecuci�n |
| `backlog.md` | �tems **fuera** del sprint actual | Planificaci�n futura |
| `sprint_bugs.md` | Bugs **reportados** durante sprint | Al detectar defecto |

### Flujo de Trabajo:

1. **Detectar desviaci�n**: Durante implementaci�n de HX.Y
2. **Evaluar criticidad**: �Bloquea sprint? �Viola pilares?
3. **Decidir**: 
   - Si **CR�TICA** ? `sprint_deviations.md` + pausar tarea actual
   - Si **NO CR�TICA** ? `backlog.md` + continuar sprint
4. **Documentar**: Crear entrada DEV-XXX en `sprint_deviations.md`
5. **Implementar**: Resolver desviaci�n antes de continuar sprint
6. **Actualizar daily.md**: Reflejar trabajo realizado

### Formato de ID:

- **DEV-XXX**: Desviaci�n general
- **DEV-XXX.Y**: Subtarea de desviaci�n

### Ejemplo Real (DEV-001 - Refactorizaci�n AAA Sprint v1.3.0):

```markdown
## DEV-001: Refactorizaci�n AAA - Condicional de visibilidad UI

**Tipo**: Ajuste Arquitect�nico
**Detectado en**: H2.3 (integraci�n DX12 backend)
**Fecha**: 2025-01-18
**Prioridad**: CR�TICA

### Contexto:
ImGui se procesaba SIEMPRE sin respetar m_uiVisible, violando est�ndares AAA.

### �Por qu� NO backlog?
- Bloqueante para H4 (Editor Panels)
- Viola pilares AAA (docs/MAIN.md - "Hacer bien desde el principio")
- Impacto inmediato en arquitectura de 3 capas

### Decisi�n:
Pausar H4, implementar arquitectura AAA inmediatamente

### Tareas Derivadas:
- DEV-001.1: ? Condicional IsUIVisible() en main.cpp
- DEV-001.2: ? DockSpaceOverViewport()
- DEV-001.3: ? #ifdef _DEBUG para ShowDemoWindow()
- DEV-001.4: ? Crear docs/AAA_STANDARDS.md

### Resultado:
? Completado (commit: 011270b)
- Compilaci�n limpia (CMake + MSBuild)
- Validaci�n usuario: OK 100%

### Impacto en Sprint:
- Progreso: 62.5% ? 75% (+12.5%)
- H4.1-H4.5 pueden continuar sobre base s�lida

### Lecciones Aprendidas:
- Auditor�a post-tarea cr�tica obligatoria
- No esperar a H4 para detectar problemas arquitect�nicos
```

### Contenido de `sprint_deviations.md`:

Cada entrada debe incluir:
- **ID**: DEV-XXX
- **Tipo**: Ajuste Arquitect�nico | Tarea Emergente | Deuda T�cnica | Bloqueador
- **Detectado en**: HX.Y - nombre de tarea
- **Fecha**: YYYY-MM-DD
- **Prioridad**: CR�TICA | ALTA | MEDIA | BAJA
- **Contexto**: Descripci�n del problema
- **�Por qu� NO backlog?**: Justificaci�n de criticidad
- **Decisi�n**: Acci�n tomada
- **Tareas Derivadas**: DEV-XXX.1, DEV-XXX.2, etc.
- **Resultado**: Estado actual y validaci�n
- **Impacto en Sprint**: Tareas afectadas y progreso
- **Lecciones Aprendidas**: Reflexi�n para futuros sprints

### Versionado:

- `sprint_deviations.md` se archiva como `docs/sprints/sprint_deviations_vX.Y.Z.md` al finalizar sprint
- Permite trazabilidad de decisiones arquitect�nicas en sprints pasados
- Nuevo fichero vac�o para pr�ximo sprint

### Plantilla:

```markdown
## DEV-XXX: [T�tulo de la desviaci�n]

**Tipo**: [Tipo]
**Detectado en**: [HX.Y]
**Fecha**: [YYYY-MM-DD]
**Prioridad**: [PRIORIDAD]

### Contexto:
[Descripci�n del problema]

### �Por qu� NO backlog?
- Raz�n 1
- Raz�n 2

### Decisi�n:
[Acci�n tomada]

### Tareas Derivadas:
- DEV-XXX.1: [Subtarea]

### Resultado:
[Estado y validaci�n]

### Impacto en Sprint:
[Tareas afectadas]

### Lecciones Aprendidas:
[Reflexi�n]
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
- **`docs/sprint_ia_sessions.md`** ? **NUEVO**

Al final de un sprint (release), el asistente archivar� **TODOS** los ficheros a `docs/sprints/`:

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
+-- sprints/
�   +-- sprint_v1.0.0.md
�   +-- sprint_histories_v1.0.0.md
�   +-- sprint_tasks_v1.0.0.md
�   +-- sprint_bugs_v1.0.0.md
�   +-- sprint_bug_attempts_v1.0.0.md
�   +-- sprint_fix_v1.0.0.md
�   +-- sprint_deviations_v1.0.0.md (si aplica)
�   +-- sprint_ia_sessions_v1.0.0.md (si aplica)
�   +-- sprint_v1.1.0.md
�   +-- ...
�   +-- sprint_ia_sessions_v1.3.0.md
```

**Regla**: Los hist�ricos de sprints se mantienen en `docs/sprints/` para mejor organizaci�n y facilitar b�squeda de decisiones pasadas.

---

## ?? **OBLIGATORIO AL FINALIZAR SPRINT: Registrar Sesi�n IA**

**CR�TICO**: Al finalizar un sprint (ejecutar `close-sprint.ps1`), el asistente **DEBE**:

1. **Registrar la sesi�n IA** en `docs/sprint_ia_sessions.md` **ANTES** de archivar
2. **Incluir TODA la informaci�n** de la sesi�n:
   - Prompts ejecutados (lista completa)
   - Tareas completadas durante la sesi�n
   - Commits creados
   - Archivos creados/modificados
   - M�tricas finales del sprint
   - Duraci�n de la sesi�n
   - Consumo de tokens
3. **Archivar** el fichero con el script: `sprint_ia_sessions_v<version>.md`

**�Por qu� es OBLIGATORIO?**
- ? **Trazabilidad completa** del desarrollo asistido por IA
- ? **An�lisis retrospectivo** de metodolog�a y prompts efectivos
- ? **Documentaci�n valiosa** para futuros sprints
- ? **Auditor�a** de decisiones tomadas durante el desarrollo
- ? **Continuidad** entre sesiones y sprints

**Proceso al cerrar sprint**:
```powershell
# 1. Actualizar docs/sprint_ia_sessions.md con resumen de sesi�n
# 2. Ejecutar script de cierre (archiva autom�ticamente)
.\scripts\close-sprint.ps1 -Version "X.Y.Z"
# 3. Commit final de cierre
git add -A
git commit -m "chore: Cerrar Sprint vX.Y.Z - 100% completado"
```

**Plantilla m�nima obligatoria** (ver secci�n "Gesti�n de Sesiones IA" para template completo):
```markdown
## Sesi�n IA #001 - Sprint vX.Y.Z

**LLM**: GitHub Copilot (Claude 3.5 Sonnet)
**Fecha/Hora**: [YYYY-MM-DD HH:MM-HH:MM UTC]
**Usuario (Orquestador)**: [username]
**Consumo de Tokens**: [~XXX,000] / 1,000,000 ([XX.X]%
**Estado**: ?? SPRINT COMPLETADO

### Resumen de la Sesi�n:
[Lista de logros principales]

### Prompt Ejecutado:
[Lista de prompts usados]

### Tareas completadas en sesi�n:
[Lista de todas las tareas HX.Y]

### Commits Creados:
[Lista de commits con hash]

### M�tricas del Sprint (Final):
[Tareas, tests, performance, bugs, etc.]
```

**Excepciones**: Si el sprint NO tuvo sesiones IA (desarrollo manual), dejar `sprint_ia_sessions.md` vac�o con nota:
```markdown
# Sprint IA Sessions - vX.Y.Z

*No hubo sesiones IA durante este sprint. Desarrollo realizado manualmente.*
```

---
