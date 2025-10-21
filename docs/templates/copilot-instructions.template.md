# GitHub Copilot Instructions - [PLACEHOLDER: Project Name]

> **Quick Context**: [PLACEHOLDER: Brief project description - e.g., "Python web API, FastAPI + PostgreSQL"]

---

## ? QUICK START (Read Once)

**First session**: Read these **once**:
1. [`docs/MAIN.md`](../docs/MAIN.md) - [PLACEHOLDER: Project fundamentals document name]
2. [`docs/methodology/CORE.md`](../docs/methodology/CORE.md) - Methodology fundamentals

**Every session**: Check:
1. [`docs/daily.md`](../docs/daily.md) - Last task + current task  
2. This file - Core workflow rules

---

## ?? WORKFLOW (6 Steps)

```
1. Read daily.md ? 2. Check code ? 3. Propose next step
      ?
4. Implement ? 5. Validate ([PLACEHOLDER]) ? 6. Commit + Update docs
      ?
Repeat from 1
```

### Validation (MANDATORY)

**[PLACEHOLDER: Validation commands for your stack]**

Examples by technology:
- **C++/CMake**: `cmake --build build --config Debug`
- **Python**: `pytest && mypy . && black --check .`
- **Node.js**: `npm run lint && npm test && npm run build`
- **Rust**: `cargo clippy && cargo test && cargo build`
- **Go**: `go fmt && go vet && go test && go build`

**Success criteria**: [PLACEHOLDER: 0 errors, 0 warnings / All tests pass / etc.]

---

## ?? FILE STRUCTURE

| File | Purpose | When |
|------|---------|------|
| `docs/MAIN.md` | Project pillars | Read once |
| `docs/daily.md` | Current task | Every session |
| `docs/sprint.md` | Sprint goals | When needed |
| `docs/sprint_tasks.md` | Task details | When needed |
| `docs/sprint_bugs.md` | Bug tracking | When debugging |

**Full methodology**: [`docs/methodology/CORE.md`](../docs/methodology/CORE.md)

---

## ?? CRITICAL RULES

### 1. Never Modify [PLACEHOLDER: External Dependencies Path]
- [PLACEHOLDER: Third-party libraries / npm packages / pip packages] are **READ-ONLY**
- Fix bugs in **OUR** code, not in dependencies
- Create wrappers in `src/` if needed

### 2. Validation Before Commit
- [PLACEHOLDER: Build/test/lint] must be clean
- Auto-fix errors when possible
- Retry until clean

### 3. Bug Workflow
- Register attempts in `sprint_bug_attempts.md` **BEFORE** coding
- User validation **REQUIRED** (don't auto-resolve)
- See [`docs/methodology/BUGS.md`](../docs/methodology/BUGS.md)

### 4. Auto-Update Docs
- `daily.md` + `sprint_commits.md` after **every** commit
- No push to remote (only local commits)

### 5. [PLACEHOLDER: Code Style Rules]
Examples:
- **C++**: Include order (project first, then system, alphabetically)
- **Python**: PEP 8, type hints mandatory
- **JavaScript**: ESLint + Prettier, JSDoc for public APIs

---

## ?? ITERATION FORMAT

**After each iteration**, report with **3 sections**:

### 1. Titles
```markdown
? Done: H1.3 - [PLACEHOLDER: Task completed]
?? Next: H1.4 - [PLACEHOLDER: Next task]
```

### 2. Visualization (MANDATORY)
```markdown
### ??? Visualization:

**Changes visible after [PLACEHOLDER: F5 / npm start / etc.]?**: **YES** ? / **NO** ?

[If YES]
**You should see**:
1. ? [PLACEHOLDER: Specific visual change 1]
2. ? [PLACEHOLDER: Specific visual change 2]

[If NO]
**Reason**: [PLACEHOLDER: Why no visual changes]
```

### 3. Progress Bar
```
+--------------------------------------------------------------------+
?????????????????????????????????????????????????????? 18.8%?
+--------------------------------------------------------------------+
```

---

## ?? ADVANCED TOPICS

**Only read when needed**:
- Bug management ? [`docs/methodology/BUGS.md`](../docs/methodology/BUGS.md)
- IA sessions (85% limit) ? [`docs/methodology/SESSIONS.md`](../docs/methodology/SESSIONS.md)
- Sprint deviations ? [`docs/methodology/DEVIATIONS.md`](../docs/methodology/DEVIATIONS.md)
- Full workflow ? [`docs/methodology/WORKFLOW.md`](../docs/methodology/WORKFLOW.md)

---

## ??? TROUBLESHOOTING

**[PLACEHOLDER: Common issues and solutions for your stack]**

Examples:
- **Build fails?**: Check [PLACEHOLDER: build file] exists, verify dependencies
- **[PLACEHOLDER: Error type]?**: [PLACEHOLDER: Solution]
- **Encoding issues?**: [PLACEHOLDER: How to fix - e.g., UTF-8 with BOM, CRLF]

---

## ?? PREFERENCES

[PLACEHOLDER: Quality preferences and coding standards]

Examples:
- **Quality first**: No temporary/incomplete solutions
- **[PLACEHOLDER: Standards name]**: Every module [PLACEHOLDER: quality level] from start
- **[PLACEHOLDER: Code style preference]**
- **Validate before commit**: Always [PLACEHOLDER: build/test/lint] + fix errors

---

## ?? SPRINT VERSIONING

**Active sprint files**:
- `docs/sprint.md`, `sprint_histories.md`, `sprint_tasks.md`
- `docs/sprint_bugs.md`, `sprint_bug_attempts.md`, `sprint_fix.md`
- `docs/sprint_deviations.md`, `sprint_ia_sessions.md`

**At sprint end**:
```[PLACEHOLDER: shell language - bash/powershell/etc.]
# Archives all to docs/sprints/ with version suffix
[PLACEHOLDER: move/mv command] "docs/sprint_v<version>.md" "docs/sprints/"
```

---

**Version**: 2.0  
**Last update**: [PLACEHOLDER: Date]  
**Full methodology**: [`docs/methodology/CORE.md`](../docs/methodology/CORE.md)  
**Technology**: [PLACEHOLDER: Main stack - e.g., "Python 3.11 + FastAPI + PostgreSQL"]
