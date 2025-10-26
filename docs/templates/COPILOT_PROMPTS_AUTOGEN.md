# GitHub Copilot Prompt - Deploy AutoGen System

> **Purpose**: Complete automation prompt for GitHub Copilot to deploy AutoGen + MCP infrastructure.

---

## ?? FULL DEPLOYMENT PROMPT

Copy and paste this in GitHub Copilot Chat:

```
@workspace 

I want to set up the AutoGen multi-agent system in my project.

Project details:
- Name: [YOUR_PROJECT_NAME]
- Type: [LANGUAGE] (e.g., C++, Python, React, Rust, Unity)
- Build command: [YOUR_BUILD_COMMAND]
- Test command: [YOUR_TEST_COMMAND]
- Lint command: [YOUR_LINT_COMMAND]

Please perform the following tasks:

1. **Initialize AutoGen infrastructure**:
   - Run: docs/templates/init-autogen-project.ps1 with my project details
   - Verify all files generated in autogen/ and mcp-server/
   - Confirm 4 agent JSON files exist

2. **Create sprint structure**:
   - Generate docs/sprint.md for Sprint v1.0.0
   - Generate docs/sprint_tasks.md with 6-8 initial tasks (IDs: H1.1-H1.8)
   - Generate docs/daily.md with first task as current
   - Create docs/MAIN.md with project pillars

3. **Generate AutoGen context**:
   - Create .github/autogen-context.md adapted to my project
   - Include MCP tools reference
   - Add project-specific workflow examples

4. **Configure MCP Server**:
   - Verify mcp-server/dist/index.js compiled
   - Generate GitHub Copilot config.json entry
   - Test MCP tools (list_autogen_teams)

5. **Create first prompt**:
   - Generate autogen/prompts/sprint_v1.0.0/H1.1.md
   - Follow template structure from Imagine Studio

6. **Generate documentation**:
   - Create autogen/README.md specific to my project
   - Update .github/copilot-instructions.md with AutoGen section

Provide step-by-step confirmation and show any errors encountered.
Ready to deploy complete AutoGen system.
```

---

## ?? INCREMENTAL PROMPTS

If you prefer step-by-step, use these individual prompts:

### Step 1: Initialize Infrastructure

```
@workspace 

Initialize AutoGen infrastructure for my project:
- Name: [PROJECT_NAME]
- Type: [LANGUAGE]
- Build: [BUILD_COMMAND]

Run: .\docs\templates\init-autogen-project.ps1 with these parameters.
Confirm files created in autogen/ and mcp-server/.
```

### Step 2: Create Sprint Files

```
@workspace 

Create Sprint v1.0.0 for [PROJECT_NAME]:

Sprint goals:
1. [GOAL_1]
2. [GOAL_2]
3. [GOAL_3]

Generate:
- docs/sprint.md (description, dates, goals)
- docs/sprint_tasks.md (6-8 tasks with IDs H1.1-H1.8)
- docs/daily.md (current task: H1.1)

Follow Imagine Studio format.
```

### Step 3: Configure MCP

```
@workspace 

Configure MCP Server for [PROJECT_NAME]:

1. Verify mcp-server/dist/index.js exists
2. Generate config for ~/.github-copilot/config.json:
   - Server name: [project-name]-autogen-studio
   - Command: node
   - Args: [FULL_PATH]/mcp-server/dist/index.js
   - Env: PROJECT_ROOT=[FULL_PATH]

3. Test with: mcp_[project]_list_autogen_teams()

Show the complete config.json entry.
```

### Step 4: Create AutoGen Context

```
@workspace 

Create .github/autogen-context.md for [PROJECT_NAME]:

Include:
1. MCP tools reference (3 tools)
2. 4 agents description (Planner, Coder, Reviewer, Tester)
3. Team configuration (Round Robin)
4. Termination keywords (WORKFLOW_FINISHED_OK, etc.)
5. Project-specific build commands
6. Example prompts for common tasks

Adapt from Imagine Studio's autogen-context.md.
```

### Step 5: Generate First Prompt

```
@workspace 

Create first AutoGen prompt for task H1.1:

Task: [DESCRIPTION]

Generate: autogen/prompts/sprint_v1.0.0/H1.1.md

Include:
- Task description
- Files to create/modify
- Acceptance criteria
- Workflow steps (Planner -> Coder -> Reviewer -> Tester)

Follow _template.md structure.
```

---

## ?? PROJECT-SPECIFIC TEMPLATES

### For Python FastAPI Project

```
@workspace 

Set up AutoGen for Python FastAPI project:

Project: MyAPI
Type: Python
Build: pytest && mypy . && black --check .
Test: pytest tests/ -v
Lint: flake8 . && mypy .

Language: Python 3.11+
Style: PEP 8
Encoding: UTF-8
Line endings: LF

Deploy complete AutoGen system.
```

### For React + TypeScript Project

```
@workspace 

Set up AutoGen for React TypeScript project:

Project: MyReactApp
Type: React
Build: npm run build
Test: npm test -- --run
Lint: npm run lint && npm run type-check

Language: React 18 + TypeScript 5
Style: ESLint + Prettier
Encoding: UTF-8
Line endings: LF

Deploy complete AutoGen system.
```

### For Rust Project

```
@workspace 

Set up AutoGen for Rust project:

Project: MyRustApp
Type: Rust
Build: cargo build --all-features
Test: cargo test --all-features
Lint: cargo clippy -- -D warnings && cargo fmt --check

Language: Rust 1.70+
Style: rustfmt + Clippy
Encoding: UTF-8
Line endings: LF

Deploy complete AutoGen system.
```

### For Unity C# Project

```
@workspace 

Set up AutoGen for Unity project:

Project: MyUnityGame
Type: Unity
Build: Unity batch build script
Test: Unity Edit Mode tests
Lint: Visual Studio Code Analysis

Language: C# 9.0 (Unity 2022 LTS)
Style: Unity conventions
Encoding: UTF-8 with BOM
Line endings: CRLF

Deploy complete AutoGen system.
```

---

## ?? VERIFICATION PROMPTS

After deployment, use these to verify:

### Verify Infrastructure

```
@workspace 

Verify AutoGen infrastructure:

Check:
1. autogen/agents/ has 4 JSON files
2. autogen/teams/team_complete.json exists
3. mcp-server/dist/index.js compiled
4. docs/sprint.md created
5. .github/autogen-context.md exists

List any missing files.
```

### Test MCP Server

```
@workspace 

Test MCP Server integration:

1. Run: mcp_[project]_list_autogen_teams()
   - Should list: team_complete.json

2. Run: mcp_[project]_run_build(configuration: "Debug")
   - Should execute build command

3. Run: mcp_[project]_save_autogen_log(
     task_id: "TEST",
     status: "SUCCESS",
     log_content: "Test log",
     duration: 10
   )
   - Should create: autogen/sessions/[date]_TEST_success.md

Report results for each tool.
```

### Generate Sample Prompt

```
@workspace 

Generate a sample prompt to test AutoGen workflow:

Task: H1.1 - [Simple task description]

Create: autogen/prompts/sprint_v1.0.0/H1.1_test.md

This prompt should:
1. Ask Planner to read task details
2. Ask Coder to implement simple class/function
3. Ask Reviewer to check style
4. Ask Tester to run build

Should take ~5 minutes to execute in AutoGen Studio.
```

---

## ?? ADVANCED PROMPTS

### Create Custom Agent

```
@workspace 

Create new AutoGen agent:

Agent name: SecurityAnalyzer
Role: Security vulnerability detection
Tools: scan_dependencies, check_cves, analyze_code
System message:
  - Scan for known vulnerabilities
  - Check dependency versions
  - Analyze code for security issues
  - Say "SECURITY_CHECK_PASSED" when OK

Generate: autogen/agents/security_agent.json
Follow structure of reviewer_agent.json.
```

### Create Custom Team

```
@workspace 

Create new AutoGen team:

Team name: SecurityTeam
Type: RoundRobinGroupChat
Agents: [planner, security_analyzer, coder, tester]
Workflow:
  1. Planner reads task
  2. SecurityAnalyzer checks for vulnerabilities
  3. Coder fixes issues
  4. Tester validates

Termination: SECURITY_WORKFLOW_COMPLETE

Generate: autogen/teams/team_security.json
```

### Migrate Existing Project

```
@workspace 

I have an existing project at [PATH] with:
- Active sprints in docs/
- Commits in git history
- Build system configured

Migrate to AutoGen system:
1. Preserve existing docs/sprint*.md files
2. Generate agents adapted to current build process
3. Create MCP server with current project structure
4. Generate prompts for remaining sprint tasks
5. Archive previous manual logs

Maintain git history and existing commits.
```

---

## ?? TRAINING PROMPTS

### Learn AutoGen Workflow

```
@workspace 

Explain AutoGen workflow for my project:

1. How do 4 agents collaborate?
2. What's the Round Robin order?
3. When does workflow terminate?
4. How to handle build failures?
5. How to log successful sessions?

Provide examples specific to [PROJECT_TYPE].
```

### Create Documentation

```
@workspace 

Generate comprehensive AutoGen documentation for my team:

Create: docs/AUTOGEN_GUIDE.md

Include:
1. System overview (4 agents + MCP)
2. How to start AutoGen Studio
3. How to execute prompts
4. How to read execution logs
5. Troubleshooting common errors
6. Best practices for prompt writing

Target audience: developers new to AutoGen.
```

---

## ?? TROUBLESHOOTING PROMPTS

### Debug Build Failures

```
@workspace 

AutoGen session failed with build error:

Error: [PASTE_ERROR_MESSAGE]

Debug:
1. Check Tester Agent configuration in autogen/agents/tester_agent.json
2. Verify build command works manually: [BUILD_COMMAND]
3. Check if all required tools are in PATH
4. Suggest fix for Tester Agent system_message

Provide corrected tester_agent.json.
```

### Debug Agent Not Responding

```
@workspace 

AutoGen agent [AGENT_NAME] is not responding in workflow.

Debug:
1. Check agent JSON in autogen/agents/[agent]_agent.json
2. Verify system_message has clear instructions
3. Check if termination keyword is correct
4. Verify agent is in team participants list

Suggest fixes for agent configuration.
```

### Regenerate Corrupted Files

```
@workspace 

AutoGen configuration corrupted. Regenerate:
- autogen/agents/[AGENT]_agent.json
- autogen/teams/team_complete.json

Use original project parameters:
- Project: [NAME]
- Type: [LANGUAGE]
- Build: [COMMAND]

Preserve existing prompts in autogen/prompts/.
```

---

**Version**: 1.0  
**Last update**: 2025-01-21  
**Compatible with**: AutoGen Studio 0.1.0+, MCP SDK 0.4.0+
