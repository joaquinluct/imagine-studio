# Sprint Deviations - Desviaciones y Ajustes Arquitect�nicos

> **Prop�sito**: Gesti�n de desviaciones arquitect�nicas y tareas emergentes bloqueantes durante la ejecuci�n del sprint.

---

## ?? �CU�NDO USAR SPRINT DEVIATIONS?

### Situaciones que Califican

Usa este archivo cuando **durante la ejecuci�n** del sprint surja:

1. **Ajuste Arquitect�nico**
   - Cambio en dise�o que afecta tareas futuras
   - Viola principios AAA (docs/MAIN.md)
   - Impacta arquitectura de 3 capas

2. **Tarea Emergente Bloqueante**
   - Descubierta durante implementaci�n
   - Bloquea progreso del sprint actual
   - No puede diferirse al siguiente sprint

3. **Deuda T�cnica Cr�tica**
   - C�digo no cumple est�ndares AAA
   - Afecta estabilidad/performance
   - Debe corregirse inmediatamente

4. **Refactorizaci�n AAA**
   - C�digo funcional pero no profesional
   - Necesario para continuar con calidad
   - "Hacer bien desde el principio"

---

## ?? CRITERIOS PARA NO USAR BACKLOG

### Diferencia Clave

| Caracter�stica | Backlog | Deviations |
|----------------|---------|------------|
| **Timing** | Planificaci�n futura | Durante ejecuci�n |
| **Urgencia** | Puede esperar | Bloqueante |
| **Impacto** | Sprint futuro | Sprint actual |
| **Criticidad** | Baja/Media/Alta | CR�TICA |

### Checklist de Criticidad

Usar `sprint_deviations.md` si cumple **AL MENOS UNO**:
- ? **Bloqueante**: Impide completar tareas del sprint actual
- ? **Viola pilares**: Contradice docs/MAIN.md o AAA_STANDARDS.md
- ? **Impacto inmediato**: Afecta tareas en progreso o pr�ximas inmediatas

---

## ?? ARCHIVO: `docs/sprint_deviations.md`

### Formato de Entrada

```markdown
## DEV-001: Refactorizaci�n AAA - Condicional de visibilidad UI

**Tipo**: Ajuste Arquitect�nico
**Detectado en**: H2.3 - Integraci�n DX12 backend
**Fecha**: 2025-01-18
**Prioridad**: CR�TICA

### Contexto:
ImGui se procesaba SIEMPRE sin respetar m_uiVisible, 
violando est�ndares AAA ("hacer bien desde el principio").

El c�digo funcional pero no cumpl�a arquitectura de 3 capas:
- Renderer ? Scene ? Editor
- UI debe ser condicional (F1 toggle)

### �Por qu� NO pas� a backlog?
- ? **Bloqueante para H4** (Editor Panels): 
  Los panels necesitan UI condicional funcional
- ? **Viola pilares AAA** (docs/MAIN.md): 
  "Hacer bien desde el principio, no soluciones temporales"
- ? **Impacto inmediato**: 
  Afecta arquitectura de 3 capas (core del proyecto)

### Decisi�n:
Pausar H4 temporalmente e implementar arquitectura AAA 
inmediatamente antes de continuar.

### Tareas Derivadas:
- DEV-001.1: ? A�adir condicional IsUIVisible() en main.cpp
- DEV-001.2: ? Implementar DockSpaceOverViewport() correctamente
- DEV-001.3: ? A�adir #ifdef _DEBUG para ShowDemoWindow()
- DEV-001.4: ? Crear docs/AAA_STANDARDS.md con est�ndares

### Implementaci�n:
```cpp
// ANTES (incorrecto - violaba AAA)
void RenderFrame() {
    OpaquePass();
    UIPass(); // SIEMPRE ejecutado
    Present();
}

// DESPU�S (correcto - AAA)
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
- Compilaci�n limpia (CMake + MSBuild: 0 errores, 0 warnings)
- Validaci�n usuario: OK 100%
- F1 toggle funciona correctamente

### Impacto en Sprint:
- **Progreso antes**: 62.5% (10/16 tareas)
- **Progreso despu�s**: 75.0% (12/16 tareas)
- **Ganancia**: +12.5% (+2 tareas: DEV-001 cuenta como tarea completada)
- **H4.1-H4.5**: Pueden continuar sobre base s�lida AAA

### Lecciones Aprendidas:
1. **Auditor�a post-tarea cr�tica obligatoria**
   - Despu�s de H2.3, revisar si cumple est�ndares AAA
   - No asumir que "funcional" = "correcto"

2. **No esperar a H4 para detectar problemas arquitect�nicos**
   - Validar arquitectura en cada hito (H1, H2, H3)
   - Prevenir deuda t�cnica desde el inicio

3. **"Hacer bien desde el principio" es m�s r�pido**
   - Pausar y refactorizar: 30 minutos
   - Refactorizar despu�s de H4: 2+ horas (c�digo ya dependiente)

---
```

### Formato de ID

- **DEV-XXX**: Desviaci�n principal
- **DEV-XXX.Y**: Subtarea de desviaci�n

**Ejemplos**:
- `DEV-001`: Refactorizaci�n AAA UI
- `DEV-001.1`: Condicional IsUIVisible()
- `DEV-001.2`: DockSpaceOverViewport()

---

## ?? FLUJO DE TRABAJO

### Vista General

```
???????????????????
? 1. DETECTAR     ? ? Durante implementaci�n de HX.Y
?    DESVIACI�N   ?
???????????????????
        ?
???????????????????
? 2. EVALUAR      ? ? �Bloquea sprint? �Viola pilares?
?    CRITICIDAD   ?
???????????????????
        ?
    �Cr�tica?
    ?       ?
  S�        NO
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
? 5. IMPLEMENTAR  ? ? Resolver desviaci�n primero
?    DESVIACI�N   ?
???????????????????
        ?
???????????????????
? 6. VALIDAR      ? ? Compilar + validaci�n usuario
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

### 1. Detectar Desviaci�n

**Ejemplo durante implementaci�n**:
```markdown
Implementando H2.3 - Integraci�n DX12 backend

? Problema detectado:
ImGui se procesa SIEMPRE, incluso si m_uiVisible = false

? �Es esto cr�tico?
- Viola pilares AAA (hacer bien desde principio)
- Bloquear� H4 (Editor Panels necesitan UI condicional)
- Afecta arquitectura core del proyecto

? S� ? Es una desviaci�n cr�tica
```

### 2. Evaluar Criticidad

**Checklist**:
- [ ] �Bloquea tareas del sprint actual?
- [ ] �Viola docs/MAIN.md o AAA_STANDARDS.md?
- [ ] �Afecta tareas en progreso o pr�ximas inmediatas?
- [ ] �Impacta arquitectura core?
- [ ] �Puede diferirse al siguiente sprint?

**Decisi�n**:
- **SI al menos UNA** de las primeras 4: `sprint_deviations.md`
- **SI puede esperar**: `backlog.md`

### 3. Documentar en sprint_deviations.md

**Crear entrada DEV-XXX** con:
- Tipo (Ajuste/Emergente/Deuda/Bloqueador)
- Detectado en (qu� tarea)
- Fecha
- Prioridad
- Contexto del problema
- Justificaci�n de criticidad
- Decisi�n tomada
- Tareas derivadas (DEV-XXX.Y)

### 4. Pausar Tarea Actual

**Actualizar daily.md**:
```markdown
Hecho: H2.2 - TextureImporter implementation
Siguiente: DEV-001 - Refactorizaci�n AAA UI (pausa de H2.3)
```

### 5. Implementar Desviaci�n

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
- Compilaci�n limpia
- Validaci�n usuario: OK 100%
- [Detalle resultado]
```

### 7. Actualizar daily.md

**Reflejar trabajo realizado**:
```markdown
Hecho: DEV-001 - Refactorizaci�n AAA UI (resuelto)
Siguiente: H2.3 - Integraci�n DX12 backend (retomar)
```

### 8. Continuar Sprint

**Retomar tarea original** (H2.3) sobre base s�lida.

---

## ?? IMPACTO EN SPRINT

### C�mo Contar Desviaciones en Progreso

**Opci�n A** (recomendada):
- DEV-XXX cuenta como **tarea completada** del sprint
- Incrementa progreso: +1 tarea
- Raz�n: Es trabajo real realizado

**Opci�n B**:
- DEV-XXX NO cuenta en progreso oficial
- Se registra como "overhead" del sprint
- Raz�n: No estaba planificado inicialmente

**Este proyecto usa Opci�n A**.

### Ejemplo de Progreso

```markdown
Sprint v1.3.0 - 16 tareas planificadas

Progreso inicial: 10/16 (62.5%)
?
DEV-001 detectada y resuelta (+1 tarea real)
?
Progreso actualizado: 12/16 (75.0%)

Justificaci�n: DEV-001 fue trabajo significativo que 
mejor� la calidad del sprint, merece ser contado.
```

---

## ?? TIPOS DE DESVIACIONES

### 1. Ajuste Arquitect�nico
```markdown
**Ejemplo**: Cambiar arquitectura de 2 a 3 capas
**Impacto**: Afecta m�ltiples tareas futuras
**Urgencia**: Alta (bloquea progreso)
```

### 2. Tarea Emergente Bloqueante
```markdown
**Ejemplo**: Bug cr�tico descubierto durante implementaci�n
**Impacto**: Imposible continuar sin resolverlo
**Urgencia**: Cr�tica (sprint no puede avanzar)
```

### 3. Deuda T�cnica Cr�tica
```markdown
**Ejemplo**: C�digo funcional pero no escalable
**Impacto**: Afecta performance o estabilidad
**Urgencia**: Media-Alta (puede causar problemas)
```

### 4. Refactorizaci�n AAA
```markdown
**Ejemplo**: C�digo funcional pero no cumple est�ndares
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

**Crear nuevo** `sprint_deviations.md` vac�o:
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

**Versi�n**: 2.0  
**�ltima actualizaci�n**: 2025-01-18  
**Proyecto**: Imagine Studio (C++ Game Engine)
