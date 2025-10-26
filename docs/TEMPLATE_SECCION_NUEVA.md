## ?? Formato de la Explicaci�n Final de Cada Iteraci�n

### Requisito:
Al final de cada iteraci�n (cuando se informa lo realizado y el siguiente punto), la explicaci�n debe contener **obligatoriamente**:

### 1. **Dos t�tulos numerados** siguiendo el esquema del Sprint:
- **"Hecho: \<n�mero\> \<t�tulo\>"** (ej: "Hecho: 9.18 Shading/material...") que describe en breve lo completado
- **"Siguiente: \<n�mero\> \<t�tulo\>"** que describe el siguiente punto propuesto

### 2. **??? Visualizaci�n** (�Qu� cambia visualmente al ejecutar?):

**Formato**: Indica claramente si hay cambios visuales tras la ejecuci�n y qu� se deber�a ver

**Ejemplo**:
```
### ??? Visualizaci�n:

**�Algo nuevo que cambie la vista tras ejecuci�n?**: **S�** ? / **NO** ?

**Qu� deber�as ver al ejecutar** (si S�):
1. ? [PLACEHOLDER: Cambio visual 1]
2. ? [PLACEHOLDER: Cambio visual 2]
3. ? [PLACEHOLDER: Nuevo componente UI visible]

**Qu� ha cambiado** (si aplica):
- ?? [PLACEHOLDER: Componente modificado]
- ?? [PLACEHOLDER: Comportamiento actualizado]

**Ausente** (si aplica):
- ? [PLACEHOLDER: Elemento que ya no se muestra]
- ? [PLACEHOLDER: Feature deshabilitada]
```

**Especificaciones**:
- La secci�n debe indicar claramente si hay cambios visuales con **S�** ? o **NO** ?
- Si es **S�**: Listar qu� se deber�a ver al ejecutar la aplicaci�n (F5, `npm run dev`, etc.)
- Incluir qu� es **nuevo**, qu� ha **cambiado** y qu� est� **ausente**
- Usar emojis para claridad visual:
  - ? = Nuevo/Visible
  - ?? = Modificado/Actualizado
  - ? = Ausente/Deshabilitado
- Esta secci�n debe aparecer **ANTES** de la barra de progreso del sprint

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
- El porcentaje se calcula como: `(tareas completadas / total tareas sprint) � 100`
- Usar emojis/caracteres Unicode:
  - ?? (verde) = tarea completada
  - ? (negro) = tarea pendiente
- Incluir el porcentaje en formato `"XX.X%"` al final de la barra

**Datos para el c�lculo**:
El asistente rellenar� los n�meros y calcular� el progreso gui�ndose por:
- `docs/sprint.md`
- `docs/sprint_tasks.md`
- `docs/daily.md`
