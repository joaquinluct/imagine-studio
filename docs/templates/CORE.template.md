# Metodolog�a de Desarrollo �gil Asistido por IA - CORE

> **Prop�sito**: Documento fundamental de la metodolog�a. **Leer UNA VEZ** al inicio de colaboraci�n con el asistente IA.

> **Adaptaci�n**: Reemplazar todos los `[PLACEHOLDER]` con valores espec�ficos de tu proyecto.

---

## ?? PRINCIPIOS FUNDAMENTALES

Esta metodolog�a se basa en:

1. **Iteraciones peque�as y funcionales**: Cada tarea deja el proyecto en estado [PLACEHOLDER: compilable/ejecutable/funcional]
2. **Documentaci�n continua**: Actualizar docs en cada commit
3. **Automatizaci�n IA**: El asistente ejecuta el ciclo completo sin intervenci�n manual innecesaria
4. **Calidad desde el inicio**: No soluciones temporales, solo c�digo [PLACEHOLDER: production-ready/AAA/enterprise-grade]
5. **Trazabilidad**: Versionado de sprints, commits descriptivos, historial claro
6. **Doble validaci�n**: [PLACEHOLDER: Compilaci�n/Tests/Lint] limpia + validaci�n usuario para bugs

---

## ?? ESTRUCTURA DE ARCHIVOS

### Archivos Activos del Sprint

| Archivo | Prop�sito | Frecuencia de consulta |
|---------|-----------|------------------------|
| `docs/MAIN.md` | [PLACEHOLDER: Pilares del proyecto] | Una vez (inicio) |
| `docs/daily.md` | �ltima tarea + actual | Cada sesi�n |
| `docs/sprint.md` | Objetivo sprint actual | Al inicio sprint |
| `docs/sprint_histories.md` | Historias de usuario | Cuando necesario |
| `docs/sprint_tasks.md` | Tareas detalladas | Cuando necesario |
| `docs/sprint_commits.md` | Convenciones commits | Referencia |
| `docs/sprint_bugs.md` | Bugs pendientes | Durante debugging |
| `docs/sprint_bug_attempts.md` | Intentos de soluci�n | Durante debugging |
| `docs/sprint_fix.md` | Bugs resueltos | Referencia |
| `docs/sprint_deviations.md` | Desviaciones sprint | Cuando bloqueo |
| `docs/sprint_ia_sessions.md` | Sesiones IA (>85%) | Al 85% tokens |
| `docs/backlog.md` | �tems fuera sprint | Planificaci�n |

---

## ?? FLUJO DE TRABAJO COMPLETO

### Ciclo por Iteraci�n (6 Pasos)

```
???????????????????????????????????????????????????????????????
? 1. LEER                                                     ?
?    - Consultar docs/daily.md                                ?
?    - Ver �ltima tarea completada + tarea actual             ?
???????????????????????????????????????????????????????????????
                            ?
???????????????????????????????????????????????????????????????
? 2. COMPARAR                                                 ?
?    - Revisar c�digo fuente actual                           ?
?    - Entender estado del proyecto                           ?
???????????????????????????????????????????????????????????????
                            ?
???????????????????????????????????????????????????????????????
? 3. PROPONER                                                 ?
?    - Asistente propone siguiente paso                       ?
?    - Una frase explicando qu� se har�                       ?
???????????????????????????????????????????????????????????????
                            ?
???????????????????????????????????????????????????????????????
? 4. IMPLEMENTAR                                              ?
?    - Si usuario confirma (o auto-ejecuci�n)                 ?
?    - Implementar cambio seg�n sprint_tasks.md               ?
???????????????????????????????????????????????????????????????
                            ?
???????????????????????????????????????????????????????????????
? 5. VALIDAR (OBLIGATORIO)                                    ?
?    - [PLACEHOLDER: Compilar/ejecutar/probar]                ?
?    - Corregir errores hasta validaci�n limpia               ?
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

### Excepciones al Flujo Autom�tico

El asistente PAUSAR� si:
- ? Bloqueo t�cnico (no se puede avanzar)
- ? Decisi�n arquitect�nica cr�tica requerida
- ? Dependencia externa no disponible
- ? Validaci�n de usuario necesaria (bugs)

---

## ?? VERSIONADO DE SPRINTS

### Archivos Versionados al Cerrar Sprint

Al finalizar un sprint (release), el asistente archivar� TODOS los ficheros:

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
- Criterios de aceptaci�n globales
- Comparaci�n con versi�n anterior
- Estado del sprint (historias/tareas completadas)
- Barra de progreso visual

### 2. `docs/sprint_histories.md` - Historias de Usuario

**Contenido**:
- ID de historia (H1, H2, H3...)
- T�tulo y descripci�n
- Prioridad (Cr�tica/Alta/Media/Baja)
- Criterios de aceptaci�n
- Lista de tareas derivadas

**Formato**:
```markdown
### H1 - [PLACEHOLDER: T�tulo de Historia]
**ID**: H1
**Prioridad**: [PLACEHOLDER: CR�TICA/ALTA/MEDIA/BAJA]
**Descripci�n**: [PLACEHOLDER: Detalle completo]

**Criterios de aceptaci�n**:
- ? [PLACEHOLDER: Criterio 1]
- ? [PLACEHOLDER: Criterio 2]

**Tareas derivadas**: H1.1, H1.2, H1.3
```

### 3. `docs/sprint_tasks.md` - Tareas Detalladas

**Contenido**:
- ID de tarea (H1.1, H1.2...)
- Historia asociada
- Descripci�n
- Implementaci�n (c�digo ejemplo, pasos)
- Archivos afectados
- Criterio de �xito

**Formato**:
```markdown
### H1.1 - [PLACEHOLDER: T�tulo de Tarea]
**Estado**: ? Pendiente / ? Completada
**Historia**: H1
**Descripci�n**: [PLACEHOLDER: Detalle completo]

**Implementaci�n**:
```[PLACEHOLDER: language]
// [PLACEHOLDER: c�digo ejemplo]
```

**Archivos afectados**: [PLACEHOLDER: lista]
**Criterio de �xito**: [PLACEHOLDER: condici�n]
```

### 4. `docs/daily.md` - Seguimiento Diario

**Contenido** (2 l�neas):
```markdown
Hecho: H1.3 - [PLACEHOLDER: Tarea completada]
Siguiente: H1.4 - [PLACEHOLDER: Pr�xima tarea]
```

**Sin sprint activo**:
```markdown
Sprint v[PLACEHOLDER: versi�n] cerrado. Sin sprint activo.
```

---

## ?? FORMATO DE ITERACI�N FINAL (MANDATORY - ALL 3 SECTIONS)

Al final de cada iteraci�n, reportar con **3 secciones MANDATORY**:

### 1. T�tulos (MANDATORY)

```markdown
✅ Hecho: H1.3 - [PLACEHOLDER: Tarea completada]
🔜 Siguiente: H1.4 - [PLACEHOLDER: Pr�xima tarea]
```

### 2. Visualizaci�n (MANDATORY) ⚠️ **NEVER SKIP THIS SECTION**

```markdown
### 🎨 Visualizaci�n:

**�Algo nuevo que cambie la vista tras ejecuci�n?**: **S�** ✅ / **NO** ❌

[SI ES "S� ✅"]
**Qu� deber�as ver al ejecutar** ([PLACEHOLDER: F5 / npm start / etc.]):
1. ✅ [PLACEHOLDER: Cambio visual espec�fico 1]
2. ✅ [PLACEHOLDER: Cambio visual espec�fico 2]

**Qu� ha cambiado**:
- 🔄 [PLACEHOLDER: Componente modificado]

**Ausente**:
- ❌ [PLACEHOLDER: Elemento que ya no se muestra]

[SI ES "NO ❌"]
**Raz�n**: [PLACEHOLDER: Por qu� no hay cambios visuales]
**Visualizaci�n vendr� en**: [PLACEHOLDER: Futuras tareas que mostrar�n cambios]
```

**Posici�n**: DESPU�S de t�tulos, ANTES de barra de progreso  
**Why MANDATORY**: 
- Sets clear expectations (user knows if they should see changes)
- Prevents wasted time (user won't execute expecting something that isn't there)
- Documents when visual features will be available
- [PLACEHOLDER: AAA/Professional] standard: Always document "What will the user see?"

### 3. Barra de Progreso (MANDATORY)

```
+--------------------------------------------------------------------+
███████████████████████ 18.8%⬛
+--------------------------------------------------------------------+
```

**C�lculo**: `(tareas completadas / total tareas sprint) � 100`

---

## ?? PREFERENCIAS DE CALIDAD

### Est�ndares de C�digo

[PLACEHOLDER: Preferencias espec�ficas del proyecto]

Ejemplos:
1. **Hacer bien desde el principio**: Evitar partes temporales o incompletas
2. **Cada m�dulo cumple est�ndares [PLACEHOLDER: AAA/Enterprise/Production]**: Desde su concepci�n
3. **Validaci�n antes de commit**: Siempre [PLACEHOLDER: compilar/ejecutar/probar]
4. **Estilo consistente**: Seguir convenciones del proyecto

### [PLACEHOLDER: Codificaci�n de Archivos / Linting / Formatting]

Ejemplos:
- **C++**: UTF-8 con BOM + CRLF (Windows)
- **Python**: UTF-8 + LF (Unix), Black formatter
- **JavaScript**: UTF-8 + LF, Prettier + ESLint

---

## ?? REGLAS CR�TICAS

### 1. NUNCA Modificar [PLACEHOLDER: External Dependencies Path]

**Razones**:
1. Imposibilita actualizar la biblioteca en el futuro
2. Oculta bugs en nuestro c�digo (tapa s�ntomas, no arregla causas)
3. Rompe la reproducibilidad del proyecto

**Pol�tica**:
- TODO el c�digo de [PLACEHOLDER: dependencias externas] debe permanecer **INTACTO**
- Si necesitas personalizar: crear **wrappers** en `src/`
- Si encuentras bug relacionado: investigar **NUESTRO** c�digo primero

### 2. [PLACEHOLDER: Validaci�n] Obligatoria

**Requisito**: [PLACEHOLDER: N�mero de validaciones] limpias antes de commit:

```[PLACEHOLDER: shell language]
# Validaci�n 1 - [PLACEHOLDER: Nombre]
[PLACEHOLDER: comando validaci�n 1]

# Validaci�n 2 - [PLACEHOLDER: Nombre] (si aplica)
[PLACEHOLDER: comando validaci�n 2]
```

**Criterio de �xito**: [PLACEHOLDER: 0 errores, 0 warnings / All tests pass]

### 3. Commits y Documentaci�n

**Regla**: Siempre actualizar autom�ticamente:
- `docs/daily.md`
- `docs/sprint_commits.md`
- `README.md` (cuando aplique)

**NO** hacer push autom�tico (solo local commits)

---

## ?? TROUBLESHOOTING

[PLACEHOLDER: Problemas comunes y soluciones espec�ficas del proyecto]

### Ejemplos por Tecnolog�a

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
- **Sesiones IA**: [`docs/methodology/SESSIONS.md`](SESSIONS.md) - Gesti�n al 85%
- **Desviaciones**: [`docs/methodology/DEVIATIONS.md`](DEVIATIONS.md) - Sprint deviations
- **Workflow**: [`docs/methodology/WORKFLOW.md`](WORKFLOW.md) - Flujo detallado

---

**Versi�n**: 2.0  
**�ltima actualizaci�n**: [PLACEHOLDER: Fecha]  
**Proyecto**: [PLACEHOLDER: Nombre del proyecto]  
**Tecnolog�a**: [PLACEHOLDER: Stack principal]
