# Bug Management - Metodolog�a Completa

> **Prop�sito**: Sistema completo de gesti�n de bugs con doble validaci�n (Agente + Usuario).

---

## ?? FLUJO COMPLETO DE BUGS

### Vista General

```
???????????????????
? 1. REPORTAR BUG ? ? Usuario detecta problema
???????????????????
        ?
???????????????????
? 2. REGISTRAR    ? ? A�adir a sprint_bugs.md (BUG-XXX)
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
?8. VALIDACI�N    ? ? Usuario ejecuta y verifica
?   USUARIO       ?
???????????????????
        ?
    �Fix OK?
    ?     ?
  S�      NO
   ?       ?
??????  ??????????????
?FIX ?  ? VOLVER A 4 ?
? OK ?  ?(Intento #N+1)?
??????  ??????????????
```

---

## ?? ARCHIVO: `docs/sprint_bugs.md`

### Prop�sito
Contenedor **inicial** de bugs reportados durante el sprint activo.

### Formato de Entrada

```markdown
### BUG-001 - Crash al renderizar quad sin shader
**ID**: BUG-001
**Prioridad**: Cr�tica
**Estado**: Reportado
**Fecha de entrada**: 2025-01-15

**Descripci�n**: 
La aplicaci�n crashea al intentar renderizar el quad si no se 
compila correctamente el shader HLSL.

**Pasos para reproducir**:
1. Eliminar o renombrar shaders/quad.hlsl
2. Ejecutar aplicaci�n (F5)
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

| Estado | Descripci�n |
|--------|-------------|
| `Reportado` | Bug detectado, a�n no se est� trabajando en �l |
| `En progreso` | Asistente trabajando activamente en el fix |
| `Pendiente validaci�n usuario` | Fix implementado, esperando validaci�n |
| `Resuelto` | ? Usuario confirm� que funciona ? Mover a sprint_fix.md |

---

## ?? ARCHIVO: `docs/sprint_bug_attempts.md`

### Prop�sito
Registro detallado de **TODOS** los intentos de soluci�n (fallidos, parciales, exitosos).

### Formato de Entrada

```markdown
### BUG-001 - Crash al renderizar quad sin shader

**Intento #1**
- **Descripci�n**: A�adir validaci�n de shader blob antes de crear PSO
- **Resultado**: Fallido
- **Detalles**: El cambio evita el crash pero no resuelve el problema ra�z. 
  La aplicaci�n muestra pantalla negra sin mensaje de error.
- **Fecha y hora**: 2025-01-15 10:00
- **Archivos modificados**: src/renderer/DX12Renderer.cpp

**Intento #2**
- **Descripci�n**: A�adir logs de error en CompileShaderFromFile y early return
- **Resultado**: Parcial
- **Detalles**: Ahora muestra mensaje de error en log pero la aplicaci�n 
  contin�a ejecut�ndose con estado corrupto (pantalla negra, no cierra).
- **Fecha y hora**: 2025-01-15 10:15
- **Archivos modificados**: src/renderer/DX12Renderer.cpp

**Intento #3**
- **Descripci�n**: Verificar archivo shader existe ANTES de compilar + 
  abortar Initialize() si falla
- **Resultado**: �xito
- **Detalles**: La aplicaci�n ahora detecta el shader faltante, muestra 
  error claro en log y cierra limpiamente sin crash.
- **Fecha y hora**: 2025-01-15 10:30
- **Archivos modificados**: 
  - src/renderer/DX12Renderer.cpp (Initialize + CompileShaderFromFile)
  - src/main.cpp (verificar return de Initialize)
```

### Reglas de Registro

**ANTES de tocar c�digo**:
1. A�adir nueva entrada con **Intento #N**
2. Describir **QU�** se va a intentar
3. Incluir fecha/hora

**DESPU�S de compilar**:
1. Actualizar entrada con **Resultado** (�xito/Parcial/Fallido)
2. Describir **QU�** pas� (logs, comportamiento)
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
- **Descripci�n**: [QU� se va a intentar]
- **Resultado**: [Dejar vac�o - llenar despu�s]
- **Detalles**: [Dejar vac�o - llenar despu�s]
- **Fecha y hora**: 2025-01-18 14:30
- **Archivos modificados**: [Dejar vac�o - llenar despu�s]
```

### 3. Implementaci�n

Codificar el fix seg�n la descripci�n del intento.

### 4. Compilaci�n

**Doble build obligatoria**:
```powershell
cmake --build build --config Debug
msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /m
```

### 5. Registro de Resultado (DESPU�S de compilar)

**Actualizar entrada en `sprint_bug_attempts.md`**:
```markdown
**Intento #N**
- **Descripci�n**: [Ya completado]
- **Resultado**: �xito / Parcial / Fallido
- **Detalles**: [QU� pas� - incluir errores si hubo]
- **Fecha y hora**: [Ya completado]
- **Archivos modificados**: src/file1.cpp, src/file2.h
```

### 6. Validaci�n Usuario

**Si compilaci�n limpia**:
1. Actualizar estado en `sprint_bugs.md` a **"Pendiente validaci�n usuario"**
2. **PAUSAR** y notificar al usuario:
   ```
   ? Fix implementado para BUG-001
   ? Compilaci�n limpia (CMake + MSBuild)
   ? Por favor ejecuta la aplicaci�n (F5) y verifica si el bug est� resuelto
   ```
3. **ESPERAR** confirmaci�n del usuario

### 7. Iteraci�n (Si falla)

**Si intento fallido o parcial**:
1. Volver al **Paso 2** con nuevo intento (#N+1)
2. **NO repetir** intentos ya probados (consultar `sprint_bug_attempts.md`)

### 8. Resoluci�n Confirmada

**Solo cuando usuario confirme**:
1. Crear entrada en `docs/sprint_fix.md`:
   ```markdown
   ### FIX-001 - Crash al renderizar quad sin shader
   **ID Original**: BUG-001
   **Prioridad**: Cr�tica
   **Fecha de entrada**: 2025-01-15
   **Fecha de resoluci�n**: 2025-01-15
   
   **Descripci�n del problema**: [Copiar de BUG-001]
   
   **Soluci�n implementada**: 
   Verificar existencia de archivo shader ANTES de compilar.
   Si falta, mostrar error en log y abortar Initialize() limpiamente.
   
   **Archivos afectados**: 
   - src/renderer/DX12Renderer.cpp (Initialize, CompileShaderFromFile)
   - src/main.cpp (verificar return de Initialize)
   
   **Commit de resoluci�n**: abc123def
   ```

2. **Eliminar** entrada de `docs/sprint_bugs.md`

3. **Mantener** historial en `docs/sprint_bug_attempts.md` (archivar al cerrar sprint)

---

## ?? REGLAS CR�TICAS

### ? El Asistente NO DEBE:

1. **Modificar c�digo sin registrar intento**
   ```markdown
   ? MAL: Codificar directamente
   ? BIEN: Registrar intento ? Codificar ? Actualizar resultado
   ```

2. **Marcar bug como resuelto solo por compilaci�n limpia**
   ```markdown
   ? MAL: Compilaci�n limpia ? Mover a sprint_fix.md
   ? BIEN: Compilaci�n limpia ? Pausar ? Esperar validaci�n usuario
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

### ? Excepciones (Auto-Resoluci�n Permitida)

**Solo en estos casos**:
1. Fix es trivial (typo, warning cosm�tico)
2. Prueba es determinista y verificable solo con compilaci�n
3. Usuario da confirmaci�n expl�cita sin necesidad de prueba manual

**Ejemplo de excepci�n v�lida**:
```cpp
// BUG: Warning C4100: unreferenced parameter 'deltaTime'
void Update(float deltaTime) {
    // No usa deltaTime
}

// FIX: A�adir (void)deltaTime;
void Update(float deltaTime) {
    (void)deltaTime; // Suppress warning
}

// ? OK auto-resolver: Es un warning cosm�tico verificable con compilaci�n
```

---

## ?? BENEFICIOS DEL FLUJO

### 1. Evita Repetir Soluciones Fallidas
```markdown
Intento #2 fall� con enfoque X
? Intento #3 prueba enfoque Y (diferente)
? No se repite enfoque X
```

### 2. Contexto Hist�rico Invaluable
```markdown
Bug futuro similar ? Consultar sprint_bug_attempts.md hist�ricos
? Ver qu� funcion� y qu� no
? Aplicar lecciones aprendidas
```

### 3. Facilita Colaboraci�n
```markdown
Otro desarrollador puede ver:
- Qu� se intent�
- Por qu� fall�
- Qu� funcion� finalmente
```

### 4. Permite An�lisis Post-Mortem
```markdown
Sprint Review:
- Bugs m�s complejos (m�s intentos)
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

**Versi�n**: 2.0  
**�ltima actualizaci�n**: 2025-01-18  
**Proyecto**: Imagine Studio (C++ Game Engine)
