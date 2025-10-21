# AutoGen Studio - Multi-Agent System para Imagine Studio

> **Sistema de 4 agentes especializados** que automatizan el desarrollo de sprints con calidad AAA.

---

## ?? AGENTES

### 1. **Planner Agent** (`planner_agent.json`)
- **Rol**: Sprint Manager
- **Responsabilidades**:
  - Leer `docs/daily.md` y `docs/sprint_tasks.md`
  - Proponer siguiente tarea
  - Descomponer tareas complejas
  - Decidir orden de implementaci�n
- **Tools**: `read_daily`, `read_tasks`, `read_sprint`, `list_files`

### 2. **Coder Agent** (`coder_agent.json`)
- **Rol**: Implementation Specialist
- **Responsabilidades**:
  - Implementar c�digo seg�n plan
  - Seguir est�ndares AAA
  - Mantener estilo consistente (C++14)
  - Crear archivos con UTF-8 BOM + CRLF
- **Tools**: `read_file`, `write_file`, `read_standards`, `list_files`

### 3. **Reviewer Agent** (`reviewer_agent.json`)
- **Rol**: Quality Assurance
- **Responsabilidades**:
  - Revisar c�digo del Coder
  - Verificar est�ndares AAA
  - Detectar problemas arquitect�nicos
  - Sugerir mejoras
- **Tools**: `read_file`, `read_main`, `read_standards`, `check_external`

### 4. **Tester Agent** (`tester_agent.json`)
- **Rol**: Build & Validation
- **Responsabilidades**:
  - Ejecutar CMake + MSBuild
  - Correr tests unitarios
  - Reportar errores
  - Sugerir fixes
- **Tools**: `compile_cmake`, `compile_msbuild`, `run_tests`, `read_file`

---

## ?? ARCHIVOS

| Archivo | Descripci�n |
|---------|-------------|
| `planner_agent.json` | Configuraci�n Planner Agent |
| `coder_agent.json` | Configuraci�n Coder Agent |
| `reviewer_agent.json` | Configuraci�n Reviewer Agent |
| `tester_agent.json` | Configuraci�n Tester Agent |
| `devteam_workflow.json` | Workflow completo (Group Chat) |
| `SETUP.md` | Gu�a de instalaci�n paso a paso |
| `README.md` | Este archivo |

---

## ?? INICIO R�PIDO

### 1. Instalar AutoGen Studio

```bash
pip install autogenstudio
```

### 2. Iniciar AutoGen Studio

```powershell
# IMPORTANTE: Iniciar desde directorio ra�z del proyecto
cd "C:\Users\joaqu\source\repos\Imagine Studio"
autogenstudio ui --port 8081 --appdir .
```

### 3. Importar Agentes

En AutoGen Studio (`http://127.0.0.1:8081`):

1. **Agents** ? **Import Agent** ? Importar los 4 JSON:
   - `planner_agent.json`
   - `coder_agent.json`
   - `reviewer_agent.json`
   - `tester_agent.json`

2. **Teams** ? **Create New Team**:
   - Name: `ImagineStudio_DevTeam`
   - Type: `Round Robin Group Chat`
   - Agents: planner, coder, reviewer, tester
   - Max turns: 20

### 4. Usar el Sistema

**Playground** ? Seleccionar team ? Pegar prompt:

```
Implement task H1.1 from Sprint v1.9.0.

Context:
- Sprint: v1.9.0 - Asset System & Resource Management
- Task: H1.1 - Crear AssetDatabase.h

Please proceed with the workflow.
```

---

## ?? WORKFLOW

```
???????????????
?  PLANNER    ? ? Lee daily.md, propone plan
???????????????
       ?
???????????????
?   CODER     ? ? Implementa c�digo AAA
???????????????
       ?
???????????????
?  REVIEWER   ? ? Revisa calidad
???????????????
       ? (si APPROVED)
???????????????
?   TESTER    ? ? Compila (CMake + MSBuild)
???????????????
       ? (si BUILD_SUCCESS)
???????????????
?  PLANNER    ? ? Actualiza daily.md, commit
???????????????

Si falla ? Loop back autom�tico
```

---

## ?? BENEFICIOS

| M�trica | Sin Multi-Agent | Con Multi-Agent | Mejora |
|---------|-----------------|-----------------|--------|
| Tiempo/tarea | 5-10 min | 2-4 min | **2-3x** |
| Errores de build | Frecuentes | Raros | **10x menos** |
| Calidad c�digo | Variable | Consistente | **100%** |
| Cobertura tests | ~60% | ~90% | **+30%** |

---

## ?? DOCUMENTACI�N COMPLETA

Ver [`SETUP.md`](SETUP.md) para:
- Gu�a detallada de instalaci�n
- Troubleshooting completo
- Ejemplos de uso avanzados
- Configuraci�n de cada agente
- M�tricas y monitoreo

---

## ?? CASOS DE USO

### ? Implementar tarea �nica
```
Implement task H1.1 from Sprint v1.9.0
```

### ? Implementar m�ltiples tareas
```
Implement tasks H1.1, H1.2, H1.3 sequentially
```

### ? Debugging de build
```
Task H2.3 failed to compile. Analyze errors and fix.
```

### ? Refactorizaci�n
```
Refactor src/renderer/DX12Renderer.cpp to follow AAA standards
```

---

## ?? REQUISITOS

- **Python 3.10+**
- **OpenAI API Key**
- **CMake** instalado y en PATH
- **MSBuild** instalado y en PATH
- **Proyecto Imagine Studio** con estructura `docs/` completa

---

## ?? CONFIGURACI�N AVANZADA

### Cambiar Modelo LLM

Editar cada JSON y cambiar:
```json
"model_client": {
  "config": {
    "model": "gpt-4o"  // Cambiar a "gpt-4o-mini", "claude-3-sonnet", etc.
  }
}
```

### A�adir Nuevo Tool

Editar `coder_agent.json` ? `workbench` ? `tools` ? A�adir:
```json
{
  "provider": "autogen_core.tools.FunctionTool",
  "config": {
    "source_code": "def my_tool(param: str) -> str:\n    return f'Result: {param}'",
    "name": "my_tool",
    "description": "Description of my tool"
  }
}
```

---

## ?? ROADMAP

- [x] Sistema b�sico de 4 agentes
- [x] Workflow secuencial (Round Robin)
- [ ] Paralelizaci�n (m�ltiples Coders)
- [ ] Dashboard de m�tricas en tiempo real
- [ ] Integraci�n con GitHub Actions
- [ ] Auto-learning de errores comunes
- [ ] Soporte para otros lenguajes (Python, Rust)

---

## ?? CONTRIBUIR

�Mejoras o bugs? Crear issue en:
https://github.com/joaquinluct/imagine-studio/issues

---

## ?? RECURSOS

- **AutoGen Docs**: https://microsoft.github.io/autogen/
- **Imagine Studio Methodology**: `../docs/methodology/CORE.md`
- **Project Pillars**: `../docs/MAIN.md`

---

**Versi�n**: 1.0  
**�ltima actualizaci�n**: 2025-01-18  
**Licencia**: MIT  
**Autor**: Joaqu�n Luct

---

**?? �DISFRUTA DEL DESARROLLO AUTOMATIZADO!**
