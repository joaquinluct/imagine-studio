# Workflow Detallado - Desarrollo con Asistente IA

> **Propósito**: Documentación paso a paso del flujo de trabajo con asistentes IA.

---

## ?? CICLO COMPLETO DE ITERACIÓN

### Vista General

```
????????????????
?  1. LEER     ? ? Consultar daily.md + código actual
????????????????
       ?
????????????????
? 2. COMPARAR  ? ? Entender estado del proyecto
????????????????
       ?
????????????????
? 3. PROPONER  ? ? Asistente propone siguiente paso
????????????????
       ?
????????????????
?4. IMPLEMENTAR? ? Codificar cambio
????????????????
       ?
????????????????
? 5. VALIDAR   ? ? Compilar/ejecutar/probar (2x builds)
????????????????
       ?
????????????????
?6. COMMIT+DOCS? ? Git commit + actualizar markdown
????????????????
       ?
    REPETIR
```

---

## ?? PASO 1: LEER

### Archivos a Consultar

**Obligatorio cada sesión**:
```markdown
1. docs/daily.md
   ?? Ver última tarea completada
   ?? Ver tarea actual en progreso

2. .github/copilot-instructions.md
   ?? Recordar reglas críticas
   ?? Verificar workflow
```

**Cuando sea necesario**:
```markdown
3. docs/sprint_tasks.md
   ?? Detalles de implementación de tarea actual

4. docs/sprint_histories.md
   ?? Contexto de la historia asociada

5. docs/sprint.md
   ?? Objetivo general del sprint
```

### Formato Esperado en daily.md

```markdown
# Daily Log

Hecho: H1.3 - EntityManager implementado
Siguiente: H1.4 - Tests unitarios Entity System
```

---

## ?? PASO 2: COMPARAR

### Verificar Estado Actual

**Checklist**:
- [ ] Leer código fuente de archivos relacionados
- [ ] Verificar última modificación (git log)
- [ ] Comprobar compilación actual (si dudas)
- [ ] Identificar punto exacto donde continuar

### Ejemplo

```cpp
// Tarea actual: H1.4 - Tests unitarios Entity System
// Verificar: ¿Existe tests/entity_test.cpp?
// Verificar: ¿Está EntityManager implementado?
// Verificar: ¿Hay framework de tests (Google Test)?
```

---

## ?? PASO 3: PROPONER

### Formato de Propuesta

```markdown
?? **Siguiente: H1.4 - Tests unitarios Entity System**

Implementaré tests unitarios para EntityManager con Google Test:
- Crear tests/entity_test.cpp
- Tests de CreateEntity, DestroyEntity, GetEntity
- Integrar con CMake

¿Procedo?
```

**Ejecución automática**: Si usuario confirmó previamente "auto-ejecución", el asistente procede sin esperar confirmación.

---

## ?? PASO 4: IMPLEMENTAR

### Proceso de Implementación

1. **Leer task details** en `docs/sprint_tasks.md`:
   ```markdown
   ### H1.4 - Tests unitarios Entity System
   **Descripción**: Implementar tests con Google Test
   **Archivos afectados**: tests/entity_test.cpp, CMakeLists.txt
   **Criterio de éxito**: 15 tests PASSED
   ```

2. **Crear/modificar archivos**:
   - Seguir ejemplos de código en task
   - Mantener estilo consistente
   - Comentar código complejo

3. **Verificar includes**:
   - Project includes primero (`"..."`)
   - System includes después (`<...>`)
   - Orden alfabético dentro de cada grupo

### Ejemplo de Implementación

```cpp
// tests/entity_test.cpp
#include "scene/Entity.h"
#include "scene/EntityManager.h"
#include <gtest/gtest.h>

TEST(EntityTest, CreateEntity) {
    Scene::EntityManager manager;
    Scene::Entity* entity = manager.CreateEntity("TestEntity");
    
    ASSERT_NE(entity, nullptr);
    EXPECT_EQ(entity->GetName(), "TestEntity");
}
```

---

## ? PASO 5: VALIDAR (OBLIGATORIO)

### Doble Compilación Obligatoria

**Build 1 - CMake**:
```powershell
cmake --build build --config Debug
```

**Criterio de éxito**: 
```
Build succeeded.
    0 Warning(s)
    0 Error(s)
```

**Build 2 - MSBuild (Main Solution)**:
```powershell
msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /m
```

**Criterio de éxito**:
```
Build succeeded.
    0 Warning(s)
    0 Error(s)
```

### Si Compilación Falla

**Proceso de corrección**:
1. Leer mensaje de error completo
2. Identificar archivo y línea del error
3. Corregir el error
4. Repetir compilación desde Build 1
5. No avanzar hasta tener ambas compilaciones limpias

### Errores Comunes y Soluciones

| Error | Causa | Solución |
|-------|-------|----------|
| `LNK2019: unresolved external` | Archivo no incluido en proyecto | Añadir a `.vcxproj` / `CMakeLists.txt` |
| `C2065: undeclared identifier` | Falta include | Añadir `#include` necesario |
| `C4100: unreferenced parameter` | Parámetro no usado | Añadir `(void)param;` o eliminar |
| `LNK1120: unresolved externals` | Falta implementación | Implementar función declarada |

---

## ?? PASO 6: COMMIT + DOCS

### Crear Commit

**Formato de mensaje**:
```
<tipo>(<ámbito>): <mensaje corto>

<mensaje largo opcional>

Refs: <issue/track-id>
```

**Tipos comunes**:
- `feat`: Nueva funcionalidad
- `fix`: Corrección de errores
- `docs`: Cambios en documentación
- `test`: Añadir/corregir tests
- `refactor`: Refactorización
- `perf`: Mejoras de rendimiento

**Ejemplo**:
```
test(scene): implementar tests Entity System (H1.4)

Crear tests/entity_test.cpp con Google Test:
- 15 tests para EntityManager (CreateEntity, DestroyEntity, GetEntity)
- Integrado con CMakeLists.txt
- Todos los tests pasan (15/15 PASSED)

Refs: H1.4
```

### Ejecutar Commit

```powershell
git add -A
git commit -m "test(scene): implementar tests Entity System (H1.4)"
```

### Actualizar Documentación

**Automático tras commit**:

1. **docs/daily.md**:
   ```markdown
   Hecho: H1.4 - Tests unitarios Entity System
   Siguiente: H2.1 - Transform Component
   ```

2. **docs/sprint_commits.md**:
   Añadir entrada con hash del commit

3. **docs/sprint_tasks.md**:
   Actualizar estado de H1.4 a ? Completada

4. **README.md** (si aplica):
   Actualizar sección de tests o cambios relevantes

---

## ?? ITERACIÓN COMPLETA

### Ejemplo Real

**Inicio**:
```
Leer daily.md: "Siguiente: H1.4 - Tests unitarios Entity System"
```

**Propuesta**:
```
Implementaré tests con Google Test para EntityManager
```

**Implementación**:
```cpp
// Crear tests/entity_test.cpp
// 15 tests implementados
```

**Validación**:
```
Build 1 (CMake): ? 0 errors, 0 warnings
Build 2 (MSBuild): ? 0 errors, 0 warnings
Ejecutar tests: ? 15/15 PASSED
```

**Commit**:
```
git commit -m "test(scene): implementar tests Entity System (H1.4)"
```

**Actualizar docs**:
```markdown
daily.md: Hecho H1.4, Siguiente H2.1
sprint_tasks.md: H1.4 ? Completada
sprint_commits.md: Añadir hash abc123
```

**Reportar**:
```markdown
? Hecho: H1.4 - Tests unitarios Entity System
?? Siguiente: H2.1 - Transform Component

### ??? Visualización:
**¿Algo nuevo?**: **NO** ?
**Razón**: Tests internos sin efecto en UI

+--------------------------------------------------------------------+
??????????????????????????????????????????????????????? 25.0%?
+--------------------------------------------------------------------+
```

---

## ? EJECUCIÓN AUTOMÁTICA

### Cuando NO Pausar

El asistente ejecuta automáticamente si:
- ? Tarea es clara y específica
- ? No requiere decisión arquitectónica
- ? No hay ambigüedad en la implementación
- ? Usuario ha confirmado "auto-ejecución" previamente

### Cuando SÍ Pausar

El asistente PAUSA si:
- ? Bloqueo técnico (no se puede continuar)
- ? Decisión arquitectónica crítica necesaria
- ? Ambigüedad en requisitos
- ? Bug necesita validación de usuario
- ? Dependencia externa no disponible

---

## ?? MÉTRICAS DE ITERACIÓN

### Duración Estimada por Tarea

| Complejidad | Tiempo (con IA) | Tiempo (sin IA) |
|-------------|-----------------|-----------------|
| Baja | 15-30 min | 1-2 horas |
| Media | 30-60 min | 2-4 horas |
| Alta | 1-2 horas | 4-8 horas |

### Velocidad de Sprint

**Ejemplo Sprint v1.9.0**:
- 20 tareas totales
- Duración estimada: 20-40 horas (con IA)
- Duración tradicional: 80-120 horas (sin IA)
- **Aceleración**: 4-6x más rápido

---

## ?? DOCUMENTOS RELACIONADOS

- **CORE**: [`docs/methodology/CORE.md`](CORE.md) - Fundamentos
- **Bugs**: [`docs/methodology/BUGS.md`](BUGS.md) - Bug workflow
- **Sessions**: [`docs/methodology/SESSIONS.md`](SESSIONS.md) - IA sessions
- **Deviations**: [`docs/methodology/DEVIATIONS.md`](DEVIATIONS.md) - Sprint deviations

---

**Versión**: 2.0  
**Última actualización**: 2025-01-18  
**Proyecto**: Imagine Studio (C++ Game Engine)
