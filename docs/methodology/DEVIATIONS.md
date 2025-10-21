# Sprint Deviations - Desviaciones y Ajustes Arquitectónicos

> **Propósito**: Gestión de desviaciones arquitectónicas y tareas emergentes bloqueantes durante la ejecución del sprint.

---

## ?? ¿CUÁNDO USAR SPRINT DEVIATIONS?

### Situaciones que Califican

Usa este archivo cuando **durante la ejecución** del sprint surja:

1. **Ajuste Arquitectónico**
   - Cambio en diseño que afecta tareas futuras
   - Viola principios AAA (docs/MAIN.md)
   - Impacta arquitectura de 3 capas

2. **Tarea Emergente Bloqueante**
   - Descubierta durante implementación
   - Bloquea progreso del sprint actual
   - No puede diferirse al siguiente sprint

3. **Deuda Técnica Crítica**
   - Código no cumple estándares AAA
   - Afecta estabilidad/performance
   - Debe corregirse inmediatamente

4. **Refactorización AAA**
   - Código funcional pero no profesional
   - Necesario para continuar con calidad
   - "Hacer bien desde el principio"

---

## ?? CRITERIOS PARA NO USAR BACKLOG

### Diferencia Clave

| Característica | Backlog | Deviations |
|----------------|---------|------------|
| **Timing** | Planificación futura | Durante ejecución |
| **Urgencia** | Puede esperar | Bloqueante |
| **Impacto** | Sprint futuro | Sprint actual |
| **Criticidad** | Baja/Media/Alta | CRÍTICA |

### Checklist de Criticidad

Usar `sprint_deviations.md` si cumple **AL MENOS UNO**:
- ? **Bloqueante**: Impide completar tareas del sprint actual
- ? **Viola pilares**: Contradice docs/MAIN.md o AAA_STANDARDS.md
- ? **Impacto inmediato**: Afecta tareas en progreso o próximas inmediatas

---

## ?? ARCHIVO: `docs/sprint_deviations.md`

### Formato de Entrada

```markdown
## DEV-001: Refactorización AAA - Condicional de visibilidad UI

**Tipo**: Ajuste Arquitectónico
**Detectado en**: H2.3 - Integración DX12 backend
**Fecha**: 2025-01-18
**Prioridad**: CRÍTICA

### Contexto:
ImGui se procesaba SIEMPRE sin respetar m_uiVisible, 
violando estándares AAA ("hacer bien desde el principio").

El código funcional pero no cumplía arquitectura de 3 capas:
- Renderer ? Scene ? Editor
- UI debe ser condicional (F1 toggle)

### ¿Por qué NO pasó a backlog?
- ? **Bloqueante para H4** (Editor Panels): 
  Los panels necesitan UI condicional funcional
- ? **Viola pilares AAA** (docs/MAIN.md): 
  "Hacer bien desde el principio, no soluciones temporales"
- ? **Impacto inmediato**: 
  Afecta arquitectura de 3 capas (core del proyecto)

### Decisión:
Pausar H4 temporalmente e implementar arquitectura AAA 
inmediatamente antes de continuar.

### Tareas Derivadas:
- DEV-001.1: ? Añadir condicional IsUIVisible() en main.cpp
- DEV-001.2: ? Implementar DockSpaceOverViewport() correctamente
- DEV-001.3: ? Añadir #ifdef _DEBUG para ShowDemoWindow()
- DEV-001.4: ? Crear docs/AAA_STANDARDS.md con estándares

### Implementación:
```cpp
// ANTES (incorrecto - violaba AAA)
void RenderFrame() {
    OpaquePass();
    UIPass(); // SIEMPRE ejecutado
    Present();
}

// DESPUÉS (correcto - AAA)
void RenderFrame() {
    OpaquePass();
    if (m_uiVisible) { // Condicional
        UIPass();
    }
    Present();
}
```

### Resultado:
? **Completado** (commit: 011270b)
- Compilación limpia (CMake + MSBuild: 0 errores, 0 warnings)
- Validación usuario: OK 100%
- F1 toggle funciona correctamente

### Impacto en Sprint:
- **Progreso antes**: 62.5% (10/16 tareas)
- **Progreso después**: 75.0% (12/16 tareas)
- **Ganancia**: +12.5% (+2 tareas: DEV-001 cuenta como tarea completada)
- **H4.1-H4.5**: Pueden continuar sobre base sólida AAA

### Lecciones Aprendidas:
1. **Auditoría post-tarea crítica obligatoria**
   - Después de H2.3, revisar si cumple estándares AAA
   - No asumir que "funcional" = "correcto"

2. **No esperar a H4 para detectar problemas arquitectónicos**
   - Validar arquitectura en cada hito (H1, H2, H3)
   - Prevenir deuda técnica desde el inicio

3. **"Hacer bien desde el principio" es más rápido**
   - Pausar y refactorizar: 30 minutos
   - Refactorizar después de H4: 2+ horas (código ya dependiente)

---
```

### Formato de ID

- **DEV-XXX**: Desviación principal
- **DEV-XXX.Y**: Subtarea de desviación

**Ejemplos**:
- `DEV-001`: Refactorización AAA UI
- `DEV-001.1`: Condicional IsUIVisible()
- `DEV-001.2`: DockSpaceOverViewport()

---

## ?? FLUJO DE TRABAJO

### Vista General

```
???????????????????
? 1. DETECTAR     ? ? Durante implementación de HX.Y
?    DESVIACIÓN   ?
???????????????????
        ?
???????????????????
? 2. EVALUAR      ? ? ¿Bloquea sprint? ¿Viola pilares?
?    CRITICIDAD   ?
???????????????????
        ?
    ¿Crítica?
    ?       ?
  SÍ        NO
   ?         ?
??????  ???????????
?DEV ?  ? BACKLOG ?
??????  ???????????
   ?
???????????????????
? 3. DOCUMENTAR   ? ? Crear entrada DEV-XXX
???????????????????
        ?
???????????????????
? 4. PAUSAR       ? ? Pausar tarea HX.Y actual
?    TAREA        ?
???????????????????
        ?
???????????????????
? 5. IMPLEMENTAR  ? ? Resolver desviación primero
?    DESVIACIÓN   ?
???????????????????
        ?
???????????????????
? 6. VALIDAR      ? ? Compilar + validación usuario
???????????????????
        ?
???????????????????
? 7. ACTUALIZAR   ? ? daily.md refleja trabajo en DEV-XXX
?    DAILY.MD     ?
???????????????????
        ?
???????????????????
? 8. CONTINUAR    ? ? Retomar tarea HX.Y original
?    SPRINT       ?
???????????????????
```

---

## ?? PROCESO PASO A PASO

### 1. Detectar Desviación

**Ejemplo durante implementación**:
```markdown
Implementando H2.3 - Integración DX12 backend

? Problema detectado:
ImGui se procesa SIEMPRE, incluso si m_uiVisible = false

? ¿Es esto crítico?
- Viola pilares AAA (hacer bien desde principio)
- Bloqueará H4 (Editor Panels necesitan UI condicional)
- Afecta arquitectura core del proyecto

? SÍ ? Es una desviación crítica
```

### 2. Evaluar Criticidad

**Checklist**:
- [ ] ¿Bloquea tareas del sprint actual?
- [ ] ¿Viola docs/MAIN.md o AAA_STANDARDS.md?
- [ ] ¿Afecta tareas en progreso o próximas inmediatas?
- [ ] ¿Impacta arquitectura core?
- [ ] ¿Puede diferirse al siguiente sprint?

**Decisión**:
- **SI al menos UNA** de las primeras 4: `sprint_deviations.md`
- **SI puede esperar**: `backlog.md`

### 3. Documentar en sprint_deviations.md

**Crear entrada DEV-XXX** con:
- Tipo (Ajuste/Emergente/Deuda/Bloqueador)
- Detectado en (qué tarea)
- Fecha
- Prioridad
- Contexto del problema
- Justificación de criticidad
- Decisión tomada
- Tareas derivadas (DEV-XXX.Y)

### 4. Pausar Tarea Actual

**Actualizar daily.md**:
```markdown
Hecho: H2.2 - TextureImporter implementation
Siguiente: DEV-001 - Refactorización AAA UI (pausa de H2.3)
```

### 5. Implementar Desviación

**Resolver ANTES de continuar sprint**:
- Implementar fix
- Compilar (CMake + MSBuild)
- Validar con usuario
- Commit

### 6. Validar Resultado

**Actualizar entrada en sprint_deviations.md**:
```markdown
### Resultado:
? Completado (commit: abc123)
- Compilación limpia
- Validación usuario: OK 100%
- [Detalle resultado]
```

### 7. Actualizar daily.md

**Reflejar trabajo realizado**:
```markdown
Hecho: DEV-001 - Refactorización AAA UI (resuelto)
Siguiente: H2.3 - Integración DX12 backend (retomar)
```

### 8. Continuar Sprint

**Retomar tarea original** (H2.3) sobre base sólida.

---

## ?? IMPACTO EN SPRINT

### Cómo Contar Desviaciones en Progreso

**Opción A** (recomendada):
- DEV-XXX cuenta como **tarea completada** del sprint
- Incrementa progreso: +1 tarea
- Razón: Es trabajo real realizado

**Opción B**:
- DEV-XXX NO cuenta en progreso oficial
- Se registra como "overhead" del sprint
- Razón: No estaba planificado inicialmente

**Este proyecto usa Opción A**.

### Ejemplo de Progreso

```markdown
Sprint v1.3.0 - 16 tareas planificadas

Progreso inicial: 10/16 (62.5%)
?
DEV-001 detectada y resuelta (+1 tarea real)
?
Progreso actualizado: 12/16 (75.0%)

Justificación: DEV-001 fue trabajo significativo que 
mejoró la calidad del sprint, merece ser contado.
```

---

## ?? TIPOS DE DESVIACIONES

### 1. Ajuste Arquitectónico
```markdown
**Ejemplo**: Cambiar arquitectura de 2 a 3 capas
**Impacto**: Afecta múltiples tareas futuras
**Urgencia**: Alta (bloquea progreso)
```

### 2. Tarea Emergente Bloqueante
```markdown
**Ejemplo**: Bug crítico descubierto durante implementación
**Impacto**: Imposible continuar sin resolverlo
**Urgencia**: Crítica (sprint no puede avanzar)
```

### 3. Deuda Técnica Crítica
```markdown
**Ejemplo**: Código funcional pero no escalable
**Impacto**: Afecta performance o estabilidad
**Urgencia**: Media-Alta (puede causar problemas)
```

### 4. Refactorización AAA
```markdown
**Ejemplo**: Código funcional pero no cumple estándares
**Impacto**: Calidad del proyecto
**Urgencia**: Media (pero importante para AAA)
```

---

## ?? VERSIONADO

### Al Cerrar Sprint

**Archivar** `sprint_deviations.md`:
```powershell
Move-Item "docs/sprint_deviations_v1.9.0.md" "docs/sprints/"
```

**Crear nuevo** `sprint_deviations.md` vacío:
```markdown
# Sprint Deviations - vX.Y.Z

*No se detectaron desviaciones durante este sprint.*
```

---

## ?? DOCUMENTOS RELACIONADOS

- **CORE**: [`docs/methodology/CORE.md`](CORE.md) - Fundamentos
- **Workflow**: [`docs/methodology/WORKFLOW.md`](WORKFLOW.md) - Flujo general
- **Bugs**: [`docs/methodology/BUGS.md`](BUGS.md) - Bug management
- **Sessions**: [`docs/methodology/SESSIONS.md`](SESSIONS.md) - IA sessions

---

**Versión**: 2.0  
**Última actualización**: 2025-01-18  
**Proyecto**: Imagine Studio (C++ Game Engine)
