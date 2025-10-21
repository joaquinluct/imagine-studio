# Metodología de Desarrollo Ágil Asistido por IA - CORE

> **Propósito**: Documento fundamental de la metodología. **Leer UNA VEZ** al inicio de colaboración con el asistente IA.

---

## ?? PRINCIPIOS FUNDAMENTALES

Esta metodología se basa en:

1. **Iteraciones pequeñas y funcionales**: Cada tarea deja el proyecto en estado compilable/ejecutable
2. **Documentación continua**: Actualizar docs en cada commit
3. **Automatización IA**: El asistente ejecuta el ciclo completo sin intervención manual innecesaria
4. **Calidad desde el inicio**: No soluciones temporales, solo código production-ready
5. **Trazabilidad**: Versionado de sprints, commits descriptivos, historial claro
6. **Doble validación**: Compilación limpia + validación usuario para bugs

---

## ?? ESTRUCTURA DE ARCHIVOS

### Archivos Activos del Sprint

| Archivo | Propósito | Frecuencia de consulta |
|---------|-----------|------------------------|
| `docs/MAIN.md` | Pilares del proyecto | Una vez (inicio) |
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
?    - Compilar/ejecutar/probar                               ?
?    - Corregir errores hasta validación limpia               ?
?    - 0 errores, 0 warnings                                  ?
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
### H1 - Título de Historia
**ID**: H1
**Prioridad**: CRÍTICA
**Descripción**: Detalle completo

**Criterios de aceptación**:
- ? Criterio 1
- ? Criterio 2

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
### H1.1 - Título de Tarea
**Estado**: ? Pendiente / ? Completada
**Historia**: H1
**Descripción**: Detalle completo

**Implementación**:
```cpp
// Código ejemplo
```

**Archivos afectados**: lista
**Criterio de éxito**: condición
```

### 4. `docs/daily.md` - Seguimiento Diario

**Contenido** (2 líneas):
```markdown
Hecho: H1.3 - EntityManager implementado
Siguiente: H1.4 - Tests unitarios Entity System
```

**Sin sprint activo**:
```markdown
Sprint v1.9.0 cerrado. Sin sprint activo.
```

---

## ?? FORMATO DE ITERACIÓN FINAL

Al final de cada iteración, reportar con **3 secciones obligatorias**:

### 1. Títulos (Hecho/Siguiente)

```markdown
? Hecho: H1.3 - EntityManager implementado
?? Siguiente: H1.4 - Tests unitarios Entity System
```

### 2. Visualización (OBLIGATORIO)

```markdown
### ??? Visualización:

**¿Algo nuevo que cambie la vista tras ejecución?**: **SÍ** ? / **NO** ?

[SI ES "SÍ ?"]
**Qué deberías ver al ejecutar** (F5 / npm run dev / etc):
1. ? Panel "Hierarchy" visible en parte izquierda
2. ? Árbol expandible con objetos
3. ?? Dock/undock funcional

**Qué ha cambiado**:
- ?? Layout incluye panel funcional

**Ausente**:
- ? Demo Window ya no aparece

[SI ES "NO ?"]
**Razón**: Esta tarea implementa clases internas sin efecto en UI
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

1. **Hacer bien desde el principio**: Evitar partes temporales o incompletas
2. **Cada módulo cumple estándares AAA**: Desde su concepción
3. **Validación antes de commit**: Siempre compilar/ejecutar/probar
4. **Estilo consistente**: Seguir convenciones del proyecto

### Codificación de Archivos

**OBLIGATORIO** para TODOS los archivos de texto:
- Encoding: **UTF-8 con BOM**
- Line endings: **CRLF** (Windows)
- Verificar con `.\scripts\check-encoding.ps1 -Fix` si detecta problemas

---

## ?? REGLAS CRÍTICAS

### 1. NUNCA Modificar `external/`

**Razones**:
1. Imposibilita actualizar la biblioteca en el futuro
2. Oculta bugs en nuestro código (tapa síntomas, no arregla causas)
3. Rompe la reproducibilidad del proyecto

**Política**:
- TODO el código de bibliotecas externas debe permanecer **INTACTO**
- Si necesitas personalizar: crear **wrappers** en `src/`
- Si encuentras bug relacionado: investigar **NUESTRO** código primero
- Consultar `docs/THIRD_PARTY.md` para políticas específicas

### 2. Doble Compilación Obligatoria

**Requisito**: DOS compilaciones limpias antes de commit:

```powershell
# Compilación 1 - CMake Build (Debug)
cmake --build build --config Debug

# Compilación 2 - Visual Studio Build Solution
msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /m
```

**Criterio de éxito**: 0 errores, 0 warnings (ambas)

### 3. Commits y Documentación

**Regla**: Siempre actualizar automáticamente:
- `docs/daily.md`
- `docs/sprint_commits.md`
- `README.md` (cuando aplique)

**NO** hacer push automático (solo local commits)

---

## ?? TROUBLESHOOTING

### Build Fails

1. Comprobar existencia de solución: `Get-ChildItem *.sln`
2. Verificar archivos en `.vcxproj` / `CMakeLists.txt`
3. Compilar explícitamente: `msbuild "Imagine Studio.sln" /p:Configuration=Debug /m`
4. Si error de enlace (`LNK2019`): verificar subsistema (CONSOLE vs WINDOWS)
5. Corregir errores/warnings hasta build limpia

### File Not Found

- Confirmar ficheros incluidos en proyecto
- Añadir a `.vcxproj` o `CMakeLists.txt` si falta

### Encoding Issues

- Ejecutar `.\scripts\check-encoding.ps1 -Fix`
- Verificar UTF-8 con BOM + CRLF

---

## ?? DOCUMENTOS RELACIONADOS

- **Bugs**: [`docs/methodology/BUGS.md`](BUGS.md) - Bug management completo
- **Sesiones IA**: [`docs/methodology/SESSIONS.md`](SESSIONS.md) - Gestión al 85%
- **Desviaciones**: [`docs/methodology/DEVIATIONS.md`](DEVIATIONS.md) - Sprint deviations
- **Workflow**: [`docs/methodology/WORKFLOW.md`](WORKFLOW.md) - Flujo detallado

---

**Versión**: 2.0  
**Última actualización**: 2025-01-18  
**Proyecto**: Imagine Studio (C++ Game Engine)
