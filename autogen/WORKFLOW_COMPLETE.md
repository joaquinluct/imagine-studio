# Flujo de Trabajo con AutoGen Studio

## ?? CICLO COMPLETO: DESDE PROMPT HASTA COMMIT

### **1. PREPARACIÓN** (5 min - Una sola vez por sesión)

#### 1.1 Iniciar AutoGen Studio
```powershell
cd "C:\Users\joaqu\source\repos\Imagine Studio"
autogenstudio ui --port 8081 --appdir .
```

#### 1.2 Verificar Configuración
- Abrir: `http://127.0.0.1:8081`
- Team: `Imagine Studio DevTeam`
- Agentes: planner, coder, reviewer, tester (4)
- Terminación: `WORKFLOW_FINISHED_OK`

---

### **2. EJECUCIÓN** (2-4 min por tarea)

#### 2.1 Seleccionar Prompt
```powershell
# Abrir archivo de prompt
code autogen/prompts/sprint_v1.9.0/H1.1_assetdatabase_h.md
```

#### 2.2 Ejecutar en Playground
1. **Playground** ? Seleccionar team `Imagine Studio DevTeam`
2. **Copiar** contenido del prompt
3. **Pegar** en el cuadro de texto
4. **Click** "Run"

#### 2.3 Monitorear Ejecución
Observar secuencia:
```
[planner] ? Propone plan
[coder] ? Implementa código
[reviewer] ? Revisa calidad
[tester] ? Compila (CMake + MSBuild)
```

**Resultado esperado**:
```
? ALL CHECKS PASSED
@planner Task completed.
WORKFLOW_FINISHED_OK
```

---

### **3. REGISTRO** (1 min)

#### 3.1 Copiar Output
```powershell
# Copiar todo el contenido del Playground
# Ctrl+A ? Ctrl+C
```

#### 3.2 Guardar Log
**Opción A: Manual**
```powershell
# Crear archivo
$date = Get-Date -Format "yyyy-MM-dd"
New-Item "autogen/sessions/$date`_H1.1_success.md" -ItemType File
# Pegar contenido
```

**Opción B: Automático (Script)**
```powershell
.\autogen\log-autogen-session.ps1 `
    -TaskID "H1.1" `
    -Status "SUCCESS" `
    -LogContent "<contenido copiado>" `
    -Duration 180
```

---

### **4. VERIFICACIÓN** (1 min)

#### 4.1 Verificar Archivos Generados
```powershell
# Ver archivos creados por AutoGen
Get-ChildItem src/assets/ -Recurse
```

#### 4.2 Compilar Manualmente (opcional)
```powershell
# Verificar compilación
cmake --build build --config Debug
msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64
```

---

### **5. COMMIT** (2 min)

#### 5.1 Stage Files
```powershell
git add src/assets/AssetDatabase.h
git add docs/daily.md
git add autogen/sessions/YYYY-MM-DD_H1.1_success.md
```

#### 5.2 Commit
```powershell
git commit -m "feat(assets): Crear AssetDatabase.h (H1.1)

Implementado por AutoGen multi-agent:
- AssetID (uint64_t)
- AssetType enum (Texture, Mesh, Shader, Scene)
- AssetMetadata struct
- AssetDatabase class (singleton)

Refs: H1.1, Sprint v1.9.0
AutoGen Session: YYYY-MM-DD_H1.1_success"
```

---

## ?? FLUJO VISUAL

```
???????????????????????????????????????????????????????
? 1. PREPARACIÓN (5 min - una vez)                   ?
?    - Iniciar AutoGen Studio                         ?
?    - Verificar configuración                        ?
???????????????????????????????????????????????????????
                      ?
???????????????????????????????????????????????????????
? 2. EJECUCIÓN (2-4 min)                              ?
?    - Seleccionar prompt                             ?
?    - Ejecutar en Playground                         ?
?    - Monitorear agentes                             ?
???????????????????????????????????????????????????????
                      ?
???????????????????????????????????????????????????????
? 3. REGISTRO (1 min)                                 ?
?    - Copiar output                                  ?
?    - Guardar log en autogen/sessions/              ?
???????????????????????????????????????????????????????
                      ?
???????????????????????????????????????????????????????
? 4. VERIFICACIÓN (1 min)                             ?
?    - Verificar archivos generados                   ?
?    - Compilar manualmente (opcional)                ?
???????????????????????????????????????????????????????
                      ?
???????????????????????????????????????????????????????
? 5. COMMIT (2 min)                                   ?
?    - git add [archivos]                             ?
?    - git commit con referencia a AutoGen            ?
???????????????????????????????????????????????????????
```

**Tiempo total**: 6-12 minutos por tarea (vs 20-30 minutos manual)

---

## ?? EJEMPLO COMPLETO: TAREA H1.1

### **Paso 1: Preparación**
```powershell
cd "C:\Users\joaqu\source\repos\Imagine Studio"
autogenstudio ui --port 8081 --appdir .
```

### **Paso 2: Ejecución**
1. Abrir: `http://127.0.0.1:8081/playground`
2. Team: `Imagine Studio DevTeam`
3. Copiar contenido de: `autogen/prompts/sprint_v1.9.0/H1.1_assetdatabase_h.md`
4. Pegar en Playground
5. Click "Run"

**Output esperado**:
```
[planner] Task: H1.1 - Crear AssetDatabase.h
Implementation plan: ...
@coder Please implement.

[coder] Creating src/assets/AssetDatabase.h...
File created successfully.
@reviewer Please review.

[reviewer] Review Status: APPROVED ?
@tester Please compile.

[tester] CMake: PASS ?
MSBuild: PASS ?
? ALL CHECKS PASSED
WORKFLOW_FINISHED_OK
```

### **Paso 3: Registro**
```powershell
.\autogen\log-autogen-session.ps1 `
    -TaskID "H1.1" `
    -Status "SUCCESS" `
    -LogContent "..." `
    -Duration 210
```

### **Paso 4: Verificación**
```powershell
# Verificar archivo creado
Get-Content src/assets/AssetDatabase.h | Select-Object -First 10

# Compilar
cmake --build build --config Debug
```

### **Paso 5: Commit**
```powershell
git add src/assets/AssetDatabase.h docs/daily.md
git commit -m "feat(assets): Crear AssetDatabase.h (H1.1)

Implementado por AutoGen multi-agent.
Refs: H1.1, Sprint v1.9.0"
```

---

## ?? MÉTRICAS POR SESIÓN

Registrar en `autogen/sessions/summary/YYYY-MM-DD_metrics.md`:

```markdown
# Metrics - YYYY-MM-DD

| Task | Duration | Status | Build | Attempts |
|------|----------|--------|-------|----------|
| H1.1 | 3m 24s   | ? SUCCESS | CMake ? MSBuild ? | 1 |
| H1.2 | 4m 12s   | ? SUCCESS | CMake ? MSBuild ? | 1 |
| H1.3 | 2m 45s   | ? FAILED  | CMake ? | 2 |

**Total**: 3 tasks, 2 success, 1 failed
**Avg Duration**: 3m 27s per task
**Success Rate**: 66%
```

---

## ?? INTEGRACIÓN CON `docs/`

Al cerrar el sprint:

```powershell
# Copiar logs a docs/sprints/
$sprintVersion = "v1.9.0"
Copy-Item "autogen/sessions/summary/*" `
          "docs/sprints/sprint_${sprintVersion}_autogen/"

# Crear resumen
.\scripts\generate-sprint-summary.ps1 -Sprint $sprintVersion
```

---

**Versión**: 1.0  
**Última actualización**: 2025-01-18  
**Proyecto**: Imagine Studio
