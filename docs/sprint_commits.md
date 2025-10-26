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
- **style**: Formato, estilo, sin cambios funcionales
- **refactor**: Refactorización sin cambios en comportamiento
- **perf**: Mejoras de rendimiento
- **test**: Añadir o corregir tests
- **chore**: Tareas de mantenimiento

---

## Historial de Commits (Sprint v2.0.0)

### Commit 1 - Sprint Kickoff
**Fecha**: 2025-01-21  
**Tipo**: chore  
**Archivos**:
- `docs/sprint.md` (nuevo)
- `docs/sprint_tasks.md` (nuevo)
- `docs/sprint_histories.md` (nuevo)
- `docs/sprint_commits.md` (nuevo)
- `docs/sprints/sprint_v1.9.1/` (archivado)

**Mensaje**:
```
chore(docs): Sprint v2.0.0 kickoff - Material System (PBR)

Archivar Sprint v1.9.1:
- sprint_v1.9.1.md (CERRADO 100%)
- sprint_tasks_v1.9.1.md
- sprint_histories_v1.9.1.md
- sprint_commits_v1.9.1.md
- sprint_bugs_v1.9.1.md
- sprint_deviations_v1.9.1.md
- sprint_fix_v1.9.1.md
- sprint_ia_sessions_v1.9.1.md
- sprint_bug_attempts_v1.9.1.md

Nuevo Sprint v2.0.0:
- Objetivo: Material System PBR completo
- 5 historias, 19 tareas
- Duración estimada: 5 horas
- Texturas PBR reales disponibles (Brick, Dirt, Grass, Rock, Sand, Snow)

Refs: Sprint v2.0.0 kickoff
```

---

### Commit 2 - H1.1, H1.2, H1.3, H1.4: Historia H1 completada
**Fecha**: 2025-01-21  
**Tipo**: feat  
**Archivos**:
- `src/materials/Material.h` (nuevo)
- `src/materials/Material.cpp` (nuevo)
- `src/materials/MaterialInstance.h` (nuevo)
- `src/materials/MaterialInstance.cpp` (nuevo)
- `src/materials/MaterialManager.h` (nuevo)
- `src/materials/MaterialManager.cpp` (nuevo)
- `assets/textures/pbr/brick/` (6 materiales PBR copiados)
- `docs/daily.md` (actualizado)
- `.github/copilot-instructions.md` (actualizado - GIT COMMIT WORKFLOW)

**Mensaje**:
```
feat(materials): H1 completada - Material Core

Material.h, Material.cpp, MaterialInstance, MaterialManager creados
Texturas PBR copiadas (Brick, Dirt, Grass, Rock, Sand, Snow)
Compilacion limpia: 0 errores, 0 warnings

Refs: H1 Sprint v2.0.0
```

**Detalles**:

**H1.1: Material.h creado**
- MaterialProperties struct (albedo RGBA, metallic, roughness, padding)
- Material class con 5 texture slots (Albedo, Normal, Roughness, Metallic, AO)
- Setters: SetAlbedoTexture/Normal/Roughness/Metallic/AO (AssetID)
- Setters: SetAlbedoColor, SetMetallic, SetRoughness (clamped 0.0-1.0)
- Getters: GetProperties, GetTexture, HasTexture
- Default values: albedo (1,1,1,1), metallic 0.0, roughness 0.5

**H1.2: Material.cpp implementado**
- Constructor con logging (CORE_LOG_INFO)
- SetTexture methods con AssetID tracking
- SetProperties con clamping (std::max/std::min)
- IsValid() validation (nombre no vacío)
- Logging automático en Console panel

**H1.3: MaterialInstance creado**
- Hereda de Material (comparte shader)
- Constructor copia propiedades y texturas de base Material
- IsValid() override (requiere base Material válido)
- GetBaseMaterial() accessor

**H1.4: MaterialManager singleton creado**
- Meyer's Singleton pattern
- Thread-safe con std::mutex
- CreateMaterial() y CreateMaterialInstance()
- GetMaterial(), HasMaterial(), RemoveMaterial()
- GetAllMaterials() para iteración
- ClearAll() cleanup
- ReloadMaterial() y CheckForChanges() placeholders (H5)

**Texturas PBR copiadas**:
- 6 materiales: Brick, Dirt, Grass, Rock, Sand, Snow
- 5 texturas por material: BaseColor, Normal, Roughness, Metallic, AO
- Estructura: assets/textures/pbr/<material>/<textures>

**Compilación**: 0 errores, 0 warnings (CMake + MSBuild)

**HISTORIA H1 COMPLETADA** (Material Core - 4/4 tareas) ?  
Primera historia del Sprint v2.0.0 completada (21% del sprint)

---

## ?? Estadísticas del Sprint

**Total commits**: 2  
**Historias completadas**: 1/5 (? H1)  
**Tareas completadas**: 4/19 (21%)  
**Progreso sprint**: 21%

### Desglose por tipo
- **chore**: 1 commit (50%)
- **feat**: 1 commit (50%)

### Compilación
- **CMake builds**: 2/2 exitosos ?
- **MSBuild builds**: 2/2 exitosos ?
- **Errores**: 0
- **Warnings**: 0

---

**Versión**: v2.0.0  
**Última actualización**: 2025-01-21  
**Sprint**: v2.0.0 - Material System (PBR) - **EN PROGRESO** (21%)  
**Historias completadas**: 1/5 (? H1)  
**Próximo objetivo**: H2.1 - Crear PBR vertex shader (HLSL)
