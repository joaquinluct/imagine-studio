# AutoGen Template - GitHub Copilot Context

> **Purpose**: Context for GitHub Copilot to understand and deploy the AutoGen multi-agent template.

---

## ?? WHAT IS THIS TEMPLATE?

A **reusable package** to deploy AutoGen Studio + MCP Server + Agile Methodology in **any project**.

### Key Components
1. **4 specialized agents** (Planner, Coder, Reviewer, Tester)
2. **MCP Server** (3 tools for GitHub Copilot)
3. **Sprint methodology** (docs structure)
4. **Initialization script** (PowerShell automation)
5. **Complete documentation** (setup guides, prompts)

### Source Project
**Imagine Studio** (C++ Game Engine) - Proven in production with:
- Sprint v1.9.0 (30+ tasks completed)
- 2-3x faster development
- 90%+ code quality improvement

---

## ?? COPILOT INSTRUCTIONS

When a user says:
- "Set up AutoGen in my project"
- "Deploy multi-agent system"
- "Initialize AutoGen template"
- "Add AI agents to my workflow"

**You should**:

### Step 1: Gather Project Info
Ask:
```
To set up AutoGen, I need:
1. Project name (e.g., "MyApp")
2. Main language (C++, Python, React, Rust, Unity, etc.)
3. Build command (e.g., "npm run build", "cargo build", "pytest")
4. Test command (optional)
5. Lint command (optional)
```

### Step 2: Run Initialization Script
```powershell
.\docs\templates\init-autogen-project.ps1 `
  -ProjectName "[NAME]" `
  -ProjectType "[LANGUAGE]" `
  -BuildCommand "[COMMAND]" `
  -TestCommand "[TEST_CMD]" `
  -LintCommand "[LINT_CMD]"
```

### Step 3: Verify Structure
Check these files exist:
- `autogen/agents/planner_agent.json`
- `autogen/agents/coder_agent.json`
- `autogen/agents/reviewer_agent.json`
- `autogen/agents/tester_agent.json`
- `autogen/teams/team_complete.json`
- `mcp-server/dist/index.js`

### Step 4: Generate Sprint Files
Create:
- `docs/sprint.md` (current sprint description)
- `docs/sprint_tasks.md` (6-8 tasks with IDs)
- `docs/daily.md` (current task tracking)
- `docs/MAIN.md` (project pillars)

### Step 5: Create AutoGen Context
Generate `.github/autogen-context.md` with:
- MCP tools reference
- 4 agents description
- Team configuration
- Termination keywords
- Project-specific examples

### Step 6: Configure MCP
Add to `~/.github-copilot/config.json`:
```json
{
  "mcpServers": {
    "[project-name]-autogen-studio": {
      "command": "node",
      "args": ["[PROJECT_PATH]/mcp-server/dist/index.js"],
      "env": {
        "PROJECT_ROOT": "[PROJECT_PATH]"
      }
    }
  }
}
```

### Step 7: Generate First Prompt
Create `autogen/prompts/sprint_v1.0.0/H1.1.md` following template structure.

---

## ?? LANGUAGE-SPECIFIC CONFIGURATIONS

### Python Projects
```json
// Coder Agent
"system_message": "Python 3.11+ expert. PEP 8 style. Type hints required. async/await for I/O. UTF-8, LF."

// Tester Agent
"validation": "pytest && mypy . && black --check . && flake8 ."
```

### React + TypeScript
```json
// Coder Agent
"system_message": "React 18 + TypeScript 5 expert. Functional components, hooks. ESLint + Prettier. UTF-8, LF."

// Tester Agent
"validation": "npm run lint && npm run type-check && npm test -- --run && npm run build"
```

### Rust
```json
// Coder Agent
"system_message": "Rust 1.70+ expert. Idiomatic patterns. Result<T,E> for errors. Clippy compliant. UTF-8, LF."

// Tester Agent
"validation": "cargo clippy -- -D warnings && cargo fmt --check && cargo test && cargo build"
```

### C++
```json
// Coder Agent
"system_message": "C++14 expert. Google C++ Style. Include order: project first, system second, alphabetically. UTF-8 BOM, CRLF."

// Tester Agent
"validation": "cmake --build build --config Debug && msbuild 'Project.sln' /t:Build /p:Configuration=Debug"
```

### Unity C#
```json
// Coder Agent
"system_message": "C# 9.0 Unity expert. Unity conventions. [SerializeField], avoid Update() for logic. UTF-8 BOM, CRLF."

// Tester Agent
"validation": "Unity batch build + Edit Mode tests"
```

---

## ?? PROMPT TEMPLATES FOR COPILOT

### Full Deployment (Recommended)
```
@workspace 

Set up AutoGen multi-agent system:

Project: [NAME]
Type: [LANGUAGE]
Build: [COMMAND]
Test: [TEST_COMMAND]
Lint: [LINT_COMMAND]

Deploy complete infrastructure:
1. Run init-autogen-project.ps1
2. Generate sprint files (sprint.md, sprint_tasks.md, daily.md)
3. Create .github/autogen-context.md
4. Configure MCP Server
5. Generate first prompt

Provide step-by-step confirmation.
```

### Incremental Setup
```
@workspace 

Step 1: Initialize AutoGen for [PROJECT_NAME] ([LANGUAGE]).
Run: .\docs\templates\init-autogen-project.ps1 with my project details.
Confirm files created.
```

### Verification
```
@workspace 

Verify AutoGen setup:
1. Check 4 agent JSON files exist
2. Verify MCP Server compiled (dist/index.js)
3. Test MCP tool: mcp_[project]_list_autogen_teams()
4. Check sprint files (sprint.md, sprint_tasks.md, daily.md)

Report any missing components.
```

---

## ?? AGENT CUSTOMIZATION GUIDE

### When to Customize Agents

**Planner Agent**:
- Project uses different task ID format (e.g., US-123 instead of H1.1)
- Sprint files are in different location
- Custom planning methodology

**Coder Agent**:
- Different code style (e.g., Airbnb vs Google)
- Special file structure requirements
- Custom error handling patterns
- Different encoding/line endings

**Reviewer Agent**:
- Additional security checks
- Performance profiling required
- Custom linting rules
- Different code standards

**Tester Agent**:
- Multi-step build process
- Custom test frameworks
- Integration test requirements
- Deployment validation

### How to Customize

Edit `autogen/agents/[agent]_agent.json`:

```json
{
  "config": {
    "system_message": "You are a [ROLE] for [PROJECT].\n\n[CUSTOM INSTRUCTIONS]\n\nYour role:\n1. [STEP_1]\n2. [STEP_2]\n...\n\nAlways:\n- [RULE_1]\n- [RULE_2]\n...\n\nNever:\n- [FORBIDDEN_1]\n- [FORBIDDEN_2]\n...\n\nSay '[TERMINATION_KEYWORD]' when done."
  }
}
```

---

## ?? WORKFLOW EXAMPLES

### Example 1: Simple Feature (2-3 min)

**Prompt**:
```
Implement task H1.1: Create User model with fields (id, email, name)
```

**Execution**:
1. Planner reads task, proposes file structure
2. Coder creates `models/user.py` with validation
3. Reviewer checks PEP 8 compliance
4. Tester runs `pytest && mypy && black`
5. Result: WORKFLOW_FINISHED_OK ?

---

### Example 2: Complex Refactor (5-8 min)

**Prompt**:
```
Refactor authentication system:
- Extract auth logic to separate module
- Add JWT token validation
- Update all imports
- Maintain backward compatibility
```

**Execution**:
1. Planner breaks into subtasks (extract, validate, update, test)
2. Coder implements each subtask sequentially
3. Reviewer checks at each step (may request changes)
4. Tester validates after each step
5. Loop until all approved
6. Result: WORKFLOW_FINISHED_OK ?

---

### Example 3: Bug Fix (3-5 min)

**Prompt**:
```
Fix bug: NullPointerException in UserService.getProfile()
Error occurs when user.profile is null.
Add proper null checks and logging.
```

**Execution**:
1. Planner analyzes error, proposes fix locations
2. Coder adds null checks + logging
3. Reviewer verifies error handling
4. Tester runs tests (including edge cases)
5. Result: WORKFLOW_FINISHED_OK ?

---

## ?? METRICS TO TRACK

### Time Savings
- Task completion time (before/after AutoGen)
- Code review time (manual vs automated)
- Build error resolution time

### Quality Improvements
- Build success rate (%)
- Code style compliance (%)
- Test coverage (%)
- Bug escape rate

### Process Efficiency
- Tasks per sprint (velocity)
- Rework percentage
- Documentation completeness

---

## ?? COMMON ISSUES & SOLUTIONS

### Issue: "Agent produces code with wrong style"
**Cause**: Coder Agent system_message not specific enough.
**Solution**: Add explicit style rules:
```
Code style:
- Functions: camelCase (JavaScript) / snake_case (Python)
- Classes: PascalCase
- Constants: UPPER_SNAKE_CASE
- Indentation: 2 spaces (JS) / 4 spaces (Python)
```

---

### Issue: "Build always fails"
**Cause**: Build command incorrect or PATH issues.
**Solution**: Test command manually:
```powershell
# Test build command
[YOUR_BUILD_COMMAND]

# If fails, check PATH, tool versions
# Update Tester Agent with correct command
```

---

### Issue: "Reviewer rejects everything"
**Cause**: Standards too strict or conflicting.
**Solution**: Align Coder and Reviewer rules:
```json
// Both should have identical style definitions
"style_guide": "[SAME_REFERENCE]"
```

---

### Issue: "Workflow never terminates"
**Cause**: Missing termination keyword or wrong keyword used.
**Solution**: Verify each agent has clear termination:
- Planner: `TASK_COMPLETED`
- Coder: `CODE_READY`
- Reviewer: `REVIEW_APPROVED`
- Tester: `WORKFLOW_FINISHED_OK`

**Critical**: Team termination condition must match Tester's keyword.

---

## ?? BEST PRACTICES

### For Prompt Writing
1. **Be specific**: State exact task ID, files, requirements
2. **Provide context**: Mention related files to read
3. **Set constraints**: Max lines, performance targets, etc.
4. **Define success**: Clear acceptance criteria

### For Agent Configuration
1. **Single responsibility**: Each agent has ONE clear role
2. **Explicit termination**: Always define termination keyword
3. **Tool access**: Only grant tools needed for role
4. **Temperature**: Low (0.1-0.3) for consistency

### For Team Setup
1. **Sequential order**: Planner ? Coder ? Reviewer ? Tester
2. **Max rounds**: 20 (prevents infinite loops)
3. **Max messages**: 100 (safety limit)
4. **Allow repeat**: false (strict round-robin)

### For Project Integration
1. **Start small**: Test with 1-2 simple tasks first
2. **Iterate**: Refine agent prompts based on results
3. **Log everything**: Save all session logs for analysis
4. **Review**: Manually check first 5 tasks before full automation

---

## ?? DOCUMENTATION FILES

| File | Purpose | Audience |
|------|---------|----------|
| `AUTOGEN_TEMPLATE_README.md` | Overview & quick start | All |
| `AUTOGEN_TEMPLATE_SETUP.md` | Detailed setup guide | Developers |
| `COPILOT_PROMPTS_AUTOGEN.md` | Prompt library | Copilot users |
| `init-autogen-project.ps1` | Automation script | DevOps |
| `autogen/README.md` | AutoGen Studio usage | Developers |
| `mcp-server/README.md` | MCP Server setup | DevOps |

---

## ?? EXTERNAL REFERENCES

- **AutoGen**: https://microsoft.github.io/autogen/
- **AutoGen Studio**: https://github.com/microsoft/autogen/tree/main/python/packages/autogen-studio
- **MCP Protocol**: https://modelcontextprotocol.io/
- **Imagine Studio**: https://github.com/joaquinluct/imagine-studio

---

## ? DEPLOYMENT CHECKLIST

Use this to verify complete setup:

- [ ] `init-autogen-project.ps1` executed successfully
- [ ] 4 agent JSON files exist in `autogen/agents/`
- [ ] Team configuration exists in `autogen/teams/team_complete.json`
- [ ] MCP Server compiled (`mcp-server/dist/index.js` present)
- [ ] MCP config added to `~/.github-copilot/config.json`
- [ ] Sprint files created (`sprint.md`, `sprint_tasks.md`, `daily.md`)
- [ ] `.github/autogen-context.md` generated
- [ ] AutoGen Studio started (`http://localhost:8081`)
- [ ] Agents imported in AutoGen Studio UI
- [ ] Team created in AutoGen Studio UI
- [ ] First prompt executed successfully
- [ ] Session log saved in `autogen/sessions/`

---

**Version**: 1.0  
**Compatibility**: AutoGen Studio 0.1.0+, Node.js 18+, Python 3.11+  
**Last Updated**: 2025-01-21

---

## ?? REMEMBER

**This template is a starting point**. Customize agents, workflows, and prompts to fit your project's unique needs.

**Key principle**: The agents should follow YOUR project's standards, not impose their own.

**Feedback loop**: After each sprint, review logs and refine agent configurations for better results.

**GitHub Copilot is your guide**: Use it to deploy, customize, and troubleshoot the system. It knows this template inside-out.
