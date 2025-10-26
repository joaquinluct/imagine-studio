# Sprint Archives - Hist�rico de Sprints Completados

Esta carpeta contiene los archivos versionados de **sprints completados** del proyecto Imagine Studio.

---

## ?? Prop�sito

Al finalizar un sprint (release), **todos** los archivos activos del sprint se mueven a esta carpeta con versionado (`_vX.Y.Z`).

Esto permite:
- ? **Trazabilidad hist�rica**: Consultar decisiones de sprints pasados
- ? **Organizaci�n**: Mantener `docs/` limpio con solo archivos activos
- ? **Auditor�a**: Revisar evoluci�n del proyecto sprint a sprint
- ? **Aprendizaje**: Analizar qu� funcion� y qu� no en sprints anteriores

---

## ?? Estructura

```
docs/sprints/
??? sprint_v1.0.0.md
??? sprint_histories_v1.0.0.md
??? sprint_tasks_v1.0.0.md
??? sprint_bugs_v1.0.0.md (si hab�a bugs pendientes)
??? sprint_bug_attempts_v1.0.0.md (si hubo debugging)
??? sprint_fix_v1.0.0.md (si se resolvieron bugs)
??? sprint_deviations_v1.0.0.md (si hubo desviaciones)
??? sprint_ia_sessions_v1.0.0.md (si se registraron sesiones IA)
??? sprint_v1.1.0.md
??? sprint_histories_v1.1.0.md
??? ...
??? sprint_ia_sessions_v1.3.0.md
```

---

## ?? Proceso de Archivado (al finalizar sprint)

**Autom�tico**: El asistente IA ejecuta estos pasos al cerrar un sprint:

```powershell
# 1. Crear carpeta si no existe
New-Item -ItemType Directory -Force -Path "docs/sprints"

# 2. Mover archivos versionados
Move-Item "docs/sprint_v<version>.md" "docs/sprints/"
Move-Item "docs/sprint_histories_v<version>.md" "docs/sprints/"
Move-Item "docs/sprint_tasks_v<version>.md" "docs/sprints/"
Move-Item "docs/sprint_bugs_v<version>.md" "docs/sprints/" # Si existe
Move-Item "docs/sprint_bug_attempts_v<version>.md" "docs/sprints/" # Si existe
Move-Item "docs/sprint_fix_v<version>.md" "docs/sprints/" # Si existe
Move-Item "docs/sprint_deviations_v<version>.md" "docs/sprints/" # Si existe
Move-Item "docs/sprint_ia_sessions_v<version>.md" "docs/sprints/" # Si existe

# 3. Crear nuevos archivos vac�os para pr�ximo sprint
# (se hace en el momento de planificaci�n del siguiente sprint)
```

---

## ?? Archivos Incluidos por Sprint

Cada sprint archivado incluye (si fueron utilizados):

| Archivo | Contenido | Siempre Presente |
|---------|-----------|------------------|
| `sprint_vX.Y.Z.md` | Sprint de alto nivel (objetivos, hitos) | ? S� |
| `sprint_histories_vX.Y.Z.md` | Historias de usuario | ? S� |
| `sprint_tasks_vX.Y.Z.md` | Tareas detalladas | ? S� |
| `sprint_bugs_vX.Y.Z.md` | Bugs pendientes al cerrar | ?? Si quedaron bugs sin resolver |
| `sprint_bug_attempts_vX.Y.Z.md` | Intentos de soluci�n a bugs | ?? Si hubo debugging |
| `sprint_fix_vX.Y.Z.md` | Bugs resueltos | ?? Si se resolvieron bugs |
| `sprint_deviations_vX.Y.Z.md` | Desviaciones arquitect�nicas | ?? Si hubo ajustes emergentes |
| `sprint_ia_sessions_vX.Y.Z.md` | Sesiones IA registradas | ?? Si se alcanz� 85% tokens |

---

## ?? C�mo Consultar Hist�ricos

### Por Versi�n:
```bash
# Ver todos los archivos del sprint v1.2.0
ls docs/sprints/*_v1.2.0.md
```

### Por Tipo de Archivo:
```bash
# Ver todos los sprints de alto nivel
ls docs/sprints/sprint_v*.md

# Ver todas las desviaciones hist�ricas
ls docs/sprints/sprint_deviations_v*.md
```

### Buscar Decisi�n Espec�fica:
```bash
# Buscar "DockSpace" en todos los sprints
grep -r "DockSpace" docs/sprints/
```

---

## ?? Notas Importantes

- **NO modificar** archivos en esta carpeta (son hist�ricos inmutables)
- **NO eliminar** archivos de sprints completados (trazabilidad del proyecto)
- Si necesitas **consultar** decisiones pasadas, busca en el archivo correspondiente
- Los archivos **activos** del sprint actual est�n en `docs/` (ra�z)

---

## ?? Sprints Archivados

**Total de sprints completados**: 0  
**�ltimo sprint archivado**: -  
**Pr�ximo sprint**: v1.3.0 (ImGui Integration) - **EN PROGRESO**

---

**�ltima actualizaci�n**: 2025-01-18  
**Versi�n de la metodolog�a**: 1.0
