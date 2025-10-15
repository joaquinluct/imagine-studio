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
1) El asistente consultar� `docs/roadmap_log.md` y propondr� el siguiente punto a ejecutar, acompa�ado de una breve explicaci�n (una frase) sobre en qu� consiste.

Ejecuci�n autom�tica (sin necesidad de confirmaci�n previa):
2) Tras proponer el punto, el asistente implementar� el cambio, compilar� la soluci�n y corregir� los errores/warnings hasta lograr una compilaci�n limpia.
3) Si la compilaci�n queda limpia, el asistente realizar� directamente el commit local siguiendo `docs/commits.md` y actualizar� los ficheros necesarios (`docs/roadmap_log.md`, `docs/commits.md`, `README.md` y `.github/copilot-instructions.md`) sin pedir permiso adicional.
4) El asistente informar� de lo realizado y del siguiente punto propuesto y continuar� el ciclo.

Excepciones: Si aparece un bloqueo t�cnico, decisi�n arquitect�nica cr�tica o dependencia que impida avanzar, el asistente pausar� y solicitar� instrucciones al propietario.

Regla estricta sobre commits y documentaci�n:
- Requisito: Siempre que el asistente realice un commit local como resultado de una iteraci�n (es decir, la compilaci�n queda limpia), actualizar� autom�ticamente `docs/roadmap_log.md` y `docs/commits.md` para reflejar el cambio sin pedir confirmaci�n adicional. El push al repositorio remoto no se realizar� autom�ticamente salvo instrucci�n expl�cita del propietario.

Formato de la explicaci�n final de cada iteraci�n:
- Requisito: Al final de cada iteraci�n (cuando se informa lo realizado y el siguiente punto), la explicaci�n debe contener obligatoriamente dos t�tulos numerados siguiendo el esquema del Roadmap:
  - "Hecho: <n�mero> <t�tulo>" (por ejemplo, "Hecho: 9.18 Shading/material...") que describe en breve lo completado.
  - "Siguiente: <n�mero> <t�tulo>" que describe el siguiente punto propuesto.
  Estos t�tulos deben estar presentes en la explicaci�n final que acompa�a al commit/documentaci�n. El asistente rellenar� los n�meros gui�ndose por `docs/roadmap.md` y `docs/roadmap_log.md`.

Nota sobre est�ndar C++:
- Este repositorio ahora requiere C++23 como est�ndar de compilaci�n en `CMakeLists.txt`. Aseg�rate de que tu entorno local/CI tenga toolchains compatibles (MSVC/Clang/GCC) antes de compilar.
