# GitHub Copilot Instructions - Imagine Studio

> **Quick Context**: C++ Game Engine (AAA standards), Agile sprints, DirectX 12 rendering, AutoGen Multi-Agent System

---

## ? QUICK START (Read Once)

**First session**: Read these **once**:
1. [`docs/MAIN.md`](../docs/MAIN.md) - Project pillars & AAA standards  
2. [`docs/methodology/CORE.md`](../docs/methodology/CORE.md) - Methodology fundamentals
3. **[`.github/autogen-context.md`](autogen-context.md) - AutoGen multi-agent system** ? **NEW**

**Every session**: Check:
1. [`docs/daily.md`](../docs/daily.md) - Last task + current task  
2. This file - Core workflow rules

---

## ?? AUTOGEN MULTI-AGENT SYSTEM

**When working with AutoGen** (tasks in `autogen/`):
- Read: [`.github/autogen-context.md`](autogen-context.md) for complete context
- **4 specialized agents**: Planner, Coder, Reviewer, Tester
- **Workflow**: Round Robin (sequential turns)
- **Termination keyword**: `WORKFLOW_FINISHED_OK`

**Quick commands**:
```powershell
# Start AutoGen Studio
autogenstudio ui --port 8081 --appdir .

# Create prompt from template
Copy-Item "autogen/prompts/_template.md" "autogen/prompts/sprint_v1.9.0/HX.Y.md"

# Log session
.\autogen\log-autogen-session.ps1 -TaskID "H1.1" -Status "SUCCESS" -LogContent "..." -Duration 210
```

**Structure**:
- `autogen/agents/` - 4 agent JSON files
- `autogen/teams/` - Team configurations
- `autogen/prompts/` - Reusable prompts by sprint
- `autogen/sessions/` - Execution logs
- `autogen/outputs/` - Generated files (temporary)

---

## 🔥 WORKFLOW (6 Steps)

```
1. Read daily.md → 2. Check code → 3. Propose next step
      ↓
4. Implement → 5. Validate (DOUBLE BUILD MANDATORY) → 6. Commit + Update docs
      ↓
Repeat from 1
```

**Validation (MANDATORY - BOTH BUILDS)**:

**Build 1 (CMake - Multi-platform publisher)**:
```powershell
cmake --build build --config Debug
```

**Build 2 (MSBuild - Visual Studio F5 debug)**:
```powershell
msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /m
```

**Success criteria**: **BOTH builds** must pass (0 errors, 0 warnings)

**Why BOTH are mandatory**:
- **MSBuild**: Daily development (F5 debugging, Ctrl+Shift+B in Visual Studio)
- **CMake**: Multi-platform publishing (Windows, Mac, Linux, Consoles)
- **Game engines MUST compile on all platforms for release**
- If CMake fails, the game cannot be published → **blocking issue**

**If CMake fails locally**:
1. ❌ **DO NOT COMMIT** until both builds pass
2. 🔧 **FIX the CMake configuration** (platform mismatch, missing libs, etc.)
3. ✅ **Validate again** - only commit when BOTH are clean

---

## ?? FILE STRUCTURE

| File | Purpose | When |
|------|---------|------|
| `docs/MAIN.md` | Project pillars | Read once |
| `docs/daily.md` | Current task | Every session |
| `docs/sprint.md` | Sprint goals | When needed |
| `docs/sprint_tasks.md` | Task details | When needed |
| `docs/sprint_bugs.md` | Bug tracking | When debugging |
| **`.github/autogen-context.md`** | **AutoGen system** | **When using AutoGen** |

**Full methodology**: [`docs/methodology/CORE.md`](../docs/methodology/CORE.md)

---

## ?? CRITICAL RULES

### 1. Never Modify `external/`
- Third-party libraries are **READ-ONLY**
- Fix bugs in **OUR** code, not in libraries
- Create wrappers in `src/` if needed
- See [`docs/THIRD_PARTY.md`](../docs/THIRD_PARTY.md)

### 2. Validation Before Commit
- **BOTH** builds must be clean (CMake + MSBuild)
- Auto-fix errors when possible
- Retry until clean

### 3. Bug Workflow
- Register attempts in `sprint_bug_attempts.md` **BEFORE** coding
- User validation **REQUIRED** (don't auto-resolve)
- See [`docs/methodology/BUGS.md`](../docs/methodology/BUGS.md)

### 4. Auto-Update Docs
- `daily.md` + `sprint_commits.md` after **every** commit
- No push to remote (only local commits)

### 5. File Encoding
- **UTF-8 with BOM** + **CRLF** line endings (Windows)
- Run `.\scripts\check-encoding.ps1 -Fix` if issues detected

### 6. AutoGen Termination Keywords (NEW)
- **Use**: `WORKFLOW_FINISHED_OK` (Tester Agent when builds pass)
- **Avoid**: `BUILD_SUCCESS` in prompts (causes premature termination)
- See: [`autogen/TERMINATION_FIX.md`](../autogen/TERMINATION_FIX.md)

---

## ?? ITERATION FORMAT

**After each iteration**, report with **3 sections**:

### 1. Titles
```markdown
? Done: H1.3 - EntityManager implemented
?? Next: H1.4 - Unit tests Entity System
```

### 2. Visualization (MANDATORY)
```markdown
### ??? Visualization:

**Changes visible after F5?**: **YES** ? / **NO** ?

[If YES]
**You should see**:
1. ? Panel "Hierarchy" visible on left side
2. ? Expandable tree with objects

[If NO]
**Reason**: This task implements internal classes without UI effect
```

**Position**: AFTER titles, BEFORE progress bar

### 3. Progress Bar
```
+--------------------------------------------------------------------+
?????????????????????????????????????????????????????? 18.8%?
+--------------------------------------------------------------------+
```

**Calculate**: `(completed tasks / total sprint tasks) � 100`

---

## ?? ADVANCED TOPICS

**Only read when needed**:
- Bug management ? [`docs/methodology/BUGS.md`](../docs/methodology/BUGS.md)
- IA sessions (85% limit) ? [`docs/methodology/SESSIONS.md`](../docs/methodology/SESSIONS.md)
- Sprint deviations ? [`docs/methodology/DEVIATIONS.md`](../docs/methodology/DEVIATIONS.md)
- Full workflow ? [`docs/methodology/WORKFLOW.md`](../docs/methodology/WORKFLOW.md)
- **AutoGen setup** ? [`autogen/SETUP.md`](../autogen/SETUP.md)
- **AutoGen workflow** ? [`autogen/WORKFLOW_COMPLETE.md`](../autogen/WORKFLOW_COMPLETE.md)

---

## ??? TROUBLESHOOTING

**Build fails?**
1. Check solution exists: `Get-ChildItem *.sln`
2. Verify files in `.vcxproj` / `CMakeLists.txt`
3. Clean rebuild: `cmake --build build --target clean`

**LNK2019 main error?**
- Check CONSOLE vs WINDOWS subsystem
- Ensure `WinMain` or `main` signature matches

**Encoding issues?**
- Run `.\scripts\check-encoding.ps1 -Fix`
- Verify UTF-8 with BOM + CRLF

**AutoGen issues?**
- Read: [`autogen/TERMINATION_FIX.md`](../autogen/TERMINATION_FIX.md)
- Verify: AutoGen Studio running from project root
- Check: Team has `WORKFLOW_FINISHED_OK` termination (not `BUILD_SUCCESS`)

---

## ?? PREFERENCES

- **Quality first**: No temporary/incomplete solutions
- **AAA standards**: Every module production-ready from start
- **Include order**: Project headers (`"..."`) first, then system (`<...>`), alphabetically within groups
- **Validate before commit**: Always compile + fix errors/warnings
- **AutoGen logging**: Always register successful sessions in `autogen/sessions/`

---

## ?? SPRINT VERSIONING

**Active sprint files**:
- `docs/sprint.md`, `sprint_histories.md`, `sprint_tasks.md`
- `docs/sprint_bugs.md`, `sprint_bug_attempts.md`, `sprint_fix.md`
- `docs/sprint_deviations.md`, `sprint_ia_sessions.md`

**At sprint end**:
```powershell
# Archives all to docs/sprints/ with version suffix
Move-Item "docs/sprint_v<version>.md" "docs/sprints/"
# ... (script handles all files)

# Copy AutoGen logs
Copy-Item "autogen/sessions/*_success.md" "docs/sprints/sprint_v<version>_autogen/"
```

---
**Version**: 2.1  
**Last update**: 2025-01-18  
**Full methodology**: [`docs/methodology/CORE.md`](../docs/methodology/CORE.md)  
**AutoGen context**: [`.github/autogen-context.md`](autogen-context.md)  
**Templates (for other projects)**: [`docs/templates/`](../docs/templates/)
