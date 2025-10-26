# AutoGen Studio Setup - Imagine Studio Multi-Agent System

> **Prop�sito**: Gu�a completa para configurar los 4 agentes especializados en AutoGen Studio.

---

## ? QUICK START

### Requisitos Previos

1. **AutoGen Studio instalado y corriendo**:
   ```bash
   # Si a�n no est� instalado:
   pip install autogenstudio
   
   # Iniciar AutoGen Studio:
   autogenstudio ui --port 8081
   ```
   
   Acceder a: `http://127.0.0.1:8081`

2. **OpenAI API Key configurada**:
   - Tener cuenta de OpenAI
   - API Key v�lida
   - Configurar en AutoGen Studio ? Settings ? API Keys

3. **Proyecto Imagine Studio**:
   - Repositorio clonado localmente
   - Estructura `docs/` completa (daily.md, sprint_tasks.md, etc.)

---

## ?? INSTALACI�N PASO A PASO

### Paso 1: Importar los 4 Agentes

En AutoGen Studio:

1. **Ir a "Agents" en el men� lateral**
2. **Click en "Import Agent"**
3. **Importar en este orden**:
   
   **a) Planner Agent**:
   - File: `autogen/planner_agent.json`
   - Verificar que tools `read_daily`, `read_tasks`, `read_sprint`, `list_files` est�n presentes
   
   **b) Coder Agent**:
   - File: `autogen/coder_agent.json`
   - Verificar tools `read_file`, `write_file`, `read_standards`, `list_files`
   
   **c) Reviewer Agent**:
   - File: `autogen/reviewer_agent.json`
   - Verificar tools `read_file`, `read_main`, `read_standards`, `check_external`
   
   **d) Tester Agent**:
   - File: `autogen/tester_agent.json`
   - Verificar tools `compile_cmake`, `compile_msbuild`, `run_tests`, `read_file`

4. **Configurar modelo para cada agente**:
   - Planner: `gpt-4o` (recomendado para planning)
   - Coder: `gpt-4o` (necesita contexto amplio)
   - Reviewer: `gpt-4o` (an�lisis detallado)
   - Tester: `gpt-4o-mini` (parseo de logs, menos complejo)

---

### Paso 2: Crear Group Chat (DevTeam Workflow)

1. **Ir a "Teams" en el men� lateral**
2. **Click en "Create New Team"**
3. **Configurar**:
   - Name: `ImagineStudio_DevTeam`
   - Type: `Round Robin Group Chat`
   - Agents: A�adir los 4 agentes en orden:
     1. `planner`
     2. `coder`
     3. `reviewer`
     4. `tester`
   - Max turns: `20`
   - Max messages: `100`

4. **Configurar terminaci�n**:
   - Add termination condition: `Text Mention`
   - Text: `BUILD_SUCCESS`
   - Add another: `Max Messages`
   - Max: `100`

5. **Guardar el team**

---

### Paso 3: Configurar Working Directory

**CR�TICO**: AutoGen Studio debe ejecutarse desde el directorio ra�z del proyecto.

```powershell
# Navegar al proyecto
cd "C:\Users\joaqu\source\repos\Imagine Studio"

# Iniciar AutoGen Studio desde aqu�
autogenstudio ui --port 8081 --appdir .
```

**Por qu� es importante**:
- Los agentes leen archivos relativos: `docs/daily.md`, `docs/sprint_tasks.md`
- Los comandos de compilaci�n esperan `build/` y `Imagine Studio.sln` en ra�z
- Los tools `write_file` crean archivos relativos a este directorio

---

## ?? USO DEL SISTEMA

### Ejemplo 1: Implementar Tarea H1.1

**Prompt inicial** (en AutoGen Studio ? Playground):

```
Implement task H1.1 from Sprint v1.9.0.

Context:
- Sprint: v1.9.0 - Asset System & Resource Management
- Task: H1.1 - Crear AssetDatabase.h
- Files to read: docs/daily.md, docs/sprint_tasks.md

Workflow:
1. Planner: Read task details and create implementation plan
2. Coder: Implement AssetDatabase.h
3. Reviewer: Review code quality
4. Tester: Compile (CMake + MSBuild)
5. If all OK: Update docs/daily.md and commit

Please proceed with the workflow.
```

**Ejecuci�n esperada**:

```
[Turno 1 - Planner]
Reading docs/daily.md...
Reading docs/sprint_tasks.md...

Task: H1.1 - Crear AssetDatabase.h

Implementation plan:
1. Create src/assets/AssetDatabase.h
2. Define AssetID (uint64_t)
3. Define AssetType enum
4. Define AssetMetadata struct
5. Define AssetDatabase class (singleton)

@coder Please implement this.

[Turno 2 - Coder]
Creating src/assets/AssetDatabase.h...

[Implementation code]

@reviewer Please review this implementation.

[Turno 3 - Reviewer]
Reviewing src/assets/AssetDatabase.h...

Review Status: APPROVED ?

@tester Please compile and test.

[Turno 4 - Tester]
Executing CMake build...
? CMake Build: PASS (0 errors, 0 warnings)

Executing MSBuild...
? MSBuild: PASS (0 errors, 0 warnings)

? ALL CHECKS PASSED
? Ready for commit

@planner Task completed successfully.

[Turno 5 - Planner]
Updating docs/daily.md...
Hecho: H1.1 - Crear AssetDatabase.h
Siguiente: H1.2 - Implementar AssetDatabase.cpp

WORKFLOW_COMPLETE
```

---

### Ejemplo 2: Implementar M�ltiples Tareas

**Prompt para batch processing**:

```
Implement tasks H1.1, H1.2, and H1.3 from Sprint v1.9.0 sequentially.

For each task:
1. Planner reads task details
2. Coder implements
3. Reviewer reviews
4. Tester compiles
5. Update docs/daily.md

Continue until all 3 tasks are completed or max turns reached.
```

---

## ?? TROUBLESHOOTING

### Error: "File not found: docs/daily.md"

**Problema**: AutoGen Studio no est� en el directorio correcto.

**Soluci�n**:
```powershell
# Detener AutoGen Studio (Ctrl+C)
# Navegar al proyecto
cd "C:\Users\joaqu\source\repos\Imagine Studio"
# Reiniciar desde aqu�
autogenstudio ui --port 8081 --appdir .
```

---

### Error: "Tool 'compile_cmake' failed"

**Problema**: CMake no est� en PATH o build/ no existe.

**Soluci�n**:
```powershell
# Verificar CMake instalado
cmake --version

# Crear build directory si no existe
mkdir build
cd build
cmake ..
cd ..
```

---

### Error: "Tool 'compile_msbuild' failed"

**Problema**: MSBuild no est� en PATH.

**Soluci�n**:
```powershell
# A�adir MSBuild al PATH temporalmente
$env:Path += ";C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin"

# Verificar
msbuild -version

# Reiniciar AutoGen Studio desde aqu�
```

---

### Error: "Review rejected - Include order incorrect"

**Problema**: Coder no sigui� est�ndares AAA.

**Comportamiento esperado**: 
- Reviewer detecta el problema
- Pide a Coder que corrija
- Coder reescribe el c�digo
- Loop contin�a hasta que Reviewer aprueba

**No requiere intervenci�n manual** (el sistema se auto-corrige).

---

## ?? M�TRICAS Y MONITOREO

### Tiempo Esperado por Tarea

| Fase | Duraci�n | Tokens |
|------|----------|--------|
| Planner | 10-20s | ~500 |
| Coder | 30-60s | ~1,500 |
| Reviewer | 20-30s | ~800 |
| Tester | 60-120s | ~1,000 |
| **Total** | **2-4 min** | **~3,800** |

**Comparaci�n**:
- Sin multi-agent: 5-10 min/tarea
- Con multi-agent: 2-4 min/tarea
- **Aceleraci�n**: 2-3x m�s r�pido

---

## ?? BENEFICIOS DEL SISTEMA

### 1. Calidad Garantizada
- Reviewer SIEMPRE revisa (no se salta)
- Tester SIEMPRE compila (no se olvida)
- Planner SIEMPRE descompone tareas

### 2. Auto-Correcci�n
- Si build falla ? Tester sugiere fix ? Coder corrige
- Si review falla ? Reviewer pide cambios ? Coder reescribe
- Loop autom�tico hasta �xito

### 3. Consistencia
- Mismo Reviewer con mismos est�ndares
- Mismo Tester con mismas validaciones
- Mismo Planner con mismo formato

### 4. Trazabilidad
- Cada agente registra su trabajo
- Logs completos de decisiones
- Auditable para an�lisis post-mortem

---

## ?? PR�XIMOS PASOS

### Mejoras Futuras

1. **Paralelizaci�n**:
   - M�ltiples Coder Agents para tareas independientes
   - Reviewer revisa en batch

2. **Aprendizaje Continuo**:
   - Reviewer aprende de errores pasados
   - Coder mejora con feedback acumulado

3. **Integraci�n CI/CD**:
   - Hook de GitHub para auto-ejecutar workflow
   - Comentarios autom�ticos en PRs

4. **Dashboard**:
   - M�tricas en tiempo real
   - Gr�ficos de velocidad del sprint
   - Alertas de bloqueos

---

## ?? RECURSOS ADICIONALES

- **AutoGen Docs**: https://microsoft.github.io/autogen/
- **AutoGen Studio**: https://github.com/microsoft/autogen/tree/main/python/packages/autogen-studio
- **Imagine Studio Methodology**: `docs/methodology/CORE.md`

---

**Versi�n**: 1.0  
**�ltima actualizaci�n**: 2025-01-18  
**Autor**: Joaqu�n Luct  
**Proyecto**: Imagine Studio (C++ Game Engine)

---

## ?? �LISTO PARA USAR!

Ya tienes todo configurado. Simplemente:

1. Abrir AutoGen Studio: `http://127.0.0.1:8081`
2. Ir a "Playground"
3. Seleccionar team: `ImagineStudio_DevTeam`
4. Pegar prompt inicial
5. **Disfrutar del desarrollo automatizado** ??
