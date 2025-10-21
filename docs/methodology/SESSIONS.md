# IA Sessions Management - Gesti�n al 85% de Contexto

> **Prop�sito**: Sistema de gesti�n de sesiones IA cuando se alcanza el 85% de consumo de la ventana de contexto.

---

## ?? �QU� ES EL 85% DE CONTEXTO?

### Aclaraci�n Importante

**NO se refiere a**:
- ? L�mite total del modelo LLM (1M tokens)
- ? L�mite de tu plan/suscripci�n

**S� se refiere a**:
- ? Ventana de contexto **activa** de la sesi�n de chat actual
- ? T�picamente: ~128k-200k tokens (depende del modelo)

### �Por Qu� 85%?

```
0%                    85%                    100%
?????????????????????????????????????????????
?   CONTEXTO FRESCO   ?   CONTEXTO TRUNCADO ?
?   ? Coherente      ?   ?? Olvida mensajes?
?   ? R�pido         ?   ?? Lento          ?
?   ? Preciso        ?   ?? Inconsistente  ?
```

**Razones**:
- ?? **Evitar p�rdida de contexto**: Al 85%, el LLM empieza a "olvidar" mensajes antiguos
- ? **Prevenir estado 'busy'**: La sesi�n se vuelve lenta
- ?? **Garantizar coherencia**: Evita contradicciones con decisiones del inicio
- ?? **Trazabilidad completa**: Documentar prompts usados

---

## ?? FLUJO AUTOM�TICO AL 85%

### Vista General

```
???????????????????
? 1. DETECTAR 85% ? ? Sesi�n supera umbral
???????????????????
        ?
???????????????????
? 2. COMPLETAR    ? ? Terminar tarea actual
?    TAREA        ?
???????????????????
        ?
???????????????????
? 3. COMPILAR     ? ? CMake + MSBuild limpias
???????????????????
        ?
???????????????????
? 4. COMMIT       ? ? Git commit local
???????????????????
        ?
???????????????????
? 5. REGISTRAR    ? ? A�adir a sprint_ia_sessions.md
?    SESI�N       ?
???????????????????
        ?
???????????????????
? 6. NOTIFICAR    ? ? Informar al usuario
?    USUARIO      ?
???????????????????
        ?
???????????????????
? 7. PAUSAR       ? ? Esperar cierre de sesi�n
???????????????????
```

---

## ?? ARCHIVO: `docs/sprint_ia_sessions.md`

### Prop�sito
Registro de sesiones IA que superan el 85% de consumo de ventana de contexto.

### Formato de Entrada

```markdown
## Sesi�n IA #001 - Sprint v1.9.0

**LLM**: GitHub Copilot (Claude 3.5 Sonnet)
**Fecha/Hora**: 2025-01-18 14:00-18:30 UTC
**Usuario (Orquestador)**: joaquinluct
**Ventana de Contexto**: 172,000 / 200,000 (86.0%)
**Estado**: ?? L�MITE 85% ALCANZADO - Sesi�n cerrada

### Resumen de la Sesi�n:

Esta sesi�n ha sido muy productiva:

1. ? **Asset Database Core** (H1.1-H1.4)
   - AssetDatabase.h/cpp implementado
   - Asset folders creados
   - Tests unitarios (12 tests PASSED)

2. ? **Texture Importer** (H2.1-H2.3)
   - stb_image integrado
   - TextureImporter.h/cpp implementado
   - DX12 Texture2D creado desde pixel data

3. ? **Validaci�n completa**
   - Compilaci�n limpia (CMake + MSBuild)
   - Validaci�n usuario: OK 100%

### Prompts Ejecutados:

```
1. "Vamos con Sprint v1.9.0 - Asset System"
   ? Planificaci�n completa (5 historias, 20 tareas)

2. "Comenzar con H1.1 - AssetDatabase.h"
   ? Implementaci�n estructuras b�sicas

3. "Continuar con H1.2-H1.4"
   ? EntityManager + tests completos

4. "Vamos con H2 - Texture Importer"
   ? Integraci�n stb_image + DX12

[... contin�a con todos los prompts ejecutados]
```

### Contexto de la Sesi�n:

- **Sprint activo**: v1.9.0 (Asset System & Resource Management)
- **Tareas completadas en sesi�n**:
  - H1.1: AssetDatabase.h ?
  - H1.2: AssetDatabase.cpp ?
  - H1.3: Asset folders ?
  - H1.4: Tests AssetDatabase ?
  - H2.1: Integrar stb_image ?
  - H2.2: TextureImporter ?
  - H2.3: DX12 Texture2D ?

- **Tareas pendientes al cierre**:
  - H2.4: Testing TextureImporter
  - H3.1-H3.4: Mesh Importer (OBJ parser)
  - H4.1-H4.4: Asset Browser Panel
  - H5.1-H5.4: Scene Serialization

### Commits Creados:

1. `abc123d` - feat(assets): Implementar AssetDatabase core (H1.1-H1.2)
2. `def456e` - feat(assets): Crear asset folder structure (H1.3)
3. `ghi789f` - test(assets): Tests AssetDatabase (H1.4) - 12 tests PASSED
4. `jkl012g` - feat(assets): Integrar stb_image library (H2.1)
5. `mno345h` - feat(assets): TextureImporter implementation (H2.2)
6. `pqr678i` - feat(renderer): DX12 Texture2D from pixel data (H2.3)

### Archivos Creados:

**Asset System**:
- `src/assets/AssetDatabase.h/cpp`
- `src/assets/TextureImporter.h/cpp`
- `external/stb/stb_image.h`
- `tests/asset_database_test.cpp`

**Asset Folders**:
- `assets/textures/`
- `assets/meshes/`
- `assets/shaders/`
- `assets/scenes/`

### Archivos Modificados:

- `CMakeLists.txt` (a�adir asset system + stb_image)
- `Imagine Studio.vcxproj` (a�adir archivos asset)
- `src/renderer/DX12ResourceManager.h/cpp` (CreateTexture2DFromData)
- `docs/daily.md` (actualizado progreso)
- `docs/sprint_commits.md` (6 commits a�adidos)

### M�tricas de la Sesi�n:

- **Duraci�n**: 4.5 horas
- **Commits**: 6
- **Archivos creados**: 8
- **Archivos modificados**: 5
- **Tests escritos**: 12 (12 PASSED)
- **Compilaciones**: 12 (todas limpias)
- **Progreso sprint**: 0% ? 35% (+7 tareas de 20)

### Pr�xima Sesi�n (Recomendaciones):

- **Continuar desde**: H2.4 - Testing TextureImporter
- **Leer antes**:
  - `docs/daily.md` (estado actual)
  - `docs/sprint_tasks.md` (detalles H2.4)
  - `docs/methodology/CORE.md` (refrescar metodolog�a)
- **Validar**:
  - Compilaci�n limpia (CMake + MSBuild)
  - Tests de textura PNG real (assets/textures/test.png)
- **Objetivo siguiente sesi�n**:
  - Completar H2.4 (tests TextureImporter)
  - Avanzar H3.1-H3.2 (MeshImporter base)

---
```

---

## ?? NOTIFICACI�N AL USUARIO

### Formato de Notificaci�n

```markdown
?? L�MITE DE VENTANA DE CONTEXTO ALCANZADO (>85%)

? Tarea actual completada: H2.3 - DX12 Texture2D from pixel data
? Commit creado: pqr678i
? Sesi�n registrada en docs/sprint_ia_sessions.md

?? Ventana de contexto: 172,000 / 200,000 tokens (86.0%)

?? RECOMENDACI�N: Cerrar esta sesi�n y abrir nuevo prompt

**Raz�n**: Al superar el 85% de la ventana de contexto, el LLM
empieza a perder informaci�n de mensajes antiguos, lo que puede
causar inconsistencias o contradicciones.

?? Pr�xima tarea sugerida: H2.4 - Testing TextureImporter
?? Leer antes de continuar: docs/daily.md, docs/methodology/CORE.md

---

? **BENEFICIOS DE CERRAR AHORA**:
- ? Nuevo chat con contexto fresco (100% disponible)
- ? Leer recomendaciones de sesi�n anterior
- ? Continuar con claridad desde donde se dej�
- ? Evitar errores por contexto truncado
```

---

## ?? PROCESO OBLIGATORIO AL 85%

### 1. Detectar Umbral

**Condici�n**:
```
Ventana de contexto actual > 85% de l�mite
```

**Ejemplo**:
```
L�mite: 200,000 tokens
Umbral 85%: 170,000 tokens
Actual: 172,000 tokens ? ?? SUPERA 85%
```

### 2. Completar Tarea Actual

**NO interrumpir** en medio de:
- Implementaci�n de c�digo
- Compilaci�n en progreso
- Commit a medio hacer

**S� pausar** al terminar:
- ? Tarea completada
- ? Compilaci�n limpia
- ? Commit creado
- ? Docs actualizados

### 3. Registrar Sesi�n

**Crear entrada en `docs/sprint_ia_sessions.md`**:
- ID secuencial (Sesi�n #001, #002...)
- Prompt completo ejecutado
- Nombre y versi�n del LLM
- Timestamp + usuario orquestador
- Consumo de ventana (actual/l�mite/porcentaje)
- Contexto (sprint, tareas completadas/pendientes)
- Recomendaciones para pr�xima sesi�n

### 4. Notificar y Pausar

**Mostrar notificaci�n** al usuario con:
- Estado actual (tarea completada, commit)
- M�tricas de contexto
- Recomendaci�n de cerrar sesi�n
- Pr�xima tarea sugerida

**PAUSAR** y esperar que usuario cierre la sesi�n.

---

## ?? VERSIONADO

### Al Cerrar Sprint

**Archivar** `sprint_ia_sessions.md`:
```powershell
Move-Item "docs/sprint_ia_sessions_v1.9.0.md" "docs/sprints/"
```

**Crear nuevo** `sprint_ia_sessions.md` vac�o para siguiente sprint.

---

## ?? OBLIGATORIO AL FINALIZAR SPRINT

**CR�TICO**: Al cerrar un sprint (`close-sprint.ps1`), el asistente **DEBE**:

1. **Registrar sesi�n IA** en `docs/sprint_ia_sessions.md` **ANTES** de archivar
2. **Incluir TODA la informaci�n** de la sesi�n:
   - Prompts ejecutados (lista completa)
   - Tareas completadas
   - Commits creados
   - Archivos creados/modificados
   - M�tricas finales del sprint
3. **Archivar** con script: `sprint_ia_sessions_v<version>.md`

### Por Qu� es Obligatorio

- ?? **Trazabilidad completa** del desarrollo asistido por IA
- ?? **An�lisis retrospectivo** de metodolog�a efectiva
- ?? **Documentaci�n valiosa** para futuros sprints
- ?? **Auditor�a** de decisiones tomadas
- ?? **Continuidad** entre sesiones y sprints

### Excepci�n

Si el sprint **NO** tuvo sesiones IA (desarrollo manual):
```markdown
# Sprint IA Sessions - v1.9.0

*No hubo sesiones IA durante este sprint. Desarrollo realizado manualmente.*
```

---

## ?? DOCUMENTOS RELACIONADOS

- **CORE**: [`docs/methodology/CORE.md`](CORE.md) - Fundamentos
- **Workflow**: [`docs/methodology/WORKFLOW.md`](WORKFLOW.md) - Flujo general
- **Bugs**: [`docs/methodology/BUGS.md`](BUGS.md) - Bug management
- **Deviations**: [`docs/methodology/DEVIATIONS.md`](DEVIATIONS.md) - Sprint deviations

---

**Versi�n**: 2.0  
**�ltima actualizaci�n**: 2025-01-18  
**Proyecto**: Imagine Studio (C++ Game Engine)
