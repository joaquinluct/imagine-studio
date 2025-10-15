# Convenciones de commits

Formato recomendado para los mensajes de commit:

```
<tipo>(<�mbito>): <mensaje corto>

<mensaje largo opcional>

Refs: <issue/track-id>
```

Tipos comunes:
- feat: Nueva funcionalidad
- fix: Correcci�n de errores
- docs: Cambios en documentaci�n
- style: Formato, estilo, sin cambios funcionales
- refactor: Refactorizaci�n sin cambios en comportamiento
- perf: Mejoras de rendimiento
- test: A�adir o corregir tests
- chore: Tareas de mantenimiento

Ejemplo:
```
feat(renderer): inicializa estructura base del renderer

A�ade clases b�sicas y estructura de carpetas para el renderer.
```

Historial de commits relevantes:

- feat(core): implementar m�dulo core (logging, asserts, types) - incluye `core/Log.h`, `core/Assert.h`, `core/Types.h`. (Iteraci�n inicial: estructura y compilaci�n limpia)

