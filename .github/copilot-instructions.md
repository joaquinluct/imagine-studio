# GitHub Copilot Instructions - Imagine Studio

> **Quick Context**: C++ Game Engine (AAA standards), Agile sprints, DirectX 12 rendering, AutoGen Multi-Agent System

---

## 🚨 SYSTEM PROMPT (READ FIRST - MANDATORY)

**CRITICAL**: Before ANY code change or commit, you MUST follow this exact sequence:

### Pre-Commit Validation Sequence (MANDATORY)
1. **Implement** code changes
2. **CMake Build**: `cmake --build build --config Debug`
   - Must return: 0 errors, 0 warnings
3. **MSBuild**: `msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /m`
   - Must return: 0 errors, 0 warnings
4. **Tests** (if applicable): Run and verify PASSED
5. **ONLY THEN**: `git commit`

### If You Skip Step 3 (MSBuild):
- **STOP IMMEDIATELY**
- Report: "MSBuild validation skipped - cannot commit"
- Ask user for confirmation before proceeding

### No Exceptions
- No "I'll validate later"
- No "CMake passed so MSBuild should work"
- No "Looks simple, probably fine"

**Follow the checklist in "📋 VALIDATION CHECKLIST" section below**.

---

## 🚫 POWERSHELL TERMINAL BLOCKING - **CRITICAL RULES**

### ⚠️ COMMANDS THAT BLOCK THE CHAT (NEVER USE):

**These commands will HANG the terminal and force user to cancel**:

```powershell
# ❌ NEVER USE Select-Object BEFORE pipeline completes
msbuild "Imagine Studio.sln" /t:Build 2>&1 | Select-Object -Last 5

# ❌ NEVER USE Select-String BEFORE pipeline completes
cmake --build build 2>&1 | Select-String "error"

# ❌ NEVER USE complex regex with -replace in multi-line commands
$content = Get-Content file.txt -Raw
$content = $content -replace '(?s)<ComplexRegex>.*?</ComplexRegex>', 'replacement'
Set-Content file.txt $content -NoNewline  # ❌ Can hang

# ❌ NEVER USE XML manipulation inside long pipelines
$xml = [xml](Get-Content file.vcxproj); $xml.Project.ItemDefinitionGroup | Where-Object { ... }
```

**Why they block**:
- `Select-Object` waits for entire stream before selecting
- `Select-String` buffers output before filtering
- Complex regex can cause parsing hangs
- XML manipulation can lock files

---

### ✅ COMMANDS THAT ARE SAFE:

```powershell
# ✅ CORRECT: Use /v:q (quiet) or /v:minimal for msbuild
msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /v:q

# ✅ CORRECT: Let output flow naturally with Out-String
cmake --build build --config Debug 2>&1 | Out-String

# ✅ CORRECT: Simple commands without pipelines
Remove-Item "file.pdb" -Force -ErrorAction SilentlyContinue

# ✅ CORRECT: Short operations with immediate output
Test-Path "Imagine Studio.vcxproj"

# ✅ CORRECT: Process manipulation (kill hanging processes)
Get-Process | Where-Object { $_.ProcessName -eq "cl" }
Stop-Process -Id 12345 -Force
```

---

### 🔧 IF TERMINAL GETS BLOCKED:

**Symptoms**:
- Command shows `>>` prompt indefinitely
- No output for 30+ seconds
- User reports "te has quedado colgado/pillado/parado/bloqueado"

**Recovery**:
1. **STOP IMMEDIATELY** - Don't try more commands
2. Apologize to user
3. Wait for user to cancel (`Ctrl+C` in terminal)
4. **Document the blocked command** (add to this section if not listed)
5. Use SAFE alternative from list above

---

### 📝 MSBUILD BEST PRACTICES:

```powershell
# ✅ Option 1: Quiet output (minimal logs)
msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /v:q

# ✅ Option 2: Minimal output (only errors/warnings)
msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /v:minimal

# ✅ Option 3: Normal output with Out-String (if logs needed)
msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 2>&1 | Out-String

# ❌ NEVER: Filter with Select-Object
msbuild ... 2>&1 | Select-Object -Last 5  # BLOCKS CHAT

# ⚠️ IMPORTANT: CMake generates project files in ROOT directory
# Configuration: cmake -S . -B . (NOT cmake -S . -B build)
# User opens: Imagine Studio.sln (root) in Visual Studio
```

---

### 🔍 CMAKE BEST PRACTICES:

```powershell
# ✅ CORRECT: Simple command (no filtering)
cmake --build build --config Debug

# ✅ CORRECT: With Out-String if logs needed
cmake --build build --config Debug 2>&1 | Out-String

# ❌ NEVER: Filter with Select-Object
cmake --build build 2>&1 | Select-Object -Last 10  # BLOCKS CHAT
```

---

### 💡 GENERAL RULE:

**If command takes >5 seconds to complete**:
- ✅ DO: Let it run naturally (no filtering)
- ✅ DO: Use `/v:q` or `/v:minimal` flags
- ❌ DON'T: Add `Select-Object`, `Select-String`, or complex pipelines
- ❌ DON'T: Try to "optimize" output with filters

**Remember**: User would rather see full output than have chat blocked for 5+ minutes.

---

## 🚀 QUICK START (Read Once)

**First session**: Read these **once**:
1. [`docs/MAIN.md`](../docs/MAIN.md) - Project pillars & AAA standards  
2. [`docs/methodology/CORE.md`](../docs/methodology/CORE.md) - Methodology fundamentals
3. **[`.github/autogen-context.md`](autogen-context.md) - AutoGen multi-agent system** 🆕 **NEW**

**Every session**: Check:
1. [`docs/daily.md`](../docs/daily.md) - Last task + current task  
2. This file - Core workflow rules

---

## 🤖 AUTOGEN MULTI-AGENT SYSTEM

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
4. Implement → 5. Validate (COMPLETE VALIDATION) → 6. Commit + Update docs
      ↓
Repeat from 1
```

---

## ✅ VALIDATION WORKFLOW (COMPLETE) - **FOLLOW ALWAYS**

**CRITICAL**: This section documents the **COMPLETE** validation flow. **DO NOT SKIP ANY STEP**.

### Step 1: Run Validation Script (if exists)
```powershell
# C++ projects: Check tests are NOT in main project
.\scripts\validate-no-tests-in-main.ps1

# Other validation scripts as needed
```

**Why**: Prevents LNK2005 errors (multiple `main()` functions) before build.

---

### Step 2: Build 1 (CMake - Multi-platform publisher)
```powershell
cmake --build . --config Debug 2>&1 | Out-String
```

**Why**: Multi-platform builds (Windows, Mac, Linux, Consoles). If this fails, game cannot be published → **blocking issue**.

**Success criteria**: 0 errors, 0 warnings

**Note**: 
- Use `2>&1 | Out-String` to prevent terminal blocking. Never use `Select-Object` before completion.
- CMake builds from root directory (`.` not `build/`)
- Configuration was done with `cmake -S . -B .` (generates files in root)

---

### Step 3: Build 2 (MSBuild - Visual Studio F5 debug)
```powershell
# IMPORTANTE: Usuario trabaja con Imagine Studio.sln en el ROOT del proyecto
# CMake genera archivos de proyecto en ROOT (no en build/)
# F5 y Ctrl+Shift+B se ejecutan sobre Imagine Studio.sln (root)

# Comando de validación MSBuild (desde línea de comandos):
msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /m
```

**Why**: This is what user presses with **Ctrl+Shift+B** or **F5** in Visual Studio. If this fails, user **CANNOT WORK**.

**CRITICAL**: 
- ✅ **CORRECT solution file**: `Imagine Studio.sln` (root directory)
- ✅ **User workflow**: Open `Imagine Studio.sln` in Visual Studio → Press F5 or Ctrl+Shift+B
- ⚠️ **CMake configuration**: `cmake -S . -B .` (generates files in root, NOT in build/)

**Success criteria**: 0 errors, 0 warnings

---

### Step 4: Execute Tests (if task creates tests)
```powershell
# Example: mesh_importer_test
.\build\Debug\mesh_importer_test.exe

# Check output shows PASSED
```

**Why**: Verifying tests **COMPILE** is not enough. They must **RUN** and **PASS**.

**Success criteria**: All assertions PASS (e.g., "27/27 passed")

---

### Step 5: Verify Application Compiles (if applicable)
```powershell
# Check Visual Studio Output Window for build status
# Or run get_output_window_logs tool
```

**Why**: Some errors only appear in Visual Studio UI, not in terminal output.

**Success criteria**: "Compilation: 0 errors, 0 warnings" in Output Window

---

## 💾 GIT COMMIT WORKFLOW - **CRITICAL**

### ⚠️ NEVER RETRY FAILED COMMITS WITH SAME COMMAND

**PowerShell parses special characters differently than Git**. If a commit fails, **STOP IMMEDIATELY** and use the **CORRECT** method below.

### ✅ CORRECT METHOD (Multiple -m flags):

```powershell
git add -A
git commit -m "feat(scope): Short title" `
           -m "Detail line 1" `
           -m "Detail line 2" `
           -m "Refs: H1 Sprint v2.0.0"
Write-Host "Commit realizado"
```

**Why this works**:
- Each `-m` flag is a separate argument (no parsing issues)
- PowerShell backticks (`) allow multi-line commands
- No special characters in strings

### ❌ WRONG METHODS (DO NOT USE):

```powershell
# ❌ WRONG 1: Here-string with $msg variable
$msg = @"
feat(scope): Title
- Line with (parentheses)
"@
git commit -m $msg  # ❌ PowerShell parses parentheses as code

# ❌ WRONG 2: Single -m with line breaks
git commit -m "Line 1
Line 2"  # ❌ PowerShell splits on newlines

# ❌ WRONG 3: Escaped quotes
git commit -m "feat(scope): \"Title\""  # ❌ Complex escaping
```

### 📝 COMMIT FORMAT (Multiple -m):

```powershell
git commit -m "feat(materials): H1 completada - Material Core" `
           -m "Material.h, Material.cpp, MaterialInstance created" `
           -m "Texturas PBR copiadas (6 materiales)" `
           -m "Compilacion limpia: 0 errores, 0 warnings" `
           -m "Refs: H1 Sprint v2.0.0"
```

### 🔥 IF COMMIT FAILS:

1. **STOP** - Do NOT retry same command
2. Use multiple `-m` flags (method above)
3. If still fails, use `git commit` (opens editor)
4. **NEVER** waste tokens retrying PowerShell parsing errors

---

## 🔴 CRITICAL ERRORS AND FIXES

### LNK2005: Multiple `main()` Definitions

**Symptom**:
```
mesh_importer_test.obj : error LNK2005: ya se definió main en main.obj
fatal error LNK1169: se encontraron uno o más símbolos definidos simultáneamente
```

**Cause**: Test file (`tests/*.cpp`) was added to main project (`Imagine Studio.vcxproj`)

**Fix**: Run validation script
```powershell
.\scripts\validate-no-tests-in-main.ps1
```

**Prevention**: **ALWAYS** run this script after creating new test files.

---

### CMake Builds OK, MSBuild Fails

**Symptom**: `cmake --build` succeeds, but `msbuild "Imagine Studio.sln"` fails

**Cause**: CMake and Visual Studio have separate configurations. Tests may be separate in CMake but included in MSBuild.

**Fix**:
1. Check `Imagine Studio.vcxproj` for test files
2. Remove manually or run validation script
3. Rebuild with MSBuild

---

## 📋 VALIDATION CHECKLIST (Use EVERY iteration)

**Before commit**:
- [ ] Validation scripts executed (if exist)
- [ ] CMake build: 0 errors, 0 warnings
- [ ] MSBuild: 0 errors, 0 warnings
- [ ] Tests executed (if applicable): All PASSED
- [ ] Visual Studio Output Window checked
- [ ] Application can be launched (F5) if task affects app

**Only commit when ALL checkboxes are ✅**

---

## 🔥 ITERATION FORMAT (MANDATORY - ALL 3 SECTIONS)

**After each iteration**, report with **3 MANDATORY sections**:

### 1. Titles (MANDATORY)
```markdown
✅ Done: H1.3 - EntityManager implemented
🔜 Next: H1.4 - Unit tests Entity System
```

### 2. Visualization (MANDATORY) ⚠️ **NEVER SKIP THIS SECTION**
```markdown
### 🎨 Visualization:

**Changes visible after F5?**: **YES** ✅ / **NO** ❌

[If YES]
**You should see**:
1. ✅ Panel "Hierarchy" visible on left side
2. ✅ Expandable tree with objects

[If NO]
**Reason**: This task implements internal classes without UI effect
**Visualization will come in**: [List future tasks that will show visual changes]
```

**Position**: AFTER titles, BEFORE progress bar  
**Why MANDATORY**: 
- Sets clear expectations (user knows if they should see changes)
- Prevents wasted time (user won't press F5 expecting something that isn't there)
- Documents when visual features will be available
- AAA standard: Professional engines always document "What will the user see?"

### 3. Progress Bar (MANDATORY)
```
+--------------------------------------------------------------------+
███████████████████████ 18.8%⬛
+--------------------------------------------------------------------+
```

**Calculate**: `(completed tasks / total sprint tasks) × 100`

---

## 🚨 CRITICAL: AUTO-UPDATE DOCS AFTER EVERY TASK/COMMIT

**RULE**: After EVERY task completion or commit, you MUST update these files:

### Files to Update (MANDATORY):

1. **`docs/daily.md`** - Update progress section:
   - ✅ Done: [Task ID] - [Description]
   - 🔜 Next: [Next Task ID] - [Description]
   - Add 🎨 Visualization section (MANDATORY)
   - Update progress bar
   - Update "Logros de la sesion" list

2. **`docs/sprint_commits.md`** - Add commit entry:
   - Commit hash
   - Date
   - Type (feat/fix/docs/etc.)
   - Full description
   - Files modified
   - Compilation status
   - Reference to task

### When to Update:

- ✅ **AFTER** every successful commit
- ✅ **BEFORE** asking user for next action
- ✅ **AFTER** completing a task
- ✅ **AFTER** fixing a bug

### Consequences of NOT Updating:

- ❌ User loses context between sessions
- ❌ Progress tracking broken
- ❌ Sprint history incomplete
- ❌ User frustration increases
- ❌ **UNACCEPTABLE** - Workflow must be followed strictly

### Auto-Update Sequence (MANDATORY):

```
1. Task implementation
2. CMake build validation
3. MSBuild validation
4. git commit
5. ✅ UPDATE docs/daily.md (MANDATORY)
6. ✅ UPDATE docs/sprint_commits.md (MANDATORY)
7. git commit (docs update)
8. ONLY THEN: Report to user with 3 MANDATORY sections
```

**NO EXCEPTIONS**. This is not optional. This is **CRITICAL WORKFLOW**.

---

## 📂 FILE STRUCTURE

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

## ⚠️ CRITICAL RULES

### 1. Never Modify `external/`
- Third-party libraries are **READ-ONLY**
- Fix bugs in **OUR** code, not in libraries
- Create wrappers in `src/` if needed
- See [`docs/THIRD_PARTY.md`](../docs/THIRD_PARTY.md)

### 2. Never Add Tests to Main Project
- **Test files** (`tests/*.cpp`) are **SEPARATE EXECUTABLES**
- They **MUST NOT** be included in `Imagine Studio.vcxproj`
- If tests appear in main project → **LNK2005 error** (multiple `main()`)
- **Solution**: Remove tests from `.vcxproj` immediately:
  ```powershell
  $vcxproj = "Imagine Studio.vcxproj"
  $content = Get-Content $vcxproj -Raw
  $content = $content -replace '\s*<ClCompile Include="tests\\[^"]+\.cpp" />\s*', ''
  Set-Content $vcxproj $content -NoNewline
  ```
- Tests are compiled separately by CMake as standalone executables

### 3. Validation Before Commit
- **BOTH** builds must be clean (CMake + MSBuild)
- Auto-fix errors when possible
- Retry until clean

### 4. Bug Workflow
- Register attempts in `sprint_bug_attempts.md` **BEFORE** coding
- User validation **REQUIRED** (don't auto-resolve)
- See [`docs/methodology/BUGS.md`](../docs/methodology/BUGS.md)

### 5. Auto-Update Docs
- `daily.md` + `sprint_commits.md` after **every** commit
- No push to remote (only local commits)

### 6. File Encoding
- **UTF-8 with BOM** + **CRLF** line endings (Windows)
- Run `.\scripts\check-encoding.ps1 -Fix` if issues detected

### 7. AutoGen Termination Keywords
- **Use**: `WORKFLOW_FINISHED_OK` (Tester Agent when builds pass)
- **Avoid**: `BUILD_SUCCESS` in prompts (causes premature termination)
- See: [`autogen/TERMINATION_FIX.md`](../autogen/TERMINATION_FIX.md)

### 8. Git Commits with Special Characters
- **ALWAYS** use multiple `-m` flags (see "GIT COMMIT WORKFLOW" section)
- **NEVER** retry failed commits with same command
- **NEVER** use here-strings or complex escaping

---

## 🔧 ADVANCED TOPICS

**Only read when needed**:
- Bug management → [`docs/methodology/BUGS.md`](../docs/methodology/BUGS.md)
- IA sessions (85% limit) → [`docs/methodology/SESSIONS.md`](../docs/methodology/SESSIONS.md)
- Sprint deviations → [`docs/methodology/DEVIATIONS.md`](../docs/methodology/DEVIATIONS.md)
- Full workflow → [`docs/methodology/WORKFLOW.md`](../docs/methodology/WORKFLOW.md)
- **AutoGen setup** → [`autogen/SETUP.md`](../autogen/SETUP.md)
- **AutoGen workflow** → [`autogen/WORKFLOW_COMPLETE.md`](../autogen/WORKFLOW_COMPLETE.md)

---

## 🆘 TROUBLESHOOTING

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

**Git commit fails?**
- **STOP** retrying same command
- Use multiple `-m` flags (see "GIT COMMIT WORKFLOW" section)
- If desperate, use `git commit` (opens editor)
