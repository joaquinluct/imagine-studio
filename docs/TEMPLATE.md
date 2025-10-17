# Metodolog�a de Desarrollo �gil Asistido por IA - Template

> **Prop�sito**: Este documento define la metodolog�a gen�rica de trabajo con asistentes IA (GitHub Copilot, ChatGPT, Claude, etc.) aplicable a cualquier proyecto de software.

**Nota para uso**: Adapta las secciones marcadas con `[PLACEHOLDER]` al contexto espec�fico de tu proyecto (tecnolog�a, lenguaje, sistema de build, est�ndares, etc.).

---

## ?? LEER PRIMERO

**OBLIGATORIO**: Antes de comenzar cualquier sesi�n con el asistente IA, leer:  
?? **[`docs/MAIN.md`](MAIN.md)** - [PLACEHOLDER: Documento fundamental del proyecto]

Este documento define los pilares fundamentales, arquitectura y principios del proyecto.  
**Todas las decisiones de implementaci�n DEBEN alinearse con estos principios.**

---

## ?? Estructura de Archivos de Trabajo

Archivos principales que el asistente debe consultar en cada sesi�n:

- **`docs/MAIN.md`** - [LEER PRIMERO] Fundamentos del proyecto (pilares, arquitectura, est�ndares)
- **`docs/sprint.md`** - Sprint actual con hitos y objetivos
- **`docs/daily.md`** - �ltima tarea completada y tarea actual en progreso (o "Sprint vX.Y.Z cerrado. Sin sprint activo." si no hay sprint activo)
- **`docs/commits.md`** - Convenciones de commits y mensajes
- **`docs/backlog.md`** - Repositorio de �tems no incluidos en el sprint actual
- **`docs/sprint_bugs.md`** - Tracking de bugs reportados (pendientes de resoluci�n)
- **`docs/sprint_fix.md`** - Historial de bugs resueltos durante el sprint

---

## ?? Flujo de Trabajo por Sesi�n

### Proceso iterativo paso a paso:

1. **Leer instrucciones** (este documento o el espec�fico del proyecto)
2. **Consultar `docs/daily.md`** para ver la �ltima tarea completada y la tarea actual
3. **Comparar con el c�digo fuente** para entender el estado actual
4. **Proponer el siguiente paso** (por parte del asistente)
5. **Si se confirma**, el asistente implementa el paso:
   
   **a) Implementar el cambio**
   
   **b) Validar el cambio** (compilar/ejecutar/probar seg�n el proyecto):
   - **[PLACEHOLDER: Sistema de validaci�n]**
     - Ejemplos seg�n tecnolog�a:
       - **C++/C#**: Compilaci�n (CMake, MSBuild, gcc, clang, etc.)
       - **Web (React/Vue/Angular)**: Ejecutar dev server + hot-reload
       - **Python**: Ejecutar tests unitarios (pytest, unittest)
       - **Backend (Node/Django/FastAPI)**: Ejecutar servidor + tests
       - **M�vil (iOS/Android)**: Compilar + ejecutar en simulador/emulador
   
   - **Requisito obligatorio**: [PLACEHOLDER: Criterio de validaci�n exitosa]
     - Ejemplos:
       - **Compilados**: 0 errores, 0 warnings
       - **Web/Scripting**: Ejecuci�n sin errores, tests pasando
       - **Calidad**: Linter limpio, cobertura de tests > X%
   
   - Si la validaci�n **falla**, el asistente corregir� los errores y repetir� hasta lograr validaci�n limpia
   - El asistente ejecutar� autom�ticamente la validaci�n antes de crear commits
   
   **c) Una vez validaci�n limpia**, crear el commit y actualizar los archivos Markdown pertinentes:
   - `docs/daily.md`
   - `docs/commits.md`
   - `README.md` (cuando proceda)
   - `.github/copilot-instructions.md` (cuando proceda)

6. **Repetir desde 1**

---

## ?? Preferencias de Calidad y Est�ndares

**[PLACEHOLDER: Preferencias del propietario del repositorio]**

Ejemplos gen�ricos:
- Prefiero hacer bien las cosas desde el principio: evitar partes intermedias, temporales, incompletas o no funcionales
- Cada m�dulo debe dise�arse para cumplir los est�ndares [PLACEHOLDER: AAA/Enterprise/Production] desde su concepci�n
- En cada iteraci�n, antes del commit, siempre se debe validar (compilar/ejecutar/probar) y corregir errores hasta lograr builds/ejecuciones limpias
- **[PLACEHOLDER: Estilo de c�digo espec�fico]**
  - Ejemplos:
    - C++: Includes ordenados (proyecto primero, luego sistema, alfab�tico)
    - Python: PEP 8, type hints, docstrings
    - JavaScript: ESLint, Prettier, JSDoc
    - Cualquier lenguaje: Nombres descriptivos, funciones peque�as, DRY, SOLID

---

## ?? Secci�n de Resoluci�n de Problemas

**[PLACEHOLDER: Troubleshooting espec�fico del proyecto]**

Ejemplos gen�ricos seg�n tecnolog�a:

### Para proyectos compilados (C++, C#, Java, Go, Rust):
1. Comprobar la existencia y ruta exacta del archivo de soluci�n/proyecto
2. Compilar expl�citamente con el comando correcto (ej: `msbuild`, `cargo build`, `go build`, `javac`)
3. Si faltan archivos fuente, confirmar que est�n incluidos en el archivo de proyecto/build
4. Si hay errores de enlace/linking: verificar dependencias, librer�as, paths
5. Si la compilaci�n muestra errores/warnings: corregirlos hasta obtener build limpia

### Para proyectos web (React, Vue, Angular, Node):
1. Verificar `package.json` y dependencias instaladas (`npm install` / `yarn install`)
2. Ejecutar el servidor de desarrollo (`npm run dev`, `yarn dev`, etc.)
3. Verificar hot-reload funcional
4. Ejecutar linter (`npm run lint`)
5. Ejecutar tests (`npm test`)

### Para proyectos Python:
1. Verificar entorno virtual activado
2. Verificar `requirements.txt` o `pyproject.toml` y dependencias instaladas
3. Ejecutar tests (`pytest`, `python -m unittest`)
4. Ejecutar linter/formatter (`black`, `flake8`, `mypy`)

**Registrar en la bit�cora** (`docs/daily.md`) cualquier incidencia relevante y su resoluci�n para futuras referencias.

---

## ?? Iteraci�n Controlada (Proceso Obligatorio)

### Ejecuci�n autom�tica (sin necesidad de confirmaci�n previa):

1. **Consulta**: El asistente consultar� `docs/daily.md` y propondr� el siguiente punto a ejecutar, acompa�ado de una breve explicaci�n (una frase) sobre en qu� consiste

2. **Implementaci�n**: Tras proponer el punto, el asistente implementar� el cambio, validar� (compilar�/ejecutar�/probar�) y corregir� errores/warnings hasta lograr validaci�n limpia

3. **Commit autom�tico**: Si la validaci�n queda limpia, el asistente realizar� directamente el commit local siguiendo `docs/commits.md` y actualizar� los ficheros necesarios (`docs/daily.md`, `docs/commits.md`, `README.md`, `.github/copilot-instructions.md`) **sin pedir permiso adicional**

4. **Informe**: El asistente informar� de lo realizado y del siguiente punto propuesto y continuar� el ciclo

### Excepciones:
Si aparece un **bloqueo t�cnico**, **decisi�n arquitect�nica cr�tica** o **dependencia que impida avanzar**, el asistente pausar� y solicitar� instrucciones al propietario.

---

## ?? Regla Estricta sobre Commits y Documentaci�n

- **Requisito**: Siempre que el asistente realice un commit local como resultado de una iteraci�n (es decir, la validaci�n queda limpia), actualizar� autom�ticamente `docs/daily.md` y `docs/commits.md` para reflejar el cambio **sin pedir confirmaci�n adicional**
- El **push al repositorio remoto** NO se realizar� autom�ticamente salvo instrucci�n expl�cita del propietario

---

## ?? Versionado de los Ficheros del Sprint (Snapshots)

### Ficheros activos del sprint actual:
- `docs/sprint.md` - Sprint de alto nivel con hitos y objetivos
- `docs/sprint_histories.md` - Historias de usuario para el sprint
- `docs/sprint_tasks.md` - Tareas detalladas por historia (unidad m�nima de trabajo e iteraci�n)
- `docs/sprint_bugs.md` - Tracking de bugs reportados (pendientes de resoluci�n)
- `docs/sprint_fix.md` - Historial de bugs resueltos durante el sprint

### Al final de un sprint (release):
1. El asistente **archivar�** los ficheros de trabajo renombr�ndolos con la versi�n:
   - `docs/sprint_v<version>.md`
   - `docs/sprint_histories_v<version>.md`
   - `docs/sprint_tasks_v<version>.md`
   - `docs/sprint_bugs_v<version>.md` (bugs que quedaron pendientes al cerrar sprint)
   - `docs/sprint_fix_v<version>.md` (bugs resueltos durante el sprint)

2. El asistente **crear� ficheros nuevos y vac�os** con los nombres activos para el siguiente sprint

Esta pol�tica garantiza **trazabilidad** de los sprints completados y mantiene los ficheros activos peque�os y enfocados.

---

## ?? Fichero Backlog (`docs/backlog.md`)

### Prop�sito:
`docs/backlog.md` es el repositorio a largo plazo para los �tems que **NO forman parte del sprint activo**.

�salo como "ba�l" persistente para:
- Ideas
- Historias diferidas
- Tareas de baja prioridad
- Deuda t�cnica

### Flujo y uso:
1. Cuando se identifique una historia o tarea pero **no se seleccione para el sprint actual**, a�adir una entrada breve en `docs/backlog.md`:
   - Descripci�n corta
   - Etiqueta de prioridad (Baja/Media/Alta)
   - Opcionalmente: referencia al ID de historia/tarea en `docs/sprint_histories.md` o `docs/sprint_tasks.md`

2. El backlog es la **fuente para la planificaci�n del sprint**:
   - Durante la planificaci�n, los �tems pueden moverse de `docs/backlog.md` a `docs/sprint_histories.md` (como historia)
   - Luego se descomponen en tareas en `docs/sprint_tasks.md` para el sprint activo

3. Las entradas en `docs/backlog.md` deben ser **concisas y enlazables** (incluir una ruta o ancla al c�digo relacionado si procede)

4. El asistente **NO implementar�** �tems directamente desde el backlog a menos que se muevan a los ficheros activos del sprint

---

## ?? Fichero Sprint Bugs (`docs/sprint_bugs.md`)

### Prop�sito:
`docs/sprint_bugs.md` es el **contenedor inicial** de bugs reportados durante el sprint activo. Permite mantener trazabilidad de defectos encontrados y su estado actual.

### Contenido:
Cada entrada de bug debe incluir:
- **ID**: Identificador �nico del bug con prefijo BUG- (ej: BUG-001, BUG-002)
- **T�tulo**: Descripci�n breve del error
- **Descripci�n**: Detalle del error, pasos para reproducir, comportamiento esperado vs observado
- **Prioridad**: Cr�tica/Alta/Media/Baja
- **Estado**: Reportado/En progreso
- **Fecha de entrada**: Fecha en que se report� el bug
- **Archivos afectados**: Lista de archivos relacionados con el bug

### Formato ejemplo:
```markdown
### BUG-001 - [PLACEHOLDER: T�tulo del bug]
**ID**: BUG-001
**Prioridad**: Cr�tica
**Estado**: Reportado
**Fecha de entrada**: 2025-01-15

**Descripci�n**: [PLACEHOLDER: Descripci�n detallada del error, pasos para reproducir, comportamiento esperado vs observado]

**Archivos afectados**: [PLACEHOLDER: lista de archivos]
```

### Flujo de trabajo:
1. Cuando el usuario **reporte un bug** durante el sprint, el asistente a�adir� una entrada en `docs/sprint_bugs.md` con ID **BUG-XXX** y estado "Reportado"
2. Al comenzar a trabajar en el bug, **actualizar estado** a "En progreso"
3. Al resolver el bug (commit exitoso), el asistente **mover� autom�ticamente** la entrada a `docs/sprint_fix.md` con:
   - ID cambiado a **FIX-XXX**
   - Estado "Resuelto"
   - Fecha de resoluci�n
   - Hash del commit de resoluci�n
   - Descripci�n de la soluci�n implementada
4. El bug se elimina de `docs/sprint_bugs.md` tras moverlo a `docs/sprint_fix.md`
5. Los bugs pendientes se **archivan** como `docs/sprint_bugs_v<version>.md` al finalizar el sprint

### Regla:
El asistente **NO implementar�** bugs directamente a menos que se indique expl�citamente; primero los registrar� en `docs/sprint_bugs.md` para priorizaci�n.

---

## ? Fichero Sprint Fix (`docs/sprint_fix.md`)

### Prop�sito:
`docs/sprint_fix.md` es el **historial de bugs resueltos** durante el sprint activo. Contiene bugs que fueron reportados en `docs/sprint_bugs.md` y posteriormente solucionados.

### Contenido:
Cada entrada de bug resuelto debe incluir:
- **ID Original**: Identificador original del bug (BUG-XXX del sprint_bugs.md)
- **ID Resoluci�n**: Identificador con prefijo FIX- (ej: FIX-001, FIX-002)
- **T�tulo**: Descripci�n breve del error
- **Descripci�n del problema**: Detalle original del error
- **Soluci�n implementada**: Descripci�n de c�mo se resolvi� el bug
- **Prioridad**: Cr�tica/Alta/Media/Baja
- **Fecha de entrada**: Fecha en que se report� originalmente
- **Fecha de resoluci�n**: Fecha en que se resolvi�
- **Archivos afectados**: Lista de archivos relacionados con el bug y su resoluci�n
- **Commit de resoluci�n**: Hash del commit que resolvi� el bug

### Formato ejemplo:
```markdown
### FIX-001 - [PLACEHOLDER: T�tulo del bug]
**ID Original**: BUG-001
**Prioridad**: Cr�tica
**Fecha de entrada**: 2025-01-15
**Fecha de resoluci�n**: 2025-01-15

**Descripci�n del problema**: [PLACEHOLDER: Descripci�n detallada del error, pasos para reproducir, comportamiento esperado vs observado]

**Soluci�n implementada**: [PLACEHOLDER: Descripci�n de c�mo se resolvi� el bug]

**Archivos afectados**: [PLACEHOLDER: lista de archivos relacionados con el bug y su resoluci�n]

**Commit de resoluci�n**: [PLACEHOLDER: hash del commit]
```

### Flujo de trabajo:
1. Bug reportado ? Se registra en `docs/sprint_bugs.md` con ID **BUG-XXX**
2. Bug en progreso ? Estado actualizado en `docs/sprint_bugs.md`
3. Bug resuelto ? Se **mueve autom�ticamente** a `docs/sprint_fix.md` con:
   - ID cambiado a **FIX-XXX**
   - Estado "Resuelto"
   - Fecha de resoluci�n
   - Hash del commit de resoluci�n
   - Descripci�n de la soluci�n implementada
4. Bug eliminado de `docs/sprint_bugs.md`
5. Los bugs resueltos se **archivan** como `docs/sprint_fix_v<version>.md` al finalizar el sprint

### Regla:
Los bugs se mueven autom�ticamente desde `docs/sprint_bugs.md` al resolverse. El asistente actualiza el ID (BUG-XXX ? FIX-XXX), a�ade fecha de resoluci�n, commit hash y descripci�n de la soluci�n.

---

## ?? Sincronizaci�n con Metodolog�a Gen�rica

### Regla de sincronizaci�n:
Este documento (`docs/TEMPLATE.md`) es el **template maestro** de la metodolog�a gen�rica de trabajo con asistentes IA.

**IMPORTANTE**: Cuando se modifique la metodolog�a espec�fica del proyecto (por ejemplo, en `.github/copilot-instructions.md` o similar):
- El asistente debe **actualizar tambi�n este template** para reflejar los cambios de forma gen�rica
- Usar **placeholders** `[PLACEHOLDER]` para mantener la reusabilidad en otros proyectos
- Ejemplos de cambios que requieren sincronizaci�n:
  - A�adir/eliminar secciones de la metodolog�a
  - Cambiar flujo de trabajo
  - Actualizar proceso de versionado
  - A�adir nuevos ficheros de tracking (ej: `sprint_fix.md`)

Esta regla asegura que el template est� siempre actualizado y pueda ser reutilizado en otros proyectos.

---

## ?? Formato de la Explicaci�n Final de Cada Iteraci�n

### Requisito:
Al final de cada iteraci�n (cuando se informa lo realizado y el siguiente punto), la explicaci�n debe contener **obligatoriamente**:

### 1. **Dos t�tulos numerados** siguiendo el esquema del Sprint:
- **"Hecho: \<n�mero\> \<t�tulo\>"** (ej: "Hecho: 9.18 Shading/material...") que describe en breve lo completado
- **"Siguiente: \<n�mero\> \<t�tulo\>"** que describe el siguiente punto propuesto

### 2. **Barra de progreso visual del sprint** mostrando el avance de tareas completadas:

**Formato**: Barra horizontal con fondo negro (?), progreso verde (??), bordes blancos (?), y porcentaje centrado

**Ejemplo** con 2 de 15 tareas (13.3%):
```
??????????????????????????????????????????????????????????????????????
????????????????????????????????????????????????????? 13.3%?
??????????????????????????????????????????????????????????????????????
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

---

## ?? Fichero Daily (`docs/daily.md`)

### Prop�sito:
`docs/daily.md` es el fichero **simplificado de seguimiento diario**.

### Contenido:
El fichero solo contiene **DOS cosas**:

1. **�ltima tarea realizada**: N�mero y descripci�n de la tarea completada  
   Formato: `"Hecho: <n> <descripci�n>"`

2. **Tarea actual**: N�mero y descripci�n de la siguiente tarea a realizar  
   Formato: `"Siguiente: <n> <descripci�n>"`

### Formato ejemplo:
```markdown
# Daily Log

Hecho: 3.01 [PLACEHOLDER: Nombre de tarea completada]
Siguiente: 4.00 [PLACEHOLDER: Nombre de siguiente tarea]
```

### Formato ejemplo (sin sprint activo):
```markdown
# Daily Log

Sprint v[PLACEHOLDER: versi�n] cerrado. Sin sprint activo.
```

### Actualizaci�n:
- El asistente actualiza este fichero **autom�ticamente** tras cada commit exitoso
- Al finalizar un sprint (release), el contenido de `daily.md` se actualiza a "Sprint vX.Y.Z cerrado. Sin sprint activo." y se archiva en los ficheros versionados del sprint. Se crea un nuevo `daily.md` para el siguiente sprint cuando este comienza

---

## ?? Estructura de Documentos del Sprint

### 1. `docs/sprint.md` - Sprint de Alto Nivel
**Contenido**:
- Objetivo del sprint
- Hitos principales
- Criterios de aceptaci�n globales
- Comparaci�n con versi�n anterior (opcional)
- Estado del sprint (historias/tareas completadas)

### 2. `docs/sprint_histories.md` - Historias de Usuario
**Contenido**:
- ID de historia
- T�tulo y descripci�n
- Prioridad
- Criterios de aceptaci�n
- Lista de tareas derivadas

**Ejemplo de historia**:
```markdown
### H1 - [PLACEHOLDER: T�tulo de Historia]
**ID**: H1
**Prioridad**: CR�TICA
**Descripci�n**: [PLACEHOLDER: Descripci�n detallada]

**Criterios de aceptaci�n**:
- ? [PLACEHOLDER: Criterio 1]
- ? [PLACEHOLDER: Criterio 2]

**Tareas derivadas**: T1.1, T1.2, T1.3
```

### 3. `docs/sprint_tasks.md` - Tareas Detalladas
**Contenido**:
- ID de tarea
- Historia asociada
- Descripci�n
- Implementaci�n (c�digo de ejemplo, pasos)
- Archivos afectados
- Criterio de �xito

**Ejemplo de tarea**:
```markdown
### T1.1 - [PLACEHOLDER: T�tulo de Tarea]
**ID**: T1.1
**Historia**: H1
**Descripci�n**: [PLACEHOLDER: Descripci�n detallada]

**Implementaci�n**:
```[PLACEHOLDER: lenguaje]
// [PLACEHOLDER: c�digo de ejemplo]
```

**Archivos afectados**: `[PLACEHOLDER: lista de archivos]`

**Criterio de �xito**: [PLACEHOLDER: condici�n de �xito]
```

---

## ?? Proceso de Ejecuci�n de Tareas

Cada tarea debe seguir las **reglas de iteraci�n**:

1. **Implementar** el c�digo seg�n la descripci�n en `docs/sprint_tasks.md`
2. **Validar** (compilar/ejecutar/probar seg�n el proyecto):
   - [PLACEHOLDER: Comando de validaci�n espec�fico]
   - Ejemplos: `npm test`, `cargo test`, `msbuild`, `pytest`, `go test`
3. **Corregir errores** hasta obtener validaci�n limpia (0 errores, 0 warnings)
4. **Commit** y actualizar `docs/daily.md` y `docs/commits.md`

---

## ?? Convenciones de Commits (`docs/commits.md`)

### Formato recomendado:
```
<tipo>(<�mbito>): <mensaje corto>

<mensaje largo opcional>

Refs: <issue/track-id>
```

### Tipos comunes:
- **feat**: Nueva funcionalidad
- **fix**: Correcci�n de errores
- **docs**: Cambios en documentaci�n
- **style**: Formato, estilo, sin cambios funcionales
- **refactor**: Refactorizaci�n sin cambios en comportamiento
- **perf**: Mejoras de rendimiento
- **test**: A�adir o corregir tests
- **chore**: Tareas de mantenimiento

### Ejemplo:
```
feat(auth): implementar login con OAuth2

A�ade soporte para autenticaci�n v�a Google y GitHub.
Incluye validaci�n de tokens y refresh autom�tico.

Refs: #123
```

---

## ?? Principios de la Metodolog�a

Esta metodolog�a se basa en:

1. **Iteraciones peque�as y compilables/ejecutables**: Cada tarea debe dejar el proyecto en estado funcional
2. **Documentaci�n continua**: Actualizar docs en cada commit
3. **Progreso visual**: Barra de progreso para feedback inmediato
4. **Automatizaci�n IA**: El asistente ejecuta el ciclo completo sin intervenci�n manual innecesaria
5. **Trazabilidad**: Versionado de sprints, commits descriptivos, historial claro
6. **Calidad desde el inicio**: No soluciones temporales, solo c�digo production-ready
7. **Flexibilidad tecnol�gica**: Aplicable a cualquier stack (backend, frontend, m�vil, desktop, embedded, etc.)

---

## ?? Adaptaci�n a Tu Proyecto

Para adaptar este template a tu proyecto espec�fico:

1. **Copia este archivo** como base para tu `.github/copilot-instructions.md`
2. **Reemplaza todos los `[PLACEHOLDER]`** con:
   - Tecnolog�as espec�ficas (lenguaje, framework, herramientas)
   - Comandos de build/validaci�n reales
   - Est�ndares de c�digo del proyecto
   - Preferencias del equipo
3. **A�ade secciones espec�ficas** si tu proyecto requiere:
   - Configuraci�n de CI/CD
   - Despliegue automatizado
   - Gesti�n de base de datos
   - APIs externas
   - etc.
4. **Crea `docs/MAIN.md`** con los pilares fundamentales de tu proyecto
5. **Inicializa los archivos de sprint**:
   - `docs/sprint.md`
   - `docs/sprint_histories.md`
   - `docs/sprint_tasks.md`
   - `docs/daily.md`
   - `docs/backlog.md`
   - `docs/commits.md`

---

## ? Ejemplo de Uso en Diferentes Tecnolog�as

### Para un proyecto React + TypeScript:
- **Validaci�n**: `npm run lint && npm test && npm run build`
- **Estilo**: ESLint + Prettier, componentes funcionales, hooks
- **Stack**: React 18, TypeScript 5, Vite, Vitest

### Para un proyecto Python FastAPI:
- **Validaci�n**: `pytest && mypy . && black --check .`
- **Estilo**: PEP 8, type hints obligatorios, async/await
- **Stack**: FastAPI, SQLAlchemy, Pydantic, pytest

### Para un proyecto Rust:
- **Validaci�n**: `cargo clippy && cargo test && cargo build`
- **Estilo**: rustfmt, Clippy warnings = errores
- **Stack**: Rust 1.70+, tokio, serde

### Para un proyecto Unity C#:
- **Validaci�n**: Compilaci�n Unity + tests unitarios
- **Estilo**: Convenciones Unity, componentes desacoplados
- **Stack**: Unity 2022 LTS, C# 9.0, URP

---

## ?? Recursos Adicionales

- Ver sprints completados: `docs/sprint_v*.md`
- Ver historias completadas: `docs/sprint_histories_v*.md`
- Ver tareas completadas: `docs/sprint_tasks_v*.md`
- Ver bugs/errores del sprint: `docs/sprint_fix_v*.md`
- Convenciones de commits: `docs/commits.md`
- Backlog de �tems futuros: `docs/backlog.md`

---

**Fin del Template de Metodolog�a**

*Versi�n: 1.0*  
*�ltima actualizaci�n: [PLACEHOLDER: Fecha]*
