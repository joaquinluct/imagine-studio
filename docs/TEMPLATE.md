# Metodología de Desarrollo Ágil Asistido por IA - Template

> **Propósito**: Este documento define la metodología genérica de trabajo con asistentes IA (GitHub Copilot, ChatGPT, Claude, etc.) aplicable a cualquier proyecto de software.

**Nota para uso**: Adapta las secciones marcadas con `[PLACEHOLDER]` al contexto específico de tu proyecto (tecnología, lenguaje, sistema de build, estándares, etc.).

---

## 📚 LEER PRIMERO

**OBLIGATORIO**: Antes de comenzar cualquier sesión con el asistente IA, leer:  
📖 **[`docs/MAIN.md`](MAIN.md)** - [PLACEHOLDER: Documento fundamental del proyecto]

Este documento define los pilares fundamentales, arquitectura y principios del proyecto.  
**Todas las decisiones de implementación DEBEN alinearse con estos principios.**

---

## 📁 Estructura de Archivos de Trabajo

Archivos principales que el asistente debe consultar en cada sesión:

- **`docs/MAIN.md`** - [LEER PRIMERO] Fundamentos del proyecto (pilares, arquitectura, estándares)
- **`docs/sprint.md`** - Sprint actual con hitos y objetivos
- **`docs/daily.md`** - Última tarea completada y tarea actual en progreso (o "Sprint vX.Y.Z cerrado. Sin sprint activo." si no hay sprint activo)
- **`docs/commits.md`** - Convenciones de commits y mensajes
- **`docs/backlog.md`** - Repositorio de ítems no incluidos en el sprint actual
- **`docs/sprint_bugs.md`** - Tracking de bugs reportados (pendientes de resolución)
- **`docs/sprint_bug_attempts.md`** - **[DEBUGGING]** Log detallado de TODOS los intentos de solución para cada bug (fallidos, parciales, exitosos)
- **`docs/sprint_fix.md`** - Historial de bugs resueltos durante el sprint
- **`docs/sprint_deviations.md`** - Desviaciones y ajustes arquitectónicos durante sprint
- **`docs/sprint_ia_sessions.md`** - **[NUEVO]** Sesiones IA que superan 85% de consumo

---

## 🔄 Flujo de Trabajo por Sesión

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

## ⚙️ Preferencias de Calidad y Estándares

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

## 🛠️ Sección de Resolución de Problemas

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

## 🔁 Iteración Controlada (Proceso Obligatorio)

### Ejecución automática (sin necesidad de confirmación previa):

1. **Consulta**: El asistente consultará `docs/daily.md` y propondrá el siguiente punto a ejecutar, acompañado de una breve explicación (una frase) sobre en qué consiste

2. **Implementación**: Tras proponer el punto, el asistente implementará el cambio, validará (compilará/ejecutará/probará) y corregirá errores/warnings hasta lograr validación limpia

3. **Commit automático**: Si la validación queda limpia, el asistente realizará directamente el commit local siguiendo `docs/commits.md` y actualizará los ficheros necesarios (`docs/daily.md`, `docs/commits.md`, `README.md`, `.github/copilot-instructions.md`) **sin pedir permiso adicional**

4. **Informe**: El asistente informará de lo realizado y del siguiente punto propuesto y continuará el ciclo

### Excepciones:
Si aparece un **bloqueo técnico**, **decisión arquitectónica crítica** o **dependencia que impida avanzar**, el asistente pausará y solicitará instrucciones al propietario.

---

## 📝 Regla Estricta sobre Commits y Documentación

- **Requisito**: Siempre que el asistente realice un commit local como resultado de una iteración (es decir, la validación queda limpia), actualizará automáticamente `docs/daily.md` y `docs/commits.md` para reflejar el cambio **sin pedir confirmación adicional**
- El **push al repositorio remoto** NO se realizará automáticamente salvo instrucción explícita del propietario

---

## 📦 Versionado de los Ficheros del Sprint (Snapshots)

### Ficheros activos del sprint actual:
- `docs/sprint.md` - Sprint de alto nivel con hitos y objetivos
- `docs/sprint_histories.md` - Historias de usuario para el sprint
- `docs/sprint_tasks.md` - Tareas detalladas por historia (unidad mínima de trabajo e iteración)
- `docs/sprint_bugs.md` - Tracking de bugs reportados (pendientes de resolución)
- `docs/sprint_bug_attempts.md` - **[DEBUGGING]** Log detallado de TODOS los intentos de solución para cada bug (fallidos, parciales, exitosos)
- `docs/sprint_fix.md` - Historial de bugs resueltos durante el sprint
- `docs/sprint_deviations.md` - Desviaciones y ajustes arquitectónicos durante sprint
- **`docs/sprint_ia_sessions.md`** - **[NUEVO]** Sesiones IA que superan 85% de consumo

### Al final de un sprint (release):
1. El asistente **archivará** los ficheros a `docs/sprints/` renombrándolos con la versión:
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

2. El asistente **creará ficheros nuevos y vacíos** con los nombres activos para el siguiente sprint

**Estructura final**:
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
│   └── sprint_ia_sessions_v1.0.0.md (si aplica)
```
Esta política garantiza **trazabilidad** de los sprints completados y mantiene los ficheros activos pequeños y enfocados.

---

## 📋 Fichero Backlog (`docs/backlog.md`)

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

## 🐛 Fichero Sprint Bugs (`docs/sprint_bugs.md`)

### Propósito:
`docs/sprint_bugs.md` es el **contenedor inicial** de bugs reportados durante el sprint activo. Permite mantener trazabilidad de defectos encontrados y su estado actual.

### Contenido:
Cada entrada de bug debe incluir:
- **ID**: Identificador único del bug con prefijo BUG- (ej: BUG-001, BUG-002)
- **Título**: Descripción breve del error
- **Descripción**: Detalle del error, pasos para reproducir, comportamiento esperado vs observado
- **Prioridad**: Crítica/Alta/Media/Baja
- **Estado**: Reportado/En progreso/Pendiente validación usuario
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

### **Flujo de trabajo con doble validación Agente-Usuario**:
1. **Reporte**: Cuando el usuario reporte un bug, el asistente añadirá entrada en `docs/sprint_bugs.md` con ID **BUG-XXX** y estado "Reportado"
2. **En progreso**: Al comenzar a trabajar en el bug, actualizar estado a "En progreso"
3. **Implementación**: El asistente implementa el fix, valida (compila/ejecuta/prueba según proyecto) y crea commit
4. **Pendiente validación**: Actualizar estado a "Pendiente validación usuario" y **PAUSAR**
5. **Validación usuario**: El usuario ejecuta la aplicación y verifica que el bug está realmente resuelto
6. **Confirmación**: Si el usuario confirma que el fix funciona → mover a `docs/sprint_fix.md`
7. **Rechazado**: Si el usuario reporta que el bug persiste → volver a estado "En progreso" y continuar debugging

### **CRÍTICO**:
- El asistente **NO debe mover automáticamente** bugs de `sprint_bugs.md` a `sprint_fix.md` solo porque la validación automática (compilación/tests) sea limpia
- La validación del usuario es **OBLIGATORIA** para confirmar que el fix es efectivo en ejecución real
- **Excepciones** - Solo se permite mover automáticamente si:
  - El fix es trivial (typo, warning cosmético)
  - La prueba es determinista y verificable solo con validación automática (ej: error de sintaxis)
  - El usuario da confirmación explícita sin necesidad de prueba manual

### **Razón**:
**La validación automática limpia NO garantiza que el bug esté resuelto en runtime**.

Muchos bugs solo se manifiestan durante la ejecución:
- Race conditions
- Comportamiento de UI/UX
- Crashes bajo condiciones específicas
- Problemas de integración con sistemas externos
- Errores de lógica de negocio

### **Regla**:
- El asistente implementa el fix
- El asistente crea el commit
- El asistente actualiza estado a "Pendiente validación usuario"
- **PAUSA** y espera confirmación del usuario
- Solo tras confirmación → mover a `sprint_fix.md`

Los bugs pendientes se **archivan** como `docs/sprint_bugs_v<version>.md` al finalizar el sprint.

---

## 🔍 Fichero Sprint Bug Attempts (`docs/sprint_bug_attempts.md`)

### Propósito:
`docs/sprint_bug_attempts.md` es el registro detallado de **TODOS los intentos de solución** para cada bug reportado durante el sprint activo.

Permite realizar un seguimiento exhaustivo de las acciones realizadas, incluyendo:
- Enfoques fallidos
- Soluciones parciales
- Intentos exitosos

### Contenido:
Cada intento de solución debe incluir:
- **ID de Bug**: Identificador del bug asociado (ej: BUG-001)
- **Intento #**: Número secuencial del intento de solución
- **Descripción del intento**: Breve descripción de lo que se intentó
- **Resultado**: Éxito/Parcial/Fallido
- **Detalles del resultado**: Explicación breve del resultado; incluir mensajes de error si los hubiese
- **Fecha y hora**: Timestamp del intento
- **Archivos modificados**: Lista de archivos que fueron cambiados en el intento

### Formato ejemplo:
```markdown
### BUG-001 - [PLACEHOLDER: Título del bug]

**Intento #1**
- **Descripción**: [PLACEHOLDER: Qué se intentó]
- **Resultado**: Fallido
- **Detalles**: [PLACEHOLDER: Por qué falló, mensajes de error]
- **Fecha y hora**: 2025-01-15 10:00
- **Archivos modificados**: `[PLACEHOLDER: lista de archivos]`

**Intento #2**
- **Descripción**: [PLACEHOLDER: Segundo enfoque]
- **Resultado**: Parcial
- **Detalles**: [PLACEHOLDER: Qué funcionó y qué no]
- **Fecha y hora**: 2025-01-15 10:15
- **Archivos modificados**: `[PLACEHOLDER: lista de archivos]`

**Intento #3**
- **Descripción**: [PLACEHOLDER: Solución final]
- **Resultado**: Éxito
- **Detalles**: [PLACEHOLDER: Explicación de la solución exitosa]
- **Fecha y hora**: 2025-01-15 10:30
- **Archivos modificados**: `[PLACEHOLDER: lista de archivos]`
```

### **Flujo completo de resolución de bugs (OBLIGATORIO)**:

Cuando el asistente trabaje en la resolución de un bug, DEBE seguir este proceso:

#### 1. **Contexto inicial**
Antes de comenzar cualquier intento de solución, el asistente DEBE:
- Leer `docs/sprint_bugs.md` para conocer el bug actual
- Leer `docs/sprint_bug_attempts.md` para revisar intentos previos (si existen)
- Leer `.github/copilot-instructions.md` (o documento de instrucciones del proyecto)
- Leer `docs/sprint.md` y `docs/daily.md` para entender el contexto del sprint

#### 2. **Registro de intento**
ANTES de modificar código, el asistente DEBE:
- Añadir una nueva entrada en `docs/sprint_bug_attempts.md` con:
  - Intento # (secuencial)
  - Descripción clara del enfoque que se va a probar
  - Fecha y hora actual
- Esta entrada debe crearse **ANTES** de tocar código

#### 3. **Implementación**
El asistente implementa el cambio propuesto

#### 4. **Validación**
El asistente ejecuta la validación obligatoria del proyecto:
- **[PLACEHOLDER: Sistema de validación específico]**
  - Ejemplos: compilación, tests, linter, ejecución manual

#### 5. **Registro de resultado**
DESPUÉS de validar, el asistente DEBE:
- Actualizar la entrada en `docs/sprint_bug_attempts.md` con:
  - Resultado (Éxito/Parcial/Fallido)
  - Detalles del resultado (errores de compilación, comportamiento observado, etc.)
  - Archivos modificados en este intento

#### 6. **Validación usuario**
Si validación automática es limpia:
- Actualizar estado en `docs/sprint_bugs.md` a "Pendiente validación usuario"
- **PAUSAR** y esperar confirmación del usuario
- El asistente NO debe marcar el bug como resuelto automáticamente

#### 7. **Iteración**
Si el intento falla o es parcial:
- Volver al paso 2 con un nuevo intento
- **IMPORTANTE**: NO repetir intentos ya probados (consultar `sprint_bug_attempts.md`)

#### 8. **Resolución confirmada**
Solo cuando el usuario confirme que el fix funciona:
- Mover bug de `docs/sprint_bugs.md` a `docs/sprint_fix.md`
- Copiar el resumen de intentos exitosos en `docs/sprint_fix.md`
- Archivar la entrada de `docs/sprint_bug_attempts.md` para ese bug (mantener historial)

### **REGLA CRÍTICA**:
El asistente NO debe:
- ❌ Modificar código sin antes registrar el intento en `sprint_bug_attempts.md`
- ❌ Marcar un bug como resuelto solo porque la validación automática sea limpia
- ❌ Repetir intentos de solución ya probados y registrados en `sprint_bug_attempts.md`
- ❌ Olvidar actualizar el resultado del intento después de validar

### **Beneficios de este flujo**:
- ✅ Evita repetir soluciones fallidas
- ✅ Proporciona contexto histórico invaluable para futuros bugs similares
- ✅ Facilita la colaboración (otro desarrollador puede ver qué se ha intentado)
- ✅ Permite análisis post-mortem de bugs complejos
- ✅ Detecta patrones en errores recurrentes

### **IMPORTANTE**:
Siempre revisar `docs/sprint_bugs.md`, `docs/sprint_bug_attempts.md` y el código relacionado **ANTES** de comenzar a implementar cualquier solución para un bug.

Esto asegura que se comprende completamente el problema y se evita repetir intentos fallidos.

---

## ✅ Fichero Sprint Fix (`docs/sprint_fix.md`)

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
1. Bug reportado → Se registra en `docs/sprint_bugs.md` con ID **BUG-XXX**
2. Bug en progreso → Estado actualizado en `docs/sprint_bugs.md`
3. Bug resuelto → Se **mueve automáticamente** a `docs/sprint_fix.md` con:
   - ID cambiado a **FIX-XXX**
   - Estado "Resuelto"
   - Fecha de resolución
   - Hash del commit de resolución
   - Descripción de la solución implementada
4. Bug eliminado de `docs/sprint_bugs.md`
5. Los bugs resueltos se **archivan** como `docs/sprint_fix_v<version>.md` al finalizar el sprint

### Regla:
Los bugs se mueven automáticamente desde `docs/sprint_bugs.md` al resolverse. El asistente actualiza el ID (BUG-XXX → FIX-XXX), añade fecha de resolución, commit hash y descripción de la solución.

---

## 🔄 Sincronización con Metodología Genérica

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

## 📊 Formato de la Explicación Final de Cada Iteración

### Requisito:
Al final de cada iteración (cuando se informa lo realizado y el siguiente punto), la explicación debe contener **obligatoriamente** estas **3 secciones en orden**:

### 1. **Dos títulos numerados** siguiendo el esquema del Sprint:
- **"✅ Hecho: \<número\> \<título\>"** (ej: "✅ H1.3 - EntityManager implementado") que describe en breve lo completado
- **"📋 Siguiente: \<número\> \<título\>"** (ej: "📋 H1.4 - Tests unitarios Entity System") que describe el siguiente punto propuesto

### 2. **🎨 Visualización** - ¿Qué cambia visualmente al ejecutar? (OBLIGATORIO)

**⚠️ ESTA SECCIÓN ES OBLIGATORIA Y DEBE APARECER ANTES DE LA BARRA DE PROGRESO**

**Formato estricto**:
```markdown
### 🎨 Visualización:

**¿Algo nuevo que cambie la vista tras ejecución?**: **SÍ** ✅ / **NO** ❌

[SI ES "SÍ ✅"]
**Qué deberías ver al ejecutar** (F5 / npm run dev / etc):
1. ✅ [PLACEHOLDER: Cambio visual específico 1]
2. ✅ [PLACEHOLDER: Cambio visual específico 2]
3. 🆕 [PLACEHOLDER: Nuevo componente UI visible]

**Qué ha cambiado** (si aplica):
- 🔄 [PLACEHOLDER: Componente modificado]
- 🔄 [PLACEHOLDER: Comportamiento actualizado]

**Ausente** (si aplica):
- ❌ [PLACEHOLDER: Elemento que ya no se muestra]
- ❌ [PLACEHOLDER: Feature deshabilitada temporalmente]

[SI ES "NO ❌"]
**Razón**: [PLACEHOLDER: Explicación breve de por qué no hay cambios visuales]
Ejemplo: "Esta tarea implementa clases internas sin efecto en UI"
```

**Especificaciones**:
- **OBLIGATORIO**: La sección debe aparecer en TODAS las iteraciones (incluso si la respuesta es "NO ❌")
- Indicar claramente con **SÍ** ✅ o **NO** ❌ si hay cambios visuales
- Si es **SÍ** ✅: Listar QUÉ se debería ver al ejecutar (F5, ejecutable, navegador, etc.)
- Si es **NO** ❌: Explicar brevemente POR QUÉ (ej: "implementación interna", "tests unitarios", "refactorización sin UI")
- Incluir emojis para claridad:
  - ✅ = Nuevo/Visible
  - 🔄 = Modificado/Actualizado
  - ❌ = Ausente/Deshabilitado
  - 🆕 = Nuevo componente añadido
- **POSICIÓN**: Esta sección DEBE aparecer DESPUÉS de "Hecho/Siguiente" y ANTES de la barra de progreso

**Ejemplos según tipo de tarea**:

**Ejemplo 1 - Tarea CON cambios visuales** (ej: H4.1 - Panel Hierarchy):
```markdown
### 🎨 Visualización:

**¿Algo nuevo que cambie la vista tras ejecución?**: **SÍ** ✅

**Qué deberías ver al ejecutar** (F5 en Visual Studio):
1. ✅ Panel "Hierarchy" visible en la parte izquierda de la ventana
2. ✅ Árbol expandible con objetos: Scene Root → Camera, Lights, Geometry
3. 🆕 Posibilidad de hacer dock/undock del panel arrastrándolo

**Qué ha cambiado**:
- 🔄 El layout del editor ahora incluye un panel funcional (no solo ventana vacía)

**Ausente**:
- ❌ Demo Window de ImGui ya no aparece (reemplazado por Hierarchy)
```

**Ejemplo 2 - Tarea SIN cambios visuales** (ej: H1.3 - EntityManager):
```markdown
### 🎨 Visualización:

**¿Algo nuevo que cambie la vista tras ejecución?**: **NO** ❌

**Razón**: Esta tarea implementa clases internas (EntityManager, Entity) que no tienen representación visual directa. Los cambios visuales aparecerán en H4 cuando el panel Hierarchy muestre entities reales en lugar de placeholder.
```

### 3. **Barra de progreso visual del sprint** mostrando el avance de tareas completadas:

**Formato**: Barra horizontal con fondo negro (⬛), progreso verde (🟩), bordes blancos (│), y porcentaje centrado

**Ejemplo** con 2 de 15 tareas (13.3%):
```
┌────────────────────────────────────────────────────────────────────┐
│🟩🟩⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛ 13.3%│
└────────────────────────────────────────────────────────────────────┘
```

**Especificaciones**:
- La barra debe ocupar todo el ancho disponible del prompt (aprox. 70-80 caracteres)
- El porcentaje se calcula como: `(tareas completadas / total tareas sprint) × 100`
- Usar emojis/caracteres Unicode:
  - 🟩 (verde) = tarea completada
  - ⬛ (negro) = tarea pendiente
- Incluir el porcentaje en formato `"XX.X%"` al final de la barra

**Datos para el cálculo**:
El asistente rellenará los números y calculará el progreso guiándose por:
- `docs/sprint.md`
- `docs/sprint_tasks.md`
- `docs/daily.md`

---

## 📅 Fichero Daily (`docs/daily.md`)

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

## 📑 Estructura de Documentos del Sprint

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
- ✅ [PLACEHOLDER: Criterio 1]
- ✅ [PLACEHOLDER: Criterio 2]

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

### 4. `docs/sprint_deviations.md` - Desviaciones del Sprint (NUEVO)

**Propósito**: Registrar **desviaciones**, **ajustes arquitectónicos** y **tareas emergentes** que surgen **durante** la ejecución del sprint.

**Contenido**:
- ID de desviación (DEV-XXX)
- Tipo (Ajuste Arquitectónico | Tarea Emergente | Deuda Técnica | Bloqueador)
- Punto de detección (en qué tarea se detectó)
- Prioridad
- Contexto del problema
- Justificación de por qué NO pasó a backlog
- Decisión tomada
- Tareas derivadas (DEV-XXX.Y)
- Resultado
- Impacto en sprint
- Lecciones aprendidas

**Ejemplo de desviación**:
```markdown
## DEV-001: [PLACEHOLDER: Título de la desviación]

**Tipo**: Ajuste Arquitectónico
**Detectado en**: H2.3 - [PLACEHOLDER: nombre de tarea]
**Fecha**: 2025-01-18
**Prioridad**: CRÍTICA

### Contexto:
[PLACEHOLDER: Descripción del problema arquitectónico/técnico detectado]

### ¿Por qué NO pasó a backlog?
- ❌ **Bloqueante para H4**: [PLACEHOLDER: razón específica]
- ❌ **Viola pilares del proyecto**: [PLACEHOLDER: referencia a MAIN.md]
- ✅ **Impacto inmediato**: [PLACEHOLDER: consecuencias si no se arregla]

### Decisión:
Pausar H4 temporalmente e implementar [PLACEHOLDER: solución] inmediatamente

### Tareas Derivadas:
- DEV-001.1: ✅ [PLACEHOLDER: subtarea 1]
- DEV-001.2: ✅ [PLACEHOLDER: subtarea 2]

### Resultado:
✅ **Completado** (commit: [PLACEHOLDER: hash])

### Impacto en Sprint:
- H4.1-H4.5: [PLACEHOLDER: impacto positivo/negativo]
- Progreso: [PLACEHOLDER: antes/después]

### Lecciones Aprendidas:
[PLACEHOLDER: reflexión para evitar en futuros sprints]
```

**Criterios de Uso**:
- ✅ Bloqueante para sprint actual
- ✅ Viola pilares del proyecto (docs/MAIN.md o estándares del proyecto)
- ✅ Impacto inmediato en tareas en progreso o próximas inmediatas

**Diferencia con backlog**:
| Archivo | Propósito | Cuándo usar |
|---------|-----------|-------------|
| `sprint_tasks.md` | Tareas **planificadas** del sprint | Al inicio del sprint |
| `sprint_deviations.md` | Tareas **emergentes/ajustes** durante sprint | Durante ejecución |
| `backlog.md` | Ítems **fuera** del sprint actual | Planificación futura |

**Versionado**: Archivado como `sprint_deviations_vX.Y.Z.md` al finalizar sprint

---

### 5. `docs/sprint_ia_sessions.md` - Sesiones IA (NUEVO)

**Propósito**: Registrar sesiones de trabajo con asistentes IA que superan el **85% de consumo de tokens/contexto**.

**Contenido**:
- ID de sesión (Sesión #XXX)
- LLM utilizado (nombre + versión)
- Fecha/hora y usuario orquestador
- Consumo de tokens/contexto (actual/límite/porcentaje)
- Prompt completo ejecutado
- Contexto de sesión (sprint, tareas completadas, pendientes)
- Recomendaciones para próxima sesión

**Ejemplo de sesión**:
```markdown
## Sesión IA #001 - Sprint vX.Y.Z

**LLM**: [PLACEHOLDER: Nombre LLM] ([PLACEHOLDER: Versión])
**Fecha/Hora**: [YYYY-MM-DD HH:MM:SS UTC]
**Usuario (Orquestador)**: [PLACEHOLDER: username]
**Consumo**: [actual] / [límite] ([porcentaje]%)
**Estado**: ⚠️ LÍMITE 85% ALCANZADO - Sesión cerrada

### Resumen de la Sesión:

Esta sesión ha sido muy productiva:

1. ✅ **[PLACEHOLDER: Logro principal 1]**
   - [Sub-logro 1]
   - [Sub-logro 2]

2. ✅ **[PLACEHOLDER: Logro principal 2]**
   - [Sub-logro 1]
   - [Sub-logro 2]

3. ✅ **Validación completa**
   - Validación automática: [resultado]
   - Validación usuario: [resultado si aplica]

### Prompt Ejecutado:
```
[PLACEHOLDER: Prompt completo]
```

### Contexto:
- **Sprint**: vX.Y.Z ([nombre])
- **Completadas**: [lista tareas]
- **Pendientes**: [lista tareas]

### Próxima Sesión:
- Continuar: [referencia daily.md]
- Leer: [archivos contexto]
- Validar: [aspectos verificar]
```

**Flujo Automático al 85%**:

1. **Detectar umbral**: Consumo > 85%
2. **Completar tarea**: Implementar + validar + commit
3. **Registrar**: Crear entrada en `sprint_ia_sessions.md`
4. **Notificar**:
   ```
   ⚠️ LÍMITE DE SESIÓN (>85%)
   ✅ Tarea completada: [nombre]
   ✅ Commit: [hash]
   📊 Consumo: XXX/YYY (ZZ%)
   🔄 Cerrar sesión y abrir nuevo prompt
   ```
5. **PAUSAR**: Esperar cierre usuario

**Versionado**: Archivado como `docs/sprints/sprint_ia_sessions_vX.Y.Z.md` al finalizar sprint

---

## ✔️ Proceso de Ejecución de Tareas

Cada tarea debe seguir las **reglas de iteración**:

1. **Implementar** el código según la descripción en `docs/sprint_tasks.md`
2. **Validar** (compilar/ejecutar/probar según el proyecto):
   - [PLACEHOLDER: Comando de validación específico]
   - Ejemplos: `npm test`, `cargo test`, `msbuild`, `pytest`, `go test`
3. **Corregir errores** hasta obtener validación limpia (0 errores, 0 warnings)
4. **Commit** y actualizar `docs/daily.md` y `docs/commits.md`

---

## 💬 Convenciones de Commits (`docs/commits.md`)

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

## 🎯 Principios de la Metodología

Esta metodología se basa en:

1. **Iteraciones pequeñas y compilables/ejecutables**: Cada tarea debe dejar el proyecto en estado funcional
2. **Documentación continua**: Actualizar docs en cada commit
3. **Progreso visual**: Barra de progreso para feedback inmediato
4. **Automatización IA**: El asistente ejecuta el ciclo completo sin intervención manual innecesaria
5. **Trazabilidad**: Versionado de sprints, commits descriptivos, historial claro
6. **Calidad desde el inicio**: No soluciones temporales, solo código production-ready
7. **Flexibilidad tecnológica**: Aplicable a cualquier stack (backend, frontend, móvil, desktop, embedded, etc.)

---

## 🔧 Adaptación a Tu Proyecto

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

## 💡 Ejemplo de Uso en Diferentes Tecnologías

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

## 📚 Recursos Adicionales

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

