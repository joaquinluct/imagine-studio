# AutoGen Multi-Agent Template for Any Project

> **Purpose**: Reusable template to deploy AutoGen Studio + MCP Server + Agile Methodology in any software project.

---

## ?? WHAT IS THIS?

A **production-ready template** that deploys:
- ? **4 specialized AI agents** (Planner, Coder, Reviewer, Tester)
- ? **AutoGen Studio** multi-agent workflow
- ? **MCP Server** for GitHub Copilot integration
- ? **Agile methodology** with sprint tracking
- ? **Automated logging** and traceability

### Adapted from: Imagine Studio (C++ Game Engine)

This template extracts the multi-agent system into a **language-agnostic** structure that works with:
- C++ / C# / Java
- Python / Ruby / PHP
- React / Vue / Angular
- Rust / Go
- Unity / Unreal Engine
- And more...

---

## ?? QUICK START (3 Steps)

### Step 1: Copy Template to Your Project

```powershell
# From your new project root
git clone https://github.com/joaquinluct/imagine-studio imagine-studio-template
cd imagine-studio-template

# Copy template files
Copy-Item -Recurse "docs/templates/autogen_template_package/" "../my-project/autogen_template/"
cd ../my-project
```

### Step 2: Run Initialization Script

```powershell
.\autogen_template\init-autogen-project.ps1 `
  -ProjectName "MyProject" `
  -ProjectType "Python" `
  -BuildCommand "pytest && mypy . && black --check ."
```

**Parameters**:
- `ProjectName`: Your project name (no spaces)
- `ProjectType`: Main language (C++, Python, React, Rust, Unity, etc.)
- `BuildCommand`: How to build/validate your project
- `TestCommand`: (Optional) Test suite command
- `LintCommand`: (Optional) Linter command

### Step 3: Deploy with GitHub Copilot

Open GitHub Copilot Chat and paste:

```
@workspace 

I've run init-autogen-project.ps1 for my [LANGUAGE] project.

Project: [NAME]
Build: [COMMAND]

Please:
1. Verify autogen/ structure created
2. Generate initial sprint files (sprint.md, sprint_tasks.md, daily.md)
3. Create .github/autogen-context.md
4. Test MCP Server (mcp_[project]_list_autogen_teams)
5. Generate first prompt (autogen/prompts/sprint_v1.0.0/H1.1.md)

Ready to start development with AutoGen.
```

---

## ?? WHAT GETS CREATED?

```
your-project/
??? autogen/                          ? AutoGen Studio files
?   ??? agents/
?   ?   ??? planner_agent.json        ? Sprint Manager
?   ?   ??? coder_agent.json          ? Implementation Specialist
?   ?   ??? reviewer_agent.json       ? Quality Assurance
?   ?   ??? tester_agent.json         ? Build & Validation
?   ?
?   ??? teams/
?   ?   ??? team_complete.json        ? 4-agent workflow
?   ?
?   ??? prompts/                      ? Reusable prompt templates
?   ?   ??? _template.md
?   ?   ??? sprint_v1.0.0/            ? Sprint-specific prompts
?   ?
?   ??? sessions/                     ? Execution logs
?   ?   ??? [date]_[task]_[status].md
?   ?
?   ??? log-autogen-session.ps1       ? Logging script
?
??? mcp-server/                       ? GitHub Copilot integration
?   ??? src/
?   ?   ??? index.ts                  ? MCP Server (3 tools)
?   ??? package.json
?   ??? tsconfig.json
?   ??? dist/                         ? Compiled (npm run build)
?
??? docs/                             ? Agile methodology
?   ??? methodology/
?   ?   ??? CORE.md                   ? Complete methodology
?   ?   ??? WORKFLOW.md               ? Step-by-step flow
?   ?   ??? BUGS.md                   ? Bug management
?   ?
?   ??? sprint.md                     ? Current sprint
?   ??? sprint_tasks.md               ? Task breakdown
?   ??? daily.md                      ? Current task tracking
?   ??? MAIN.md                       ? Project pillars
?
??? .github/
    ??? copilot-instructions.md       ? GitHub Copilot rules
    ??? autogen-context.md            ? AutoGen system context
```

---

## ?? THE 4 AGENTS

### 1. Planner Agent
**Role**: Sprint Manager

**What it does**:
- Reads `docs/daily.md` to know current task
- Reads `docs/sprint_tasks.md` for task details
- Creates implementation plan
- Breaks complex tasks into steps
- Proposes file structure

**Termination keyword**: `TASK_COMPLETED`

---

### 2. Coder Agent
**Role**: Implementation Specialist

**What it does**:
- Reads Planner's implementation plan
- Reads existing files (if modifying)
- Writes clean, production-ready code
- Follows project style guide
- Creates/modifies files

**Termination keyword**: `CODE_READY`

**Language-specific**:
- **C++**: Google C++ Style, UTF-8 BOM, CRLF
- **Python**: PEP 8, UTF-8, LF, type hints
- **React**: ESLint + Prettier, functional components
- **Rust**: rustfmt + Clippy, idiomatic patterns

---

### 3. Reviewer Agent
**Role**: Quality Assurance

**What it does**:
- Reads files Coder modified
- Checks code style adherence
- Verifies no external library modifications
- Checks error handling
- Validates security considerations

**Termination keyword**: `REVIEW_APPROVED` or `REVIEW_REJECTED: [reason]`

**Review checklist**:
1. ? Code style (language-specific)
2. ? File encoding/line endings
3. ? Include/import order
4. ? No external/ modifications
5. ? Error handling present
6. ? Comments clear
7. ? No security issues
8. ? Performance OK
9. ? DRY/SOLID principles
10. ? Test coverage (if applicable)

---

### 4. Tester Agent
**Role**: Build & Validation

**What it does**:
- Executes build command
- Parses output for errors/warnings
- Runs test suite
- Runs linter
- Reports results

**Termination keyword**: `WORKFLOW_FINISHED_OK` (if all pass) or `BUILD_FAILED: [reason]`

**Success criteria**:
- Build: 0 errors, 0 warnings
- Tests: All pass
- Linter: No violations

---

## ?? WORKFLOW EXAMPLE

### Typical execution (2-4 minutes per task):

```
[AutoGen Studio - Playground]

User prompt:
"Implement task H1.1 from Sprint v1.0.0"

[Turn 1 - Planner] (10-20s)
Reading docs/daily.md...
Reading docs/sprint_tasks.md...

Task: H1.1 - Create User model

Plan:
1. Create models/user.py
2. Define User class with fields: id, email, name
3. Add validation methods
4. Create unit tests

@coder Please implement.

[Turn 2 - Coder] (30-60s)
Creating models/user.py...

[Code implementation with proper style]

Creating tests/test_user.py...

[Test code]

@reviewer Please review.

[Turn 3 - Reviewer] (20-30s)
Reviewing models/user.py...

? Code style: PEP 8 compliant
? Type hints: Present
? Error handling: Proper validation
? Tests: Comprehensive coverage

REVIEW_APPROVED ?

@tester Please validate.

[Turn 4 - Tester] (60-120s)
Running: pytest && mypy . && black --check .

pytest: 12 passed ?
mypy: Success, no issues found ?
black: All files formatted ?

Build: 0 errors, 0 warnings
Tests: 12/12 passed
Linter: No violations

? WORKFLOW_FINISHED_OK

Ready for commit.
```

**Total time**: ~2-3 minutes  
**Manual equivalent**: ~10-15 minutes  
**Speedup**: 5x faster

---

## ??? MCP SERVER TOOLS

The MCP Server provides **3 tools** for GitHub Copilot:

### 1. `list_autogen_teams`
Lists all configured AutoGen teams.

**Usage**:
```typescript
mcp_myproject_list_autogen_teams()
```

**Returns**:
```json
{
  "teams": [
    {
      "name": "MyProject DevTeam",
      "file": "team_complete.json",
      "type": "RoundRobinGroupChat",
      "agents": ["planner", "coder", "reviewer", "tester"]
    }
  ]
}
```

---

### 2. `run_build`
Executes project build command.

**Usage**:
```typescript
mcp_myproject_run_build(configuration: "Debug")
```

**Returns**:
```
Build Result:

Status: SUCCESS ?

Errors: 0
Warnings: 0

[Build output...]
```

---

### 3. `save_autogen_log`
Saves AutoGen session log to `autogen/sessions/`.

**Usage**:
```typescript
mcp_myproject_save_autogen_log(
  task_id: "H1.1",
  status: "SUCCESS",
  log_content: "[agent conversation transcript]",
  duration: 180
)
```

**Creates**: `autogen/sessions/2025-01-21_H1.1_success.md`

---

## ?? BENEFITS

### Without AutoGen
- ?? 10-15 min per task
- ?? 30-40% build errors
- ?? Manual code reviews (often skipped)
- ?? Inconsistent code style
- ?? Manual task tracking

### With AutoGen
- ? 2-5 min per task (5x faster)
- ? 5-10% build errors (6-8x reduction)
- ?? Automated, consistent reviews
- ?? 100% style compliance
- ?? Automatic logging & traceability

### ROI Example (Sprint with 30 tasks)
- **Without**: 30 tasks � 12 min = 6 hours
- **With**: 30 tasks � 3 min = 1.5 hours
- **Saved**: 4.5 hours per sprint (75% reduction)

---

## ?? SUPPORTED LANGUAGES

### ? Fully Tested
- C++ (CMake, MSBuild)
- Python (pytest, mypy, black)
- C# (MSBuild, Unity)

### ? Template Available
- React + TypeScript (npm, vitest, eslint)
- Rust (cargo, clippy, rustfmt)
- Go (go test, go vet, gofmt)

### ? Easy to Adapt
Any language with:
- Build/compile command
- Test framework
- Linter/formatter

**To add a new language**: Edit agent JSON system messages with language-specific rules.

---

## ?? DOCUMENTATION

### For Developers
- `docs/templates/AUTOGEN_TEMPLATE_SETUP.md` - Complete setup guide
- `docs/templates/COPILOT_PROMPTS_AUTOGEN.md` - GitHub Copilot prompts
- `.github/autogen-context.md` - AutoGen system reference

### For Team Leads
- `docs/methodology/CORE.md` - Full agile methodology
- `docs/methodology/WORKFLOW.md` - Development workflow
- `docs/methodology/BUGS.md` - Bug management process

### For DevOps
- `mcp-server/README.md` - MCP Server setup
- `autogen/README.md` - AutoGen Studio configuration

---

## ?? CUSTOMIZATION

### Change Agent Behavior

Edit `autogen/agents/[agent]_agent.json`:

```json
{
  "config": {
    "system_message": "You are a [ROLE] for [PROJECT].\n\n[CUSTOM INSTRUCTIONS]"
  }
}
```

### Change Build Process

Edit `autogen/agents/tester_agent.json`:

```json
{
  "config": {
    "system_message": "Validation steps:\n1. Run build: [YOUR_BUILD_COMMAND]\n2. Run tests: [YOUR_TEST_COMMAND]\n..."
  }
}
```

### Add Custom Agent

1. Copy existing agent JSON
2. Modify `label`, `description`, `system_message`
3. Define new tools (if needed)
4. Add to team configuration

### Change Workflow Order

Edit `autogen/teams/team_complete.json`:

```json
{
  "config": {
    "participants": [
      { "component_ref": "planner" },
      { "component_ref": "security" },    // New agent
      { "component_ref": "coder" },
      { "component_ref": "reviewer" },
      { "component_ref": "tester" }
    ]
  }
}
```

---

## ?? TROUBLESHOOTING

### "Agent not responding"
**Solution**: Check system_message has clear termination keyword.

```json
"system_message": "...\n\nSay 'WORKFLOW_FINISHED_OK' when done."
```

---

### "Build command failed"
**Solution**: Test command manually first.

```powershell
# Test your build command
pytest && mypy . && black --check .

# If it works, update Tester Agent system_message
```

---

### "MCP Server not found"
**Solution**: Recompile and update config.

```powershell
cd mcp-server
npm run build

# Verify dist/index.js exists
Get-ChildItem dist/

# Update ~/.github-copilot/config.json with correct path
```

---

### "Review keeps rejecting"
**Solution**: Check if code actually violates style guide.

**Common issues**:
- Wrong file encoding (UTF-8 vs UTF-8-BOM)
- Wrong line endings (LF vs CRLF)
- Incorrect include order
- Missing error handling

**Fix**: Update Coder Agent system_message with correct standards.

---

## ?? RESOURCES

- **AutoGen Docs**: https://microsoft.github.io/autogen/
- **AutoGen Studio**: https://github.com/microsoft/autogen/tree/main/python/packages/autogen-studio
- **MCP Protocol**: https://modelcontextprotocol.io/
- **Imagine Studio (reference)**: https://github.com/joaquinluct/imagine-studio

---

## ?? LICENSE

MIT License - Feel free to use in any project (personal or commercial).

---

## ?? CREDITS

**Created by**: Joaqu�n Luct  
**Based on**: Imagine Studio Multi-Agent System  
**Inspired by**: Microsoft AutoGen, GitHub Copilot, Model Context Protocol

---

## ?? GET STARTED NOW

```powershell
# 1. Copy template
Copy-Item -Recurse "imagine-studio/docs/templates/autogen_template_package/" "./autogen_template/"

# 2. Initialize
.\autogen_template\init-autogen-project.ps1 -ProjectName "MyProject" -ProjectType "Python" -BuildCommand "pytest"

# 3. Deploy with Copilot
# (Open GitHub Copilot Chat and use prompts from COPILOT_PROMPTS_AUTOGEN.md)

# 4. Start AutoGen Studio
autogenstudio ui --port 8081 --appdir .

# 5. Execute first prompt
# (Go to http://localhost:8081/playground)
```

---

**Version**: 1.0  
**Last Updated**: 2025-01-21  
**Tested with**: AutoGen Studio 0.1.0+, Python 3.11+, Node.js 18+, OpenAI GPT-4

**Star ? this repo if it helped your project!**
