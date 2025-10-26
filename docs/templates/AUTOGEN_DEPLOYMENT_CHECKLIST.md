# AutoGen Template - Deployment Verification Checklist

> **Purpose**: Systematic verification that AutoGen system is correctly deployed and operational.

---

## ? PRE-DEPLOYMENT CHECKLIST

### System Requirements

- [ ] **Python 3.11+** installed
  ```powershell
  python --version
  # Expected: Python 3.11.x or higher
  ```

- [ ] **Node.js 18+** installed
  ```powershell
  node --version
  # Expected: v18.x.x or higher
  ```

- [ ] **AutoGen Studio** installed
  ```powershell
  pip show autogenstudio
  # Expected: Version info displayed
  ```

- [ ] **npm** working
  ```powershell
  npm --version
  # Expected: 9.x.x or higher
  ```

- [ ] **OpenAI API Key** available
  - Have valid API key
  - Know where to paste it (AutoGen Studio ? Settings)

- [ ] **Project build command** documented
  - Know exact command to build your project
  - Tested manually and works

---

## ? DEPLOYMENT CHECKLIST

### Step 1: Template Copy

- [ ] Template copied to project
  ```powershell
  Test-Path "autogen_template/"
  # Expected: True
  ```

- [ ] `init-autogen-project.ps1` present
  ```powershell
  Test-Path "autogen_template/init-autogen-project.ps1"
  # Expected: True
  ```

### Step 2: Initialization Script

- [ ] Script executed successfully
  ```powershell
  .\autogen_template\init-autogen-project.ps1 `
    -ProjectName "MyApp" `
    -ProjectType "Python" `
    -BuildCommand "pytest"
  # Expected: "? AutoGen infrastructure deployed!"
  ```

- [ ] No errors in script output
  - Check for red error messages
  - All steps should show green checkmarks

### Step 3: Directory Structure

- [ ] `autogen/` directory created
  ```powershell
  Test-Path "autogen/"
  # Expected: True
  ```

- [ ] `autogen/agents/` has 4 JSON files
  ```powershell
  (Get-ChildItem "autogen/agents/*.json").Count
  # Expected: 4
  ```

- [ ] Agent files exist:
  - [ ] `planner_agent.json`
  - [ ] `coder_agent.json`
  - [ ] `reviewer_agent.json`
  - [ ] `tester_agent.json`

- [ ] `autogen/teams/` has team config
  ```powershell
  Test-Path "autogen/teams/team_complete.json"
  # Expected: True
  ```

- [ ] `mcp-server/` directory created
  ```powershell
  Test-Path "mcp-server/"
  # Expected: True
  ```

### Step 4: MCP Server Compilation

- [ ] `package.json` present
  ```powershell
  Test-Path "mcp-server/package.json"
  # Expected: True
  ```

- [ ] Dependencies installed
  ```powershell
  Test-Path "mcp-server/node_modules/"
  # Expected: True
  ```

- [ ] TypeScript compiled
  ```powershell
  Test-Path "mcp-server/dist/index.js"
  # Expected: True
  ```

- [ ] No compilation errors
  ```powershell
  cd mcp-server
  npm run build
  # Expected: Success, no errors
  cd ..
  ```

---

## ? AUTOGEN STUDIO CHECKLIST

### Step 1: Start AutoGen Studio

- [ ] AutoGen Studio starts without errors
  ```powershell
  autogenstudio ui --port 8081 --appdir .
  # Expected: "Running on http://127.0.0.1:8081"
  ```

- [ ] Web interface accessible
  - Open browser: http://localhost:8081
  - Page loads correctly

### Step 2: OpenAI API Key

- [ ] API key configured
  - Click **Settings** (gear icon)
  - Click **API Keys**
  - Add OpenAI API key
  - Test connection (green checkmark)

### Step 3: Import Agents

- [ ] Agents page accessible
  - Click **Agents** in left sidebar

- [ ] **Planner Agent** imported
  - Click **Import Agent**
  - Select `autogen/agents/planner_agent.json`
  - Agent appears in list
  - Label: "Planner"

- [ ] **Coder Agent** imported
  - Import `autogen/agents/coder_agent.json`
  - Label: "Coder"

- [ ] **Reviewer Agent** imported
  - Import `autogen/agents/reviewer_agent.json`
  - Label: "Reviewer"

- [ ] **Tester Agent** imported
  - Import `autogen/agents/tester_agent.json`
  - Label: "Tester"

- [ ] All 4 agents visible
  ```
  Expected agents list:
  - Planner (model: gpt-4o)
  - Coder (model: gpt-4o)
  - Reviewer (model: gpt-4o)
  - Tester (model: gpt-4o-mini)
  ```

### Step 4: Import Team

- [ ] Teams page accessible
  - Click **Teams** in left sidebar

- [ ] Team imported
  - Click **Import Team**
  - Select `autogen/teams/team_complete.json`
  - Team appears in list
  - Label: "[ProjectName] DevTeam"

- [ ] Team configuration correct
  - Click team to view
  - Type: "RoundRobinGroupChat"
  - Participants: 4 agents (planner, coder, reviewer, tester)
  - Max rounds: 20
  - Termination: "WORKFLOW_FINISHED_OK"

---

## ? FIRST EXECUTION CHECKLIST

### Step 1: Playground Setup

- [ ] Playground accessible
  - Click **Playground** in left sidebar

- [ ] Team selected
  - Dropdown shows "[ProjectName] DevTeam"
  - Select it

### Step 2: Test Prompt

- [ ] Paste test prompt
  ```
  Create a simple "Hello World" function in Python.
  
  Requirements:
  - Function name: say_hello()
  - Return: "Hello, World!"
  - Add type hints
  - Add docstring
  
  Workflow:
  1. Planner: Create plan
  2. Coder: Implement function
  3. Reviewer: Check PEP 8
  4. Tester: Run pytest
  
  Please proceed.
  ```

- [ ] Click **Run**

### Step 3: Execution Monitoring

- [ ] **Turn 1 - Planner** executes
  - Message appears from Planner
  - Contains implementation plan
  - Mentions @coder at end

- [ ] **Turn 2 - Coder** executes
  - Message appears from Coder
  - Contains Python code
  - Follows PEP 8 style
  - Has type hints and docstring
  - Mentions @reviewer at end

- [ ] **Turn 3 - Reviewer** executes
  - Message appears from Reviewer
  - Reviews code quality
  - Checks style compliance
  - Says "REVIEW_APPROVED" if OK
  - Mentions @tester at end

- [ ] **Turn 4 - Tester** executes
  - Message appears from Tester
  - Runs pytest (or equivalent)
  - Reports build/test results
  - Says "WORKFLOW_FINISHED_OK" if all pass

- [ ] Workflow terminates correctly
  - No infinite loop
  - Final message: "WORKFLOW_FINISHED_OK"
  - Total turns: 4-6 (depending on complexity)

### Step 4: Output Validation

- [ ] Code looks correct
  - Function `say_hello()` defined
  - Type hints present: `-> str`
  - Docstring present
  - Returns `"Hello, World!"`

- [ ] Style is correct
  - PEP 8 compliant (Python)
  - Or project style guide (other languages)

- [ ] No errors reported
  - Tester reports 0 errors
  - Tester reports 0 warnings
  - Tests pass (if applicable)

---

## ? MCP SERVER CHECKLIST (Optional)

### Step 1: GitHub Copilot Config

- [ ] Config file exists
  ```powershell
  Test-Path "$env:USERPROFILE\.github-copilot\config.json"
  # Expected: True
  ```

- [ ] MCP Server entry added
  ```json
  {
    "mcpServers": {
      "[project-name]-autogen-studio": {
        "command": "node",
        "args": ["[FULL_PATH]/mcp-server/dist/index.js"],
        "env": {
          "PROJECT_ROOT": "[FULL_PATH]"
        }
      }
    }
  }
  ```

- [ ] Paths are absolute (not relative)
- [ ] PROJECT_ROOT points to project root

### Step 2: VS Code Restart

- [ ] VS Code restarted completely
  - Close all instances
  - Reopen project

- [ ] GitHub Copilot extension active
  - Check status bar (Copilot icon)

### Step 3: MCP Tools Test

- [ ] **list_autogen_teams** works
  ```typescript
  // In GitHub Copilot Chat:
  Use MCP tool to list AutoGen teams
  
  // Expected: Returns team_complete.json info
  ```

- [ ] **run_build** works
  ```typescript
  // In GitHub Copilot Chat:
  Use MCP tool to run Debug build
  
  // Expected: Executes build command, returns results
  ```

- [ ] **save_autogen_log** works
  ```typescript
  // In GitHub Copilot Chat:
  Use MCP tool to save log for task TEST
  
  // Expected: Creates file in autogen/sessions/
  ```

---

## ? DOCUMENTATION CHECKLIST

### Sprint Files (Generated by Copilot)

- [ ] `docs/sprint.md` created
  - Sprint version (e.g., v1.0.0)
  - Goals (3-5 items)
  - Duration (2 weeks)

- [ ] `docs/sprint_tasks.md` created
  - 6-8 tasks with IDs (H1.1-H1.8)
  - Each task has:
    - Description
    - Priority
    - Estimated time
    - Acceptance criteria

- [ ] `docs/daily.md` created
  - Current task listed
  - Last completed task
  - Format: "Hecho: [...] | Siguiente: [...]"

- [ ] `docs/MAIN.md` created
  - Project name
  - Technology stack
  - Key principles (3-5 items)
  - Standards reference

### AutoGen Context

- [ ] `.github/autogen-context.md` created
  - MCP tools reference
  - 4 agents description
  - Team configuration
  - Termination keywords
  - Project-specific examples

---

## ? INTEGRATION CHECKLIST

### Build System

- [ ] Build command works manually
  ```powershell
  [YOUR_BUILD_COMMAND]
  # Expected: Success, 0 errors
  ```

- [ ] Build command in Tester Agent matches
  - Open `autogen/agents/tester_agent.json`
  - Find `system_message`
  - Verify "Run build: [YOUR_BUILD_COMMAND]" is correct

### Test System

- [ ] Test command works manually
  ```powershell
  [YOUR_TEST_COMMAND]
  # Expected: Tests pass
  ```

- [ ] Test command in Tester Agent matches
  - Verify in `tester_agent.json`

### Linter

- [ ] Linter command works manually (if applicable)
  ```powershell
  [YOUR_LINT_COMMAND]
  # Expected: No violations
  ```

- [ ] Linter command in Tester Agent matches
  - Verify in `tester_agent.json`

---

## ? OPERATIONAL CHECKLIST

### Daily Use

- [ ] Can create prompts from template
  ```powershell
  Copy-Item "autogen/prompts/_template.md" "autogen/prompts/sprint_v1.0.0/H1.X.md"
  # Edit with task details
  ```

- [ ] Can execute prompts in Playground
  - Paste prompt
  - Click Run
  - Wait for WORKFLOW_FINISHED_OK

- [ ] Can save session logs
  ```powershell
  .\autogen\log-autogen-session.ps1 `
    -TaskID "H1.1" `
    -Status "SUCCESS" `
    -LogContent "[full transcript]" `
    -Duration 180
  ```

### Metrics Tracking

- [ ] Measure task completion time
  - Before AutoGen: [X] minutes
  - After AutoGen: [Y] minutes
  - Savings: [(X-Y)/X * 100]%

- [ ] Track build success rate
  - Successful builds / Total tasks
  - Target: >90%

- [ ] Track review coverage
  - Reviewed tasks / Total tasks
  - Target: 100% (automated)

---

## ? TROUBLESHOOTING CHECKLIST

If something doesn't work:

### AutoGen Studio Issues

- [ ] Check AutoGen Studio logs in terminal
  - Look for error messages
  - Common: API key missing/invalid

- [ ] Verify OpenAI API key
  - Settings ? API Keys
  - Test connection

- [ ] Restart AutoGen Studio
  ```powershell
  # Ctrl+C to stop
  autogenstudio ui --port 8081 --appdir .
  ```

### Agent Execution Issues

- [ ] Verify agent imported correctly
  - Agents list shows all 4
  - Can view each agent configuration

- [ ] Check termination keywords
  - Planner: "TASK_COMPLETED"
  - Coder: "CODE_READY"
  - Reviewer: "REVIEW_APPROVED"
  - Tester: "WORKFLOW_FINISHED_OK"

- [ ] Verify team configuration
  - Correct agent order
  - Correct termination text

### Build Issues

- [ ] Test build manually
  ```powershell
  [YOUR_BUILD_COMMAND]
  ```

- [ ] Update Tester Agent if command wrong
  - Edit `autogen/agents/tester_agent.json`
  - Update `system_message` with correct command

### MCP Server Issues

- [ ] Recompile MCP Server
  ```powershell
  cd mcp-server
  npm run build
  cd ..
  ```

- [ ] Verify config.json paths
  - Must be absolute paths
  - Check PROJECT_ROOT is correct

- [ ] Restart VS Code completely

---

## ? FINAL VERIFICATION

### System Status

- [ ] AutoGen Studio running: http://localhost:8081 ?
- [ ] 4 agents imported and visible ?
- [ ] 1 team configured ?
- [ ] Test prompt executed successfully ?
- [ ] WORKFLOW_FINISHED_OK received ?
- [ ] Code generated is correct ?
- [ ] Build/tests pass ?
- [ ] MCP Server working (optional) ?
- [ ] Documentation generated (optional) ?

### Performance Metrics

- [ ] Test task completed in < 5 minutes
- [ ] Build success rate = 100%
- [ ] Code style compliance = 100%
- [ ] No manual intervention needed

### Ready for Production

- [ ] Completed 5+ test tasks successfully
- [ ] Time savings measured (>50%)
- [ ] Team trained (if applicable)
- [ ] Documentation complete
- [ ] Backup/version control configured

---

## ?? DEPLOYMENT SCORE

**Count your checkmarks:**

- **80-100%**: ? **Excellent** - Production ready
- **60-79%**: ?? **Good** - Minor fixes needed
- **40-59%**: ?? **Fair** - Some issues to resolve
- **<40%**: ? **Poor** - Major troubleshooting required

**If <80%**: Review sections with unchecked items and resolve issues.

---

## ?? SUCCESS!

**If all critical items checked**:

Congratulations! Your AutoGen multi-agent system is **fully deployed and operational**.

### Next Steps

1. **Execute real tasks** (5-10 from your project)
2. **Measure metrics** (time, quality, errors)
3. **Refine configs** (based on results)
4. **Train team** (if applicable)
5. **Scale usage** (more tasks, more developers)

### Expected Results

- **Week 1**: 50%+ time savings
- **Month 1**: 75%+ time savings
- **Month 3**: 5x productivity increase

---

**Need help?** See: `docs/templates/AUTOGEN_TEMPLATE_SETUP.md` (Troubleshooting section)

**Questions?** Ask GitHub Copilot: `@workspace How do I...`

**Version**: 1.0  
**Last Updated**: 2025-01-21
