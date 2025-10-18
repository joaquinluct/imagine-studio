# Script close-sprint.ps1 - Documentación

## Propósito

Script automatizado para cerrar un sprint y archivar sus ficheros en `docs/sprints/`.

**Ubicación**: `scripts/close-sprint.ps1`

## ?? REGLA CRÍTICA

**NUNCA** ejecutar comandos PowerShell directamente en el terminal para archivar sprints.  
**SIEMPRE** usar este script dedicado.

**Razón**: Los comandos PowerShell ejecutados directamente desde el asistente pueden quedarse bloqueados sin output o fallar silenciosamente. Este script es más robusto y proporciona feedback claro.

## Uso

```powershell
# Cerrar sprint v1.3.0 y archivar ficheros
.\scripts\close-sprint.ps1 -Version "1.3.0"
```

**Parámetros**:
- `-Version`: Versión del sprint en formato X.Y.Z (obligatorio)

## Qué hace el script

1. ? Valida que la versión tenga formato correcto (X.Y.Z)
2. ? Crea directorio `docs/sprints/` si no existe
3. ? Copia todos los ficheros del sprint actual a `docs/sprints/` con sufijo `_vX.Y.Z`
4. ? Verifica que los archivos se copiaron correctamente
5. ? Crea ficheros vacíos para el siguiente sprint con templates correctos
6. ? Proporciona feedback visual con colores (Verde=OK, Rojo=Error, Gris=Skip)

## ?? VERIFICACIÓN POST-CIERRE (OBLIGATORIA)

**Después de ejecutar el script, SIEMPRE verificar**:

### 1. Verificar archivos en `docs/sprints/`:

```powershell
ls docs/sprints/*v1.3.0* | Select-Object Name
```

**Debe mostrar 8 archivos**:
- `sprint_v1.3.0.md`
- `sprint_histories_v1.3.0.md`
- `sprint_tasks_v1.3.0.md`
- `sprint_bugs_v1.3.0.md`
- `sprint_bug_attempts_v1.3.0.md`
- `sprint_fix_v1.3.0.md`
- `sprint_deviations_v1.3.0.md`
- `sprint_ia_sessions_v1.3.0.md`

### 2. Verificar archivos en `docs/`:

```powershell
ls docs/sprint*.md | Select-Object Name
```

**Debe mostrar 8 archivos (templates vacíos)**:
- `sprint.md`
- `sprint_histories.md`
- `sprint_tasks.md`
- `sprint_bugs.md`
- `sprint_bug_attempts.md`
- `sprint_fix.md`
- `sprint_deviations.md`
- `sprint_ia_sessions.md`

### 3. Verificar contenido de `docs/sprint.md`:

```powershell
Get-Content docs/sprint.md -Head 5
```

**Debe mostrar**:
```markdown
# Sprint vX.Y.Z - [NOMBRE DEL SPRINT]

> **Estado**: ?? En progreso  
> **Fecha inicio**: [FECHA]  
> **Fecha fin estimada**: [FECHA]

## ?? Objetivo del Sprint

[Descripción del objetivo principal del sprint]

## ?? Historias de Usuario

Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias detalladas.

| ID | Historia | Prioridad | Estado |
|----|----------|-----------|--------|
| H1 | [Título] | Alta | ?? Pendiente |

## ?? Progreso

**Historias completadas**: 0/X (0%)
**Tareas completadas**: 0/X (0%)

---

*Última actualización*: [FECHA]
```

**Si el contenido es diferente** (por ejemplo, tiene datos del sprint v1.3.0), **el cierre NO se completó correctamente**.

### 4. Checklist de verificación:

- [ ] 8 archivos copiados a `docs/sprints/` con sufijo `_v1.3.0`
- [ ] 8 archivos vacíos creados en `docs/` con templates correctos
- [ ] `docs/sprint.md` contiene template vacío (no datos v1.3.0)
- [ ] `docs/daily.md` actualizado con "Sprint v1.3.0 cerrado"
- [ ] Commit creado con todos los cambios

**Si alguno falla**, ejecutar nuevamente el script.

## Ficheros que archiva

### Obligatorios (deben existir):
- `sprint.md` ? `sprint_v1.3.0.md`
- `sprint_histories.md` ? `sprint_histories_v1.3.0.md`
- `sprint_tasks.md` ? `sprint_tasks_v1.3.0.md`
- `sprint_bugs.md` ? `sprint_bugs_v1.3.0.md`
- `sprint_bug_attempts.md` ? `sprint_bug_attempts_v1.3.0.md`
- `sprint_fix.md` ? `sprint_fix_v1.3.0.md`

### Opcionales (pueden no existir):
- `sprint_deviations.md` ? `sprint_deviations_v1.3.0.md`
- `sprint_ia_sessions.md` ? `sprint_ia_sessions_v1.3.0.md`

## Output esperado

```
=== Cerrando Sprint v1.3.0 ===
[OK] Directorio docs/sprints/ ya existe

Archivando ficheros obligatorios:
  [OK] sprint.md -> sprint_v1.3.0.md
  [OK] sprint_histories.md -> sprint_histories_v1.3.0.md
  [OK] sprint_tasks.md -> sprint_tasks_v1.3.0.md
  [OK] sprint_bugs.md -> sprint_bugs_v1.3.0.md
  [OK] sprint_bug_attempts.md -> sprint_bug_attempts_v1.3.0.md
  [OK] sprint_fix.md -> sprint_fix_v1.3.0.md

Archivando ficheros opcionales:
  [OK] sprint_deviations.md -> sprint_deviations_v1.3.0.md
  [OK] sprint_ia_sessions.md -> sprint_ia_sessions_v1.3.0.md

Verificando archivos copiados:
  [OK] sprint_v1.3.0.md
  [OK] sprint_histories_v1.3.0.md
  [OK] sprint_tasks_v1.3.0.md
  [OK] sprint_bugs_v1.3.0.md
  [OK] sprint_bug_attempts_v1.3.0.md
  [OK] sprint_fix_v1.3.0.md
  [OK] sprint_deviations_v1.3.0.md
  [OK] sprint_ia_sessions_v1.3.0.md

Total archivados: 8 ficheros

Creando ficheros vacíos para el siguiente sprint:
  [OK] sprint.md (vacío)
  [OK] sprint_histories.md (vacío)
  [OK] sprint_tasks.md (vacío)
  [OK] sprint_bugs.md (vacío)
  [OK] sprint_bug_attempts.md (vacío)
  [OK] sprint_fix.md (vacío)
  [OK] sprint_deviations.md (vacío)
  [OK] sprint_ia_sessions.md (vacío)

=== Sprint v1.3.0 cerrado exitosamente ===
Archivos archivados en: docs/sprints/
Ficheros vacíos creados para el siguiente sprint

Próximo paso: Actualizar docs/daily.md manualmente con:
  'Sprint v1.3.0 cerrado. Sin sprint activo.'
```

## Paso adicional manual

Después de ejecutar el script, actualizar `docs/daily.md`:

```markdown
# Daily Log

Hecho: Sprint v1.3.0 cerrado y archivado en docs/sprints/
Siguiente: Sprint v1.3.0 cerrado. Sin sprint activo.
```

## Errores comunes

### Error: Versión incorrecta

```
Error: Versión debe tener formato X.Y.Z (ejemplo: 1.3.0)
```

**Solución**: Usar formato correcto con tres números separados por puntos.

```powershell
# ? Incorrecto
.\scripts\close-sprint.ps1 -Version "1.3"
.\scripts\close-sprint.ps1 -Version "v1.3.0"

# ? Correcto
.\scripts\close-sprint.ps1 -Version "1.3.0"
```

### Error: Fichero obligatorio no encontrado

```
[ERROR] Fichero obligatorio no encontrado: docs/sprint.md
```

**Solución**: Verificar que todos los ficheros obligatorios existan en `docs/` antes de cerrar el sprint.

### Error: Templates no se crearon correctamente

**Síntoma**: `docs/sprint.md` contiene datos del sprint anterior en lugar de template vacío.

**Causa**: El script se ejecutó pero los templates no se sobreescribieron correctamente.

**Solución**:
```powershell
# Ejecutar nuevamente el script
.\scripts\close-sprint.ps1 -Version "1.3.0"

# Verificar que los templates se crearon
Get-Content docs/sprint.md -Head 10
```

## Templates de ficheros vacíos

El script crea ficheros vacíos con templates correctos para el siguiente sprint:

### `sprint.md`
```markdown
# Sprint vX.Y.Z - [NOMBRE DEL SPRINT]

> **Estado**: ?? En progreso  
> **Fecha inicio**: [FECHA]  
> **Fecha fin estimada**: [FECHA]

## ?? Objetivo del Sprint

[Descripción del objetivo principal del sprint]

## ?? Historias de Usuario

Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias detalladas.

| ID | Historia | Prioridad | Estado |
|----|----------|-----------|--------|
| H1 | [Título] | Alta | ?? Pendiente |

## ?? Progreso

**Historias completadas**: 0/X (0%)
**Tareas completadas**: 0/X (0%)

---

*Última actualización*: [FECHA]
```

### `sprint_histories.md`, `sprint_tasks.md`, etc.

Cada fichero tiene un template apropiado con formato correcto y placeholders para facilitar la planificación del siguiente sprint.

## Flujo de trabajo completo

1. **Completar sprint actual**: Todas las tareas finalizadas
2. **Ejecutar script**: `.\scripts\close-sprint.ps1 -Version "X.Y.Z"`
3. **Verificar output**: Comprobar que todos los ficheros se archivaron correctamente
4. **Verificar archivos** (OBLIGATORIO):
   - `ls docs/sprints/*vX.Y.Z*` ? Debe mostrar 8 archivos
   - `ls docs/sprint*.md` ? Debe mostrar 8 templates vacíos
   - `Get-Content docs/sprint.md -Head 5` ? Debe mostrar template vacío
5. **Actualizar daily.md**: Indicar que el sprint está cerrado
6. **Crear commit**: Archivar cambios en Git
7. **Planificar siguiente sprint**: Usar templates vacíos para documentar nuevo sprint

## Ventajas del script

? **Automático**: No requiere copiar ficheros manualmente  
? **Robusto**: Valida que todo se copie correctamente  
? **Visual**: Feedback claro con colores (Verde/Rojo/Gris)  
? **Seguro**: No sobreescribe archivos existentes (usa `-Force` solo cuando es necesario)  
? **Completo**: Crea templates vacíos para el siguiente sprint  
? **Documentado**: Output detallado de cada paso  
? **Verificable**: Checklist de verificación post-cierre

## Comandos a EVITAR

**NUNCA** usar estos comandos directamente:

```powershell
# ? NO HACER
Copy-Item "docs/sprint.md" "docs/sprints/sprint_v1.3.0.md"
Move-Item "docs/sprint.md" "docs/sprints/sprint_v1.3.0.md"
Get-ChildItem "docs/*.md" | Copy-Item -Destination "docs/sprints/"
```

**Razón**: Estos comandos pueden:
- Quedarse bloqueados sin output
- Fallar silenciosamente
- No proporcionar feedback claro
- Sobrescribir archivos sin avisar
- No validar que los archivos se copiaron correctamente

**SIEMPRE** usar:

```powershell
# ? CORRECTO
.\scripts\close-sprint.ps1 -Version "1.3.0"
```

## Referencias

- Script: `scripts/close-sprint.ps1`
- Documentación metodología: `.github/copilot-instructions.md`
- Plantilla metodología: `docs/TEMPLATE.md`
- Directorio de históricos: `docs/sprints/`

---

*Última actualización*: 2025-01-19
