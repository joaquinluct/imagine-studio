# AutoGen Studio Context - Imagine Studio

> **Prop�sito**: Contexto para GitHub Copilot Chat sobre el sistema multi-agente de AutoGen Studio.

---

## ?? SISTEMA MULTI-AGENTE ACTUAL

### **4 Agentes Especializados**

1. **Planner Agent** (`autogen/agents/planner_agent_fixed.json`)
   - Rol: Sprint Manager
   - Tools: `read_daily`, `read_tasks`, `read_sprint`, `list_files`
   - System Message: Propone implementaci�n de tareas del sprint

2. **Coder Agent** (`autogen/agents/coder_agent_fixed.json`)
   - Rol: Implementation Specialist
   - Tools: `read_file`, `write_file`, `read_standards`, `list_files`
   - System Message: Implementa c�digo C++14 con est�ndares AAA (UTF-8 BOM, CRLF)

3. **Reviewer Agent** (`autogen/agents/reviewer_agent_fixed.json`)
   - Rol: Quality Assurance
   - Tools: `read_file`, `read_main`, `read_standards`, `check_external`
   - System Message: Revisa c�digo contra est�ndares AAA, dice "REVIEW_APPROVED" si OK

4. **Tester Agent** (`autogen/agents/tester_agent_fixed.json`)
   - Rol: Build & Validation
   - Tools: `compile_cmake`, `compile_msbuild`, `run_tests`, `read_file`
   - System Message: Ejecuta CMake + MSBuild, dice "WORKFLOW_FINISHED_OK" si todo pasa

### **Team Configuration**

- **Type**: `RoundRobinGroupChat` (turnos secuenciales fijos)
- **Order**: planner ? coder ? reviewer ? tester ? (loop)
- **Termination**: `WORKFLOW_FINISHED_OK` o 100 mensajes max
- **File**: `autogen/teams/team_complete.json`

---

## ?? ESTRUCTURA DE CARPETAS

```
autogen/
??? agents/              [4 agentes JSON]
??? teams/               [Configuraciones de teams]
??? prompts/             [Plantillas reutilizables por sprint]
?   ??? sprint_v1.9.0/
?   ??? common/
??? sessions/            [Logs de ejecuci�n]
?   ??? summary/
??? outputs/             [Archivos generados temporalmente]
```

---

## ?? WORKFLOW T�PICO

1. **Crear prompt** en `autogen/prompts/sprint_vX.Y.Z/TASK_ID.md`
2. **Ejecutar** en AutoGen Studio Playground (http://127.0.0.1:8081)
3. **Registrar log** con `autogen/log-autogen-session.ps1`
4. **Commit** archivos generados + log

---

## ?? PROMPTS EFECTIVOS PARA GITHUB COPILOT

### **Para crear nuevo agente**
```
Create a new AutoGen agent JSON file for [ROLE] that:
- Uses tools: [list tools]
- Follows the structure in autogen/agents/planner_agent_fixed.json
- System message should say [TERMINATION_WORD] when done
```

### **Para crear nuevo prompt**
```
Create a prompt for task [TASK_ID] in autogen/prompts/sprint_v1.9.0/
Following the template in autogen/prompts/_template.md
Task details: [description]
```

### **Para debugging**
```
AutoGen session failed with error: [error message]
Check team configuration in autogen/teams/team_complete.json
And agent system messages for correct termination keywords
```

### **Para crear team**
```
Create a new team JSON in autogen/teams/ with:
- Type: RoundRobinGroupChat
- Agents: [list]
- Termination: [keyword]
Following autogen/teams/team_complete.json structure
```

---

## ?? KEYWORDS IMPORTANTES

- `WORKFLOW_FINISHED_OK` ? Tester dice esto cuando build OK
- `REVIEW_APPROVED` ? Reviewer dice esto cuando code OK
- `CODE_READY` ? Coder dice esto cuando implementaci�n completa
- `TASK_COMPLETED` ? Planner dice esto cuando tarea terminada

---

## ?? INTEGRACI�N CON METODOLOG�A

- **Sprint files**: `docs/sprint.md`, `docs/sprint_tasks.md`
- **Daily tracking**: `docs/daily.md`
- **AAA Standards**: `docs/MAIN.md`
- **Methodology**: `docs/methodology/CORE.md`
- **AutoGen logs** ? `docs/sprints/sprint_vX.Y.Z_autogen/` al cerrar sprint

---

## ?? REGLAS CR�TICAS

1. **Nunca modificar** `external/` (agentes tampoco pueden)
2. **UTF-8 with BOM + CRLF** para archivos C++ (Coder Agent hace esto)
3. **2 builds obligatorios**: CMake + MSBuild (Tester Agent ejecuta ambos)
4. **Terminaci�n**: Usar `WORKFLOW_FINISHED_OK`, NO `BUILD_SUCCESS` (evita terminaci�n prematura)
5. **Working directory**: AutoGen Studio debe correr desde ra�z del proyecto

---

## ?? COMANDOS �TILES

### Iniciar AutoGen Studio
```powershell
cd "C:\Users\joaqu\source\repos\Imagine Studio"
autogenstudio ui --port 8081 --appdir .
```

### Registrar log
```powershell
.\autogen\log-autogen-session.ps1 -TaskID "H1.1" -Status "SUCCESS" -LogContent "..." -Duration 210
```

### Crear nuevo prompt desde template
```powershell
Copy-Item "autogen/prompts/_template.md" "autogen/prompts/sprint_v1.9.0/H1.X.md"
```

---

## ?? ARCHIVOS DE REFERENCIA

- **Setup completo**: `autogen/SETUP.md`
- **Workflow detallado**: `autogen/WORKFLOW_COMPLETE.md`
- **Troubleshooting**: `autogen/TERMINATION_FIX.md`
- **Estructura**: `autogen/README.md`

---

**Versi�n**: 1.0  
**�ltima actualizaci�n**: 2025-01-18  
**Proyecto**: Imagine Studio Multi-Agent System
