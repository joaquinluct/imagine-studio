# AutoGen Template Package - Index

> **Complete file listing** of the AutoGen multi-agent template package.

---

## ?? PACKAGE STRUCTURE

```
docs/templates/
??? autogen_template_package/           [Copy this entire folder to new project]
?   ??? autogen/
?   ?   ??? agents/                     [Agent JSON files - auto-generated]
?   ?   ??? teams/                      [Team configurations - auto-generated]
?   ?   ??? prompts/
?   ?   ?   ??? _template.md            [Prompt template]
?   ?   ?   ??? common/
?   ?   ?       ??? simple_test.md      [Example prompt]
?   ?   ??? sessions/                   [Log storage - empty initially]
?   ?   ?   ??? summary/
?   ?   ??? outputs/                    [Temp files - empty initially]
?   ?   ??? log-autogen-session.ps1     [Logging script]
?   ?   ??? README.md                   [AutoGen usage guide]
?   ?   ??? WORKFLOW_COMPLETE.md        [Workflow documentation]
?   ?
?   ??? mcp-server/
?   ?   ??? src/
?   ?   ?   ??? index.ts.template       [MCP Server source - needs PROJECT_ROOT]
?   ?   ??? package.json.template       [npm config - needs project name]
?   ?   ??? tsconfig.json               [TypeScript config - ready to use]
?   ?   ??? README.md                   [MCP setup guide]
?   ?
?   ??? init-autogen-project.ps1        [Main initialization script]
?
??? AUTOGEN_TEMPLATE_README.md          [Main documentation - START HERE]
??? AUTOGEN_TEMPLATE_SETUP.md           [Detailed setup guide]
??? AUTOGEN_TEMPLATE_COPILOT_CONTEXT.md [GitHub Copilot instructions]
??? COPILOT_PROMPTS_AUTOGEN.md          [Prompt library for Copilot]
??? AUTOGEN_TEMPLATE_INDEX.md           [This file]
```

---

## ?? FILE DESCRIPTIONS

### Core Documentation

| File | Lines | Purpose | When to Read |
|------|-------|---------|--------------|
| **AUTOGEN_TEMPLATE_README.md** | 600 | Main overview, quick start, benefits | **Read first** |
| **AUTOGEN_TEMPLATE_SETUP.md** | 800 | Detailed step-by-step setup guide | During setup |
| **COPILOT_PROMPTS_AUTOGEN.md** | 500 | Pre-written prompts for GitHub Copilot | When using Copilot |
| **AUTOGEN_TEMPLATE_COPILOT_CONTEXT.md** | 400 | Context for Copilot to understand system | Reference for Copilot |
| **AUTOGEN_TEMPLATE_INDEX.md** | 200 | File listing (this file) | When navigating |

---

### Automation Scripts

| File | Lines | Purpose | Parameters |
|------|-------|---------|------------|
| **init-autogen-project.ps1** | 600 | Deploys entire infrastructure | `ProjectName`, `ProjectType`, `BuildCommand`, `TestCommand`, `LintCommand` |
| **log-autogen-session.ps1** | 100 | Saves AutoGen session logs | `TaskID`, `Status`, `LogContent`, `Duration` |

**Usage**:
```powershell
# Initialize project
.\init-autogen-project.ps1 -ProjectName "MyApp" -ProjectType "Python" -BuildCommand "pytest"

# Log session
.\log-autogen-session.ps1 -TaskID "H1.1" -Status "SUCCESS" -LogContent "..." -Duration 180
```

---

### Agent Configurations (Auto-Generated)

These are **created by `init-autogen-project.ps1`**, not manually edited:

| File | Size | Agent Role | Model |
|------|------|------------|-------|
| `planner_agent.json` | ~2 KB | Sprint Manager | gpt-4o |
| `coder_agent.json` | ~3 KB | Implementation Specialist | gpt-4o |
| `reviewer_agent.json` | ~2.5 KB | Quality Assurance | gpt-4o |
| `tester_agent.json` | ~2 KB | Build & Validation | gpt-4o-mini |

**Contains**:
- `component_type`: "agent"
- `label`: Human-readable name
- `provider`: "AssistantAgent"
- `config.system_message`: Detailed instructions (200-500 tokens)
- `config.llm_config`: Model configuration
- `config.max_consecutive_auto_reply`: Turn limit

**Customization**: Edit `system_message` to adapt agent behavior to project needs.

---

### Team Configurations (Auto-Generated)

| File | Size | Team Type | Agents |
|------|------|-----------|--------|
| `team_complete.json` | ~1.5 KB | RoundRobinGroupChat | Planner, Coder, Reviewer, Tester |

**Contains**:
- `component_type`: "team"
- `provider`: "RoundRobinGroupChat" (sequential turns)
- `config.participants`: Array of agent references
- `config.max_round`: 20 (prevents infinite loops)
- `termination.text`: "WORKFLOW_FINISHED_OK"

**Workflow Order**: planner ? coder ? reviewer ? tester ? (repeat until termination)

---

### MCP Server Files

| File | Lines | Purpose | Generated |
|------|-------|---------|-----------|
| `src/index.ts.template` | 300 | MCP Server source code | ? Yes (from template) |
| `package.json.template` | 30 | npm dependencies & scripts | ? Yes (from template) |
| `tsconfig.json` | 20 | TypeScript compiler config | ? No (static) |
| `README.md` | 150 | MCP Server documentation | ? No (static) |

**Build Process**:
```powershell
cd mcp-server
npm install
npm run build  # Creates dist/index.js
```

**MCP Tools Provided**:
1. `list_autogen_teams` - Lists configured teams
2. `run_build` - Executes project build
3. `save_autogen_log` - Saves session logs

---

### Prompt Templates

| File | Lines | Purpose | Customizable |
|------|-------|---------|--------------|
| `_template.md` | 100 | Generic task prompt structure | ? Yes |
| `common/simple_test.md` | 50 | Example: Hello World task | ? No (reference) |

**Template Structure**:
```markdown
# Task [TASK_ID] - [TASK_NAME]

**Sprint**: [SPRINT_VERSION]
**Priority**: [HIGH/MEDIUM/LOW]
**Estimated time**: [X minutes]

## Context
[Background information]

## Requirements
1. [Requirement 1]
2. [Requirement 2]

## Files to Create/Modify
- `path/to/file.ext`

## Acceptance Criteria
- [ ] Criterion 1
- [ ] Criterion 2

## Workflow
1. Planner: Read task details, create plan
2. Coder: Implement requirements
3. Reviewer: Check code quality
4. Tester: Validate build + tests

Expected completion: WORKFLOW_FINISHED_OK ?
```

---

### Session Logs (Generated During Execution)

**Location**: `autogen/sessions/`

**Naming**: `[DATE]_[TASK_ID]_[STATUS].md`

**Example**: `2025-01-21_H1.1_success.md`

**Format**:
```markdown
# AutoGen Session Log - H1.1

**Date**: 2025-01-21T14:30:00Z
**Task**: H1.1 - Create User Model
**Status**: ? SUCCESS
**Duration**: 180s

---

## Agent Execution Log

[Turn 1 - Planner]
...

[Turn 2 - Coder]
...

[Turn 3 - Reviewer]
...

[Turn 4 - Tester]
...

---

**Logged by**: MCP Server
```

**Purpose**: Traceability, debugging, metrics analysis

---

## ?? HOW TO USE THIS TEMPLATE

### For New Projects (Recommended Path)

1. **Read**: `AUTOGEN_TEMPLATE_README.md` (10 min)
   - Understand benefits, architecture, workflow

2. **Copy**: `autogen_template_package/` to your project root
   ```powershell
   Copy-Item -Recurse "path/to/autogen_template_package/" "./autogen_template/"
   ```

3. **Run**: `init-autogen-project.ps1` (2 min)
   ```powershell
   .\autogen_template\init-autogen-project.ps1 -ProjectName "MyApp" -ProjectType "Python" -BuildCommand "pytest"
   ```

4. **Deploy**: Use GitHub Copilot with `COPILOT_PROMPTS_AUTOGEN.md` (5 min)
   ```
   @workspace Set up AutoGen system for my Python project...
   ```

5. **Start**: AutoGen Studio (1 min)
   ```powershell
   autogenstudio ui --port 8081 --appdir .
   ```

6. **Import**: Agents in AutoGen Studio UI (3 min)
   - Agents ? Import Agent ? Select each JSON

7. **Create**: Team in AutoGen Studio UI (2 min)
   - Teams ? Import Team ? Select `team_complete.json`

8. **Test**: Execute first prompt (5 min)
   - Playground ? Select team ? Paste prompt from template

**Total setup time**: ~30 minutes  
**Total automation setup**: One-time  
**ROI**: Saves 4-5 hours per sprint

---

### For Existing Projects (Migration Path)

1. **Audit**: Current build/test/lint commands
   ```powershell
   # Document what you currently run
   [BUILD_COMMAND]
   [TEST_COMMAND]
   [LINT_COMMAND]
   ```

2. **Copy**: Template package
   ```powershell
   Copy-Item -Recurse "autogen_template_package/" "./autogen_template/"
   ```

3. **Initialize**: With existing commands
   ```powershell
   .\autogen_template\init-autogen-project.ps1 `
     -ProjectName "ExistingApp" `
     -ProjectType "Python" `
     -BuildCommand "[YOUR_CURRENT_BUILD]" `
     -TestCommand "[YOUR_CURRENT_TEST]"
   ```

4. **Preserve**: Existing sprint files (if any)
   ```powershell
   # Backup existing docs
   Copy-Item -Recurse "docs/" "docs_backup/"
   
   # Merge with generated files
   # (Copilot can help)
   ```

5. **Test**: On non-critical task first
   - Choose simple task (e.g., add utility function)
   - Execute in AutoGen Studio
   - Verify build/test still work

6. **Iterate**: Refine agent prompts based on results

7. **Scale**: Once confident, use for all new tasks

---

## ?? FILE SIZE SUMMARY

| Category | Files | Total Size | Purpose |
|----------|-------|------------|---------|
| Documentation | 5 | ~10 KB | Setup guides, prompts |
| Scripts | 2 | ~20 KB | Automation |
| Agent Configs | 4 | ~10 KB | AutoGen agents |
| Team Configs | 1 | ~2 KB | Workflow definition |
| MCP Server | 4 | ~15 KB | Copilot integration |
| Prompt Templates | 2 | ~5 KB | Task prompts |
| **Total** | **18** | **~62 KB** | Complete package |

**Lightweight**: Less than 100 KB for complete multi-agent system!

---

## ?? VERSION CONTROL

### What to Commit

? **Commit**:
- `autogen/agents/*.json` (agent configurations)
- `autogen/teams/*.json` (team configurations)
- `autogen/prompts/**/*.md` (prompt templates)
- `autogen/README.md` (documentation)
- `mcp-server/src/index.ts` (MCP Server source)
- `mcp-server/package.json` (dependencies)
- `docs/sprint*.md` (sprint tracking)
- `.github/autogen-context.md` (Copilot context)

? **Ignore** (add to `.gitignore`):
```gitignore
# AutoGen generated files
autogen/outputs/
autogen/sessions/*.md
!autogen/sessions/summary/*.md

# MCP Server compiled
mcp-server/dist/
mcp-server/node_modules/

# Logs
*.log
```

? **Archive** (at sprint end):
- `autogen/sessions/summary/sprint_v1.0.0_summary.md`
- Move to `docs/sprints/sprint_v1.0.0_autogen/`

---

## ?? LEARNING PATH

### Day 1: Setup (1 hour)
1. Read `AUTOGEN_TEMPLATE_README.md`
2. Run `init-autogen-project.ps1`
3. Start AutoGen Studio
4. Import agents & team

### Day 2: First Task (2 hours)
1. Create simple task in `sprint_tasks.md`
2. Generate prompt from template
3. Execute in AutoGen Studio
4. Review output, save log

### Day 3: Iteration (2 hours)
1. Execute 3-5 more tasks
2. Notice patterns in agent behavior
3. Refine `system_message` prompts
4. Document custom rules

### Week 1: Production Use (5 hours)
1. Complete 10-15 tasks with AutoGen
2. Measure time savings vs manual
3. Identify bottlenecks
4. Optimize agent configurations

### Month 1: Mastery
1. Custom agents for special tasks (e.g., Security Analyzer)
2. Custom teams for different workflows
3. Integration with CI/CD
4. Team training & documentation

---

## ?? CROSS-REFERENCES

| From | To | Relationship |
|------|----|----|
| `init-autogen-project.ps1` | Agent JSONs | **Generates** |
| Agent JSONs | AutoGen Studio | **Imported to** |
| Team JSON | Agent JSONs | **References** |
| MCP Server | GitHub Copilot | **Integrated with** |
| Prompt templates | AutoGen Studio Playground | **Executed in** |
| Session logs | `docs/sprints/` | **Archived to** |

---

## ?? QUICK REFERENCE CARD

```
???????????????????????????????????????????????????????
?  AUTOGEN TEMPLATE - QUICK REFERENCE                 ?
???????????????????????????????????????????????????????
?                                                     ?
?  Setup: .\init-autogen-project.ps1 -ProjectName    ?
?         "MyApp" -ProjectType "Python" -BuildCmd    ?
?         "pytest"                                    ?
?                                                     ?
?  Start: autogenstudio ui --port 8081 --appdir .    ?
?                                                     ?
?  Agents: planner ? coder ? reviewer ? tester       ?
?                                                     ?
?  Termination: WORKFLOW_FINISHED_OK                 ?
?                                                     ?
?  MCP Tools:                                         ?
?    - list_autogen_teams()                           ?
?    - run_build(configuration: "Debug")              ?
?    - save_autogen_log(task_id, status, log, dur)    ?
?                                                     ?
?  Docs: docs/templates/AUTOGEN_TEMPLATE_README.md   ?
?                                                     ?
?  Help: @workspace Set up AutoGen...                ?
?                                                     ?
???????????????????????????????????????????????????????
```

---

**Version**: 1.0  
**Last Updated**: 2025-01-21  
**Maintained by**: Joaqu�n Luct

**Next**: Read `AUTOGEN_TEMPLATE_README.md` to get started!
