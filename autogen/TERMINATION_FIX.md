# Cambiar Condición de Terminación - AutoGen Studio

## ?? Problema
El workflow termina prematuramente cuando detecta "BUILD_SUCCESS" en el prompt del usuario.

## ? Solución
Cambiar la palabra clave de terminación a `WORKFLOW_FINISHED_OK` (palabra única que nunca usarás en prompts).

---

## ?? PASOS COMPLETOS

### **1. Editar Team en AutoGen Studio**

1. **Abrir** AutoGen Studio: `http://127.0.0.1:8081`
2. **Click** en "Team Builder" (menú izquierdo)
3. **Click** en "Imagine Studio DevTeam"
4. **Buscar** sección "TERMINATIONS"
5. **Click** en "WorkflowTermination" (icono de editar ??)
6. **Cambiar**:
```json
// ANTES
{
  "type": "TextMentionTermination",
  "text": "BUILD_SUCCESS"
}

// DESPUÉS
{
  "type": "TextMentionTermination",
  "text": "WORKFLOW_FINISHED_OK"
}
```
7. **Click** "Save Changes"

---

### **2. Actualizar Tester Agent**

**Opción A: Manual en AutoGen Studio**

1. **Click** en "Gallery" (menú izquierdo)
2. **Buscar** "Tester Agent"
3. **Click** en el icono de editar (??)
4. **Scroll down** hasta "System Message"
5. **Cambiar** la última línea:
```
// ANTES
BUILD_SUCCESS

// DESPUÉS
WORKFLOW_FINISHED_OK
```
6. **Click** "Save"

**Opción B: Re-importar JSON (Recomendado)**

1. **Gallery** ? **Tester Agent** ? **Delete** (icono ???)
2. **Gallery** ? **Import** (botón azul)
3. **Seleccionar** `autogen/agents/tester_agent_fixed.json`
4. **Verificar** que se importó correctamente

---

### **3. Verificar Team está Actualizado**

1. **Team Builder** ? **Imagine Studio DevTeam**
2. **Verificar** que la terminación dice `WORKFLOW_FINISHED_OK`
3. **Verificar** que Tester Agent está en la lista de agentes del team

---

## ?? PROBAR EL CAMBIO

### **Prompt de Prueba**

```
Implement task H1.1 from Sprint v1.9.0.

Context:
- Sprint: v1.9.0 - Asset System & Resource Management
- Task: H1.1 - Crear AssetDatabase.h
- Files to read: docs/daily.md, docs/sprint_tasks.md

Implementation details:
- Create file: src/assets/AssetDatabase.h
- Define AssetID as uint64_t
- Define AssetType enum (Texture, Mesh, Shader, Scene, Unknown)
- Define AssetMetadata struct (id, type, path, name, fileSize, lastModified)
- Define AssetDatabase class (singleton pattern)

Workflow:
1. Planner: Read docs/sprint_tasks.md and propose implementation plan
2. Coder: Implement AssetDatabase.h with C++14 syntax
3. Reviewer: Review code quality
4. Tester: Compile with CMake and MSBuild

Please proceed step by step.
```

**Resultado Esperado**:
```
[planner] Using tool: read_daily
[planner] Using tool: read_tasks
[planner] Task: H1.1...
@coder Please implement.

[coder] Using tool: write_file
[coder] File created: src/assets/AssetDatabase.h
@reviewer Please review.

[reviewer] Using tool: read_file
[reviewer] Review Status: APPROVED
@tester Please compile.

[tester] Using tool: compile_cmake
[tester] CMake: PASS
[tester] Using tool: compile_msbuild
[tester] MSBuild: PASS
[tester] ? ALL CHECKS PASSED
@planner Task completed.
WORKFLOW_FINISHED_OK

Stop reason: Text 'WORKFLOW_FINISHED_OK' mentioned
```

---

## ? TROUBLESHOOTING

### **Problema: Sigue terminando prematuramente**
**Solución**: Verificar que:
1. El team tiene `WORKFLOW_FINISHED_OK` (no `BUILD_SUCCESS`)
2. El Tester Agent fue actualizado (borrar + reimportar)
3. El team está usando el Tester Agent correcto (no una versión antigua)

### **Problema: No termina nunca**
**Solución**: 
1. Verificar que el Tester Agent dice `WORKFLOW_FINISHED_OK` en su output
2. Verificar que la terminación está configurada correctamente
3. Usar MaxMessageTermination como respaldo (100 mensajes)

---

## ?? VERIFICACIÓN FINAL

Checklist antes de probar:
- [ ] Team tiene terminación `WORKFLOW_FINISHED_OK`
- [ ] Tester Agent actualizado con nueva palabra
- [ ] Tester Agent está en el team (no una versión antigua)
- [ ] AutoGen Studio corre desde directorio correcto
- [ ] Prompt NO menciona `WORKFLOW_FINISHED_OK`

---

**Versión**: 1.0  
**Última actualización**: 2025-01-18  
**Proyecto**: Imagine Studio Multi-Agent System
