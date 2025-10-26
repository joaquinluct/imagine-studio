# Metodolog�a de Desarrollo �gil Asistido por IA - Template

> **Prop�sito**: Overview de metodolog�a gen�rica reutilizable en cualquier proyecto de software.

**Para implementaci�n completa**: Ver [`docs/templates/`](templates/) con versiones detalladas con `[PLACEHOLDER]`.

---

## ? QUICK START

### Adaptaci�n a Tu Proyecto (5 pasos)

1. **Copiar estructura base**:
   ```bash
   cp docs/templates/copilot-instructions.template.md .github/copilot-instructions.md
   cp docs/templates/CORE.template.md docs/methodology/CORE.md
   cp docs/templates/*.template.md docs/methodology/
   ```

2. **Reemplazar `[PLACEHOLDER]`** con:
   - Tecnolog�as espec�ficas (C++, Python, React, etc.)
   - Comandos de build/validaci�n reales
   - Est�ndares de c�digo del proyecto
   - Preferencias del equipo

3. **Crear `docs/MAIN.md`** con pilares fundamentales del proyecto

4. **Inicializar archivos de sprint**:
   - `docs/sprint.md` - Sprint actual
   - `docs/sprint_histories.md` - Historias de usuario
   - `docs/sprint_tasks.md` - Tareas detalladas
   - `docs/daily.md` - �ltima tarea + actual
   - `docs/backlog.md` - �tems futuros

5. **Configurar validaci�n** seg�n tu stack:
   - **C++/C#**: `cmake`, `msbuild`, `gcc`, `clang`
   - **Web**: `npm run lint && npm test && npm run build`
   - **Python**: `pytest && mypy . && black --check .`
   - **Rust**: `cargo clippy && cargo test && cargo build`

---

## ?? ARQUITECTURA DE 3 CAPAS

```
?? .github/
??? copilot-instructions.md         [CORE - 150 lines]
    ??? Quick Start (read once)
    ??? Workflow (6 steps)
    ??? Critical Rules
    ??? References to methodology/

?? docs/
??? methodology/
?   ??? CORE.md                     [Read once - comprehensive]
?   ??? WORKFLOW.md                 [Detailed flow]
?   ??? BUGS.md                     [Bug management]
?   ??? SESSIONS.md                 [IA sessions]
?   ??? DEVIATIONS.md               [Sprint deviations]
?
??? templates/                      [Generic reusable versions]
?   ??? copilot-instructions.template.md
?   ??? CORE.template.md
?   ??? ...
?
??? MAIN.md                         [Project pillars]
??? sprint.md                       [Active sprint]
??? daily.md                        [Current task]
```

---

## ?? WORKFLOW (6 Steps)

```
1. Read daily.md ? 2. Check code ? 3. Propose next step
      ?
4. Implement ? 5. Validate ([PLACEHOLDER]) ? 6. Commit + Docs
      ?
Repeat from 1
```

## ?? VALIDATION WORKFLOW (COMPLETE) - **FOLLOW ALWAYS**

**CRITICAL**: This section documents the **COMPLETE** validation flow. **DO NOT SKIP ANY STEP**.

### Step 1: Run Validation Scripts (if exist)
```powershell
# Example: Check tests are NOT in main project (C++)
.\scripts\validate-no-tests-in-main.ps1

# Example: Check code formatting (Python)
black --check .

# Example: Check linting (JavaScript)
npm run lint
```

**Why**: Catch common errors **before** compilation/execution.

---

### Step 2: Build/Compile (Primary)
```powershell
# C++: cmake --build build --config Debug
# Python: python -m py_compile src/**/*.py
# JavaScript: npm run build
# Rust: cargo build
```

**Why**: Verify code compiles without errors.

**Success criteria**: 0 errors, 0 warnings

---

### Step 3: Build/Compile (IDE/Editor - if different from Step 2)
```powershell
# C++: msbuild "Project.sln" /t:Build /p:Configuration=Debug
# Unity: Verify compilation in Unity Editor
# VS Code: Check Problems panel
```

**Why**: Ensure code compiles in **developer's daily environment** (Ctrl+Shift+B, F5, etc.)

**Success criteria**: 0 errors, 0 warnings in IDE

---

### Step 4: Execute Tests (if task creates tests)
```powershell
# C++: .\build\Debug\test_executable.exe
# Python: pytest tests/ -v
# JavaScript: npm test
# Rust: cargo test
```

**Why**: Verifying tests **COMPILE** is not enough. They must **RUN** and **PASS**.

**Success criteria**: All assertions PASS

---

### Step 5: Execute Linting/Static Analysis
```powershell
# Python: mypy . && flake8 .
# JavaScript: eslint src/
# Rust: cargo clippy
# C++: cppcheck or clang-tidy
```

**Why**: Catch style issues, potential bugs, and code smells.

**Success criteria**: No warnings or errors

---

### Step 6: Verify Application Runs (if applicable)
```powershell
# Check application starts without crashes
# Verify new feature is visible (if UI change)
# Test new functionality manually
```

**Why**: Some issues only appear at runtime, not during compilation.

**Success criteria**: Application runs, feature works as expected

---

## 🔴 COMMON ERRORS AND FIXES

### Multiple Definitions Error (C++/C#)

**Symptom**:
```
error LNK2005: symbol already defined
```

**Cause**: File included in multiple projects or test files in main project

**Fix**: Run validation script or remove duplicates

---

### Tests Compile but Don't Run

**Symptom**: Tests executable built successfully but crashes or fails

**Cause**: Missing dependencies, incorrect test setup, or runtime errors

**Fix**: Execute tests **ALWAYS** after compilation, check output

---

### IDE Builds OK, CLI Builds Fail (or vice versa)

**Symptom**: `cmake` succeeds but IDE fails (or opposite)

**Cause**: Different build configurations between CLI and IDE

**Fix**: **ALWAYS validate BOTH** CLI and IDE builds

---

## 📋 VALIDATION CHECKLIST (Use EVERY iteration)

**Before commit**:
- [ ] Validation scripts executed (if exist)
- [ ] Primary build: 0 errors, 0 warnings
- [ ] IDE build: 0 errors, 0 warnings (if different)
- [ ] Tests executed (if applicable): All PASSED
- [ ] Linting/static analysis: Clean
- [ ] Application runs (if applicable)

**Only commit when ALL checkboxes are ✅**

---

## ?? FILE STRUCTURE

| File | Purpose | When |
|------|---------|------|
| `docs/MAIN.md` | Project pillars | Read once |
| `docs/daily.md` | Current task | Every session |
| `docs/sprint.md` | Sprint goals | When needed |
| `docs/sprint_tasks.md` | Task details | When needed |
| `docs/sprint_bugs.md` | Bug tracking | When debugging |

---

## ?? CRITICAL RULES

### 1. Never Modify External Libraries
- Third-party code is **READ-ONLY**
- Fix bugs in **YOUR** code, not in libraries
- Create wrappers if customization needed

### 2. Validation Before Commit
- Build/test/lint must be clean
- Auto-fix errors when possible
- Retry until clean

### 3. Bug Workflow
- Register attempts **BEFORE** coding
- User validation **REQUIRED**
- See `docs/methodology/BUGS.md`

### 4. Auto-Update Docs
- `daily.md` + `sprint_commits.md` after commit
- No push to remote (only local commits)

---

## ?? ITERATION FORMAT (MANDATORY - ALL 3 SECTIONS)

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

## ?? SPRINT VERSIONING

**Active files**:
- `sprint.md`, `sprint_histories.md`, `sprint_tasks.md`
- `sprint_bugs.md`, `sprint_bug_attempts.md`, `sprint_fix.md`
- `sprint_deviations.md`, `sprint_ia_sessions.md`

**At sprint end**: Archive to `docs/sprints/` with version suffix

---

## ?? EXAMPLES BY TECHNOLOGY

### React + TypeScript
- **Validation**: `npm run lint && npm test && npm run build`
- **Style**: ESLint + Prettier, functional components, hooks
- **Stack**: React 18, TypeScript 5, Vite, Vitest

### Python FastAPI
- **Validation**: `pytest && mypy . && black --check .`
- **Style**: PEP 8, type hints, async/await
- **Stack**: FastAPI, SQLAlchemy, Pydantic, pytest

### Rust
- **Validation**: `cargo clippy && cargo test && cargo build`
- **Style**: rustfmt, Clippy warnings = errors
- **Stack**: Rust 1.70+, tokio, serde

### Unity C#
- **Validation**: Unity compilation + unit tests
- **Style**: Unity conventions, decoupled components
- **Stack**: Unity 2022 LTS, C# 9.0, URP

### C++ Game Engine (Imagine Studio)
- **Validation**: `cmake --build` + `msbuild "Project.sln"`
- **Style**: AAA standards, includes ordered, DRY/SOLID
- **Stack**: C++14, DirectX 12, ImGui

---

## ?? FULL DOCUMENTATION

**Detailed methodology**: [`docs/templates/`](templates/)
- `CORE.template.md` - Comprehensive methodology
- `WORKFLOW.template.md` - Step-by-step flow
- `BUGS.template.md` - Bug management
- `SESSIONS.template.md` - IA sessions (85%)
- `DEVIATIONS.template.md` - Sprint deviations

**Adaptation guide**: Replace all `[PLACEHOLDER]` markers with project-specific values.

---

## ?? PRINCIPLES

1. **Small, functional iterations**: Every task leaves project in working state
2. **Continuous documentation**: Update docs in every commit
3. **IA automation**: Assistant executes complete cycle without manual intervention
4. **Quality from start**: No temporary solutions, only production-ready code
5. **Traceability**: Sprint versioning, descriptive commits, clear history
6. **Double validation**: Clean build + user validation for bugs

