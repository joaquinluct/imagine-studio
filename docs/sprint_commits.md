# Sprint Commits - v1.5.0 (Viewport Real)

> **Referencia**: Ver [`docs/sprint.md`](sprint.md) para objetivo y progreso del sprint.

Este archivo registra todos los commits realizados durante el **Sprint v1.5.0 - Viewport Real (Render Target Texture)**.

---

## Convenciones de Commits

Formato recomendado para los mensajes de commit:

```
<tipo>(<ámbito>): <mensaje corto>

<mensaje largo opcional>

Refs: <issue/track-id>
```

### Tipos comunes:
- **feat**: Nueva funcionalidad
- **fix**: Corrección de errores
- **docs**: Cambios en documentación
- **style**: Formato, estilo, sin cambios funcionales
- **refactor**: Refactorización sin cambios en comportamiento
- **perf**: Mejoras de rendimiento
- **test**: Añadir o corregir tests
- **chore**: Tareas de mantenimiento

### Ejemplo:
```
feat(renderer): implementar SRV descriptor heap

Crear descriptor heap para Shader Resource Views usado por ImGui.
Configurado como SHADER_VISIBLE para acceso desde GPU.

Refs: H1.1
```

---

## Historial de Commits (Sprint v1.5.0)

### 2025-01-18

#### `77d7473` - docs: Planificación Sprint v1.5.0 - Viewport Real

**Tipo**: Docs (Planificación)  
**Ámbito**: Sprint v1.5.0  
**Descripción**: Planificación completa del Sprint v1.5.0 - Viewport Real (Render Target Texture)

**Objetivo**: Implementar Viewport Real con render 3D en tiempo real dentro del panel del editor usando `ImGui::Image()`. Incluye controles de cámara (orbit/pan/zoom) y picking de objetos.

**Historias (4)**:
- **H1**: Render Target como Texture (4 tareas)
- **H2**: Camera System (4 tareas)
- **H3**: Viewport Integration (4 tareas)
- **H4**: Picking & Selection (3 tareas)

**Total**: 15 tareas  
**Duración estimada**: 4-6 horas  
**Prioridad**: Alta (complementa v1.4.0)

**Archivos creados/modificados**:
- `docs/sprint.md`
- `docs/sprint_histories.md`
- `docs/sprint_tasks.md`
- `docs/daily.md`

**Compilación**: ✅ Limpia (CMake + MSBuild: 0 errores, 0 warnings)

**Próxima tarea**: H1.1 - Crear Descriptor Heap para SRV

**Referencia**: Sprint v1.5.0 - Viewport Real

---

## Commits por Historia

### H1 - Render Target como Texture
*Pendiente de commits*

### H2 - Camera System
*Pendiente de commits*

### H3 - Viewport Integration
*Pendiente de commits*

### H4 - Picking & Selection
*Pendiente de commits*

---

## Estadísticas del Sprint

**Commits totales**: 1  
**Features**: 0  
**Tests**: 0  
**Fixes**: 0  
**Docs**: 1  

**Progreso**: 0/15 tareas completadas (0.0%)

---

**Versión**: v1.0  
**Última actualización**: 2025-01-18  
**Sprint**: v1.5.0 (Viewport Real) - **EN PROGRESO**

---

## 📚 Histórico de Sprints Anteriores

Los commits de sprints anteriores (v1.0.0 - v1.4.0) están archivados en:
- `docs/sprints/sprint_commits_v1.0.0-1.4.0.md`