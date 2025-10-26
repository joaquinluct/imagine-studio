# AutoGen Template - 5-Minute Quick Start

> **Goal**: Get AutoGen multi-agent system running in your project in **5 minutes**.

---

## ? ULTRA-QUICK SETUP

### Prerequisites (1 minute)
```powershell
# Check Python
python --version  # Should be 3.11+

# Check Node.js
node --version  # Should be 18+

# Install AutoGen Studio
pip install autogenstudio

# Verify
autogenstudio --version
```

---

### Setup (2 minutes)

**1. Copy template** (30 seconds):
```powershell
# From Imagine Studio project
cd your-new-project
Copy-Item -Recurse "path/to/imagine-studio/docs/templates/autogen_template_package/" "./autogen_template/"
```

**2. Initialize** (90 seconds):
```powershell
# Replace with YOUR project details
.\autogen_template\init-autogen-project.ps1 `
  -ProjectName "MyApp" `
  -ProjectType "Python" `
  -BuildCommand "pytest && mypy . && black --check ."
```

? **Done!** Files created:
- `autogen/agents/` (4 JSON files)
- `autogen/teams/team_complete.json`
- `mcp-server/dist/index.js`

---

### Start (1 minute)

**3. Launch AutoGen Studio** (30 seconds):
```powershell
cd your-new-project
autogenstudio ui --port 8081 --appdir .
```

Opens: http://localhost:8081

**4. Import agents** (30 seconds):
- Click **"Agents"** ? **"Import Agent"**
- Select `autogen/agents/planner_agent.json`
- Repeat for: `coder_agent.json`, `reviewer_agent.json`, `tester_agent.json`

? **Done!** 4 agents ready.

---

### First Test (1 minute)

**5. Create team** (30 seconds):
- Click **"Teams"** ? **"Import Team"**
- Select `autogen/teams/team_complete.json`

**6. Execute test prompt** (30 seconds):
- Click **"Playground"**
- Select team: **"MyApp DevTeam"**
- Paste:
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

- Click **"Run"**

? **Done!** Watch agents work (2-3 minutes).

---

## ?? EXPECTED OUTPUT

```
[Planner Agent]
Plan for Hello World function:
1. Create utils/greetings.py
2. Define say_hello() -> str
3. Add type hints: str
4. Add docstring
5. Create test_greetings.py

@coder Please implement.

[Coder Agent]
Creating utils/greetings.py...

def say_hello() -> str:
    """
    Returns a greeting message.
    
    Returns:
        str: "Hello, World!"
    """
    return "Hello, World!"

Creating tests/test_greetings.py...

def test_say_hello():
    assert say_hello() == "Hello, World!"

@reviewer Please review.

[Reviewer Agent]
Reviewing utils/greetings.py...

? Type hints: Present (-> str)
? Docstring: Clear and complete
? PEP 8: Compliant
? Function name: snake_case (correct)

REVIEW_APPROVED ?

@tester Please validate.

[Tester Agent]
Running: pytest && mypy . && black --check .

pytest: 1 passed ?
mypy: Success ?
black: 1 file unchanged ?

Build: 0 errors, 0 warnings
Tests: 1/1 passed

WORKFLOW_FINISHED_OK ?
```

---

## ?? SUCCESS!

You now have:
- ? 4 AI agents working together
- ? Automated code review
- ? Automated testing
- ? Clean code generation

**Next steps**:
1. Create real task in `docs/sprint_tasks.md`
2. Execute with AutoGen
3. Save log with `.\autogen\log-autogen-session.ps1`

---

## ?? FULL DOCUMENTATION

For complete details, read:
- **Main guide**: `docs/templates/AUTOGEN_TEMPLATE_README.md`
- **Setup details**: `docs/templates/AUTOGEN_TEMPLATE_SETUP.md`
- **Copilot prompts**: `docs/templates/COPILOT_PROMPTS_AUTOGEN.md`

---

## ?? TROUBLESHOOTING

### "AutoGen Studio not found"
```powershell
pip install autogenstudio
```

### "MCP Server failed to compile"
```powershell
cd mcp-server
npm install
npm run build
```

### "Build command failed in Tester Agent"
Test manually first:
```powershell
pytest && mypy . && black --check .
# If this works, AutoGen should work too
```

### "Agent not responding"
Check AutoGen Studio logs in terminal.
Common cause: OpenAI API key not configured.

**Fix**: AutoGen Studio ? Settings ? API Keys ? Add your OpenAI key

---

## ?? TIME BREAKDOWN

| Step | Time | Cumulative |
|------|------|------------|
| Check prerequisites | 1 min | 1 min |
| Copy template | 30s | 1.5 min |
| Run init script | 90s | 3 min |
| Start AutoGen Studio | 30s | 3.5 min |
| Import agents | 30s | 4 min |
| Import team | 30s | 4.5 min |
| Execute test prompt | 30s | **5 min** |

**First task execution**: +3 min  
**Total to working system**: **8 minutes**

---

## ?? WHAT YOU JUST LEARNED

1. ? How to initialize AutoGen for any project
2. ? How to configure 4 specialized agents
3. ? How to execute multi-agent workflow
4. ? How agents collaborate (Planner ? Coder ? Reviewer ? Tester)
5. ? How termination works (`WORKFLOW_FINISHED_OK`)

**You're now ready** to automate your development workflow!

---

## ?? NEXT CHALLENGE

Try a **real task** from your project:
1. Pick a simple feature (e.g., "Add logging to X function")
2. Write clear requirements
3. Execute in AutoGen Studio
4. Observe how agents handle it
5. Save the log

**Goal**: Complete 1-2 real tasks today with AutoGen.

---

**Version**: 1.0  
**Time to value**: 5 minutes  
**ROI**: Immediate (5x faster development)

**Ready to scale?** Read the full guide: `AUTOGEN_TEMPLATE_README.md`
