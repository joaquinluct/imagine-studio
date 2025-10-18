#!/usr/bin/env pwsh
# close-sprint.ps1
# Script para cerrar un sprint y archivar sus ficheros en docs/sprints/
# 
# Uso: .\scripts\close-sprint.ps1 -Version "1.3.0"
# 
# Este script:
# 1. Crea el directorio docs/sprints/ si no existe
# 2. Copia todos los ficheros del sprint actual a docs/sprints/ con sufijo _vX.Y.Z
# 3. Crea ficheros vac�os para el siguiente sprint
# 4. Actualiza daily.md indicando que el sprint est� cerrado
# 
# Ficheros que se archivan:
# - sprint.md -> sprint_vX.Y.Z.md
# - sprint_histories.md -> sprint_histories_vX.Y.Z.md
# - sprint_tasks.md -> sprint_tasks_vX.Y.Z.md
# - sprint_bugs.md -> sprint_bugs_vX.Y.Z.md
# - sprint_bug_attempts.md -> sprint_bug_attempts_vX.Y.Z.md
# - sprint_fix.md -> sprint_fix_vX.Y.Z.md
# - sprint_commits.md -> sprint_commits_vX.Y.Z.md (NUEVO)
# - sprint_deviations.md -> sprint_deviations_vX.Y.Z.md (si existe)
# - sprint_ia_sessions.md -> sprint_ia_sessions_vX.Y.Z.md (si existe)

param(
    [Parameter(Mandatory=$true)]
    [string]$Version
)

# Validar formato de versi�n (debe ser X.Y.Z)
if ($Version -notmatch '^\d+\.\d+\.\d+$') {
    Write-Error "Error: Versi�n debe tener formato X.Y.Z (ejemplo: 1.3.0)"
    exit 1
}

Write-Host "=== Cerrando Sprint v$Version ===" -ForegroundColor Cyan

# 1. Crear directorio sprints si no existe
$sprintsDir = "docs/sprints"
if (-not (Test-Path $sprintsDir)) {
    New-Item -ItemType Directory -Path $sprintsDir -Force | Out-Null
    Write-Host "[OK] Directorio $sprintsDir/ creado" -ForegroundColor Green
} else {
    Write-Host "[OK] Directorio $sprintsDir/ ya existe" -ForegroundColor Green
}

# 2. Lista de ficheros a archivar (obligatorios)
$requiredFiles = @(
    "sprint.md",
    "sprint_histories.md",
    "sprint_tasks.md",
    "sprint_bugs.md",
    "sprint_bug_attempts.md",
    "sprint_fix.md",
    "sprint_commits.md"
)

# 3. Lista de ficheros opcionales (pueden no existir)
$optionalFiles = @(
    "sprint_deviations.md",
    "sprint_ia_sessions.md"
)

# 4. Copiar ficheros obligatorios
Write-Host "`nArchivando ficheros obligatorios:" -ForegroundColor Yellow
foreach ($file in $requiredFiles) {
    $sourcePath = "docs/$file"
    $baseName = [System.IO.Path]::GetFileNameWithoutExtension($file)
    $destPath = "$sprintsDir/${baseName}_v$Version.md"
    
    if (Test-Path $sourcePath) {
        Copy-Item $sourcePath $destPath -Force
        Write-Host "  [OK] $file -> ${baseName}_v$Version.md" -ForegroundColor Green
    } else {
        Write-Error "  [ERROR] Fichero obligatorio no encontrado: $sourcePath"
        exit 1
    }
}

# 5. Copiar ficheros opcionales
Write-Host "`nArchivando ficheros opcionales:" -ForegroundColor Yellow
foreach ($file in $optionalFiles) {
    $sourcePath = "docs/$file"
    $baseName = [System.IO.Path]::GetFileNameWithoutExtension($file)
    $destPath = "$sprintsDir/${baseName}_v$Version.md"
    
    if (Test-Path $sourcePath) {
        Copy-Item $sourcePath $destPath -Force
        Write-Host "  [OK] $file -> ${baseName}_v$Version.md" -ForegroundColor Green
    } else {
        Write-Host "  [SKIP] $file (no existe, es opcional)" -ForegroundColor Gray
    }
}

# 6. Verificar archivos copiados
Write-Host "`nVerificando archivos copiados:" -ForegroundColor Yellow
$copiedFiles = Get-ChildItem "$sprintsDir/*v$Version.md" | Select-Object -ExpandProperty Name
if ($copiedFiles.Count -gt 0) {
    foreach ($file in $copiedFiles) {
        Write-Host "  [OK] $file" -ForegroundColor Green
    }
    Write-Host "`nTotal archivados: $($copiedFiles.Count) ficheros" -ForegroundColor Cyan
} else {
    Write-Error "Error: No se copiaron archivos a $sprintsDir/"
    exit 1
}

# 7. Crear ficheros vac�os para el siguiente sprint
Write-Host "`nCreando ficheros vac�os para el siguiente sprint:" -ForegroundColor Yellow

# sprint.md vac�o
$sprintContent = @"
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
| H1 | [T�tulo] | Alta | ? Pendiente |

## ?? Progreso

**Historias completadas**: 0/X (0%)
**Tareas completadas**: 0/X (0%)

---

*�ltima actualizaci�n*: [FECHA]
"@
Set-Content "docs/sprint.md" $sprintContent -Encoding UTF8
Write-Host "  [OK] sprint.md (vac�o)" -ForegroundColor Green

# sprint_histories.md vac�o
$historiesContent = @"
# Sprint Histories - vX.Y.Z (Active)

> **Referencia**: Ver [`docs/sprint_tasks.md`](sprint_tasks.md) para tareas detalladas del sprint.

Este archivo contiene las historias de usuario (alto nivel) del sprint activo vX.Y.Z.

---

## Historia 1: [NOMBRE] (H1)

### H1 - [T�tulo de la historia]
**ID**: H1  
**Prioridad**: Alta  
**Estado**: ? Pendiente  

**Descripci�n**: [Descripci�n de la historia]

**Criterios de aceptaci�n**:
- [ ] [Criterio 1]
- [ ] [Criterio 2]

**Tareas derivadas**: H1.1, H1.2, H1.3

---

*Total historias*: 1
"@
Set-Content "docs/sprint_histories.md" $historiesContent -Encoding UTF8
Write-Host "  [OK] sprint_histories.md (vac�o)" -ForegroundColor Green

# sprint_tasks.md vac�o
$tasksContent = @"
# Sprint Tasks - vX.Y.Z (Active)

> **Referencia**: Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias de usuario del sprint.

Este archivo contiene las tareas detalladas (bajo nivel) del sprint activo vX.Y.Z.

---

## Historia 1: [NOMBRE] (H1)

### Tarea H1.1: [T�tulo de la tarea]
**Estado**: ? Pendiente  
**Archivos afectados**: [lista de archivos]

**Descripci�n**: [Descripci�n de la tarea]

**Pasos**:
1. [ ] [Paso 1]
2. [ ] [Paso 2]

**Commit**: [Hash pendiente]

---

## ?? Resumen de Tareas

| Historia | Tarea | T�tulo | Estado |
|----------|-------|--------|--------|
| H1 | H1.1 | [T�tulo] | ? Pendiente |

**Total**: 1 tarea (0 completadas, 1 pendiente)
"@
Set-Content "docs/sprint_tasks.md" $tasksContent -Encoding UTF8
Write-Host "  [OK] sprint_tasks.md (vac�o)" -ForegroundColor Green

# sprint_bugs.md vac�o
$bugsContent = @"
# Sprint Bugs - vX.Y.Z (Active)

> **Referencia**: Ver [`docs/sprint_bug_attempts.md`](sprint_bug_attempts.md) para intentos de soluci�n detallados.

Este archivo contiene los bugs reportados durante el sprint activo vX.Y.Z que est�n pendientes de resoluci�n.

---

## Bugs Reportados

*No hay bugs reportados actualmente.*

---

**Total bugs**: 0 (0 reportados, 0 en progreso, 0 pendientes validaci�n)
"@
Set-Content "docs/sprint_bugs.md" $bugsContent -Encoding UTF8
Write-Host "  [OK] sprint_bugs.md (vac�o)" -ForegroundColor Green

# sprint_bug_attempts.md vac�o
$attemptsContent = @"
# Sprint Bug Attempts - vX.Y.Z (Active)

> **Referencia**: Ver [`docs/sprint_bugs.md`](sprint_bugs.md) para bugs reportados actualmente.

Este archivo contiene el registro detallado de TODOS los intentos de soluci�n para cada bug reportado durante el sprint activo vX.Y.Z.

---

## Intentos de Soluci�n

*No hay intentos de soluci�n registrados actualmente.*

---

**Nota**: Este fichero se actualiza autom�ticamente conforme se trabajan los bugs en `sprint_bugs.md`.
"@
Set-Content "docs/sprint_bug_attempts.md" $attemptsContent -Encoding UTF8
Write-Host "  [OK] sprint_bug_attempts.md (vac�o)" -ForegroundColor Green

# sprint_fix.md vac�o
$fixContent = @"
# Sprint Fix - vX.Y.Z (Active)

> **Referencia**: Ver [`docs/sprint_bugs.md`](sprint_bugs.md) para bugs actualmente pendientes.

Este archivo contiene el historial de bugs resueltos durante el sprint activo vX.Y.Z.

---

## Bugs Resueltos

*No hay bugs resueltos actualmente.*

---

**Total bugs resueltos**: 0
"@
Set-Content "docs/sprint_fix.md" $fixContent -Encoding UTF8
Write-Host "  [OK] sprint_fix.md (vac�o)" -ForegroundColor Green

# sprint_commits.md vac�o (NUEVO)
$commitsContent = @"
# Sprint Commits - vX.Y.Z (Active)

> **Referencia**: Ver [`docs/sprint.md`](sprint.md) para objetivo y progreso del sprint.

Este archivo registra todos los commits realizados durante el **Sprint vX.Y.Z**.

---

## Convenciones de Commits

Formato recomendado para los mensajes de commit:

``````
<tipo>(<�mbito>): <mensaje corto>

<mensaje largo opcional>

Refs: <issue/track-id>
``````

### Tipos comunes:
- **feat**: Nueva funcionalidad
- **fix**: Correcci�n de errores
- **docs**: Cambios en documentaci�n
- **style**: Formato, estilo, sin cambios funcionales
- **refactor**: Refactorizaci�n sin cambios en comportamiento
- **perf**: Mejoras de rendimiento
- **test**: A�adir o corregir tests
- **chore**: Tareas de mantenimiento

---

## Historial de Commits (Sprint vX.Y.Z)

*No hay commits registrados actualmente.*

---

**Versi�n**: v1.0  
**�ltima actualizaci�n**: [FECHA]  
**Sprint**: vX.Y.Z - **EN PROGRESO**
"@
Set-Content "docs/sprint_commits.md" $commitsContent -Encoding UTF8
Write-Host "  [OK] sprint_commits.md (vac�o)" -ForegroundColor Green

# sprint_deviations.md vac�o (opcional)
$deviationsContent = @"
# Sprint Deviations - vX.Y.Z (Active)

> **Referencia**: Ver [`docs/backlog.md`](backlog.md) para �tems diferidos a sprints futuros.

Este archivo contiene las desviaciones, ajustes arquitect�nicos y tareas emergentes que surgen durante la ejecuci�n del sprint activo vX.Y.Z.

---

## Desviaciones Registradas

*No hay desviaciones registradas actualmente.*

---

**Nota**: Solo se registran aqu� desviaciones CR�TICAS que NO pueden diferirse al backlog.
"@
Set-Content "docs/sprint_deviations.md" $deviationsContent -Encoding UTF8
Write-Host "  [OK] sprint_deviations.md (vac�o)" -ForegroundColor Green

# sprint_ia_sessions.md vac�o (opcional)
$sessionsContent = @"
# Sprint IA Sessions - vX.Y.Z (Active)

> **Referencia**: Ver [`.github/copilot-instructions.md`](../.github/copilot-instructions.md) para gesti�n de sesiones IA.

Este archivo contiene el registro de sesiones de trabajo con asistentes IA que superan el 85% de consumo de tokens/contexto durante el sprint activo vX.Y.Z.

---

## Sesiones IA Registradas

*No hay sesiones IA registradas actualmente.*

---

**Nota**: Las sesiones se registran autom�ticamente cuando el consumo de tokens supera el 85% del l�mite.
"@
Set-Content "docs/sprint_ia_sessions.md" $sessionsContent -Encoding UTF8
Write-Host "  [OK] sprint_ia_sessions.md (vac�o)" -ForegroundColor Green

Write-Host "`n=== Sprint v$Version cerrado exitosamente ===" -ForegroundColor Cyan
Write-Host "Archivos archivados en: $sprintsDir/" -ForegroundColor Green
Write-Host "Ficheros vac�os creados para el siguiente sprint" -ForegroundColor Green
Write-Host "`nPr�ximo paso: Actualizar docs/daily.md manualmente con:" -ForegroundColor Yellow
Write-Host "  'Sprint v$Version cerrado. Sin sprint activo.'" -ForegroundColor Gray
