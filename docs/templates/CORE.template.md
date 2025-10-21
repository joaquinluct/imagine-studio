# Metodología de Desarrollo Ágil Asistido por IA - CORE

> **Propósito**: Documento fundamental de la metodología. **Leer UNA VEZ** al inicio de colaboración con el asistente IA.

> **Adaptación**: Reemplazar todos los `[PLACEHOLDER]` con valores específicos de tu proyecto.

---

## ?? PRINCIPIOS FUNDAMENTALES

Esta metodología se basa en:

1. **Iteraciones pequeñas y funcionales**: Cada tarea deja el proyecto en estado [PLACEHOLDER: compilable/ejecutable/funcional]
2. **Documentación continua**: Actualizar docs en cada commit
3. **Automatización IA**: El asistente ejecuta el ciclo completo sin intervención manual innecesaria
4. **Calidad desde el inicio**: No soluciones temporales, solo código [PLACEHOLDER: production-ready/AAA/enterprise-grade]
5. **Trazabilidad**: Versionado de sprints, commits descriptivos, historial claro
6. **Doble validación**: [PLACEHOLDER: Compilación/Tests/Lint] limpia + validación usuario para bugs

---

## ?? ESTRUCTURA DE ARCHIVOS

### Archivos Activos del Sprint

| Archivo | Propósito | Frecuencia de consulta |
|---------|-----------|------------------------|
| `docs/MAIN.md` | [PLACEHOLDER: Pilares del proyecto] | Una vez (inicio) |
| `docs/daily.md` | Última tarea + actual | Cada sesión |
| `docs/sprint.md` | Objetivo sprint actual | Al inicio sprint |
| `docs/sprint_histories.md` | Historias de usuario | Cuando necesario |
| `docs/sprint_tasks.md` | Tareas detalladas | Cuando necesario |
| `docs/sprint_commits.md` | Convenciones commits | Referencia |
| `docs/sprint_bugs.md` | Bugs pendientes | Durante debugging |
| `docs/sprint_bug_attempts.md` | Intentos de solución | Durante debugging |
| `docs/sprint_fix.md` | Bugs resueltos | Referencia |
| `docs/sprint_deviations.md` | Desviaciones sprint | Cuando bloqueo |
| `docs/sprint_ia_sessions.md` | Sesiones IA (>85%) | Al 85% tokens |
| `docs/backlog.md` | Ítems fuera sprint | Planificación |

---

## ?? FLUJO DE TRABAJO COMPLETO

### Ciclo por Iteración (6 Pasos)

```
???????????????????????????????????????????????????????????????
? 1. LEER                                                     ?
?    - Consultar docs/daily.md                                ?
?    - Ver última tarea completada + tarea actual             ?
???????????????????????????????????????????????????????????????
                            ?
???????????????????????????????????????????????????????????????
? 2. COMPARAR                                                 ?
?    - Revisar código fuente actual                           ?
?    - Entender estado del proyecto                           ?
???????????????????????????????????????????????????????????????
                            ?
???????????????????????????????????????????????????????????????
? 3. PROPONER                                                 ?
?    - Asistente propone siguiente paso                       ?
?    - Una frase explicando qué se hará                       ?
???????????????????????????????????????????????????????????????
                            ?
???????????????????????????????????????????????????????????????
? 4. IMPLEMENTAR                                              ?
?    - Si usuario confirma (o auto-ejecución)                 ?
?    - Implementar cambio según sprint_tasks.md               ?
???????????????????????????????????????????????????????????????
                            ?
???????????????????????????????????????????????????????????????
? 5. VALIDAR (OBLIGATORIO)                                    ?
?    - [PLACEHOLDER: Compilar/ejecutar/probar]                ?
?    - Corregir errores hasta validación limpia               ?
?    - [PLACEHOLDER: 0 errores, 0 warnings / tests passing]   ?
???????????????????????????????????????????????????????????????
                            ?
???????????????????????????????????????????????????????????????
? 6. COMMIT + DOCS                                            ?
?    - Crear commit local                                     ?
?    - Actualizar daily.md, sprint_commits.md                 ?
?    - README.md (si aplica)                                  ?
???????????????????????????????????????????????????????????????
                            ?
                   REPETIR DESDE 1
```

### Excepciones al Flujo Automático

El asistente PAUSARÁ si:
- ? Bloqueo técnico (no se puede avanzar)
- ? Decisión arquitectónica crítica requerida
- ? Dependencia externa no disponible
- ? Validación de usuario necesaria (bugs)

---

## ?? VERSIONADO DE SPRINTS

### Archivos Versionados al Cerrar Sprint

Al finalizar un sprint (release), el asistente archivará TODOS los ficheros:

```[PLACEHOLDER: shell language - bash/powershell/cmd]
# Crear carpeta si no existe
[PLACEHOLDER: mkdir/New-Item command] -Path "docs/sprints"

# Mover archivos versionados
[PLACEHOLDER: move/mv command] "docs/sprint_v<version>.md" "docs/sprints/"
[PLACEHOLDER: move/mv command] "docs/sprint_histories_v<version>.md" "docs/sprints/"
[PLACEHOLDER: move/mv command] "docs/sprint_tasks_v<version>.md" "docs/sprints/"
[PLACEHOLDER: move/mv command] "docs/sprint_bugs_v<version>.md" "docs/sprints/"
[PLACEHOLDER: move/mv command] "docs/sprint_bug_attempts_v<version>.md" "docs/sprints/"
[PLACEHOLDER: move/mv command] "docs/sprint_fix_v<version>.md" "docs/sprints/"
[PLACEHOLDER: move/mv command] "docs/sprint_deviations_v<version>.md" "docs/sprints/"
[PLACEHOLDER: move/mv command] "docs/sprint_ia_sessions_v<version>.md" "docs/sprints/"
```

### Estructura Final

```
docs/
??? sprints/
    ??? sprint_v1.0.0.md
    ??? sprint_histories_v1.0.0.md
    ??? sprint_tasks_v1.0.0.md
    ??? sprint_bugs_v1.0.0.md
    ??? sprint_bug_attempts_v1.0.0.md
    ??? sprint_fix_v1.0.0.md
    ??? sprint_deviations_v1.0.0.md
    ??? sprint_ia_sessions_v1.0.0.md
```

---

## ?? FORMATO DE DOCUMENTOS

### 1. `docs/sprint.md` - Sprint de Alto Nivel

**Contenido**:
- Objetivo del sprint
- Hitos principales
- Criterios de aceptación globales
- Comparación con versión anterior
- Estado del sprint (historias/tareas completadas)
- Barra de progreso visual

### 2. `docs/sprint_histories.md` - Historias de Usuario

**Contenido**:
- ID de historia (H1, H2, H3...)
- Título y descripción
- Prioridad (Crítica/Alta/Media/Baja)
- Criterios de aceptación
- Lista de tareas derivadas

**Formato**:
```markdown
### H1 - [PLACEHOLDER: Título de Historia]
**ID**: H1
**Prioridad**: [PLACEHOLDER: CRÍTICA/ALTA/MEDIA/BAJA]
**Descripción**: [PLACEHOLDER: Detalle completo]

**Criterios de aceptación**:
- ? [PLACEHOLDER: Criterio 1]
- ? [PLACEHOLDER: Criterio 2]

**Tareas derivadas**: H1.1, H1.2, H1.3
```

### 3. `docs/sprint_tasks.md` - Tareas Detalladas

**Contenido**:
- ID de tarea (H1.1, H1.2...)
- Historia asociada
- Descripción
- Implementación (código ejemplo, pasos)
- Archivos afectados
- Criterio de éxito

**Formato**:
```markdown
### H1.1 - [PLACEHOLDER: Título de Tarea]
**Estado**: ? Pendiente / ? Completada
**Historia**: H1
**Descripción**: [PLACEHOLDER: Detalle completo]

**Implementación**:
```[PLACEHOLDER: language]
// [PLACEHOLDER: código ejemplo]
```

**Archivos afectados**: [PLACEHOLDER: lista]
**Criterio de éxito**: [PLACEHOLDER: condición]
```

### 4. `docs/daily.md` - Seguimiento Diario

**Contenido** (2 líneas):
```markdown
Hecho: H1.3 - [PLACEHOLDER: Tarea completada]
Siguiente: H1.4 - [PLACEHOLDER: Próxima tarea]
```

**Sin sprint activo**:
```markdown
Sprint v[PLACEHOLDER: versión] cerrado. Sin sprint activo.
```

---

## ?? FORMATO DE ITERACIÓN FINAL

Al final de cada iteración, reportar con **3 secciones obligatorias**:

### 1. Títulos (Hecho/Siguiente)

```markdown
? Hecho: H1.3 - [PLACEHOLDER: Tarea completada]
?? Siguiente: H1.4 - [PLACEHOLDER: Próxima tarea]
```

### 2. Visualización (OBLIGATORIO)

```markdown
### ??? Visualización:

**¿Algo nuevo que cambie la vista tras ejecución?**: **SÍ** ? / **NO** ?

[SI ES "SÍ ?"]
**Qué deberías ver al ejecutar** ([PLACEHOLDER: F5 / npm start / etc.]):
1. ? [PLACEHOLDER: Cambio visual específico 1]
2. ? [PLACEHOLDER: Cambio visual específico 2]

**Qué ha cambiado**:
- ?? [PLACEHOLDER: Componente modificado]

**Ausente**:
- ? [PLACEHOLDER: Elemento que ya no se muestra]

[SI ES "NO ?"]
**Razón**: [PLACEHOLDER: Por qué no hay cambios visuales]
```

**Posición**: DESPUÉS de títulos, ANTES de barra de progreso

### 3. Barra de Progreso

```
+--------------------------------------------------------------------+
?????????????????????????????????????????????????????? 18.8%?
+--------------------------------------------------------------------+
```

**Cálculo**: `(tareas completadas / total tareas sprint) × 100`

---

## ?? PREFERENCIAS DE CALIDAD

### Estándares de Código

[PLACEHOLDER: Preferencias específicas del proyecto]

Ejemplos:
1. **Hacer bien desde el principio**: Evitar partes temporales o incompletas
2. **Cada módulo cumple estándares [PLACEHOLDER: AAA/Enterprise/Production]**: Desde su concepción
3. **Validación antes de commit**: Siempre [PLACEHOLDER: compilar/ejecutar/probar]
4. **Estilo consistente**: Seguir convenciones del proyecto

### [PLACEHOLDER: Codificación de Archivos / Linting / Formatting]

Ejemplos:
- **C++**: UTF-8 con BOM + CRLF (Windows)
- **Python**: UTF-8 + LF (Unix), Black formatter
- **JavaScript**: UTF-8 + LF, Prettier + ESLint

---

## ?? REGLAS CRÍTICAS

### 1. NUNCA Modificar [PLACEHOLDER: External Dependencies Path]

**Razones**:
1. Imposibilita actualizar la biblioteca en el futuro
2. Oculta bugs en nuestro código (tapa síntomas, no arregla causas)
3. Rompe la reproducibilidad del proyecto

**Política**:
- TODO el código de [PLACEHOLDER: dependencias externas] debe permanecer **INTACTO**
- Si necesitas personalizar: crear **wrappers** en `src/`
- Si encuentras bug relacionado: investigar **NUESTRO** código primero

### 2. [PLACEHOLDER: Validación] Obligatoria

**Requisito**: [PLACEHOLDER: Número de validaciones] limpias antes de commit:

```[PLACEHOLDER: shell language]
# Validación 1 - [PLACEHOLDER: Nombre]
[PLACEHOLDER: comando validación 1]

# Validación 2 - [PLACEHOLDER: Nombre] (si aplica)
[PLACEHOLDER: comando validación 2]
```

**Criterio de éxito**: [PLACEHOLDER: 0 errores, 0 warnings / All tests pass]

### 3. Commits y Documentación

**Regla**: Siempre actualizar automáticamente:
- `docs/daily.md`
- `docs/sprint_commits.md`
- `README.md` (cuando aplique)

**NO** hacer push automático (solo local commits)

---

## ?? TROUBLESHOOTING

[PLACEHOLDER: Problemas comunes y soluciones específicas del proyecto]

### Ejemplos por Tecnología

**C++/CMake**:
1. Build fails ? Check `CMakeLists.txt`, verify files included
2. LNK2019 error ? Check subsystem (CONSOLE vs WINDOWS)

**Python**:
1. Import error ? Check `requirements.txt`, verify virtual env
2. Test fails ? Run `pytest -v` for detailed output

**Node.js**:
1. Build fails ? Run `npm install`, check `package.json`
2. Lint errors ? Run `npm run lint -- --fix`

---

## ?? DOCUMENTOS RELACIONADOS

- **Bugs**: [`docs/methodology/BUGS.md`](BUGS.md) - Bug management completo
- **Sesiones IA**: [`docs/methodology/SESSIONS.md`](SESSIONS.md) - Gestión al 85%
- **Desviaciones**: [`docs/methodology/DEVIATIONS.md`](DEVIATIONS.md) - Sprint deviations
- **Workflow**: [`docs/methodology/WORKFLOW.md`](WORKFLOW.md) - Flujo detallado

---

**Versión**: 2.0  
**Última actualización**: [PLACEHOLDER: Fecha]  
**Proyecto**: [PLACEHOLDER: Nombre del proyecto]  
**Tecnología**: [PLACEHOLDER: Stack principal]
