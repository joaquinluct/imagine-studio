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
- `docs/daily.md` - �ltima tarea completada y tarea actual en progreso.
- `docs/commits.md` - Convenciones de commits y mensajes.

Flujo de trabajo por sesi�n:
1) Leer estas instrucciones.
2) Consultar `docs/daily.md` para ver la �ltima tarea completada y la tarea actual.
3) Comparar con el c�digo fuente.
4) Proponer el siguiente paso (por parte del asistente).
5) Si se confirma, el asistente implementa el paso.
   - Antes de crear el commit se debe compilar la soluci�n/proyecto.
     - Requisito adicional: realizar dos compilaciones y verificar que ambas quedan limpias (0 errores, 0 warnings) antes de proceder al commit:
       1) Compilaci�n 1 (CMake): ejecutar la compilaci�n desde la l�nea de comandos usando CMake para la configuraci�n Debug. Ejemplo:
          `cmake --build build --config Debug`
       2) Compilaci�n 2 (Visual Studio � "Build Solution"): reproducir la misma acci�n que hace Visual Studio cuando pulsas "Compilar soluci�n" (Ctrl+May�s+B). El asistente ejecutar� msbuild apuntando a la soluci�n con el target `Build` y la configuraci�n Debug. Ejemplo:
          `msbuild "build/ImagineStudio.sln" /t:Build /p:Configuration=Debug /m`
       Estos dos pasos son los requeridos antes de crear un commit.
     - Si alguna de las dos compilaciones produce errores o warnings, el asistente corregir� los errores cuando sea posible y volver� a ejecutar ambas compilaciones hasta obtener builds limpias.
     - Nota: el paso 2 (Build Solution via msbuild) es obligatorio y se ejecutar� sin pedir confirmaci�n. El asistente registrar� la salida y a�adir� la informaci�n relevante en `docs/roadmap_log.md`.
     - A partir de ahora, el asistente ejecutar� autom�ticamente ambas compilaciones (CMake Debug y Visual Studio Build Solution via msbuild) antes de crear commits durante sus iteraciones.
   - Una vez ambas compilaciones est�n limpias, crear el commit y actualizar los archivos Markdown pertinentes (`docs/roadmap_log.md`, `docs/commits.md`, `README.md` cuando proceda).
6) ReAl comenzar una sesi�n, abrir y revisar:
- `docs/roadmap.md`
- `docs/daily.md`
/roadmap_log.md`
- `docs/commits.md`

Nota: Los archivos anteriores son la fuente de la verdad para la organizaci�n del proyecto y la comunicaci�n con el asistente.

Preferencias del propietario del repositorio:
- Prefiero hacer bien las cosas desde el principio: evitar partes intermedias, temporales, incompletas o no funcionales. Cada m�dulo debe dise�arse para cumplir los est�ndares AAA desde su concepci�n.
- En cada iteraci�n, antes del commit, siempre se debe compilar y corregir errores/warnings para asegurar commits limpios y funcionales.
 - Estilo de includes: las directivas `#include` deben ordenarse siempre con las cabeceras del proyecto (entre comillas `"..."`) primero y despu�s las cabeceras del sistema (`<...>`), y dentro de cada grupo deben aparecer en orden alfab�tico. Esta regla ser� verificada por el asistente y aplicada o reportada como warning al preparar commits.

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

Iteraci�n controlada (proceso obligatorio por iteraci�n)
1) El asistente consultar� `docs/daily.md` y propondr� el siguiente punto a ejecutar, acompa�ado de una breve explicaci�n (una frase) sobre en qu� consiste.

Ejecuci�n autom�tica (sin necesidad de confirmaci�n previa):
2) Tras proponer el punto, el asistente implementar� el cambio, compilar� la soluci�n y corregir� los errores/warnings hasta lograr una compilaci�n limpia.
3) Si la compilaci�n queda limpia, el asistente realizar� directamente el commit local siguiendo `docs/commits.md` y actualizar� los ficheros necesarios (`docs/daily.md`, `docs/commits.md`, `README.md` y `.github/copilot-instructions.md`) sin pedir permiso adicional.
4) El asistente informar� de lo realizado y del siguiente punto propuesto y continuar� el ciclo.

Excepciones: Si aparece un bloqueo t�cnico, decisi�n arquitect�nica cr�tica o dependencia que impida avanzar, el asistente pausar� y solicitar� instrucciones al propietario.

Regla estricta sobre commits y documentaci�n:
- Requisito: Siempre que el asistente realice un commit local como resultado de una iteraci�n (es decir, la compilaci�n queda limpia), actualizar� autom�ticamente `docs/daily.md` y `docs/commits.md` para reflejar el cambio sin pedir confirmaci�n adicional. El push al repositorio remoto no se realizar� autom�ticamente salvo instrucci�n expl�cita del propietario.

Versionado de los ficheros del Sprint (snapshots de sprint):
- Los ficheros activos del sprint actual son:
  - `docs/sprint.md` (sprint de alto nivel con hitos y objetivos)
  - `docs/sprint_histories.md` (historias de usuario para el sprint)
  - `docs/sprint_tasks.md` (tareas detalladas por historia; unidad m�nima de trabajo e iteraci�n)
- Al final de un sprint (release), el asistente archivar� los ficheros de trabajo renombr�ndolos con la versi�n, por ejemplo:
  - `docs/sprint_v<version>.md`
  - `docs/sprint_histories_v<version>.md`
  - `docs/sprint_tasks_v<version>.md`
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


Formato de la explicaci�n final de cada iteraci�n:
- Requisito: Al final de cada iteraci�n (cuando se informa lo realizado y el siguiente punto), la explicaci�n debe contener obligatoriamente dos t�tulos numerados siguiendo el esquema del Sprint:
  - "Hecho: <n�mero> <t�tulo>" (por ejemplo, "Hecho: 9.18 Shading/material...") que describe en breve lo completado.
  - "Siguiente: <n�mero> <t�tulo>" que describe el siguiente punto propuesto.
  Estos t�tulos deben estar presentes en la explicaci�n final que acompa�a al commit/documentaci�n. El asistente rellenar� los n�meros gui�ndose por `docs/sprint.md` y `docs/daily.md`.

Fichero Daily (`docs/daily.md`)
--------------------------------
- Prop�sito: `docs/daily.md` es el fichero simplificado de seguimiento diario.
- Contenido: El fichero solo contiene DOS cosas:
  1. **�ltima tarea realizada**: N�mero y descripci�n de la tarea completada (formato: "Hecho: <n> <descripci�n>")
  2. **Tarea actual**: N�mero y descripci�n de la siguiente tarea a realizar (formato: "Siguiente: <n> <descripci�n>")
- Formato ejemplo:
  ```
  # Daily Log

  Hecho: 3.01 Plataforma - robustez en creaci�n de ventana y prueba WM_PAINT
  Siguiente: 4.00 Backend de render inicial - DirectX12 minimal
  ```
- El asistente actualiza este fichero autom�ticamente tras cada commit exitoso.
- Al finalizar un sprint (release), el contenido de `daily.md` se archiva en los ficheros versionados del sprint y se crea un nuevo `daily.md` para el siguiente sprint.

Nota sobre est�ndar C++:
- Este repositorio usa C++14 como est�ndar de compilaci�n en `CMakeLists.txt`. Aseg�rate de que tu entorno local/CI tenga toolchains compatibles (MSVC/Clang/GCC) antes de compilar.
