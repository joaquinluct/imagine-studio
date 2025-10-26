# Script close-sprint.ps1 - Documentaci�n

## Prop�sito

Script automatizado para cerrar un sprint y archivar sus ficheros en `docs/sprints/`.

**Ubicaci�n**: `scripts/close-sprint.ps1`

## ?? REGLA CR�TICA

**NUNCA** ejecutar comandos PowerShell directamente en el terminal para archivar sprints.  
**SIEMPRE** usar este script dedicado.

**Raz�n**: Los comandos PowerShell ejecutados directamente desde el asistente pueden quedarse bloqueados sin output o fallar silenciosamente. Este script es m�s robusto y proporciona feedback claro.

## Uso

```powershell
# Cerrar sprint v1.3.0 y archivar ficheros
.\scripts\close-sprint.ps1 -Version "1.3.0"
```

**Par�metros**:
- `-Version`: Versi�n del sprint en formato X.Y.Z (obligatorio)

## Qu� hace el script

1. ? Valida que la versi�n tenga formato correcto (X.Y.Z)
2. ? Crea directorio `docs/sprints/` si no existe
3. ? Copia todos los ficheros del sprint actual a `docs/sprints/` con sufijo `_vX.Y.Z`
4. ? Verifica que los archivos se copiaron correctamente
5. ? Crea ficheros vac�os para el siguiente sprint con templates correctos
6. ? Proporciona feedback visual con colores (Verde=OK, Rojo=Error, Gris=Skip)

## ?? VERIFICACI�N POST-CIERRE (OBLIGATORIA)

**Despu�s de ejecutar el script, SIEMPRE verificar**:

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

**Debe mostrar 8 archivos (templates vac�os)**:
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

[Descripci�n del objetivo principal del sprint]

## ?? Historias de Usuario

Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias detalladas.

| ID | Historia | Prioridad | Estado |
|----|----------|-----------|--------|
| H1 | [T�tulo] | Alta | ?? Pendiente |

## ?? Progreso

**Historias completadas**: 0/X (0%)
**Tareas completadas**: 0/X (0%)

---

*�ltima actualizaci�n*: [FECHA]
```

**Si el contenido es diferente** (por ejemplo, tiene datos del sprint v1.3.0), **el cierre NO se complet� correctamente**.

### 4. Checklist de verificaci�n:

- [ ] 8 archivos copiados a `docs/sprints/` con sufijo `_v1.3.0`
- [ ] 8 archivos vac�os creados en `docs/` con templates correctos
- [ ] `docs/sprint.md` contiene template vac�o (no datos v1.3.0)
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

Creando ficheros vac�os para el siguiente sprint:
  [OK] sprint.md (vac�o)
  [OK] sprint_histories.md (vac�o)
  [OK] sprint_tasks.md (vac�o)
  [OK] sprint_bugs.md (vac�o)
  [OK] sprint_bug_attempts.md (vac�o)
  [OK] sprint_fix.md (vac�o)
  [OK] sprint_deviations.md (vac�o)
  [OK] sprint_ia_sessions.md (vac�o)

=== Sprint v1.3.0 cerrado exitosamente ===
Archivos archivados en: docs/sprints/
Ficheros vac�os creados para el siguiente sprint

Pr�ximo paso: Actualizar docs/daily.md manualmente con:
  'Sprint v1.3.0 cerrado. Sin sprint activo.'
```

## Paso adicional manual

Despu�s de ejecutar el script, actualizar `docs/daily.md`:

```markdown
# Daily Log

Hecho: Sprint v1.3.0 cerrado y archivado en docs/sprints/
Siguiente: Sprint v1.3.0 cerrado. Sin sprint activo.
```

## Errores comunes

### Error: Versi�n incorrecta

```
Error: Versi�n debe tener formato X.Y.Z (ejemplo: 1.3.0)
```

**Soluci�n**: Usar formato correcto con tres n�meros separados por puntos.

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

**Soluci�n**: Verificar que todos los ficheros obligatorios existan en `docs/` antes de cerrar el sprint.

### Error: Templates no se crearon correctamente

**S�ntoma**: `docs/sprint.md` contiene datos del sprint anterior en lugar de template vac�o.

**Causa**: El script se ejecut� pero los templates no se sobreescribieron correctamente.

**Soluci�n**:
```powershell
# Ejecutar nuevamente el script
.\scripts\close-sprint.ps1 -Version "1.3.0"

# Verificar que los templates se crearon
Get-Content docs/sprint.md -Head 10
```

## Templates de ficheros vac�os

El script crea ficheros vac�os con templates correctos para el siguiente sprint:

### `sprint.md`
```markdown
# Sprint vX.Y.Z - [NOMBRE DEL SPRINT]

> **Estado**: ?? En progreso  
> **Fecha inicio**: [FECHA]  
> **Fecha fin estimada**: [FECHA]

## ?? Objetivo del Sprint

[Descripci�n del objetivo principal del sprint]

## ?? Historias de Usuario

Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias detalladas.

| ID | Historia | Prioridad | Estado |
|----|----------|-----------|--------|
| H1 | [T�tulo] | Alta | ?? Pendiente |

## ?? Progreso

**Historias completadas**: 0/X (0%)
**Tareas completadas**: 0/X (0%)

---

*�ltima actualizaci�n*: [FECHA]
```

### `sprint_histories.md`, `sprint_tasks.md`, etc.

Cada fichero tiene un template apropiado con formato correcto y placeholders para facilitar la planificaci�n del siguiente sprint.

## Flujo de trabajo completo

1. **Completar sprint actual**: Todas las tareas finalizadas
2. **Ejecutar script**: `.\scripts\close-sprint.ps1 -Version "X.Y.Z"`
3. **Verificar output**: Comprobar que todos los ficheros se archivaron correctamente
4. **Verificar archivos** (OBLIGATORIO):
   - `ls docs/sprints/*vX.Y.Z*` ? Debe mostrar 8 archivos
   - `ls docs/sprint*.md` ? Debe mostrar 8 templates vac�os
   - `Get-Content docs/sprint.md -Head 5` ? Debe mostrar template vac�o
5. **Actualizar daily.md**: Indicar que el sprint est� cerrado
6. **Crear commit**: Archivar cambios en Git
7. **Planificar siguiente sprint**: Usar templates vac�os para documentar nuevo sprint

## Ventajas del script

? **Autom�tico**: No requiere copiar ficheros manualmente  
? **Robusto**: Valida que todo se copie correctamente  
? **Visual**: Feedback claro con colores (Verde/Rojo/Gris)  
? **Seguro**: No sobreescribe archivos existentes (usa `-Force` solo cuando es necesario)  
? **Completo**: Crea templates vac�os para el siguiente sprint  
? **Documentado**: Output detallado de cada paso  
? **Verificable**: Checklist de verificaci�n post-cierre

## Comandos a EVITAR

**NUNCA** usar estos comandos directamente:

```powershell
# ? NO HACER
Copy-Item "docs/sprint.md" "docs/sprints/sprint_v1.3.0.md"
Move-Item "docs/sprint.md" "docs/sprints/sprint_v1.3.0.md"
Get-ChildItem "docs/*.md" | Copy-Item -Destination "docs/sprints/"
```

**Raz�n**: Estos comandos pueden:
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
- Documentaci�n metodolog�a: `.github/copilot-instructions.md`
- Plantilla metodolog�a: `docs/TEMPLATE.md`
- Directorio de hist�ricos: `docs/sprints/`

---

*�ltima actualizaci�n*: 2025-01-19
