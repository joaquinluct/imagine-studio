# Workflow Detallado - Desarrollo con Asistente IA

> **Prop�sito**: Documentaci�n paso a paso del flujo de trabajo con asistentes IA.

---

## ?? CICLO COMPLETO DE ITERACI�N

### Vista General

```
????????????????
?  1. LEER     ? ? Consultar daily.md + c�digo actual
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

**Obligatorio cada sesi�n**:
```markdown
1. docs/daily.md
   ?? Ver �ltima tarea completada
   ?? Ver tarea actual en progreso

2. .github/copilot-instructions.md
   ?? Recordar reglas cr�ticas
   ?? Verificar workflow
```

**Cuando sea necesario**:
```markdown
3. docs/sprint_tasks.md
   ?? Detalles de implementaci�n de tarea actual

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
- [ ] Leer c�digo fuente de archivos relacionados
- [ ] Verificar �ltima modificaci�n (git log)
- [ ] Comprobar compilaci�n actual (si dudas)
- [ ] Identificar punto exacto donde continuar

### Ejemplo

```cpp
// Tarea actual: H1.4 - Tests unitarios Entity System
// Verificar: �Existe tests/entity_test.cpp?
// Verificar: �Est� EntityManager implementado?
// Verificar: �Hay framework de tests (Google Test)?
```

---

## ?? PASO 3: PROPONER

### Formato de Propuesta

```markdown
?? **Siguiente: H1.4 - Tests unitarios Entity System**

Implementar� tests unitarios para EntityManager con Google Test:
- Crear tests/entity_test.cpp
- Tests de CreateEntity, DestroyEntity, GetEntity
- Integrar con CMake

�Procedo?
```

**Ejecuci�n autom�tica**: Si usuario confirm� previamente "auto-ejecuci�n", el asistente procede sin esperar confirmaci�n.

---

## ?? PASO 4: IMPLEMENTAR

### Proceso de Implementaci�n

1. **Leer task details** en `docs/sprint_tasks.md`:
   ```markdown
   ### H1.4 - Tests unitarios Entity System
   **Descripci�n**: Implementar tests con Google Test
   **Archivos afectados**: tests/entity_test.cpp, CMakeLists.txt
   **Criterio de �xito**: 15 tests PASSED
   ```

2. **Crear/modificar archivos**:
   - Seguir ejemplos de c�digo en task
   - Mantener estilo consistente
   - Comentar c�digo complejo

3. **Verificar includes**:
   - Project includes primero (`"..."`)
   - System includes despu�s (`<...>`)
   - Orden alfab�tico dentro de cada grupo

### Ejemplo de Implementaci�n

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

### Doble Compilaci�n Obligatoria

**Build 1 - CMake**:
```powershell
cmake --build build --config Debug
```

**Criterio de �xito**: 
```
Build succeeded.
    0 Warning(s)
    0 Error(s)
```

**Build 2 - MSBuild (Main Solution)**:
```powershell
msbuild "Imagine Studio.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /m
```

**Criterio de �xito**:
```
Build succeeded.
    0 Warning(s)
    0 Error(s)
```

### Si Compilaci�n Falla

**Proceso de correcci�n**:
1. Leer mensaje de error completo
2. Identificar archivo y l�nea del error
3. Corregir el error
4. Repetir compilaci�n desde Build 1
5. No avanzar hasta tener ambas compilaciones limpias

### Errores Comunes y Soluciones

| Error | Causa | Soluci�n |
|-------|-------|----------|
| `LNK2019: unresolved external` | Archivo no incluido en proyecto | A�adir a `.vcxproj` / `CMakeLists.txt` |
| `C2065: undeclared identifier` | Falta include | A�adir `#include` necesario |
| `C4100: unreferenced parameter` | Par�metro no usado | A�adir `(void)param;` o eliminar |
| `LNK1120: unresolved externals` | Falta implementaci�n | Implementar funci�n declarada |

---

## ?? PASO 6: COMMIT + DOCS

### Crear Commit

**Formato de mensaje**:
```
<tipo>(<�mbito>): <mensaje corto>

<mensaje largo opcional>

Refs: <issue/track-id>
```

**Tipos comunes**:
- `feat`: Nueva funcionalidad
- `fix`: Correcci�n de errores
- `docs`: Cambios en documentaci�n
- `test`: A�adir/corregir tests
- `refactor`: Refactorizaci�n
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

### Actualizar Documentaci�n

**Autom�tico tras commit**:

1. **docs/daily.md**:
   ```markdown
   Hecho: H1.4 - Tests unitarios Entity System
   Siguiente: H2.1 - Transform Component
   ```

2. **docs/sprint_commits.md**:
   A�adir entrada con hash del commit

3. **docs/sprint_tasks.md**:
   Actualizar estado de H1.4 a ? Completada

4. **README.md** (si aplica):
   Actualizar secci�n de tests o cambios relevantes

---

## ?? ITERACI�N COMPLETA

### Ejemplo Real

**Inicio**:
```
Leer daily.md: "Siguiente: H1.4 - Tests unitarios Entity System"
```

**Propuesta**:
```
Implementar� tests con Google Test para EntityManager
```

**Implementaci�n**:
```cpp
// Crear tests/entity_test.cpp
// 15 tests implementados
```

**Validaci�n**:
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
sprint_commits.md: A�adir hash abc123
```

**Reportar**:
```markdown
? Hecho: H1.4 - Tests unitarios Entity System
?? Siguiente: H2.1 - Transform Component

### ??? Visualizaci�n:
**�Algo nuevo?**: **NO** ?
**Raz�n**: Tests internos sin efecto en UI

+--------------------------------------------------------------------+
??????????????????????????????????????????????????????? 25.0%?
+--------------------------------------------------------------------+
```

---

## ? EJECUCI�N AUTOM�TICA

### Cuando NO Pausar

El asistente ejecuta autom�ticamente si:
- ? Tarea es clara y espec�fica
- ? No requiere decisi�n arquitect�nica
- ? No hay ambig�edad en la implementaci�n
- ? Usuario ha confirmado "auto-ejecuci�n" previamente

### Cuando S� Pausar

El asistente PAUSA si:
- ? Bloqueo t�cnico (no se puede continuar)
- ? Decisi�n arquitect�nica cr�tica necesaria
- ? Ambig�edad en requisitos
- ? Bug necesita validaci�n de usuario
- ? Dependencia externa no disponible

---

## ?? M�TRICAS DE ITERACI�N

### Duraci�n Estimada por Tarea

| Complejidad | Tiempo (con IA) | Tiempo (sin IA) |
|-------------|-----------------|-----------------|
| Baja | 15-30 min | 1-2 horas |
| Media | 30-60 min | 2-4 horas |
| Alta | 1-2 horas | 4-8 horas |

### Velocidad de Sprint

**Ejemplo Sprint v1.9.0**:
- 20 tareas totales
- Duraci�n estimada: 20-40 horas (con IA)
- Duraci�n tradicional: 80-120 horas (sin IA)
- **Aceleraci�n**: 4-6x m�s r�pido

---

## ?? DOCUMENTOS RELACIONADOS

- **CORE**: [`docs/methodology/CORE.md`](CORE.md) - Fundamentos
- **Bugs**: [`docs/methodology/BUGS.md`](BUGS.md) - Bug workflow
- **Sessions**: [`docs/methodology/SESSIONS.md`](SESSIONS.md) - IA sessions
- **Deviations**: [`docs/methodology/DEVIATIONS.md`](DEVIATIONS.md) - Sprint deviations

---

**Versi�n**: 2.0  
**�ltima actualizaci�n**: 2025-01-18  
**Proyecto**: Imagine Studio (C++ Game Engine)
