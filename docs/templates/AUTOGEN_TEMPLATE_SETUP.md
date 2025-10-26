# AutoGen + MCP Template Setup Guide

> **Prop�sito**: Gu�a para desplegar sistema multi-agente AutoGen + MCP en cualquier proyecto nuevo.

---

## ?? OVERVIEW

Este template permite crear una infraestructura completa de desarrollo asistido por IA multi-agente en **cualquier proyecto de software** (C++, Python, React, Unity, etc.).

### �Qu� incluye?

- ? **4 agentes especializados** (Planner, Coder, Reviewer, Tester)
- ? **MCP Server** para integraci�n con GitHub Copilot
- ? **Team configurations** (Round Robin workflow)
- ? **Prompt templates** reutilizables
- ? **Logging system** autom�tico
- ? **Metodolog�a �gil** integrada

### Tiempo de setup: ~15 minutos

---

## ?? PREREQUISITOS

1. **AutoGen Studio instalado**:
   ```bash
   pip install autogenstudio
   ```

2. **Node.js 18+** (para MCP Server):
   ```bash
   node --version  # Debe ser >= 18
   ```

3. **OpenAI API Key** v�lida

4. **Metodolog�a base** instalada:
   ```bash
   # Copiar templates de metodolog�a
   cp docs/templates/copilot-instructions.template.md .github/copilot-instructions.md
   cp docs/templates/CORE.template.md docs/methodology/CORE.md
   ```

---

## ?? QUICK START (3 Pasos)

### Paso 1: Copiar estructura base

```powershell
# Desde la ra�z de tu proyecto nuevo
Copy-Item -Recurse "path/to/imagine-studio/docs/templates/autogen_template/" "./autogen_template/"

# Ejecutar script de inicializaci�n
.\autogen_template\init-autogen-project.ps1 -ProjectName "MiProyecto" -ProjectType "Python"
```

### Paso 2: Configurar con GitHub Copilot

Abre GitHub Copilot Chat y ejecuta:

```
@workspace 

I've copied the autogen_template/ folder to my project. 

Project details:
- Name: [NOMBRE_PROYECTO]
- Language: [LENGUAJE] (e.g., C++, Python, React, Unity C#)
- Build command: [COMANDO_BUILD] (e.g., npm run build, cargo build, cmake --build)
- Test command: [COMANDO_TEST] (e.g., pytest, cargo test, npm test)

Please:
1. Replace all [PLACEHOLDER] values in autogen/ files
2. Update build/test commands in agents JSON files
3. Create MCP server configuration
4. Generate initial sprint files in docs/

Ready to deploy AutoGen system.
```

### Paso 3: Iniciar servicios

```powershell
# 1. Compilar MCP Server
cd mcp-server
npm install
npm run build

# 2. Configurar GitHub Copilot
# A�adir a ~/.github-copilot/config.json:
# (Copilot te mostrar� el JSON completo)

# 3. Iniciar AutoGen Studio
cd ..
autogenstudio ui --port 8081 --appdir .
```

---

## ?? ESTRUCTURA DE CARPETAS CREADA

```
tu-proyecto/
??? autogen/
?   ??? agents/
?   ?   ??? planner_agent.json        [PLACEHOLDER: build commands]
?   ?   ??? coder_agent.json          [PLACEHOLDER: language, style]
?   ?   ??? reviewer_agent.json       [PLACEHOLDER: standards]
?   ?   ??? tester_agent.json         [PLACEHOLDER: test commands]
?   ?
?   ??? teams/
?   ?   ??? team_complete.json        [Auto-configurado]
?   ?   ??? team_simple.json          [Auto-configurado]
?   ?
?   ??? prompts/
?   ?   ??? _template.md              [Plantilla base]
?   ?   ??? common/
?   ?       ??? simple_test.md        [Ejemplo]
?   ?
?   ??? sessions/                     [Logs autom�ticos]
?   ?   ??? summary/
?   ?
?   ??? outputs/                      [Archivos generados]
?   ?
?   ??? README.md                     [Instrucciones de uso]
?   ??? SETUP.md                      [Configuraci�n detallada]
?   ??? WORKFLOW_COMPLETE.md          [Flujo completo]
?   ??? log-autogen-session.ps1       [Script de logging]
?
??? mcp-server/
?   ??? src/
?   ?   ??? index.ts                  [PLACEHOLDER: project paths]
?   ??? package.json                  [PLACEHOLDER: project name]
?   ??? tsconfig.json                 [Auto-configurado]
?   ??? README.md                     [Instrucciones MCP]
?
??? docs/
?   ??? methodology/
?   ?   ??? CORE.md                   [Ya configurado]
?   ?   ??? ...
?   ??? sprint.md                     [GitHub Copilot crear�]
?   ??? sprint_tasks.md               [GitHub Copilot crear�]
?   ??? daily.md                      [GitHub Copilot crear�]
?
??? .github/
    ??? copilot-instructions.md       [Ya configurado]
    ??? autogen-context.md            [GitHub Copilot crear�]
```

---

## ?? AGENTES INCLUIDOS

### 1. Planner Agent
- **Rol**: Sprint Manager
- **Tools**: `read_daily`, `read_tasks`, `read_sprint`, `list_files`
- **Configurable**:
  - Formato de sprint files
  - Task ID pattern (e.g., H1.1, US-123)

### 2. Coder Agent
- **Rol**: Implementation Specialist
- **Tools**: `read_file`, `write_file`, `read_standards`, `list_files`
- **Configurable**:
  - Lenguaje y versi�n
  - Style guide (PEP8, Google Style, etc.)
  - File encoding (UTF-8 BOM, UTF-8, ASCII)

### 3. Reviewer Agent
- **Rol**: Quality Assurance
- **Tools**: `read_file`, `read_main`, `read_standards`, `check_external`
- **Configurable**:
  - Code standards
  - Linting rules
  - Security checks

### 4. Tester Agent
- **Rol**: Build & Validation
- **Tools**: `compile_[LANG]`, `run_tests`, `run_linter`, `read_file`
- **Configurable**:
  - Build system (CMake, npm, cargo, etc.)
  - Test framework (pytest, vitest, gtest, etc.)
  - Success criteria

---

## ?? CONFIGURACI�N POR TECNOLOG�A

### Python (FastAPI / Flask)

```json
// Coder Agent - system_message
"You are a Python expert. Write clean, type-annotated Python 3.11+ code following PEP 8. Use async/await for I/O operations. File encoding: UTF-8 (no BOM). Line endings: LF."

// Tester Agent - tools
"compile_python": "python -m py_compile {file}",
"run_tests": "pytest tests/ -v",
"run_linter": "mypy . && black --check . && flake8 ."
```

### React + TypeScript

```json
// Coder Agent - system_message
"You are a React expert. Write functional components with TypeScript 5+. Use hooks (useState, useEffect). Follow ESLint rules. File encoding: UTF-8 (no BOM). Line endings: LF."

// Tester Agent - tools
"compile_typescript": "npm run build",
"run_tests": "npm test -- --run",
"run_linter": "npm run lint && npm run type-check"
```

### Rust

```json
// Coder Agent - system_message
"You are a Rust expert. Write idiomatic Rust 1.70+. Use Result<T,E> for error handling. Follow Clippy suggestions. File encoding: UTF-8 (no BOM). Line endings: LF."

// Tester Agent - tools
"compile_rust": "cargo build --all-features",
"run_tests": "cargo test --all-features",
"run_linter": "cargo clippy -- -D warnings && cargo fmt --check"
```

### Unity C#

```json
// Coder Agent - system_message
"You are a Unity expert. Write C# 9.0+ code following Unity conventions. Use [SerializeField], avoid Update() for logic. File encoding: UTF-8 with BOM. Line endings: CRLF."

// Tester Agent - tools
"compile_unity": "\"C:\\Program Files\\Unity\\Hub\\Editor\\2022.3.20f1\\Editor\\Unity.exe\" -quit -batchmode -projectPath . -executeMethod MyBuildScript.Build",
"run_tests": "\"C:\\Program Files\\Unity\\Hub\\Editor\\2022.3.20f1\\Editor\\Unity.exe\" -runTests -testPlatform EditMode"
```

---

## ??? SCRIPT DE INICIALIZACI�N

El script `init-autogen-project.ps1` realiza:

1. **Reemplaza placeholders** en todos los archivos JSON
2. **Configura MCP Server** con paths del proyecto
3. **Crea estructura de docs/** (sprint.md, daily.md)
4. **Genera .github/autogen-context.md** adaptado
5. **Instala dependencias** de MCP Server

### Uso:

```powershell
.\autogen_template\init-autogen-project.ps1 `
  -ProjectName "MiProyecto" `
  -ProjectType "Python" `
  -BuildCommand "pytest && mypy ." `
  -LanguageVersion "Python 3.11+" `
  -FileEncoding "UTF-8" `
  -LineEndings "LF"
```

**Par�metros**:
- `ProjectName`: Nombre del proyecto (sin espacios)
- `ProjectType`: Lenguaje principal (C++, Python, React, Rust, Unity, etc.)
- `BuildCommand`: Comando de compilaci�n/validaci�n
- `LanguageVersion`: Versi�n del lenguaje/framework
- `FileEncoding`: UTF-8 (default), UTF-8-BOM, ASCII
- `LineEndings`: LF (default), CRLF

---

## ?? PROMPTS PARA GITHUB COPILOT

### Desplegar infraestructura completa

```
@workspace 

I've run init-autogen-project.ps1 with these settings:
- Project: [NOMBRE]
- Type: [TIPO]
- Build: [COMANDO]

Please complete the AutoGen setup:
1. Verify all placeholders are replaced in autogen/ files
2. Create initial sprint files (sprint.md, sprint_tasks.md, daily.md)
3. Generate .github/autogen-context.md with project-specific context
4. Create first sprint (v1.0.0) with 3 sample tasks
5. Test MCP Server compilation (cd mcp-server && npm run build)

Ready to start using AutoGen multi-agent system.
```

### Crear primer sprint

```
@workspace 

Create Sprint v1.0.0 for project [NOMBRE]:

Sprint goals:
1. [OBJETIVO_1]
2. [OBJETIVO_2]
3. [OBJETIVO_3]

Generate:
- docs/sprint.md (sprint description)
- docs/sprint_tasks.md (6-8 tasks, IDs: H1.1-H1.8)
- docs/daily.md (set first task as current)
- autogen/prompts/sprint_v1.0.0/ directory with task prompts

Use the format from Imagine Studio project as reference.
```

### Crear agente personalizado

```
@workspace 

Create a new AutoGen agent in autogen/agents/:

Agent name: [NOMBRE]_agent.json
Role: [ROL] (e.g., Security Analyst, Performance Optimizer)
Tools: [LISTA_TOOLS] (e.g., run_security_scan, analyze_performance)
System message: [DESCRIPCI�N_COMPORTAMIENTO]
Termination keyword: [KEYWORD_WHEN_DONE]

Follow the structure of planner_agent.json as template.
```

---

## ?? VERIFICACI�N POST-SETUP

Ejecuta estos checks:

```powershell
# 1. Verificar agentes creados
Get-ChildItem autogen/agents/*.json
# Debe listar 4 archivos

# 2. Compilar MCP Server
cd mcp-server
npm run build
# Debe crear dist/index.js

# 3. Verificar configuraci�n GitHub Copilot
Get-Content ~/.github-copilot/config.json
# Debe incluir mcpServers.autogen-studio

# 4. Iniciar AutoGen Studio
autogenstudio ui --port 8081 --appdir .
# Debe abrir en http://localhost:8081

# 5. Importar agentes en AutoGen Studio UI
# Agents -> Import Agent -> Seleccionar cada JSON
```

---

## ?? M�TRICAS ESPERADAS

| M�trica | Sin AutoGen | Con AutoGen | Mejora |
|---------|-------------|-------------|--------|
| Tiempo por tarea | 10-15 min | 3-5 min | **2-3x** |
| Errores de build | 30-40% | 5-10% | **6-8x** |
| Code reviews | Manual | Autom�tico | **?** |
| Consistencia | Variable | 100% | **100%** |

---

## ?? RECURSOS ADICIONALES

- **AutoGen Docs**: https://microsoft.github.io/autogen/
- **MCP Protocol**: https://modelcontextprotocol.io/
- **Imagine Studio (ejemplo completo)**: Este proyecto
- **Templates**: `docs/templates/`

---

## ?? TROUBLESHOOTING

### Error: "Agent not found"

```powershell
# Re-importar agentes en AutoGen Studio UI
# Agents -> Import Agent -> autogen/agents/[agent].json
```

### Error: "MCP Server not responding"

```powershell
# Verificar configuraci�n
Get-Content ~/.github-copilot/config.json

# Recompilar MCP Server
cd mcp-server
Remove-Item -Recurse dist/
npm run build

# Reiniciar VS Code
```

### Error: "Build command failed"

```json
// Editar autogen/agents/tester_agent.json
// Tool: compile_[LANG]
{
  "function": {
    "name": "compile_python",
    "parameters": {
      "command": "TU_COMANDO_REAL_AQUI"
    }
  }
}
```

---

## ? CHECKLIST FINAL

- [ ] AutoGen Studio iniciado (`http://localhost:8081`)
- [ ] 4 agentes importados (Planner, Coder, Reviewer, Tester)
- [ ] Team creado (ImagineStudio_DevTeam)
- [ ] MCP Server compilado (`mcp-server/dist/index.js` existe)
- [ ] GitHub Copilot configurado (config.json actualizado)
- [ ] Sprint inicial creado (`docs/sprint.md` existe)
- [ ] Prompt de prueba ejecutado exitosamente

---

**Versi�n**: 1.0  
**�ltima actualizaci�n**: 2025-01-21  
**Proyecto base**: Imagine Studio Multi-Agent System  
**Autor**: Joaqu�n Luct

---

## ?? �LISTO PARA USAR!

Tu proyecto ahora tiene:
- ? Desarrollo automatizado con 4 agentes especializados
- ? Integraci�n con GitHub Copilot via MCP
- ? Metodolog�a �gil completa
- ? Logging y trazabilidad autom�ticos

**Siguiente paso**: Ejecuta tu primer prompt en AutoGen Studio Playground.
