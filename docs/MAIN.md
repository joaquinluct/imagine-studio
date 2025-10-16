# Imagine Studio - Decálogo Fundamental

> **LEER PRIMERO**: Este documento define los pilares y principios fundamentales del proyecto.  
> Todas las decisiones arquitectónicas y de implementación DEBEN alinearse con estos principios.

---

## ?? Objetivo Final

**Imagine Studio** es un framework de creación de juegos tipo Unity/Roblox Studio:  
Un motor de renderizado AAA modular con Editor integrado, soporte multiplataforma y extensibilidad completa.

---

## ??? Los 4 Pilares Fundamentales

### 1. **Modo Edición/Studio**
- Interfaz rica con recursos completos para crear mundos 2D y 3D
- Editor MVP: vista de escena, inspección de propiedades, jerarquía de objetos, asset window
- Separación clara Editor vs Runtime (APIs independientes)

### 2. **Multiplataforma**
- Publicación de juegos en Windows, Mac, Consolas y otras plataformas
- Pipeline de exportación configurable (build targets)
- Abstracción de backends de render (DirectX12, DX11, Vulkan, OpenGL)

### 3. **Extensibilidad**
- Programación en modo edición/studio para personalizar el motor
- Sistema de scripting y plugins
- Importadores y pipeline de assets extensible
- Binding de scripting para usuarios finales

### 4. **Performance y Calidad AAA**
- Código de nivel profesional con performance y calidad visual como objetivo PRIMORDIAL
- Multithreading y job system (thread pool, task graph)
- Streaming de assets asíncrono (VFS, pools)
- Ring-buffers para comandos GPU, sincronización con fences
- Pipeline de render optimizado, materials y shaders
- Profiler, debug overlays, herramientas de captura

---

## ??? Arquitectura de Capas

```
???????????????????????????????????????
?          EDITOR (opcional)          ?  ? Herramientas de autor
???????????????????????????????????????
?  UI Layer + Tools (Profiler, etc)  ?  ? Interfaz y debugging
???????????????????????????????????????
?     Renderer (DX12/DX11/Vulkan)     ?  ? Backend gráfico abstracto
???????????????????????????????????????
?   Assets (VFS, streaming, pools)    ?  ? Gestión de recursos
???????????????????????????????????????
?    Jobs (ThreadPool, TaskGraph)     ?  ? Sistema de trabajos paralelos
???????????????????????????????????????
?   Platform (Window, Input, OS)      ?  ? Abstracción de sistema
???????????????????????????????????????
?   Core (Log, Assert, Types, etc)    ?  ? Fundamentos
???????????????????????????????????????
```

**Principio de abstracción**:  
Desacoplar backends (DX12) del Editor/Runtime para añadir adaptadores sin reescribir lógica de alto nivel.

---

## ?? Estándares de Calidad AAA

1. **Código limpio y modular**: Headers/impl, namespaces, documentación inline
2. **Cero warnings/errores**: Compilación limpia obligatoria antes de commits
3. **Tests y validación**: Puntos de chequeo en cada módulo
4. **Estilo consistente**: Naming conventions, include guards, orden de includes
5. **Doble verificación**: CMake + MSBuild antes de cada commit
6. **Diseño desde el principio**: Evitar partes temporales o incompletas

---

## ?? Sistemas AAA Clave Implementados

- ? **Core**: Logging, assertions, types
- ? **Platform**: Windowing, input polling
- ? **Assets**: VFS con mounts, carga asíncrona, métricas
- ? **Jobs**: ThreadPool, TaskGraph con dependencias
- ? **UI**: SimpleUI con texto, overlays de FPS/Profiler
- ? **Renderer**: Stubs DX12 (Device, RenderTarget, CommandBuffer)
- ? **Tools**: Profiler básico, métricas de assets
- ? **Materials**: Sistema de materiales básico
- ?? **Renderer DX12**: Integración real en progreso (Present, command lists)

---

## ?? Criterios de Aceptación

Cada módulo debe cumplir:
- **Funcionalidad completa**: Sin stubs permanentes ni TODOs sin resolver
- **Performance medible**: Métricas, profiling, benchmarks donde aplique
- **Documentación**: README por módulo + comentarios inline
- **Tests**: Cobertura básica o ejemplos funcionales
- **Integración limpia**: Compile y funcione con el resto del sistema

---

## ?? Próximos Hitos

- **Sprint actual**: Backend DirectX12 minimal (Present, command lists)
- **Sprints futuros**: Shading/materials, Editor MVP, exportación multiplataforma

---

**Versión**: 1.0.0  
**Estándar**: C++14  
**Última actualización**: 2025-01-XX
