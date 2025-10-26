# GitHub Copilot - How to Use This Template System

> **Purpose**: Instructions for GitHub Copilot on how to deploy and use the AutoGen template package.

---

## ?? FOR GITHUB COPILOT

When a user asks to deploy AutoGen multi-agent system in their project, follow this workflow:

---

## ?? STEP-BY-STEP DEPLOYMENT WORKFLOW

### Step 1: Gather Information

**Ask the user**:
```
To deploy AutoGen, I need the following information:

1. **Project name**: (e.g., "MyApp")
2. **Main language**: (C++, Python, React, Rust, Unity, Go, Java, Other)
3. **Build command**: How do you build/validate your project? (e.g., "npm run build", "pytest", "cmake --build build")
4. **Test command** (optional): How do you run tests? (e.g., "npm test", "pytest tests/")
5. **Lint command** (optional): How do you lint code? (e.g., "npm run lint", "flake8 .")

Please provide these details.
```

---

### Step 2: Run Initialization Script

**Execute**:
```powershell
.\docs\templates\init-autogen-project.ps1 `
  -ProjectName "[USER_PROJECT_NAME]" `
  -ProjectType "[USER_LANGUAGE]" `
  -BuildCommand "[USER_BUILD_CMD]" `
  -TestCommand "[USER_TEST_CMD]" `
  -LintCommand "[USER_LINT_CMD]"
```

**Expected output**: Green checkmarks for all steps.

**Report to user**:
```
? AutoGen infrastructure deployed!

Created:
- autogen/agents/ (4 agent JSON files)
- autogen/teams/team_complete.json
- mcp-server/ (compiled to dist/index.js)

Next steps:
1. Start AutoGen Studio: autogenstudio ui --port 8081 --appdir .
2. Import agents in UI
3. Import team in UI
4. Execute test prompt
```

---

### Step 3: Generate Sprint Files

**Create `docs/sprint.md`**:
```markdown
# Sprint v1.0.0 - [PROJECT_NAME] Initial Setup

**Duration**: [START_DATE] to [END_DATE] (2 weeks)

## ?? Sprint Goals

1. Set up project infrastructure
2. Implement core [FEATURE_1]
3. Implement [FEATURE_2]

## ?? Metrics

- **Velocity target**: 8-10 tasks
- **Quality target**: 0 build errors
- **Review coverage**: 100%

## ?? Success Criteria

- [ ] All infrastructure tasks completed
- [ ] Core features implemented
- [ ] All tests passing
- [ ] Documentation updated
```

**Create `docs/sprint_tasks.md`**:
```markdown
# Sprint v1.0.0 - Task Breakdown

## H1.1 - [TASK_1_NAME]

**Priority**: High  
**Estimated time**: 30 min  
**Dependencies**: None

**Description**:
[Task description]

**Acceptance criteria**:
- [ ] [Criterion 1]
- [ ] [Criterion 2]

---

## H1.2 - [TASK_2_NAME]

...

[Generate 6-8 tasks total]
```

**Create `docs/daily.md`**:
```markdown
# Daily Progress - [PROJECT_NAME]

**�ltima actualizaci�n**: [DATE]

---

## ? Hecho

**[PREVIOUS_TASK_ID]** - [Task name]
- [What was done]

---

## ?? Siguiente

**[CURRENT_TASK_ID]** - [Task name]
- [What needs to be done]

**Estimated time**: [X] minutes
```

---

### Step 4: Create AutoGen Context

**Create `.github/autogen-context.md`**:
```markdown
# AutoGen Studio Context - [PROJECT_NAME]

> **Purpose**: Context for GitHub Copilot Chat about the AutoGen multi-agent system.

## ?? SYSTEM OVERVIEW

**4 specialized agents**:
1. **Planner** - Sprint Manager
2. **Coder** - Implementation Specialist ([LANGUAGE] expert)
3. **Reviewer** - Quality Assurance ([STYLE_GUIDE])
4. **Tester** - Build & Validation

**Workflow**: Planner ? Coder ? Reviewer ? Tester (Round Robin)

## ?? BUILD CONFIGURATION

- **Build**: [USER_BUILD_CMD]
- **Test**: [USER_TEST_CMD]
- **Lint**: [USER_LINT_CMD]

## ?? MCP TOOLS

1. `mcp_[project]_list_autogen_teams()` - List teams
2. `mcp_[project]_run_build(configuration: "Debug")` - Run build
3. `mcp_[project]_save_autogen_log(task_id, status, log, duration)` - Save log

## ?? USAGE

**Start AutoGen Studio**:
\`\`\`powershell
autogenstudio ui --port 8081 --appdir .
\`\`\`

**Execute task**:
1. Go to Playground
2. Select "[PROJECT_NAME] DevTeam"
3. Paste prompt from autogen/prompts/
4. Click Run

**Expected termination**: "WORKFLOW_FINISHED_OK"

[Include project-specific details]
```

---

### Step 5: Configure MCP Server

**Generate config entry for `~/.github-copilot/config.json`**:
```json
{
  "mcpServers": {
    "[project-name]-autogen-studio": {
      "command": "node",
      "args": ["[ABSOLUTE_PATH_TO_PROJECT]/mcp-server/dist/index.js"],
      "env": {
        "PROJECT_ROOT": "[ABSOLUTE_PATH_TO_PROJECT]"
      }
    }
  }
}
```

**Report to user**:
```
MCP Server configuration (add to ~/.github-copilot/config.json):

[Show JSON above with ACTUAL paths]

After adding:
1. Restart VS Code completely
2. Test with: @workspace Use MCP tool to list AutoGen teams
```

---

### Step 6: Generate First Prompt

**Create `autogen/prompts/sprint_v1.0.0/H1.1.md`**:
```markdown
# Task H1.1 - [TASK_NAME]

**Sprint**: v1.0.0  
**Priority**: High  
**Estimated time**: [X] minutes

## Context

[Background information about the task]

## Requirements

1. [Requirement 1]
2. [Requirement 2]
3. [Requirement 3]

## Files to Create/Modify

- \`[file/path/1]\`
- \`[file/path/2]\`

## Acceptance Criteria

- [ ] [Criterion 1]
- [ ] [Criterion 2]
- [ ] [Criterion 3]

## Workflow

1. **Planner**: Read task details from docs/sprint_tasks.md, create implementation plan
2. **Coder**: Implement according to plan, follow [STYLE_GUIDE]
3. **Reviewer**: Check code quality, verify standards compliance
4. **Tester**: Run [BUILD_CMD], ensure 0 errors/warnings

**Expected completion**: WORKFLOW_FINISHED_OK ?

## Additional Notes

[Any special considerations]
```

---

### Step 7: Verification

**Guide user through checklist**:
```
Let's verify the deployment using the checklist:

Please check:
1. AutoGen Studio starts: autogenstudio ui --port 8081 --appdir .
2. Navigate to http://localhost:8081
3. Go to Agents ? Import Agent ? Select all 4 JSONs
4. Go to Teams ? Import Team ? Select team_complete.json
5. Go to Playground ? Select team ? Paste test prompt
6. Click Run ? Wait for "WORKFLOW_FINISHED_OK"

Expected result: Task completes in 2-5 minutes with correct code generated.

For detailed verification, see: docs/templates/AUTOGEN_DEPLOYMENT_CHECKLIST.md
```

---

## ?? LANGUAGE-SPECIFIC ADAPTATIONS

### Python Projects

**Coder Agent adaptations**:
- Style: "PEP 8, type hints required, async/await for I/O"
- Encoding: "UTF-8 (no BOM)"
- Line endings: "LF"
- Import order: "Standard lib, third-party, local"

**Tester Agent adaptations**:
- Build: "python -m py_compile [file]"
- Test: "pytest tests/ -v"
- Lint: "mypy . && black --check . && flake8 ."

---

### React + TypeScript Projects

**Coder Agent adaptations**:
- Style: "ESLint + Prettier, functional components, hooks"
- Encoding: "UTF-8 (no BOM)"
- Line endings: "LF"
- Import order: "React first, then libs, then local"

**Tester Agent adaptations**:
- Build: "npm run build"
- Test: "npm test -- --run"
- Lint: "npm run lint && npm run type-check"

---

### C++ Projects

**Coder Agent adaptations**:
- Style: "Google C++ Style Guide"
- Encoding: "UTF-8 with BOM"
- Line endings: "CRLF"
- Include order: "Project headers first, system headers second, alphabetically"

**Tester Agent adaptations**:
- Build: "cmake --build build --config Debug && msbuild 'Project.sln' /t:Build"
- Test: "ctest --output-on-failure"
- Lint: "clang-tidy [file]"

---

### Rust Projects

**Coder Agent adaptations**:
- Style: "rustfmt + Clippy, idiomatic patterns"
- Encoding: "UTF-8 (no BOM)"
- Line endings: "LF"
- Import order: "std, external crates, local modules"

**Tester Agent adaptations**:
- Build: "cargo build --all-features"
- Test: "cargo test --all-features"
- Lint: "cargo clippy -- -D warnings && cargo fmt --check"

---

## ?? COMMON USER QUESTIONS

### "What if I don't have an OpenAI API key?"

**Response**:
```
You'll need an OpenAI API key for AutoGen to work. Here's how to get one:

1. Go to https://platform.openai.com/
2. Sign up / Log in
3. Go to API Keys section
4. Create new secret key
5. Copy the key
6. Paste in AutoGen Studio ? Settings ? API Keys

Cost: Typically $50-100/month for a 5-developer team.

Alternative: You can use self-hosted models (Ollama) but that requires additional setup.
```

---

### "Can I use a different model (not GPT-4)?"

**Response**:
```
Yes! You can change models for each agent:

1. Open agent JSON file (e.g., autogen/agents/coder_agent.json)
2. Find "llm_config" section
3. Change "model" field to:
   - "gpt-4o-mini" (cheaper, faster, less accurate)
   - "gpt-3.5-turbo" (much cheaper, faster, lower quality)
   - "gpt-4" (more expensive, slower, higher quality)

Recommendation:
- Planner: gpt-4o (needs good planning)
- Coder: gpt-4o (needs accuracy)
- Reviewer: gpt-4o (needs attention to detail)
- Tester: gpt-4o-mini (parsing logs, less complex)

After changing, re-import agents in AutoGen Studio UI.
```

---

### "How do I customize the agents for my project?"

**Response**:
```
To customize agents:

1. Open agent JSON (e.g., autogen/agents/coder_agent.json)
2. Find "system_message" field
3. Edit to add your project-specific rules:

Example additions:
- "Always use our custom logger: myproject.logger.log()"
- "Follow our naming convention: [describe convention]"
- "Use our error handling pattern: [describe pattern]"
- "Import order: [describe order]"

4. Save file
5. Re-import agent in AutoGen Studio UI (Agents ? Import Agent)

See: docs/templates/AUTOGEN_TEMPLATE_SETUP.md (Section: Customization)
```

---

## ?? MONITORING DEPLOYMENT SUCCESS

**After deployment, ask user to verify**:

```
Let's verify the deployment was successful:

1. ? AutoGen Studio running at http://localhost:8081
2. ? 4 agents imported (visible in Agents page)
3. ? 1 team created (visible in Teams page)
4. ? Test prompt executed successfully in Playground
5. ? Received "WORKFLOW_FINISHED_OK" message
6. ? Generated code looks correct
7. ? Build/tests pass

If all checked: Congratulations! Your AutoGen system is ready!

If any failed: Let's troubleshoot together. Which step failed?
```

---

## ?? TEACHING THE USER

**After successful deployment**:

```
Great! Your AutoGen system is deployed. Here's how to use it daily:

1. **Create a task** in docs/sprint_tasks.md:
   - Task ID (e.g., H1.5)
   - Description
   - Acceptance criteria

2. **Create a prompt** from template:
   Copy-Item "autogen/prompts/_template.md" "autogen/prompts/sprint_v1.0.0/H1.5.md"
   # Edit with your task details

3. **Execute in AutoGen Studio**:
   - Go to Playground
   - Select your team
   - Paste prompt content
   - Click Run

4. **Wait for completion** (2-5 minutes):
   - Watch agents work
   - Final message: "WORKFLOW_FINISHED_OK"

5. **Save the log** (optional):
   .\autogen\log-autogen-session.ps1 -TaskID "H1.5" -Status "SUCCESS" -LogContent "[transcript]" -Duration 180

6. **Commit the code**:
   git add .
   git commit -m "feat: [task description] (AutoGen)"

That's it! Repeat for each task.

Questions? See: docs/templates/AUTOGEN_TEMPLATE_README.md
```

---

## ? FINAL CHECKLIST FOR COPILOT

Before marking deployment as complete, verify:

- [ ] User provided: project name, language, build command
- [ ] `init-autogen-project.ps1` executed successfully
- [ ] 4 agent JSON files created in `autogen/agents/`
- [ ] Team JSON created in `autogen/teams/`
- [ ] MCP Server compiled (`mcp-server/dist/index.js` exists)
- [ ] Sprint files created (`docs/sprint.md`, `docs/sprint_tasks.md`, `docs/daily.md`)
- [ ] AutoGen context created (`.github/autogen-context.md`)
- [ ] MCP config entry generated (shown to user)
- [ ] First prompt template created
- [ ] Verification checklist provided to user
- [ ] User confirmed AutoGen Studio running
- [ ] User confirmed test prompt executed successfully

**If all checked**: ? Deployment complete!

---

## ?? TROUBLESHOOTING GUIDE FOR COPILOT

### User: "init script failed"

**Check**:
1. Python 3.11+ installed? (`python --version`)
2. Node.js 18+ installed? (`node --version`)
3. Script execution policy? (`Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass`)

**Fix**: Guide user through installing missing prerequisites.

---

### User: "MCP Server won't compile"

**Check**:
1. `mcp-server/package.json` exists?
2. `npm install` ran successfully?
3. TypeScript errors in output?

**Fix**:
```powershell
cd mcp-server
Remove-Item -Recurse node_modules/
npm install
npm run build
```

---

### User: "Agents not responding in AutoGen Studio"

**Check**:
1. OpenAI API key configured?
2. Agents imported correctly?
3. Team has correct agent references?

**Fix**: Guide through Settings ? API Keys ? Test connection.

---

### User: "Build keeps failing in Tester Agent"

**Check**:
1. Build command works manually?
2. Tester Agent has correct build command in `system_message`?
3. PATH includes necessary tools?

**Fix**: Update `tester_agent.json` with correct, tested build command.

---

## ?? DOCUMENTATION REFERENCE FOR COPILOT

When user asks for more details, refer them to:

| Question Type | Document |
|--------------|----------|
| "How do I set this up?" | `AUTOGEN_TEMPLATE_SETUP.md` |
| "What is AutoGen?" | `AUTOGEN_TEMPLATE_README.md` |
| "How do I use Copilot to help?" | `COPILOT_PROMPTS_AUTOGEN.md` |
| "How do I verify deployment?" | `AUTOGEN_DEPLOYMENT_CHECKLIST.md` |
| "What's the business case?" | `AUTOGEN_EXECUTIVE_SUMMARY.md` |
| "What files are included?" | `AUTOGEN_TEMPLATE_INDEX.md` |
| "Quick start guide?" | `AUTOGEN_QUICK_START.md` |

---

**Version**: 1.0  
**For**: GitHub Copilot AI  
**Last Updated**: 2025-01-21

**Remember**: You're guiding the user through deployment. Be patient, verify each step, and help troubleshoot issues. The goal is a working AutoGen system that saves them 75% of development time!
