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

# ⚠️ IMPORTANT: Build system architecture
# CMAKE: cmake -S . -B . (generates files in ROOT directory)
# MSBUILD: Works from ROOT directory with ImagineStudio.vcxproj (no space)
# User opens: Imagine Studio.sln (root) → Uses ImagineStudio.vcxproj (root)
# Visual Studio F5/Ctrl+Shift+B: Builds from ROOT using ImagineStudio.vcxproj
```

---

### 🔍 CMAKE BEST PRACTICES:

```powershell
# ✅ CORRECT: Simple command (no filtering)
cmake --build . --config Debug

# ✅ CORRECT: With Out-String if logs needed
cmake --build . --config Debug 2>&1 | Out-String

# ❌ NEVER: Filter with Select-Object
cmake --build . 2>&1 | Select-Object -Last 10  # BLOCKS CHAT
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
- CMake builds from **ROOT** directory (`.` not `build/`)
- Configuration was done with `cmake -S . -B .` (generates files in ROOT)
- CMake generates `ImagineStudio.vcxproj` in ROOT directory

---

### Step 3: Build 2 (MSBuild - Visual Studio F5 debug)
```powershell
# IMPORTANTE: Usuario trabaja con Imagine Studio.sln en el ROOT del proyecto
# Visual Studio usa ImagineStudio.vcxproj (ROOT, sin espacio) generado por CMake
# F5 y Ctrl+Shift+B se ejecutan sobre Imagine Studio.sln (root) → ImagineStudio.vcxproj (root)

# Comando de validación MSBuild (desde línea de comandos):
msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /m
```

**Why**: This is what user presses with **Ctrl+Shift+B** or **F5** in Visual Studio. If this fails, user **CANNOT WORK**.

**CRITICAL**: 
- ✅ **CORRECT solution file**: `Imagine Studio.sln` (root directory)
- ✅ **CORRECT project file**: `ImagineStudio.vcxproj` (root directory, **WITHOUT SPACE**)
- ✅ **User workflow**: Open `Imagine Studio.sln` in Visual Studio → Press F5 or Ctrl+Shift+B → Builds `ImagineStudio.vcxproj` (ROOT)
- ⚠️ **CMake configuration**: `cmake -S . -B .` (generates files in ROOT directory, same location as source)
- ⚠️ **Single .vcxproj file**: `ImagineStudio.vcxproj` in ROOT (used by both CMake and Visual Studio)
- ⚠️ **FIRST TIME SETUP**: After opening solution, right-click `ImagineStudio` → "Set as StartUp Project" (proyecto de inicio debe estar en NEGRITA)

**Success criteria**: 0 errors, 0 warnings

**Note**: Solution contains 17 projects (tests, CMake helpers, libraries). This is **NORMAL** for CMake projects. Only `ImagineStudio` is the main executable.

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
