# Bug Management - Metodología Completa

> **Propósito**: Sistema completo de gestión de bugs con doble validación (Agente + Usuario).

---

## ?? FLUJO COMPLETO DE BUGS

### Vista General

```
???????????????????
? 1. REPORTAR BUG ? ? Usuario detecta problema
???????????????????
        ?
???????????????????
? 2. REGISTRAR    ? ? Añadir a sprint_bugs.md (BUG-XXX)
???????????????????
        ?
???????????????????
? 3. EN PROGRESO  ? ? Actualizar estado
???????????????????
        ?
???????????????????
? 4. INTENTO #N   ? ? Registrar en sprint_bug_attempts.md
???????????????????
        ?
???????????????????
? 5. IMPLEMENTAR  ? ? Codificar fix
???????????????????
        ?
???????????????????
? 6. COMPILAR     ? ? CMake + MSBuild limpias
???????????????????
        ?
???????????????????
? 7. REGISTRAR    ? ? Actualizar resultado intento
???????????????????
        ?
???????????????????
?8. VALIDACIÓN    ? ? Usuario ejecuta y verifica
?   USUARIO       ?
???????????????????
        ?
    ¿Fix OK?
    ?     ?
  SÍ      NO
   ?       ?
??????  ??????????????
?FIX ?  ? VOLVER A 4 ?
? OK ?  ?(Intento #N+1)?
??????  ??????????????
```

---

## ?? ARCHIVO: `docs/sprint_bugs.md`

### Propósito
Contenedor **inicial** de bugs reportados durante el sprint activo.

### Formato de Entrada

```markdown
### BUG-001 - Crash al renderizar quad sin shader
**ID**: BUG-001
**Prioridad**: Crítica
**Estado**: Reportado
**Fecha de entrada**: 2025-01-15

**Descripción**: 
La aplicación crashea al intentar renderizar el quad si no se 
compila correctamente el shader HLSL.

**Pasos para reproducir**:
1. Eliminar o renombrar shaders/quad.hlsl
2. Ejecutar aplicación (F5)
3. Observar crash en DX12Renderer::OpaquePass()

**Comportamiento esperado**: 
Mensaje de error en log, no crash

**Comportamiento observado**: 
Access violation al intentar usar PSO sin shader

**Archivos afectados**: 
- src/renderer/DX12Renderer.cpp
- shaders/quad.hlsl
```

### Estados Posibles

| Estado | Descripción |
|--------|-------------|
| `Reportado` | Bug detectado, aún no se está trabajando en él |
| `En progreso` | Asistente trabajando activamente en el fix |
| `Pendiente validación usuario` | Fix implementado, esperando validación |
| `Resuelto` | ? Usuario confirmó que funciona ? Mover a sprint_fix.md |

---

## ?? ARCHIVO: `docs/sprint_bug_attempts.md`

### Propósito
Registro detallado de **TODOS** los intentos de solución (fallidos, parciales, exitosos).

### Formato de Entrada

```markdown
### BUG-001 - Crash al renderizar quad sin shader

**Intento #1**
- **Descripción**: Añadir validación de shader blob antes de crear PSO
- **Resultado**: Fallido
- **Detalles**: El cambio evita el crash pero no resuelve el problema raíz. 
  La aplicación muestra pantalla negra sin mensaje de error.
- **Fecha y hora**: 2025-01-15 10:00
- **Archivos modificados**: src/renderer/DX12Renderer.cpp

**Intento #2**
- **Descripción**: Añadir logs de error en CompileShaderFromFile y early return
- **Resultado**: Parcial
- **Detalles**: Ahora muestra mensaje de error en log pero la aplicación 
  continúa ejecutándose con estado corrupto (pantalla negra, no cierra).
- **Fecha y hora**: 2025-01-15 10:15
- **Archivos modificados**: src/renderer/DX12Renderer.cpp

**Intento #3**
- **Descripción**: Verificar archivo shader existe ANTES de compilar + 
  abortar Initialize() si falla
- **Resultado**: Éxito
- **Detalles**: La aplicación ahora detecta el shader faltante, muestra 
  error claro en log y cierra limpiamente sin crash.
- **Fecha y hora**: 2025-01-15 10:30
- **Archivos modificados**: 
  - src/renderer/DX12Renderer.cpp (Initialize + CompileShaderFromFile)
  - src/main.cpp (verificar return de Initialize)
```

### Reglas de Registro

**ANTES de tocar código**:
1. Añadir nueva entrada con **Intento #N**
2. Describir **QUÉ** se va a intentar
3. Incluir fecha/hora

**DESPUÉS de compilar**:
1. Actualizar entrada con **Resultado** (Éxito/Parcial/Fallido)
2. Describir **QUÉ** pasó (logs, comportamiento)
3. Listar **archivos modificados**

---

## ?? PROCESO OBLIGATORIO POR BUG

### 1. Contexto Inicial (ANTES de empezar)

**Leer estos archivos**:
```markdown
1. docs/sprint_bugs.md
   ?? Entender el bug actual

2. docs/sprint_bug_attempts.md
   ?? Ver intentos previos (si existen)

3. .github/copilot-instructions.md
   ?? Recordar reglas del proyecto

4. docs/sprint.md + docs/daily.md
   ?? Entender contexto del sprint
```

### 2. Registro de Intento (ANTES de codificar)

**Crear entrada en `sprint_bug_attempts.md`**:
```markdown
**Intento #N**
- **Descripción**: [QUÉ se va a intentar]
- **Resultado**: [Dejar vacío - llenar después]
- **Detalles**: [Dejar vacío - llenar después]
- **Fecha y hora**: 2025-01-18 14:30
- **Archivos modificados**: [Dejar vacío - llenar después]
```

### 3. Implementación

Codificar el fix según la descripción del intento.

### 4. Compilación

**Doble build obligatoria**:
```powershell
cmake --build build --config Debug
msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /m
```

### 5. Registro de Resultado (DESPUÉS de compilar)

**Actualizar entrada en `sprint_bug_attempts.md`**:
```markdown
**Intento #N**
- **Descripción**: [Ya completado]
- **Resultado**: Éxito / Parcial / Fallido
- **Detalles**: [QUÉ pasó - incluir errores si hubo]
- **Fecha y hora**: [Ya completado]
- **Archivos modificados**: src/file1.cpp, src/file2.h
```

### 6. Validación Usuario

**Si compilación limpia**:
1. Actualizar estado en `sprint_bugs.md` a **"Pendiente validación usuario"**
2. **PAUSAR** y notificar al usuario:
   ```
   ? Fix implementado para BUG-001
   ? Compilación limpia (CMake + MSBuild)
   ? Por favor ejecuta la aplicación (F5) y verifica si el bug está resuelto
   ```
3. **ESPERAR** confirmación del usuario

### 7. Iteración (Si falla)

**Si intento fallido o parcial**:
1. Volver al **Paso 2** con nuevo intento (#N+1)
2. **NO repetir** intentos ya probados (consultar `sprint_bug_attempts.md`)

### 8. Resolución Confirmada

**Solo cuando usuario confirme**:
1. Crear entrada en `docs/sprint_fix.md`:
   ```markdown
   ### FIX-001 - Crash al renderizar quad sin shader
   **ID Original**: BUG-001
   **Prioridad**: Crítica
   **Fecha de entrada**: 2025-01-15
   **Fecha de resolución**: 2025-01-15
   
   **Descripción del problema**: [Copiar de BUG-001]
   
   **Solución implementada**: 
   Verificar existencia de archivo shader ANTES de compilar.
   Si falta, mostrar error en log y abortar Initialize() limpiamente.
   
   **Archivos afectados**: 
   - src/renderer/DX12Renderer.cpp (Initialize, CompileShaderFromFile)
   - src/main.cpp (verificar return de Initialize)
   
   **Commit de resolución**: abc123def
   ```

2. **Eliminar** entrada de `docs/sprint_bugs.md`

3. **Mantener** historial en `docs/sprint_bug_attempts.md` (archivar al cerrar sprint)

---

## ?? REGLAS CRÍTICAS

### ? El Asistente NO DEBE:

1. **Modificar código sin registrar intento**
   ```markdown
   ? MAL: Codificar directamente
   ? BIEN: Registrar intento ? Codificar ? Actualizar resultado
   ```

2. **Marcar bug como resuelto solo por compilación limpia**
   ```markdown
   ? MAL: Compilación limpia ? Mover a sprint_fix.md
   ? BIEN: Compilación limpia ? Pausar ? Esperar validación usuario
   ```

3. **Repetir intentos ya probados**
   ```markdown
   ? MAL: Intento #5 = mismo enfoque que Intento #2
   ? BIEN: Consultar sprint_bug_attempts.md ? Probar nuevo enfoque
   ```

4. **Olvidar actualizar resultado del intento**
   ```markdown
   ? MAL: Compilar ? Commit ? Olvidar actualizar sprint_bug_attempts.md
   ? BIEN: Compilar ? Actualizar resultado ? Commit
   ```

### ? Excepciones (Auto-Resolución Permitida)

**Solo en estos casos**:
1. Fix es trivial (typo, warning cosmético)
2. Prueba es determinista y verificable solo con compilación
3. Usuario da confirmación explícita sin necesidad de prueba manual

**Ejemplo de excepción válida**:
```cpp
// BUG: Warning C4100: unreferenced parameter 'deltaTime'
void Update(float deltaTime) {
    // No usa deltaTime
}

// FIX: Añadir (void)deltaTime;
void Update(float deltaTime) {
    (void)deltaTime; // Suppress warning
}

// ? OK auto-resolver: Es un warning cosmético verificable con compilación
```

---

## ?? BENEFICIOS DEL FLUJO

### 1. Evita Repetir Soluciones Fallidas
```markdown
Intento #2 falló con enfoque X
? Intento #3 prueba enfoque Y (diferente)
? No se repite enfoque X
```

### 2. Contexto Histórico Invaluable
```markdown
Bug futuro similar ? Consultar sprint_bug_attempts.md históricos
? Ver qué funcionó y qué no
? Aplicar lecciones aprendidas
```

### 3. Facilita Colaboración
```markdown
Otro desarrollador puede ver:
- Qué se intentó
- Por qué falló
- Qué funcionó finalmente
```

### 4. Permite Análisis Post-Mortem
```markdown
Sprint Review:
- Bugs más complejos (más intentos)
- Patrones de errores comunes
- Mejoras en proceso de debugging
```

---

## ?? DOCUMENTOS RELACIONADOS

- **CORE**: [`docs/methodology/CORE.md`](CORE.md) - Fundamentos
- **Workflow**: [`docs/methodology/WORKFLOW.md`](WORKFLOW.md) - Flujo general
- **Sessions**: [`docs/methodology/SESSIONS.md`](SESSIONS.md) - IA sessions
- **Deviations**: [`docs/methodology/DEVIATIONS.md`](DEVIATIONS.md) - Sprint deviations

---

**Versión**: 2.0  
**Última actualización**: 2025-01-18  
**Proyecto**: Imagine Studio (C++ Game Engine)
