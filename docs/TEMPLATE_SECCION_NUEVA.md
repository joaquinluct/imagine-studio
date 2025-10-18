## ?? Formato de la Explicación Final de Cada Iteración

### Requisito:
Al final de cada iteración (cuando se informa lo realizado y el siguiente punto), la explicación debe contener **obligatoriamente**:

### 1. **Dos títulos numerados** siguiendo el esquema del Sprint:
- **"Hecho: \<número\> \<título\>"** (ej: "Hecho: 9.18 Shading/material...") que describe en breve lo completado
- **"Siguiente: \<número\> \<título\>"** que describe el siguiente punto propuesto

### 2. **??? Visualización** (¿Qué cambia visualmente al ejecutar?):

**Formato**: Indica claramente si hay cambios visuales tras la ejecución y qué se debería ver

**Ejemplo**:
```
### ??? Visualización:

**¿Algo nuevo que cambie la vista tras ejecución?**: **SÍ** ? / **NO** ?

**Qué deberías ver al ejecutar** (si SÍ):
1. ? [PLACEHOLDER: Cambio visual 1]
2. ? [PLACEHOLDER: Cambio visual 2]
3. ? [PLACEHOLDER: Nuevo componente UI visible]

**Qué ha cambiado** (si aplica):
- ?? [PLACEHOLDER: Componente modificado]
- ?? [PLACEHOLDER: Comportamiento actualizado]

**Ausente** (si aplica):
- ? [PLACEHOLDER: Elemento que ya no se muestra]
- ? [PLACEHOLDER: Feature deshabilitada]
```

**Especificaciones**:
- La sección debe indicar claramente si hay cambios visuales con **SÍ** ? o **NO** ?
- Si es **SÍ**: Listar qué se debería ver al ejecutar la aplicación (F5, `npm run dev`, etc.)
- Incluir qué es **nuevo**, qué ha **cambiado** y qué está **ausente**
- Usar emojis para claridad visual:
  - ? = Nuevo/Visible
  - ?? = Modificado/Actualizado
  - ? = Ausente/Deshabilitado
- Esta sección debe aparecer **ANTES** de la barra de progreso del sprint

### 3. **Barra de progreso visual del sprint** mostrando el avance de tareas completadas:

**Formato**: Barra horizontal con fondo negro (?), progreso verde (??), bordes blancos (?), y porcentaje centrado

**Ejemplo** con 2 de 15 tareas (13.3%):
```
??????????????????????????????????????????????????????????????????????
????????????????????????????????????????????????????? 13.3%?
??????????????????????????????????????????????????????????????????????
```

**Especificaciones**:
- La barra debe ocupar todo el ancho disponible del prompt (aprox. 70-80 caracteres)
- El porcentaje se calcula como: `(tareas completadas / total tareas sprint) × 100`
- Usar emojis/caracteres Unicode:
  - ?? (verde) = tarea completada
  - ? (negro) = tarea pendiente
- Incluir el porcentaje en formato `"XX.X%"` al final de la barra

**Datos para el cálculo**:
El asistente rellenará los números y calculará el progreso guiándose por:
- `docs/sprint.md`
- `docs/sprint_tasks.md`
- `docs/daily.md`
