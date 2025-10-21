# log-autogen-session.ps1
# Script para registrar logs de sesiones de AutoGen Studio

param(
    [Parameter(Mandatory=$true)]
    [string]$TaskID,
    
    [Parameter(Mandatory=$true)]
    [ValidateSet("SUCCESS", "FAILED", "PARTIAL")]
    [string]$Status,
    
    [Parameter(Mandatory=$true)]
    [string]$LogContent,
    
    [string]$SprintVersion = "v1.9.0",
    [int]$Duration = 0
)

$date = Get-Date -Format "yyyy-MM-dd"
$timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
$filename = "$date`_$TaskID`_$Status.md"
$filepath = "autogen/sessions/$filename"

# Crear contenido del log
$log = @"
# AutoGen Session Log - $TaskID

**Date**: $timestamp
**Sprint**: $SprintVersion
**Task**: $TaskID
**Status**: $(if ($Status -eq "SUCCESS") {"? $Status"} elseif ($Status -eq "FAILED") {"? $Status"} else {"?? $Status"})
**Duration**: $($Duration)s

---

## Prompt Used
[Link to: prompts/sprint_$SprintVersion/$TaskID.md]

---

## Agent Execution Log

$LogContent

---

## Files Created/Modified
[Listar archivos generados]

---

## Build Results
[Resultados de compilación]

---

## Next Steps
[Próxima tarea o acciones necesarias]

---

**Logged by**: log-autogen-session.ps1
**Version**: 1.0
"@

# Guardar log
$log | Out-File -FilePath $filepath -Encoding UTF8

Write-Host "? Log guardado: $filepath" -ForegroundColor Green

# Actualizar summary diario
$summaryFile = "autogen/sessions/summary/$date`_daily.md"
$summaryEntry = "- [$timestamp] $TaskID - $Status (${Duration}s)`n"

if (Test-Path $summaryFile) {
    Add-Content -Path $summaryFile -Value $summaryEntry
} else {
    $summaryHeader = @"
# Daily Summary - $date

## Sessions

"@
    $summaryHeader + $summaryEntry | Out-File -FilePath $summaryFile -Encoding UTF8
}

Write-Host "? Summary actualizado: $summaryFile" -ForegroundColor Green
