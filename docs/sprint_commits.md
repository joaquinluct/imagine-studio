# Sprint Commits - v2.0.0 (Active)

> **Referencia**: Ver [docs/sprint.md](sprint.md) para objetivo y progreso del sprint.

Este archivo registra todos los commits realizados durante el **Sprint v2.0.0 - Material System (PBR)**.

---

## Convenciones de Commits

Formato recomendado para los mensajes de commit:

```powershell
# CORRECTO: Múltiples -m flags (no parsing issues)
git commit -m "feat(scope): Short title" `
           -m "Detail line 1" `
           -m "Detail line 2" `
           -m "Refs: H1 Sprint v2.0.0"
```

### Tipos comunes:
- **feat**: Nueva funcionalidad
- **fix**: Corrección de errores
- **docs**: Cambios en documentación
- **revert**: Revertir commit erróneo
- **style**: Formato, estilo
- **refactor**: Refactorización
- **perf**: Mejoras de rendimiento
- **test**: Tests
- **chore**: Mantenimiento

---

## Historial de Commits (Sprint v2.0.0)

### Commit 1 - Sprint Kickoff
**Fecha**: 2025-01-21  
**Tipo**: chore  

**Mensaje**:
```
chore(docs): Sprint v2.0.0 kickoff - Material System (PBR)
Archivar Sprint v1.9.1
Nuevo Sprint v2.0.0
Refs: Sprint v2.0.0 kickoff
```

---

### Commit 2 - H1 completada
**Fecha**: 2025-01-21  
**Tipo**: feat  

**Mensaje**:
```
feat(materials): H1 completada - Material Core
Material.h, Material.cpp, MaterialInstance, MaterialManager creados
Texturas PBR copiadas (Brick, Dirt, Grass, Rock, Sand, Snow)
Compilacion limpia: 0 errores, 0 warnings
Refs: H1 Sprint v2.0.0
```

**HISTORIA H1 COMPLETADA** (Material Core - 4/4 tareas) ?

---

### Commit 3 - GIT COMMIT WORKFLOW documentado
**Fecha**: 2025-01-21  
**Tipo**: docs  

**Mensaje**:
```
docs: Documentar GIT COMMIT WORKFLOW correcto
Añadida seccion critica en copilot-instructions.md
Metodo correcto: multiples -m flags
Prohibicion explicita: NO reintentar comandos fallidos
Refs: Sprint v2.0.0
```

---

### Commit 4 - [REVERTIDO] H2.1, H2.2, H2.3
**Fecha**: 2025-01-21  
**Tipo**: feat (REVERTIDO)  
**Estado**: ? REVERTIDO - Causó errores de compilación

**Mensaje original**:
```
feat(shaders): H2.1, H2.2, H2.3 completadas - PBR Shader Pipeline
pbr_vs.hlsl, pbr_ps.hlsl, MaterialConstants.h creados
NO SE VALIDÓ COMPILACIÓN ANTES DE COMMIT
```

**Problema**: Shaders no configurados correctamente en `.vcxproj`, causando errores de compilación en MSBuild.

**ESTE COMMIT FUE REVERTIDO** en Commit 5.

---

### Commit 5 - Revertir H2 erróneo
**Fecha**: 2025-01-21  
**Tipo**: revert  

**Mensaje**:
```
revert: Revertir H2.1, H2.2, H2.3 - Shaders causaban errores MSBuild
Los shaders PBR no compilaban correctamente en MSBuild
Estado restaurado: compilacion limpia 0 errores 0 warnings
Refs: Sprint v2.0.0
```

**Acción**: Revertir commit erróneo, eliminar archivos:
- `assets/shaders/pbr_vs.hlsl` (eliminado)
- `assets/shaders/pbr_ps.hlsl` (eliminado)
- `src/renderer/MaterialConstants.h` (eliminado)

**Estado restaurado**: ? Proyecto compilando limpiamente (0 errores, 0 warnings)

---

## ?? Estadísticas del Sprint

**Total commits**: 5 (1 revertido)  
**Commits válidos**: 4  
**Historias completadas**: 1/5 (? H1)  
**Tareas completadas**: 4/19 (21%)  
**Progreso sprint**: 21%

### Desglose por tipo
- **chore**: 1 commit (20%)
- **feat**: 1 commit válido (20%)
- **docs**: 1 commit (20%)
- **revert**: 1 commit (20%)
- **feat (revertido)**: 1 commit ? (20%)

### Compilación
- **Estado actual**: ? 0 errores, 0 warnings (MSBuild + CMake)
- **Commits con build limpio**: 4/5 (80%)
- **Commits con errores (revertidos)**: 1/5 (20%)

---

**Versión**: v2.0.0  
**Última actualización**: 2025-01-21  
**Sprint**: v2.0.0 - Material System (PBR) - **EN PROGRESO** (21%)  
**Historias completadas**: 1/5 (? H1)  
**Próximo objetivo**: H2.1 - Crear PBR vertex shader (REIMPLEMENTAR con validación completa)
