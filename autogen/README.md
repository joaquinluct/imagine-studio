# AutoGen Studio - Multi-Agent System para Imagine Studio

> **Sistema de 4 agentes especializados** que automatizan el desarrollo de sprints con calidad AAA.

---

## ?? ESTRUCTURA DE CARPETAS

```
?? autogen/
??? ?? agents/                    [Agentes individuales - JSON]
?   ??? planner_agent_fixed.json
?   ??? coder_agent_fixed.json
?   ??? reviewer_agent_fixed.json
?   ??? tester_agent_fixed.json
?
??? ?? teams/                     [Configuraciones de teams]
?   ??? devteam_complete.json
?   ??? devteam_workflow.json
?   ??? devteam_simple.json
?
??? ?? prompts/                   [Plantillas de prompts]
?   ??? _template.md              [Plantilla base]
?   ??? sprint_v1.9.0/            [Prompts por sprint]
?   ?   ??? H1.1_assetdatabase_h.md
?   ?   ??? H1.2_assetdatabase_cpp.md
?   ?   ??? ...
?   ??? common/                   [Prompts comunes]
?       ??? simple_test.md
?       ??? debug_build.md
?
??? ?? sessions/                  [Logs de ejecución]
?   ??? 2025-01-18_H1.1_success.md
?   ??? 2025-01-18_H1.2_failed.md
?   ??? summary/
?       ??? 2025-01-18_daily.md
?
??? ?? outputs/                   [Archivos generados]
?   ??? 2025-01-18_H1.1/
?   ?   ??? src/assets/AssetDatabase.h
?   ??? build_logs/
?       ??? cmake_output.log
?       ??? msbuild_output.log
?
??? ?? README.md                  [Este archivo]
??? ?? SETUP.md                   [Guía de instalación]
??? ?? TERMINATION_FIX.md         [Troubleshooting]
```

---

## ?? AGENTES

### 1. **Planner Agent** (`agents/planner_agent_fixed.json`)
- **Rol**: Sprint Manager
- **Responsabilidades**:
  - Leer `docs/daily.md` y `docs/sprint_tasks.md`
  - Proponer siguiente tarea
  - Descomponer tareas complejas
  - Decidir orden de implementación
- **Tools**: `read_daily`, `read_tasks`

### 2. **Coder Agent** (`agents/coder_agent_fixed.json`)
- **Rol**: Implementation Specialist
- **Responsabilidades**:
  - Implementar código según plan
  - Seguir estándares AAA (C++14, UTF-8 BOM, CRLF)
  - Crear archivos nuevos
- **Tools**: `read_file`, `write_file`

### 3. **Reviewer Agent** (`agents/reviewer_agent_fixed.json`)
- **Rol**: Quality Assurance
- **Responsabilidades**:
  - Revisar código del Coder
  - Verificar estándares AAA
  - Detectar problemas arquitectónicos
- **Tools**: `read_file`

### 4. **Tester Agent** (`agents/tester_agent_fixed.json`)
- **Rol**: Build & Validation
- **Responsabilidades**:
  - Ejecutar CMake + MSBuild
  - Reportar errores
  - Decir `WORKFLOW_FINISHED_OK` si todo pasa
- **Tools**: `compile_cmake`, `compile_msbuild`

---

## ?? INICIO RÁPIDO

### 1. Instalar AutoGen Studio

```bash
pip install autogenstudio
```

### 2. Iniciar AutoGen Studio

```powershell
# IMPORTANTE: Iniciar desde directorio raíz del proyecto
cd "C:\Users\joaqu\source\repos\Imagine Studio"
autogenstudio ui --port 8081 --appdir .
```

### 3. Importar Agentes

En AutoGen Studio (`http://127.0.0.1:8081`):

1. **Gallery** ? **Import** ? Importar los 4 agentes:
   - `agents/planner_agent_fixed.json`
   - `agents/coder_agent_fixed.json`
   - `agents/reviewer_agent_fixed.json`
   - `agents/tester_agent_fixed.json`

### 4. Importar Team

1. **Team Builder** ? **Import** ? `teams/devteam_complete.json`
2. Verificar que tiene 4 agentes y terminación configurada

### 5. Usar Prompts

**Playground** ? Copiar contenido de:
- `prompts/sprint_v1.9.0/H1.1_assetdatabase_h.md`
- Pegar en Playground
- Click "Run"

---

## ?? USO DE PROMPTS

### **Estructura de Prompts**

Cada prompt sigue el formato:
```markdown
# Prompt: [TASK_ID] - [TASK_NAME]

## Context
- Sprint: [VERSION]
- Task: [TASK_ID]

## Implementation Details
- [Details]

## Workflow
1. Planner: ...
2. Coder: ...
3. Reviewer: ...
4. Tester: ...
```

### **Crear Nuevo Prompt**

1. Copiar `prompts/_template.md`
2. Reemplazar `[PLACEHOLDER]` con valores reales
3. Guardar en `prompts/sprint_vX.Y.Z/`

---

## ?? LOGS DE SESIÓN

### **Registrar Sesión**

Después de ejecutar un prompt en AutoGen Studio:

1. **Copiar output** del Playground
2. **Crear archivo**: `sessions/YYYY-MM-DD_TASK_ID_status.md`
3. **Formato**:
```markdown
# AutoGen Session Log - [TASK_ID]

**Date**: YYYY-MM-DD HH:MM:SS
**Sprint**: vX.Y.Z
**Task**: [TASK_ID]
**Status**: ? SUCCESS / ? FAILED
**Duration**: Xm Ys

## Agent Execution Log
[Logs de cada agente]

## Files Created/Modified
[Lista de archivos]

## Build Results
[Resultados de compilación]
```

---

## ?? WORKFLOW

```
???????????????
?  PLANNER    ? ? Lee daily.md, propone plan
???????????????
       ?
???????????????
?   CODER     ? ? Implementa código AAA
???????????????
       ?
???????????????
?  REVIEWER   ? ? Revisa calidad
???????????????
       ? (si APPROVED)
???????????????
?   TESTER    ? ? Compila (CMake + MSBuild)
???????????????
       ? (si PASS)
  WORKFLOW_FINISHED_OK

Si falla ? Loop back automático
```

---

## ?? BENEFICIOS

| Métrica | Sin Multi-Agent | Con Multi-Agent | Mejora |
|---------|-----------------|-----------------|--------|
| Tiempo/tarea | 5-10 min | 2-4 min | **2-3x** |
| Errores de build | Frecuentes | Raros | **10x menos** |
| Calidad código | Variable | Consistente | **100%** |
| Cobertura review | ~60% | ~100% | **+40%** |

---

## ?? INTEGRACIÓN CON `docs/`

### **Sincronización con Sprints**

Al cerrar un sprint:
```powershell
# Copiar logs de AutoGen a docs/sprints/
Copy-Item "autogen/sessions/summary/YYYY-MM-DD_sprint_summary.md" `
          "docs/sprints/sprint_vX.Y.Z_autogen_logs.md"
```

### **Backup de Outputs**

Archivos generados se guardan en `autogen/outputs/` antes del commit:
```powershell
# Backup automático
autogen/outputs/YYYY-MM-DD_TASK_ID/
??? [Archivos generados]
```

---

## ?? DOCUMENTACIÓN COMPLETA

Ver [`SETUP.md`](SETUP.md) para:
- Guía detallada de instalación
- Troubleshooting completo
- Ejemplos de uso avanzados
- Configuración de cada agente

---

## ?? CASOS DE USO

### ? Implementar tarea única
```
Prompt: prompts/sprint_v1.9.0/H1.1_assetdatabase_h.md
```

### ? Implementar múltiples tareas
```
Prompt: prompts/sprint_v1.9.0/batch_H1_all.md
```

### ? Testing del sistema
```
Prompt: prompts/common/simple_test.md
```

---

**Versión**: 2.0  
**Última actualización**: 2025-01-18  
**Licencia**: MIT  
**Autor**: Joaquín Luct

---

**?? ¡DISFRUTA DEL DESARROLLO AUTOMATIZADO!**
