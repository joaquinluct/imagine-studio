Este repositorio usa las siguientes instrucciones de trabajo para sesiones con el asistente (GitHub Copilot).

Archivos principales:
- `docs/roadmap.md` - Roadmap profesional con hitos y objetivos.
- `docs/roadmap_log.md` - Bitácora del avance del roadmap (último punto realizado, punto actual).
- `docs/commits.md` - Convenciones de commits y mensajes.

Flujo de trabajo por sesión:
1) Leer estas instrucciones.
2) Consultar `docs/roadmap_log.md` para ver el punto actual.
3) Comparar con el código fuente.
4) Proponer el siguiente paso (por parte del asistente).
5) Si se confirma, el asistente implementa el paso.
   - Antes de crear el commit se debe compilar la solución/proyecto.
   - Si aparecen errores o warnings, corregirlos hasta obtener una compilación limpia.
   - Una vez la compilación sea limpia, crear el commit y actualizar los archivos Markdown pertinentes (`docs/roadmap_log.md`, `docs/commits.md`, `README.md` cuando proceda).
6) Repetir desde 1.

Al comenzar una sesión, abrir y revisar:
- `docs/roadmap.md`
- `docs/roadmap_log.md`
- `docs/commits.md`

Nota: Los archivos anteriores son la fuente de la verdad para la organización del proyecto y la comunicación con el asistente.

Preferencias del propietario del repositorio:
- Prefiero hacer bien las cosas desde el principio: evitar partes intermedias, temporales, incompletas o no funcionales. Cada módulo debe diseñarse para cumplir los estándares AAA desde su concepción.
- En cada iteración, antes del commit, siempre se debe compilar y corregir errores/warnings para asegurar commits limpios y funcionales.

Sección de resolución de problemas (Troubleshooting)
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

Registrar en la bitácora (`docs/roadmap_log.md`) cualquier incidencia relevante y su resolución para futuras referencias.

Iteración controlada (proceso obligatorio por iteración)
1) El asistente consultará `docs/roadmap_log.md` y propondrá el siguiente punto a ejecutar, acompañado de una breve explicación (una frase) sobre en qué consiste.

Ejecución automática (sin necesidad de confirmación previa):
2) Tras proponer el punto, el asistente implementará el cambio, compilará la solución y corregirá los errores/warnings hasta lograr una compilación limpia.
3) Si la compilación queda limpia, el asistente realizará directamente el commit local siguiendo `docs/commits.md` y actualizará los ficheros necesarios (`docs/roadmap_log.md`, `docs/commits.md`, `README.md` y `.github/copilot-instructions.md`) sin pedir permiso adicional.
4) El asistente informará de lo realizado y del siguiente punto propuesto y continuará el ciclo.

Excepciones: Si aparece un bloqueo técnico, decisión arquitectónica crítica o dependencia que impida avanzar, el asistente pausará y solicitará instrucciones al propietario.

Regla estricta sobre commits y documentación:
- Requisito: Siempre que el asistente realice un commit local como resultado de una iteración (es decir, la compilación queda limpia), actualizará automáticamente `docs/roadmap_log.md` y `docs/commits.md` para reflejar el cambio sin pedir confirmación adicional. El push al repositorio remoto no se realizará automáticamente salvo instrucción explícita del propietario.

Formato de la explicación final de cada iteración:
- Requisito: Al final de cada iteración (cuando se informa lo realizado y el siguiente punto), la explicación debe contener obligatoriamente dos títulos numerados siguiendo el esquema del Roadmap:
  - "Hecho: <número> <título>" (por ejemplo, "Hecho: 9.18 Shading/material...") que describe en breve lo completado.
  - "Siguiente: <número> <título>" que describe el siguiente punto propuesto.
  Estos títulos deben estar presentes en la explicación final que acompaña al commit/documentación. El asistente rellenará los números guiándose por `docs/roadmap.md` y `docs/roadmap_log.md`.

Nota sobre estándar C++:
- Este repositorio ahora requiere C++23 como estándar de compilación en `CMakeLists.txt`. Asegúrate de que tu entorno local/CI tenga toolchains compatibles (MSVC/Clang/GCC) antes de compilar.
