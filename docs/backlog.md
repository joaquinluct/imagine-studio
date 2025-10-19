# Backlog (baúl)

> **Referencia**: Ver [`docs/MAIN.md`](MAIN.md) para los pilares fundamentales del proyecto.

Este fichero contiene, de forma persistente, todos los ítems que NO están en el sprint activo y que quedan para futuras iteraciones.

**Estado actual del proyecto** (2025-01-18):
- ✅ Sprint v1.6.0 CERRADO - Viewport AAA (Offscreen Render Target) completado
- ✅ Sprint v1.7.0 COMPLETADO - Performance Optimization (Frame Pipelining + Barrier Batching + Deferred Release Queue)
- 🎯 **Próximo sprint recomendado**: v1.8.0 - Scene Graph & Entity System

---

## Formato

Lista de elementos con:
- **ID**: Identificador único
- **Prioridad**: Baja/Media/Alta/Crítica
- **Descripción**: Breve descripción del ítem
- **Referencia**: Enlace a historia/tarea si procede
- **Estado**: Propuesto / En Planificación / Completado

---

## 🎯 Próximo Sprint Propuesto

### **SPRINT-v1.8.0 - Scene Graph & Entity System**
**Prioridad**: ⭐ **CRÍTICA** (siguiente sprint inmediato)  
**Descripción**: Implementar sistema de escena con entities, components y jerarquía real. Base fundamental para que Hierarchy/Inspector sean 100% funcionales (no placeholder).

**Objetivo**: Entity-Component System (ECS) básico con transform hierarchy.

**Historias de usuario**:
1. H1: Entity System Core (Entity class, Component base, Entity Manager, Testing)
2. H2: Transform Component (Position/Rotation/Scale, matriz local→world, jerarquía padre-hijo)
3. H3: Scene Graph Integration (Scene class, Add/Remove entities, Update hierarchy, Renderizado)
4. H4: Editor Integration (Conectar Hierarchy, Sistema de selección, Inspector real, Create/Delete entities)

**Duración estimada**: ~16 tareas (similar a v1.3.0)

**Beneficios**:
- ✅ Hierarchy funcional (no placeholder)
- ✅ Inspector edita objetos reales
- ✅ Crear/eliminar/manipular entities en tiempo real
- ✅ Base para Viewport real, Asset System, etc.

**Flujo de trabajo resultante**:
```
Usuario crea cube en Hierarchy → Aparece en scene graph → 
Se renderiza en 3D → Click en Hierarchy → Inspector muestra transform → 
Usuario edita position → Cube se mueve en 3D
```

**Prerequisitos**: Sprint v1.7.0 completado ✅

---

## 📦 Ítems en Backlog (Ordenados por Prioridad)

### **✅ COMPLETADOS**

#### **BACK-008: Viewport AAA - Offscreen Render Target** ✅ **COMPLETADO**
**Estado**: ✅ **RESUELTO en Sprint v1.6.0** (2025-01-18)  
**Prioridad**: Alta → ✅ Completado  

**Problema resuelto**:
- ❌ Viewport mostraba recursión visual (UI dentro de UI infinitamente)
- ❌ OpaquePass y UIPass usaban el mismo swap chain back buffer

**Solución implementada**:
1. ✅ Creado scene render target offscreen (separado del swap chain)
2. ✅ OpaquePass renderiza geometría 3D → scene RT
3. ✅ Transición scene RT: RENDER_TARGET → PIXEL_SHADER_RESOURCE
4. ✅ UIPass renderiza UI → swap chain back buffer (clear primero)
5. ✅ `ImGui::Image()` en Viewport lee scene RT (NO swap chain)
6. ✅ Sin recursión: scene RT solo contiene 3D, swap chain solo contiene UI

**Archivos modificados**:
- `src/renderer/DX12Renderer.h` (+4 miembros: m_sceneRenderTarget, RTVs, SRVs)
- `src/renderer/DX12Renderer.cpp` (CreateSceneRenderTarget, refactor OpaquePass/UIPass)
- `src/main.cpp` (SetRenderTargetSRV con scene SRV)

**Commits**: `85f190d`, `46fb951`, `2723e30`  
**Referencia**: `docs/sprints/sprint_v1.6.0.md`

---

### **🔴 ALTA PRIORIDAD** (Próximos Sprints)

#### **BACK-004: Scene Graph & Entity System** 🎯
**ID**: BACK-004  
**Prioridad**: ⭐ Crítica  
**Estado**: 🎯 **Propuesto para Sprint v1.8.0**

**Descripción**: Sistema completo de Scene Graph con Entity-Component architecture. Reemplaza placeholders de Hierarchy/Inspector por sistema real funcional.

**Componentes principales**:
- Entity class (ID único, nombre, componentes, parent/children)
- Component base class (Transform, Mesh, Light, Camera)
- EntityManager (crear/destruir entities, búsqueda, iteración)
- Transform Component (position, rotation, scale, hierarchy padre-hijo)
- Scene class (root entity, Add/Remove, Update, Serialize)
- Editor integration (selección, drag&drop, gizmos básicos)

**Beneficios**:
- ✅ Hierarchy 100% funcional (crear/eliminar/reordenar entities)
- ✅ Inspector edita transform real
- ✅ Viewport muestra objetos con transform hierarchy
- ✅ Base para Asset System, Materials, Lighting

**Prerequisitos**:
- Sprint v1.7.0 completado ✅

**Complejidad**: Alta (sistema complejo pero crítico)

**Archivos afectados estimados**:
- `src/scene/Entity.h/cpp` (nuevo)
- `src/scene/Component.h/cpp` (nuevo)
- `src/scene/EntityManager.h/cpp` (nuevo)
- `src/scene/Transform.h/cpp` (nuevo)
- `src/scene/Scene.h/cpp` (actualizar)
- `src/editor/EditorUI.cpp` (conectar Hierarchy/Inspector)

**Referencia**: Crítico para avanzar el proyecto. Sin Scene Graph, el editor es solo placeholder.

---

#### **BACK-005: Asset System & Resource Management** 📦
**ID**: BACK-005  
**Prioridad**: Alta  
**Estado**: Propuesto (Sprint v1.9.0+)

**Descripción**: Sistema completo de assets para cargar contenido real (texturas, meshes, shaders) desde disco. Actualmente el proyecto usa geometría hardcoded y sin texturas.

**Componentes principales**:
- Asset Database (tracking de archivos en disco, metadatos, dependencies)
- Asset Importers:
  - Texture Importer (PNG/JPG/TGA → DX12 texture, mipmaps)
  - Mesh Importer (OBJ/FBX → vertex/index buffers)
  - Shader Importer (HLSL → compiled bytecode)
- Asset Browser panel en editor (navegación de carpetas, preview)
- Drag & drop de assets al Viewport/Inspector
- Serialización de escenas (save/load .scene files con JSON/binary)
- Asset hot-reload (detectar cambios en disco y recargar)

**Beneficios**:
- Cargar contenido real en el engine (no hardcoded)
- Flujo de trabajo completo (importar → usar → guardar)
- Base para asset pipeline profesional (build system, compression)

**Prerequisitos**:
- Sprint v1.8.0 completado (Scene Graph para instanciar assets)

**Complejidad**: Alta (muchos subsistemas interdependientes)

**Archivos afectados estimados**:
- `src/assets/AssetDatabase.h/cpp` (nuevo)
- `src/assets/TextureImporter.h/cpp` (nuevo)
- `src/assets/MeshImporter.h/cpp` (nuevo)
- `src/assets/ShaderImporter.h/cpp` (nuevo)
- `src/editor/AssetBrowser.h/cpp` (nuevo)
- `src/scene/SceneSerializer.h/cpp` (nuevo)

**Referencia**: Diferido a Sprint v1.9.0 (requiere Scene Graph primero)

---

#### **BACK-006: Advanced Rendering Features** 🎨
**ID**: BACK-006  
**Prioridad**: Alta  
**Estado**: Propuesto (Sprint v1.10.0+)

**Descripción**: Mejorar calidad visual del render 3D con features AAA (PBR materials, lighting, shadows, post-processing).

**Componentes principales**:
- **PBR Materials**: Albedo, Metallic, Roughness, Normal maps, AO maps
- **Lighting System**:
  - Directional Light (sun)
  - Point Light (bombilla)
  - Spot Light (linterna)
  - Multiple lights (forward+ rendering o deferred)
- **Shadows**:
  - Shadow Mapping (directional + point lights)
  - Cascaded Shadow Maps (CSM) para mejor calidad
  - Soft shadows (PCF filtering)
- **Post-Processing**:
  - Bloom (glow en zonas brillantes)
  - Tone Mapping (HDR → LDR)
  - FXAA/TAA (anti-aliasing)
  - Color Grading (LUT)
- **Environment**:
  - Skybox (cubemap)
  - Image-Based Lighting (IBL) con environment maps
  - Fog (atmospheric scattering básico)

**Beneficios**:
- Renderizado AAA de calidad profesional
- Escenas visualmente impresionantes
- Base para features avanzadas (GI, reflections, volumetrics)

**Prerequisitos**:
- Sprint v1.8.0 completado (Scene Graph para lights/cameras)
- Opcional: Asset System (v1.9.0) para cargar texturas PBR

**Complejidad**: Muy Alta (múltiples sistemas complejos)

**Archivos afectados estimados**:
- `src/renderer/Material.h/cpp` (refactorizar para PBR)
- `src/renderer/Light.h/cpp` (nuevo - directional/point/spot lights)
- `src/renderer/ShadowMap.h/cpp` (nuevo - shadow mapping)
- `src/renderer/PostProcess.h/cpp` (nuevo - post-processing pipeline)
- `src/renderer/Skybox.h/cpp` (nuevo - cubemap rendering)
- `shaders/pbr.hlsl` (nuevo - PBR lighting BRDF)
- `shaders/shadow.hlsl` (nuevo - shadow mapping)
- `shaders/postprocess.hlsl` (nuevo - bloom, tone mapping, etc.)

**Referencia**: Diferido a Sprint v1.10.0+ (requiere Scene Graph)

---

### **🟡 MEDIA PRIORIDAD** (Largo Plazo)

#### **BACK-007: Console Integration (Logs Reales)** 📋
**ID**: BACK-007  
**Prioridad**: Media  
**Estado**: Propuesto (Sprint v1.11.0+)

**Descripción**: Integrar panel Console con sistema de logging real (`CORE_LOG_*`). Actualmente Console muestra logs hardcoded placeholder.

**Componentes principales**:
- Refactorizar `Log.h` para capturar mensajes (ring buffer thread-safe)
- Console muestra logs reales del sistema (sin placeholder)
- Filtros por nivel (botones: Show INFO, Show WARNING, Show ERROR)
- Búsqueda de texto en logs (search box con highlight)
- Clear button (limpiar console)
- Auto-scroll mejorado (stick to bottom, scroll up mantiene posición)
- Copy to clipboard (copiar logs seleccionados)
- Timestamps en logs (formato: `[HH:MM:SS.mmm]`)
- Dev Console (comandos ejecutables):
  - `spawn <entity>` (crear entity en escena)
  - `clear` (limpiar console)
  - `fps` (mostrar FPS counter)
  - `vsync <on|off>` (toggle vsync)

**Beneficios**:
- Console 100% funcional para debugging
- Logs reales del engine visibles en editor
- Base para sistema de comandos (dev console)

**Prerequisitos**:
- Sprint v1.8.0 completado (Scene Graph para comandos como `spawn`)

**Complejidad**: Media (principalmente UI y refactor logging)

**Archivos afectados estimados**:
- `src/core/Log.h/cpp` (refactorizar para capturar mensajes)
- `src/editor/EditorUI.cpp` (RenderConsole con logs reales)
- `src/editor/Console.h/cpp` (nuevo - console manager, comandos)

**Referencia**: Diferido a Sprint v1.11.0+ (prioridad media)

---

#### **BACK-003: Deferred Rendering (G-Buffer + Lighting Pass)** 🎯
**ID**: BACK-003  
**Prioridad**: Media  
**Estado**: Propuesto (Sprint v1.12.0+)

**Descripción**: Implementar Deferred Rendering pipeline como alternativa a Forward Rendering. Incluye G-Buffer Pass (multiple render targets para albedo, normal, depth) y Lighting Pass (combina G-Buffer con cálculos de iluminación). Permite renderizar escenas con muchas luces (100+) de forma eficiente.

**Beneficios**:
- Optimización para escenas complejas con muchas luces
- Base para efectos avanzados (SSAO, SSR, IBL)
- Usado en Unreal Engine, CRYENGINE, y otros motores AAA

**Prerequisitos**:
- Forward Rendering funcional ✅ (Sprint v1.2.0)
- Multiple Render Targets (MRT) support
- Shaders de G-Buffer y Lighting
- Opcional: Advanced Rendering (BACK-006) para aproveitar deferred

**Complejidad**: Alta

**Archivos afectados estimados**:
- `src/renderer/DX12Renderer.h` (añadir métodos RenderDeferred, GBufferPass, LightingPass)
- `src/renderer/DX12Renderer.cpp` (implementación de passes)
- `shaders/gbuffer.hlsl` (nuevo - vertex/pixel shaders para G-Buffer)
- `shaders/lighting.hlsl` (nuevo - compute/pixel shader for lighting)

**Referencia**: Diferido a Sprint v1.12.0+ (largo plazo)

---

### **🟢 BAJA PRIORIDAD** (Muy Largo Plazo)

#### **BACK-001: Custom Runtime UI Framework (Retained-Mode UI)** 🖼️
**ID**: BACK-001  
**Prioridad**: Baja  
**Estado**: Propuesto (Sprint v1.13.0+)

**Descripción**: Implementar framework de UI retained-mode para gameplay/runtime (HUD, menús, botones, etc.). Diferente a ImGui (que es para Editor/Tools). Sistema widget-based con layout declarativo (XML/JSON), event system (onClick, onHover), y rendering optimizado con DX12 pipeline.

**Objetivo**: Crear UI de gameplay AAA similar a Slate (Unreal Engine) o Unity UI Toolkit.

**Componentes principales**:
- Layout system (horizontal/vertical/grid/canvas)
- Widget library (Button, Text, Image, Panel, ScrollView)
- Event system (input routing, focus management)
- Styling system (colors, fonts, borders, padding)
- Data binding (MVP/MVVM pattern)

**Beneficios**:
- UI de gameplay con look AAA profesional
- Separación clara entre UI de Editor (ImGui) y UI de Runtime (Custom)
- Performance optimizada para 60+ FPS

**Prerequisitos**:
- Text rendering system (TTF font loading)
- Input system funcional (mouse/keyboard)

**Complejidad**: Muy Alta (framework completo)

**Archivos afectados estimados**:
- `src/ui/UIManager.h/cpp` (nuevo)
- `src/ui/Widget.h/cpp` (nuevo)
- `src/ui/Layout.h/cpp` (nuevo)
- `src/ui/Button.h/cpp` (nuevo)
- `src/ui/Text.h/cpp` (nuevo)
- `shaders/ui_runtime.hlsl` (nuevo)

**Referencia**: Diferido a Sprint v1.13.0+ (muy largo plazo)

---

#### **BACK-002: UI Theming & Styling System** 🎨
**ID**: BACK-002  
**Prioridad**: Baja  
**Estado**: Propuesto (Sprint v1.14.0+)

**Descripción**: Sistema de theming y styling para UI (tanto Editor como Runtime). CSS-like syntax para definir estilos (colors, fonts, borders, padding, margins). Soporte para temas (dark/light), custom fonts (TTF loading con FreeType/stb_truetype), y vector rendering para icons/shapes.

**Objetivo**: Permitir customización visual completa de la UI sin tocar código.

**Componentes principales**:
- Style parser (CSS-like syntax o JSON)
- Theme manager (switch between themes)
- Font manager (TTF loading, glyph atlas, SDF rendering)
- Vector renderer (SVG-like primitives: rect, circle, line, bezier)

**Beneficios**:
- Look & Feel profesional customizable
- Separación de presentación y lógica (clean code)
- Reutilización de estilos entre widgets

**Prerequisitos**:
- Custom Runtime UI completado (BACK-001)
- Font loading library integrada (FreeType o stb_truetype)

**Complejidad**: Alta

**Archivos afectados estimados**:
- `src/ui/Style.h/cpp` (nuevo)
- `src/ui/Theme.h/cpp` (nuevo)
- `src/ui/FontManager.h/cpp` (nuevo)
- `src/ui/VectorRenderer.h/cpp` (nuevo)
- `assets/themes/dark.json` (nuevo)
- `assets/themes/light.json` (nuevo)

**Referencia**: Diferido a Sprint v1.14.0+ (muy largo plazo)

---

## 📊 Resumen del Backlog (Actualizado 2025-01-18)

| ID | Nombre | Prioridad | Estado | Sprint Sugerido |
|----|--------|-----------|--------|-----------------|
| **BACK-008** | **Viewport AAA (Offscreen RT)** | Alta | ✅ **COMPLETADO v1.6.0** | ✅ Cerrado |
| **BACK-004** | **Scene Graph & Entity System** | ⭐ **CRÍTICA** | 🎯 **Propuesto** | **v1.8.0 (siguiente)** |
| BACK-005 | Asset System & Resource Management | Alta | Propuesto | v1.9.0 |
| BACK-006 | Advanced Rendering Features | Alta | Propuesto | v1.10.0 |
| BACK-007 | Console Integration (Logs Reales) | Media | Propuesto | v1.11.0 |
| BACK-003 | Deferred Rendering | Media | Propuesto | v1.12.0 |
| BACK-001 | Custom Runtime UI Framework | Baja | Propuesto | v1.13.0 |
| BACK-002 | UI Theming & Styling | Baja | Propuesto | v1.14.0 |

---

## 🎯 Próximos Pasos Recomendados

**Después de Sprint v1.7.0** (Performance Optimization completado):

1. **Sprint v1.8.0**: Scene Graph & Entity System ← **🎯 Próximo inmediato**
2. **Sprint v1.9.0**: Asset System & Resource Management
3. **Sprint v1.10.0**: Advanced Rendering Features (PBR, Shadows, Post-Processing)
4. **Sprint v1.11.0**: Console Integration (Logs Reales) + Dev Console
5. **Sprint v1.12.0**: Deferred Rendering (opcional si necesario)

**Justificación**:
- Scene Graph es **prerequisito** para casi todo lo demás (Assets, Rendering avanzado, Editor 100% funcional)
- Sin Scene Graph, los panels del editor siguen siendo placeholder (no funcionales al 100%)
- Con Scene Graph, el flujo de trabajo del editor se vuelve **profesional** (crear/editar/eliminar objetos reales)
- Performance ya está optimizada (v1.7.0), ahora el foco es **funcionalidad core**

---

## 📝 Cambios Recientes

**2025-01-18**:
- ✅ BACK-008 marcado como **COMPLETADO** (Sprint v1.6.0)
- ✅ Sprint v1.7.0 completado (Performance Optimization)
- 🎯 BACK-004 (Scene Graph) promovido a **prioridad CRÍTICA** (próximo sprint)
- 📊 Backlog reorganizado por prioridad y estado real del proyecto

---

*Nota*: El backlog se actualizará conforme se identifiquen ítems diferidos durante los sprints.

