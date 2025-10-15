# Roadmap profesional - Framework Renderer AAA

Objetivo final:
Crear un framework renderer modular (estilo Unity) orientado a proyectos 3D AAA, con soporte inicial para DirectX12 y abstracci�n para a�adir DX11/Vulkan/OpenGL posteriormente. Debe incluir sistemas AAA clave: multihilo, streaming de assets, ring-buffers para comandos de GPU, pipeline de render, UI layer, y herramientas para depuraci�n y profiling.

Fases (alto nivel):

1. Estructura y documentaci�n inicial (esta fase)
2. Sistema de build y proyecto, codificaci�n con est�ndares C++14 (headers/impl, namespaces, CMake/msbuild)
3. Sistema de plataforma/abstracci�n (Windowing + input)
4. Backend de render inicial - DirectX12 minimal
5. Command buffers y sincronizaci�n (ring-buffers, fences, command allocators)
6. Resource management y asset streaming (vfs, async load, pools)
7. Multithreading y job system (thread pool, task graph)
8. Simple UI layer + text rendering (panel + "Hola Mundo")
9. Shading/material system y pipeline state management
10. Tools: profiler, debug overlays, capture
11. Adapter layers para DX11/Vulkan/OpenGL
12. Polishing, examples, sample scenes, tests

Primera milestone pr�ctica:
- Crear estructura de proyecto y documentaci�n.
- Implementar una aplicaci�n Windows b�sica que cree una ventana y un loop.
- A�adir la estructura de m�dulos: core, platform, renderer, ui, assets, jobs.

Criterios de aceptaci�n AAA:
- C�digo limpio, modular y documentado.
- Tests b�sicos o puntos de chequeo en cada m�dulo.
- Est�ndares de estilo aplicados (naming, include guards, etc.).

Editor y objetivo de exportaci�n (Editor + Runtime)
-----------------------------------------------
Objetivo adicional (Editor): el framework debe dise�arse desde el inicio para soportar un Editor integrado (similar a Unity) y un Runtime independiente que pueda ser exportado como ejecutable.

Requisitos clave para el Editor y exportaci�n:
- Editor MVP: vista de escena, inspecci�n de propiedades, jerarqu�a de objetos y proyecto/asset window.
- Importadores y pipeline de assets: importadores para modelos, texturas, escenas y un sistema de packaging para assets.
- Separaci�n clara Editor vs Runtime: APIs y m�dulos deben permitir compilar/ejecutar el Runtime sin dependencias del Editor (build-time or runtime switches).
- Sistema de serializaci�n y escenas: persistencia de escenas y assets con formatos versionables y optimizados para runtime.
- Pipeline de exportaci�n (build targets): capacidad para generar builds para Windows (x64) y futuras plataformas (configurable).
- Scripting y plugins: dise�o para soportar scripts/plugins (p. ej. binding de scripting later) y extensibilidad del Editor.

Nota arquitect�nica:
Dise�ar la arquitectura con capas claras (core, platform, renderer, ui, assets, jobs) y l�neas de abstracci�n que permitan, por ejemplo, desacoplar el backend de render (DX12) del Editor/Runtime para poder a�adir adaptadores a DX11/Vulkan/OpenGL sin reescribir la l�gica de alto nivel.
