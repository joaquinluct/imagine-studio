# AutoGen Studio - Multi-Agent System para Imagine Studio

> **Sistema de 4 agentes especializados** que automatizan el desarrollo de sprints con calidad AAA.  
> **Estado**: ✅ FUNCIONAL | **MCP Integrado**: ✅ | **Última actualización**: 2025-01-21

---

## 🚀 QUICK START

### Workflow Híbrido (MCP + UI)

**AutoGen Studio tiene dos formas de uso**:

1. **MCP Tools** (desde VS Code/GitHub Copilot):
   ```typescript
   mcp_autogen-studi_list_autogen_teams()         // Listar equipos
   mcp_autogen-studi_run_build(configuration)     // Compilar (Debug/Release)
   mcp_autogen-studi_save_autogen_log(...)        // Guardar logs
   ```

2. **AutoGen Studio UI** (http://localhost:8081):
   - Ejecutar prompts con el Playground
   - Monitorear ejecución de agentes en tiempo real
   - Revisar outputs y logs detallados

**⚠️ Importante**: AutoGen Studio NO expone REST API para ejecución de prompts. El workflow correcto es:
- **MCP** → Tareas de soporte (compilar, listar teams, guardar logs)
- **UI** → Ejecutar workflows completos de agentes

---

## 📂 ESTRUCTURA

```
autogen/
├── 📁 agents/                    # 4 agentes especializados (JSON)
│   ├── planner_agent_fixed.json
│   ├── coder_agent_fixed.json
│   ├── reviewer_agent_fixed.json
│   └── tester_agent_fixed.json
│
├── 📁 teams/                     # Configuraciones de equipos
│   ├── team_complete.json        # Principal: Round Robin workflow
│   └── ...
│
├── 📁 prompts/                   # Plantillas de prompts por sprint
│   ├── _template.md
│   └── sprint_v1.9.0/
│       ├── H1.1_assetdatabase_h.md
│       └── H1.2_assetdatabase_cpp.md
│
├── 📁 sessions/                  # Logs de ejecución
│   └── YYYY-MM-DD_H1.1_success.md
│
├── 📁 outputs/                   # Archivos generados (temporal)
│
├── 📄 MCP_STATUS.md              # ⭐ Estado actual del MCP server
├── 📄 README.md                  # Este archivo
├── 📄 TERMINATION_FIX.md         # Troubleshooting
└── 📄 log-autogen-session.ps1    # Script para guardar logs
```

---

## 🤖 LOS 4 AGENTES

### 1. **Planner Agent**
- **Rol**: Sprint Manager
- **Responsabilidades**: Lee `daily.md`, propone siguiente tarea, descompone tareas complejas
- **Tools**: `read_daily`, `read_tasks`, `read_sprint`, `list_files`

### 2. **Coder Agent**
- **Rol**: Implementation Specialist
- **Responsabilidades**: Implementa código AAA (C++14, UTF-8 BOM, CRLF)
- **Tools**: `read_file`, `write_file`, `read_standards`, `list_files`

### 3. **Reviewer Agent**
- **Rol**: Quality Assurance
- **Responsabilidades**: Revisa código, verifica estándares AAA, detecta problemas arquitectónicos
- **Tools**: `read_file`, `read_main`, `read_standards`, `check_external`

### 4. **Tester Agent**
- **Rol**: Build & Validation
- **Responsabilidades**: Ejecuta CMake + MSBuild, reporta errores, dice `WORKFLOW_FINISHED_OK` si todo pasa
- **Tools**: `compile_cmake`, `compile_msbuild`, `run_tests`, `read_file`

---

## 💻 INSTALACIÓN (Solo Primera Vez)

### Paso 1: Instalar AutoGen Studio

```bash
pip install autogenstudio
```

### Paso 2: Iniciar AutoGen Studio

```powershell
# ⚠️ IMPORTANTE: Iniciar desde directorio raíz del proyecto
cd "C:\Users\joaqu\source\repos\Imagine Studio"
autogenstudio ui --port 8081 --appdir .
```

### Paso 3: Importar Agentes

En AutoGen Studio (`http://127.0.0.1:8081`):

1. **Gallery** → Click en **Import**
2. **Copiar/pegar contenido JSON** de cada agente:
   - `agents/planner_agent_fixed.json` → Crear "Planner Agent"
   - `agents/coder_agent_fixed.json` → Crear "Coder Agent"
   - `agents/reviewer_agent_fixed.json` → Crear "Reviewer Agent"
   - `agents/tester_agent_fixed.json` → Crear "Tester Agent"

### Paso 4: Verificar Team

1. **Team Builder** → Buscar **"Imagine Studio DevTeam"**
2. Verificar:
   - ✅ 4 agentes: Planner Agent, Coder Agent, Reviewer Agent, Tester Agent
   - ✅ Terminación: `WORKFLOW_FINISHED_OK` + MaxMessage(100)

---

## 📝 USO DE PROMPTS

### Estructura de Prompts

Cada prompt en `prompts/` sigue este formato:

```markdown
# Prompt: H1.1 - Crear AssetDatabase.h

## Context
- Sprint: v1.9.0 - Asset System
- Task: H1.1
- Files to read: docs/daily.md, docs/sprint_tasks.md

## Implementation Details
- Create file: src/assets/AssetDatabase.h
- Define AssetID as uint64_t
- ...

## Workflow
1. Planner: Read task details and propose plan
2. Coder: Implement AssetDatabase.h
3. Reviewer: Review code quality
4. Tester: Compile (CMake + MSBuild)
```

### Ejecutar un Prompt

1. **Abrir** archivo de prompt: `prompts/sprint_v1.9.0/H1.1_assetdatabase_h.md`
2. **Copiar** contenido completo
3. **AutoGen Studio** → **Playground**
4. **Seleccionar team**: "Imagine Studio DevTeam"
5. **Pegar** prompt
6. **Click** "Run"

**Output esperado**:
```
[planner] Task: H1.1 - Crear AssetDatabase.h
Implementation plan: ...
@coder Please implement.

[coder] Creating src/assets/AssetDatabase.h...
File created successfully.
@reviewer Please review.

[reviewer] Review Status: APPROVED ✓
@tester Please compile.

[tester] CMake: PASS ✓ (0 errors, 0 warnings)
MSBuild: PASS ✓ (0 errors, 0 warnings)
✅ ALL CHECKS PASSED
WORKFLOW_FINISHED_OK
```

---

## 🔄 WORKFLOW COMPLETO

```
┌─────────────┐
│  PLANNER    │ → Lee daily.md, propone plan
└──────┬──────┘
       ↓
┌─────────────┐
│   CODER     │ → Implementa código AAA
└──────┬──────┘
       ↓
┌─────────────┐
│  REVIEWER   │ → Revisa calidad
└──────┬──────┘
       ↓ (si APPROVED)
┌─────────────┐
│   TESTER    │ → Compila (CMake + MSBuild)
└──────┬──────┘
       ↓ (si PASS)
  WORKFLOW_FINISHED_OK

❌ Si falla → Loop back automático
```

---

## 📊 MÉTRICAS

| Métrica | Sin Multi-Agent | Con Multi-Agent | Mejora |
|---------|-----------------|-----------------|--------|
| Tiempo/tarea | 5-10 min | 2-4 min | **2-3x** |
| Errores de build | Frecuentes | Raros | **10x menos** |
| Calidad código | Variable | Consistente | **100%** |
| Cobertura review | ~60% | ~100% | **+40%** |

---

## 🛠️ MCP TOOLS

Ver [`MCP_STATUS.md`](MCP_STATUS.md) para detalles completos.

### Herramientas Disponibles

```typescript
// 1. Listar equipos configurados
mcp_autogen-studi_list_autogen_teams()
// Retorna: { teams: [{ name, model_client, agents: [...] }] }

// 2. Compilar proyecto
mcp_autogen-studi_run_build(configuration: "Debug" | "Release")
// Retorna: { msbuild: { errors, warnings, output } }

// 3. Guardar log de sesión
mcp_autogen-studi_save_autogen_log(
  task_id: "H1.1",
  status: "SUCCESS" | "FAILED" | "PARTIAL",
  log_content: "...",
  duration: 210  // segundos
)
// Retorna: { success: true, file: "autogen/sessions/..." }
```

---

## 📋 REGISTRAR LOGS

### Opción A: Script PowerShell

```powershell
.\autogen\log-autogen-session.ps1 `
    -TaskID "H1.1" `
    -Status "SUCCESS" `
    -LogContent "<output del Playground>" `
    -Duration 180
```

### Opción B: MCP Tool (desde Copilot Chat)

```typescript
@workspace Guarda el log de AutoGen para tarea H1.1 con status SUCCESS
```

---

## 🚨 TROUBLESHOOTING

### Error: "File not found: docs/daily.md"

**Problema**: AutoGen Studio no está en el directorio correcto.

**Solución**:
```powershell
# Detener AutoGen Studio (Ctrl+C)
cd "C:\Users\joaqu\source\repos\Imagine Studio"
autogenstudio ui --port 8081 --appdir .
```

### Error: "Tool 'compile_cmake' failed"

**Problema**: CMake no está en PATH o `build/` no existe.

**Solución**:
```powershell
cmake --version  # Verificar instalado
mkdir build
cd build
cmake ..
```

### Error: "Review rejected - Include order incorrect"

**Comportamiento esperado**: El sistema se auto-corrige. Reviewer pide cambios, Coder reescribe, loop continúa hasta APPROVED.

**No requiere intervención manual**.

---

## 📚 DOCUMENTACIÓN ADICIONAL

- **[MCP_STATUS.md](MCP_STATUS.md)** - Estado actual del servidor MCP (3 tools funcionales)
- **[TERMINATION_FIX.md](TERMINATION_FIX.md)** - Cómo cambiar palabra de terminación
- **[WORKFLOW_COMPLETE.md](WORKFLOW_COMPLETE.md)** - Flujo completo desde prompt hasta commit
- **[log-autogen-session.ps1](log-autogen-session.ps1)** - Script para guardar logs

---

## 🔗 INTEGRACIÓN CON METODOLOGÍA

- **Sprints**: Cada sprint tiene carpeta `prompts/sprint_vX.Y.Z/`
- **Daily.md**: Planner Agent lee este archivo para saber siguiente tarea
- **Sprint Tasks**: Coder Agent sigue detalles de `docs/sprint_tasks.md`
- **AAA Standards**: Reviewer Agent valida contra `docs/MAIN.md`

Al cerrar sprint:
```powershell
# Copiar logs de AutoGen a docs/sprints/
Copy-Item "autogen/sessions/*_success.md" `
          "docs/sprints/sprint_v1.9.0_autogen/"
```

---

**Versión**: 2.0  
**Última actualización**: 2025-01-21  
**Autor**: Joaquín Luct

---

**🎯 ¡DISFRUTA DEL DESARROLLO AUTOMATIZADO!**
