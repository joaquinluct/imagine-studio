# Metodología de Desarrollo Ágil Asistido por IA - Template

> **Propósito**: Este documento define la metodología genérica de trabajo con asistentes IA (GitHub Copilot, ChatGPT, Claude, etc.) aplicable a cualquier proyecto de software.

**Nota para uso**: Adapta las secciones marcadas con `[PLACEHOLDER]` al contexto específico de tu proyecto (tecnología, lenguaje, sistema de build, estándares, etc.).

---

## ?? LEER PRIMERO

**OBLIGATORIO**: Antes de comenzar cualquier sesión con el asistente IA, leer:  
?? **[`docs/MAIN.md`](MAIN.md)** - [PLACEHOLDER: Documento fundamental del proyecto]

Este documento define los pilares fundamentales, arquitectura y principios del proyecto.  
**Todas las decisiones de implementación DEBEN alinearse con estos principios.**

---

## ?? Estructura de Archivos de Trabajo

Archivos principales que el asistente debe consultar en cada sesión:

- **`docs/MAIN.md`** - [LEER PRIMERO] Fundamentos del proyecto (pilares, arquitectura, estándares)
- **`docs/sprint.md`** - Sprint actual con hitos y objetivos
- **`docs/daily.md`** - Última tarea completada y tarea actual en progreso (o "Sprint vX.Y.Z cerrado. Sin sprint activo." si no hay sprint activo)
- **`docs/commits.md`** - Convenciones de commits y mensajes
- **`docs/backlog.md`** - Repositorio de ítems no incluidos en el sprint actual
- **`docs/sprint_bugs.md`** - Tracking de bugs reportados (pendientes de resolución)
- **`docs/sprint_fix.md`** - Historial de bugs resueltos durante el sprint

---

## ?? Flujo de Trabajo por Sesión

### Proceso iterativo paso a paso:

1. **Leer instrucciones** (este documento o el específico del proyecto)
2. **Consultar `docs/daily.md`** para ver la última tarea completada y la tarea actual
3. **Comparar con el código fuente** para entender el estado actual
4. **Proponer el siguiente paso** (por parte del asistente)
5. **Si se confirma**, el asistente implementa el paso:
   
   **a) Implementar el cambio**
   
   **b) Validar el cambio** (compilar/ejecutar/probar según el proyecto):
   - **[PLACEHOLDER: Sistema de validación]**
     - Ejemplos según tecnología:
       - **C++/C#**: Compilación (CMake, MSBuild, gcc, clang, etc.)
       - **Web (React/Vue/Angular)**: Ejecutar dev server + hot-reload
       - **Python**: Ejecutar tests unitarios (pytest, unittest)
       - **Backend (Node/Django/FastAPI)**: Ejecutar servidor + tests
       - **Móvil (iOS/Android)**: Compilar + ejecutar en simulador/emulador
   
   - **Requisito obligatorio**: [PLACEHOLDER: Criterio de validación exitosa]
     - Ejemplos:
       - **Compilados**: 0 errores, 0 warnings
       - **Web/Scripting**: Ejecución sin errores, tests pasando
       - **Calidad**: Linter limpio, cobertura de tests > X%
   
   - Si la validación **falla**, el asistente corregirá los errores y repetirá hasta lograr validación limpia
   - El asistente ejecutará automáticamente la validación antes de crear commits
   
   **c) Una vez validación limpia**, crear el commit y actualizar los archivos Markdown pertinentes:
   - `docs/daily.md`
   - `docs/commits.md`
   - `README.md` (cuando proceda)
   - `.github/copilot-instructions.md` (cuando proceda)

6. **Repetir desde 1**

---

## ?? Preferencias de Calidad y Estándares

**[PLACEHOLDER: Preferencias del propietario del repositorio]**

Ejemplos genéricos:
- Prefiero hacer bien las cosas desde el principio: evitar partes intermedias, temporales, incompletas o no funcionales
- Cada módulo debe diseñarse para cumplir los estándares [PLACEHOLDER: AAA/Enterprise/Production] desde su concepción
- En cada iteración, antes del commit, siempre se debe validar (compilar/ejecutar/probar) y corregir errores hasta lograr builds/ejecuciones limpias
- **[PLACEHOLDER: Estilo de código específico]**
  - Ejemplos:
    - C++: Includes ordenados (proyecto primero, luego sistema, alfabético)
    - Python: PEP 8, type hints, docstrings
    - JavaScript: ESLint, Prettier, JSDoc
    - Cualquier lenguaje: Nombres descriptivos, funciones pequeñas, DRY, SOLID

---

## ?? Sección de Resolución de Problemas

**[PLACEHOLDER: Troubleshooting específico del proyecto]**

Ejemplos genéricos según tecnología:

### Para proyectos compilados (C++, C#, Java, Go, Rust):
1. Comprobar la existencia y ruta exacta del archivo de solución/proyecto
2. Compilar explícitamente con el comando correcto (ej: `msbuild`, `cargo build`, `go build`, `javac`)
3. Si faltan archivos fuente, confirmar que están incluidos en el archivo de proyecto/build
4. Si hay errores de enlace/linking: verificar dependencias, librerías, paths
5. Si la compilación muestra errores/warnings: corregirlos hasta obtener build limpia

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

**Registrar en la bitácora** (`docs/daily.md`) cualquier incidencia relevante y su resolución para futuras referencias.

---

## ?? Iteración Controlada (Proceso Obligatorio)

### Ejecución automática (sin necesidad de confirmación previa):

1. **Consulta**: El asistente consultará `docs/daily.md` y propondrá el siguiente punto a ejecutar, acompañado de una breve explicación (una frase) sobre en qué consiste

2. **Implementación**: Tras proponer el punto, el asistente implementará el cambio, validará (compilará/ejecutará/probará) y corregirá errores/warnings hasta lograr validación limpia

3. **Commit automático**: Si la validación queda limpia, el asistente realizará directamente el commit local siguiendo `docs/commits.md` y actualizará los ficheros necesarios (`docs/daily.md`, `docs/commits.md`, `README.md`, `.github/copilot-instructions.md`) **sin pedir permiso adicional**

4. **Informe**: El asistente informará de lo realizado y del siguiente punto propuesto y continuará el ciclo

### Excepciones:
Si aparece un **bloqueo técnico**, **decisión arquitectónica crítica** o **dependencia que impida avanzar**, el asistente pausará y solicitará instrucciones al propietario.

---

## ?? Regla Estricta sobre Commits y Documentación

- **Requisito**: Siempre que el asistente realice un commit local como resultado de una iteración (es decir, la validación queda limpia), actualizará automáticamente `docs/daily.md` y `docs/commits.md` para reflejar el cambio **sin pedir confirmación adicional**
- El **push al repositorio remoto** NO se realizará automáticamente salvo instrucción explícita del propietario

---

## ?? Versionado de los Ficheros del Sprint (Snapshots)

### Ficheros activos del sprint actual:
- `docs/sprint.md` - Sprint de alto nivel con hitos y objetivos
- `docs/sprint_histories.md` - Historias de usuario para el sprint
- `docs/sprint_tasks.md` - Tareas detalladas por historia (unidad mínima de trabajo e iteración)
- `docs/sprint_bugs.md` - Tracking de bugs reportados (pendientes de resolución)
- `docs/sprint_fix.md` - Historial de bugs resueltos durante el sprint

### Al final de un sprint (release):
1. El asistente **archivará** los ficheros de trabajo renombrándolos con la versión:
   - `docs/sprint_v<version>.md`
   - `docs/sprint_histories_v<version>.md`
   - `docs/sprint_tasks_v<version>.md`
   - `docs/sprint_bugs_v<version>.md` (bugs que quedaron pendientes al cerrar sprint)
   - `docs/sprint_fix_v<version>.md` (bugs resueltos durante el sprint)

2. El asistente **creará ficheros nuevos y vacíos** con los nombres activos para el siguiente sprint

Esta política garantiza **trazabilidad** de los sprints completados y mantiene los ficheros activos pequeños y enfocados.

---

## ?? Fichero Backlog (`docs/backlog.md`)

### Propósito:
`docs/backlog.md` es el repositorio a largo plazo para los ítems que **NO forman parte del sprint activo**.

Úsalo como "baúl" persistente para:
- Ideas
- Historias diferidas
- Tareas de baja prioridad
- Deuda técnica

### Flujo y uso:
1. Cuando se identifique una historia o tarea pero **no se seleccione para el sprint actual**, añadir una entrada breve en `docs/backlog.md`:
   - Descripción corta
   - Etiqueta de prioridad (Baja/Media/Alta)
   - Opcionalmente: referencia al ID de historia/tarea en `docs/sprint_histories.md` o `docs/sprint_tasks.md`

2. El backlog es la **fuente para la planificación del sprint**:
   - Durante la planificación, los ítems pueden moverse de `docs/backlog.md` a `docs/sprint_histories.md` (como historia)
   - Luego se descomponen en tareas en `docs/sprint_tasks.md` para el sprint activo

3. Las entradas en `docs/backlog.md` deben ser **concisas y enlazables** (incluir una ruta o ancla al código relacionado si procede)

4. El asistente **NO implementará** ítems directamente desde el backlog a menos que se muevan a los ficheros activos del sprint

---

## ?? Fichero Sprint Bugs (`docs/sprint_bugs.md`)

### Propósito:
`docs/sprint_bugs.md` es el **contenedor inicial** de bugs reportados durante el sprint activo. Permite mantener trazabilidad de defectos encontrados y su estado actual.

### Contenido:
Cada entrada de bug debe incluir:
- **ID**: Identificador único del bug con prefijo BUG- (ej: BUG-001, BUG-002)
- **Título**: Descripción breve del error
- **Descripción**: Detalle del error, pasos para reproducir, comportamiento esperado vs observado
- **Prioridad**: Crítica/Alta/Media/Baja
- **Estado**: Reportado/En progreso
- **Fecha de entrada**: Fecha en que se reportó el bug
- **Archivos afectados**: Lista de archivos relacionados con el bug

### Formato ejemplo:
```markdown
### BUG-001 - [PLACEHOLDER: Título del bug]
**ID**: BUG-001
**Prioridad**: Crítica
**Estado**: Reportado
**Fecha de entrada**: 2025-01-15

**Descripción**: [PLACEHOLDER: Descripción detallada del error, pasos para reproducir, comportamiento esperado vs observado]

**Archivos afectados**: [PLACEHOLDER: lista de archivos]
```

### Flujo de trabajo:
1. Cuando el usuario **reporte un bug** durante el sprint, el asistente añadirá una entrada en `docs/sprint_bugs.md` con ID **BUG-XXX** y estado "Reportado"
2. Al comenzar a trabajar en el bug, **actualizar estado** a "En progreso"
3. Al resolver el bug (commit exitoso), el asistente **moverá automáticamente** la entrada a `docs/sprint_fix.md` con:
   - ID cambiado a **FIX-XXX**
   - Estado "Resuelto"
   - Fecha de resolución
   - Hash del commit de resolución
   - Descripción de la solución implementada
4. El bug se elimina de `docs/sprint_bugs.md` tras moverlo a `docs/sprint_fix.md`
5. Los bugs pendientes se **archivan** como `docs/sprint_bugs_v<version>.md` al finalizar el sprint

### Regla:
El asistente **NO implementará** bugs directamente a menos que se indique explícitamente; primero los registrará en `docs/sprint_bugs.md` para priorización.

---

## ? Fichero Sprint Fix (`docs/sprint_fix.md`)

### Propósito:
`docs/sprint_fix.md` es el **historial de bugs resueltos** durante el sprint activo. Contiene bugs que fueron reportados en `docs/sprint_bugs.md` y posteriormente solucionados.

### Contenido:
Cada entrada de bug resuelto debe incluir:
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

### Formato ejemplo:
```markdown
### FIX-001 - [PLACEHOLDER: Título del bug]
**ID Original**: BUG-001
**Prioridad**: Crítica
**Fecha de entrada**: 2025-01-15
**Fecha de resolución**: 2025-01-15

**Descripción del problema**: [PLACEHOLDER: Descripción detallada del error, pasos para reproducir, comportamiento esperado vs observado]

**Solución implementada**: [PLACEHOLDER: Descripción de cómo se resolvió el bug]

**Archivos afectados**: [PLACEHOLDER: lista de archivos relacionados con el bug y su resolución]

**Commit de resolución**: [PLACEHOLDER: hash del commit]
```

### Flujo de trabajo:
1. Bug reportado ? Se registra en `docs/sprint_bugs.md` con ID **BUG-XXX**
2. Bug en progreso ? Estado actualizado en `docs/sprint_bugs.md`
3. Bug resuelto ? Se **mueve automáticamente** a `docs/sprint_fix.md` con:
   - ID cambiado a **FIX-XXX**
   - Estado "Resuelto"
   - Fecha de resolución
   - Hash del commit de resolución
   - Descripción de la solución implementada
4. Bug eliminado de `docs/sprint_bugs.md`
5. Los bugs resueltos se **archivan** como `docs/sprint_fix_v<version>.md` al finalizar el sprint

### Regla:
Los bugs se mueven automáticamente desde `docs/sprint_bugs.md` al resolverse. El asistente actualiza el ID (BUG-XXX ? FIX-XXX), añade fecha de resolución, commit hash y descripción de la solución.

---

## ?? Sincronización con Metodología Genérica

### Regla de sincronización:
Este documento (`docs/TEMPLATE.md`) es el **template maestro** de la metodología genérica de trabajo con asistentes IA.

**IMPORTANTE**: Cuando se modifique la metodología específica del proyecto (por ejemplo, en `.github/copilot-instructions.md` o similar):
- El asistente debe **actualizar también este template** para reflejar los cambios de forma genérica
- Usar **placeholders** `[PLACEHOLDER]` para mantener la reusabilidad en otros proyectos
- Ejemplos de cambios que requieren sincronización:
  - Añadir/eliminar secciones de la metodología
  - Cambiar flujo de trabajo
  - Actualizar proceso de versionado
  - Añadir nuevos ficheros de tracking (ej: `sprint_fix.md`)

Esta regla asegura que el template esté siempre actualizado y pueda ser reutilizado en otros proyectos.

---

## ?? Formato de la Explicación Final de Cada Iteración

### Requisito:
Al final de cada iteración (cuando se informa lo realizado y el siguiente punto), la explicación debe contener **obligatoriamente**:

### 1. **Dos títulos numerados** siguiendo el esquema del Sprint:
- **"Hecho: \<número\> \<título\>"** (ej: "Hecho: 9.18 Shading/material...") que describe en breve lo completado
- **"Siguiente: \<número\> \<título\>"** que describe el siguiente punto propuesto

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
- El porcentaje se calcula como: `(tareas completadas / total tareas sprint) × 100`
- Usar emojis/caracteres Unicode:
  - ?? (verde) = tarea completada
  - ? (negro) = tarea pendiente
- Incluir el porcentaje en formato `"XX.X%"` al final de la barra

**Datos para el cálculo**:
El asistente rellenará los números y calculará el progreso guiándose por:
- `docs/sprint.md`
- `docs/sprint_tasks.md`
- `docs/daily.md`

---

## ?? Fichero Daily (`docs/daily.md`)

### Propósito:
`docs/daily.md` es el fichero **simplificado de seguimiento diario**.

### Contenido:
El fichero solo contiene **DOS cosas**:

1. **Última tarea realizada**: Número y descripción de la tarea completada  
   Formato: `"Hecho: <n> <descripción>"`

2. **Tarea actual**: Número y descripción de la siguiente tarea a realizar  
   Formato: `"Siguiente: <n> <descripción>"`

### Formato ejemplo:
```markdown
# Daily Log

Hecho: 3.01 [PLACEHOLDER: Nombre de tarea completada]
Siguiente: 4.00 [PLACEHOLDER: Nombre de siguiente tarea]
```

### Formato ejemplo (sin sprint activo):
```markdown
# Daily Log

Sprint v[PLACEHOLDER: versión] cerrado. Sin sprint activo.
```

### Actualización:
- El asistente actualiza este fichero **automáticamente** tras cada commit exitoso
- Al finalizar un sprint (release), el contenido de `daily.md` se actualiza a "Sprint vX.Y.Z cerrado. Sin sprint activo." y se archiva en los ficheros versionados del sprint. Se crea un nuevo `daily.md` para el siguiente sprint cuando este comienza

---

## ?? Estructura de Documentos del Sprint

### 1. `docs/sprint.md` - Sprint de Alto Nivel
**Contenido**:
- Objetivo del sprint
- Hitos principales
- Criterios de aceptación globales
- Comparación con versión anterior (opcional)
- Estado del sprint (historias/tareas completadas)

### 2. `docs/sprint_histories.md` - Historias de Usuario
**Contenido**:
- ID de historia
- Título y descripción
- Prioridad
- Criterios de aceptación
- Lista de tareas derivadas

**Ejemplo de historia**:
```markdown
### H1 - [PLACEHOLDER: Título de Historia]
**ID**: H1
**Prioridad**: CRÍTICA
**Descripción**: [PLACEHOLDER: Descripción detallada]

**Criterios de aceptación**:
- ? [PLACEHOLDER: Criterio 1]
- ? [PLACEHOLDER: Criterio 2]

**Tareas derivadas**: T1.1, T1.2, T1.3
```

### 3. `docs/sprint_tasks.md` - Tareas Detalladas
**Contenido**:
- ID de tarea
- Historia asociada
- Descripción
- Implementación (código de ejemplo, pasos)
- Archivos afectados
- Criterio de éxito

**Ejemplo de tarea**:
```markdown
### T1.1 - [PLACEHOLDER: Título de Tarea]
**ID**: T1.1
**Historia**: H1
**Descripción**: [PLACEHOLDER: Descripción detallada]

**Implementación**:
```[PLACEHOLDER: lenguaje]
// [PLACEHOLDER: código de ejemplo]
```

**Archivos afectados**: `[PLACEHOLDER: lista de archivos]`

**Criterio de éxito**: [PLACEHOLDER: condición de éxito]
```

---

## ?? Proceso de Ejecución de Tareas

Cada tarea debe seguir las **reglas de iteración**:

1. **Implementar** el código según la descripción en `docs/sprint_tasks.md`
2. **Validar** (compilar/ejecutar/probar según el proyecto):
   - [PLACEHOLDER: Comando de validación específico]
   - Ejemplos: `npm test`, `cargo test`, `msbuild`, `pytest`, `go test`
3. **Corregir errores** hasta obtener validación limpia (0 errores, 0 warnings)
4. **Commit** y actualizar `docs/daily.md` y `docs/commits.md`

---

## ?? Convenciones de Commits (`docs/commits.md`)

### Formato recomendado:
```
<tipo>(<ámbito>): <mensaje corto>

<mensaje largo opcional>

Refs: <issue/track-id>
```

### Tipos comunes:
- **feat**: Nueva funcionalidad
- **fix**: Corrección de errores
- **docs**: Cambios en documentación
- **style**: Formato, estilo, sin cambios funcionales
- **refactor**: Refactorización sin cambios en comportamiento
- **perf**: Mejoras de rendimiento
- **test**: Añadir o corregir tests
- **chore**: Tareas de mantenimiento

### Ejemplo:
```
feat(auth): implementar login con OAuth2

Añade soporte para autenticación vía Google y GitHub.
Incluye validación de tokens y refresh automático.

Refs: #123
```

---

## ?? Principios de la Metodología

Esta metodología se basa en:

1. **Iteraciones pequeñas y compilables/ejecutables**: Cada tarea debe dejar el proyecto en estado funcional
2. **Documentación continua**: Actualizar docs en cada commit
3. **Progreso visual**: Barra de progreso para feedback inmediato
4. **Automatización IA**: El asistente ejecuta el ciclo completo sin intervención manual innecesaria
5. **Trazabilidad**: Versionado de sprints, commits descriptivos, historial claro
6. **Calidad desde el inicio**: No soluciones temporales, solo código production-ready
7. **Flexibilidad tecnológica**: Aplicable a cualquier stack (backend, frontend, móvil, desktop, embedded, etc.)

---

## ?? Adaptación a Tu Proyecto

Para adaptar este template a tu proyecto específico:

1. **Copia este archivo** como base para tu `.github/copilot-instructions.md`
2. **Reemplaza todos los `[PLACEHOLDER]`** con:
   - Tecnologías específicas (lenguaje, framework, herramientas)
   - Comandos de build/validación reales
   - Estándares de código del proyecto
   - Preferencias del equipo
3. **Añade secciones específicas** si tu proyecto requiere:
   - Configuración de CI/CD
   - Despliegue automatizado
   - Gestión de base de datos
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

## ? Ejemplo de Uso en Diferentes Tecnologías

### Para un proyecto React + TypeScript:
- **Validación**: `npm run lint && npm test && npm run build`
- **Estilo**: ESLint + Prettier, componentes funcionales, hooks
- **Stack**: React 18, TypeScript 5, Vite, Vitest

### Para un proyecto Python FastAPI:
- **Validación**: `pytest && mypy . && black --check .`
- **Estilo**: PEP 8, type hints obligatorios, async/await
- **Stack**: FastAPI, SQLAlchemy, Pydantic, pytest

### Para un proyecto Rust:
- **Validación**: `cargo clippy && cargo test && cargo build`
- **Estilo**: rustfmt, Clippy warnings = errores
- **Stack**: Rust 1.70+, tokio, serde

### Para un proyecto Unity C#:
- **Validación**: Compilación Unity + tests unitarios
- **Estilo**: Convenciones Unity, componentes desacoplados
- **Stack**: Unity 2022 LTS, C# 9.0, URP

---

## ?? Recursos Adicionales

- Ver sprints completados: `docs/sprint_v*.md`
- Ver historias completadas: `docs/sprint_histories_v*.md`
- Ver tareas completadas: `docs/sprint_tasks_v*.md`
- Ver bugs/errores del sprint: `docs/sprint_fix_v*.md`
- Convenciones de commits: `docs/commits.md`
- Backlog de ítems futuros: `docs/backlog.md`

---

**Fin del Template de Metodología**

*Versión: 1.0*  
*Última actualización: [PLACEHOLDER: Fecha]*
