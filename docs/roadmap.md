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
