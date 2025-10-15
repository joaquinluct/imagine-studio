# Roadmap profesional - Framework Renderer AAA

Objetivo final:
Crear un framework renderer modular (estilo Unity) orientado a proyectos 3D AAA, con soporte inicial para DirectX12 y abstracción para añadir DX11/Vulkan/OpenGL posteriormente. Debe incluir sistemas AAA clave: multihilo, streaming de assets, ring-buffers para comandos de GPU, pipeline de render, UI layer, y herramientas para depuración y profiling.

Fases (alto nivel):

1. Estructura y documentación inicial (esta fase)
2. Sistema de build y proyecto, codificación con estándares C++14 (headers/impl, namespaces, CMake/msbuild)
3. Sistema de plataforma/abstracción (Windowing + input)
4. Backend de render inicial - DirectX12 minimal
5. Command buffers y sincronización (ring-buffers, fences, command allocators)
6. Resource management y asset streaming (vfs, async load, pools)
7. Multithreading y job system (thread pool, task graph)
8. Simple UI layer + text rendering (panel + "Hola Mundo")
9. Shading/material system y pipeline state management
10. Tools: profiler, debug overlays, capture
11. Adapter layers para DX11/Vulkan/OpenGL
12. Polishing, examples, sample scenes, tests

Primera milestone práctica:
- Crear estructura de proyecto y documentación.
- Implementar una aplicación Windows básica que cree una ventana y un loop.
- Añadir la estructura de módulos: core, platform, renderer, ui, assets, jobs.

Criterios de aceptación AAA:
- Código limpio, modular y documentado.
- Tests básicos o puntos de chequeo en cada módulo.
- Estándares de estilo aplicados (naming, include guards, etc.).

Editor y objetivo de exportación (Editor + Runtime)
-----------------------------------------------
Objetivo adicional (Editor): el framework debe diseñarse desde el inicio para soportar un Editor integrado (similar a Unity) y un Runtime independiente que pueda ser exportado como ejecutable.

Requisitos clave para el Editor y exportación:
- Editor MVP: vista de escena, inspección de propiedades, jerarquía de objetos y proyecto/asset window.
- Importadores y pipeline de assets: importadores para modelos, texturas, escenas y un sistema de packaging para assets.
- Separación clara Editor vs Runtime: APIs y módulos deben permitir compilar/ejecutar el Runtime sin dependencias del Editor (build-time or runtime switches).
- Sistema de serialización y escenas: persistencia de escenas y assets con formatos versionables y optimizados para runtime.
- Pipeline de exportación (build targets): capacidad para generar builds para Windows (x64) y futuras plataformas (configurable).
- Scripting y plugins: diseño para soportar scripts/plugins (p. ej. binding de scripting later) y extensibilidad del Editor.

Nota arquitectónica:
Diseñar la arquitectura con capas claras (core, platform, renderer, ui, assets, jobs) y líneas de abstracción que permitan, por ejemplo, desacoplar el backend de render (DX12) del Editor/Runtime para poder añadir adaptadores a DX11/Vulkan/OpenGL sin reescribir la lógica de alto nivel.
