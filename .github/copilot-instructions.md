Este repositorio usa las siguientes instrucciones de trabajo para sesiones con el asistente (GitHub Copilot).

Archivos principales:
- `docs/roadmap.md` - Roadmap profesional con hitos y objetivos.
- `docs/roadmap_log.md` - Bit�cora del avance del roadmap (�ltimo punto realizado, punto actual).
- `docs/commits.md` - Convenciones de commits y mensajes.

Flujo de trabajo por sesi�n:
1) Leer estas instrucciones.
2) Consultar `docs/roadmap_log.md` para ver el punto actual.
3) Comparar con el c�digo fuente.
4) Proponer el siguiente paso (por parte del asistente).
5) Si se confirma, el asistente implementa el paso.
   - Antes de crear el commit se debe compilar la soluci�n/proyecto.
   - Si aparecen errores o warnings, corregirlos hasta obtener una compilaci�n limpia.
   - Una vez la compilaci�n sea limpia, crear el commit y actualizar los archivos Markdown pertinentes (`docs/roadmap_log.md`, `docs/commits.md`, `README.md` cuando proceda).
6) Repetir desde 1.

Al comenzar una sesi�n, abrir y revisar:
- `docs/roadmap.md`
- `docs/roadmap_log.md`
- `docs/commits.md`

Nota: Los archivos anteriores son la fuente de la verdad para la organizaci�n del proyecto y la comunicaci�n con el asistente.

Preferencias del propietario del repositorio:
- Prefiero hacer bien las cosas desde el principio: evitar partes intermedias, temporales, incompletas o no funcionales. Cada m�dulo debe dise�arse para cumplir los est�ndares AAA desde su concepci�n.
- En cada iteraci�n, antes del commit, siempre se debe compilar y corregir errores/warnings para asegurar commits limpios y funcionales.

Secci�n de resoluci�n de problemas (Troubleshooting)
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

Registrar en la bit�cora (`docs/roadmap_log.md`) cualquier incidencia relevante y su resoluci�n para futuras referencias.

Iteraci�n controlada (proceso obligatorio por iteraci�n)
1) Consultar `docs/roadmap_log.md` y proponer el siguiente punto (el asistente siempre presentar� exactamente un punto a ejecutar, acompa�ado de una breve explicaci�n �una frase� sobre en qu� consiste el paso).
2) El propietario responde `Si` para autorizar la ejecuci�n de la iteraci�n. Si la respuesta es `No` o hay dudas, la iteraci�n se detiene.
3) El asistente implementa el punto aprobado.
4) Compilar la soluci�n/proyecto. Corregir errores/warnings hasta obtener compilaci�n limpia.
5) Con compilaci�n limpia, el asistente crea el commit siguiendo `docs/commits.md`.
6) Actualizar `docs/commits.md` (listado de commits si procede), `README.md` y `.github/copilot-instructions.md` si es necesario.
7) Actualizar `docs/roadmap_log.md`: marcar el punto como completado y a�adir el nuevo punto por realizar.
8) El asistente informa brevemente lo realizado y el siguiente punto propuesto, y vuelve al paso 1 solicitando autorizaci�n (`Si/No`).

Nota: Si surgen problemas durante la iteraci�n (dependencias, decisiones arquitect�nicas, bloqueos), el asistente pausar� la ejecuci�n y preguntar� expl�citamente antes de seguir.
