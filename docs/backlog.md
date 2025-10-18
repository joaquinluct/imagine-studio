# Backlog (baúl)

> **Referencia**: Ver [`docs/MAIN.md`](MAIN.md) para los pilares fundamentales del proyecto.

Este fichero contiene, de forma persistente, todos los ítems que NO están en el sprint activo y que quedan para futuras iteraciones.

Después del Sprint v1.3.0 (ImGui Integration), el siguiente paso natural es **Scene Graph & Entity System** (Sprint v1.4.0).

---

## Formato

Lista de elementos con:
- **ID**: Identificador único
- **Prioridad**: Baja/Media/Alta
- **Descripción**: Breve descripción del ítem
- **Referencia**: Enlace a historia/tarea si procede

---

## 🎯 Próximo Sprint Propuesto

### **SPRINT-v1.4.0 - Scene Graph & Entity System**
**Prioridad**: ⭐ CRÍTICA (siguiente sprint inmediato)  
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

**Prerequisitos**: Sprint v1.3.0 completado ✅

---

## 📦 Ítems en Backlog (Post-v1.3.0)

### **Opción 2: Viewport Real (Render Target como Textura)** 🖼️
**ID**: BACK-004  
**Prioridad**: Alta  
**Descripción**: Exponer render target como SRV texture y renderizarlo con `ImGui::Image()` en panel Viewport. Actualmente el Viewport es placeholder con texto. Esta feature lo haría 100% funcional.

**Componentes principales**:
- Crear SRV descriptor para render target (back buffer)
- Transición de render target a SHADER_RESOURCE state
- Copiar render target a texture intermedia (si necesario)
- Renderizar texture como `ImGui::Image(srvGpuHandle, size)` en Viewport
- Manejo de resize del viewport (ajustar resolución render target)
- Controles básicos de cámara (orbit, pan, zoom con mouse)
- Picking de objetos (raycast desde viewport para selección)

**Beneficios**:
- Viewport 100% funcional (ver render 3D dentro del panel)
- Experiencia de editor profesional (Unity/Unreal style)
- Base para gizmos, grid, manipuladores 3D

**Prerequisitos**:
- Sprint v1.3.0 completado ✅
- Sprint v1.4.0 recomendado (Scene Graph para tener objetos que renderizar)

**Complejidad**: Alta (requiere trabajo significativo con DX12 SRVs y resource transitions)

**Archivos afectados estimados**:
- `src/renderer/DX12Renderer.h/cpp` (crear SRV, transiciones de estado)
- `src/editor/EditorUI.cpp` (RenderViewport con ImGui::Image)
- `src/editor/Camera.h/cpp` (nuevo - controles de cámara en viewport)

**Referencia**: Diferido a Sprint v1.5.0 o posterior (requiere Scene Graph primero)

---

### **Opción 3: Asset System & Resource Management** 📦
**ID**: BACK-005  
**Prioridad**: Alta  
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
- Sprint v1.3.0 completado ✅
- Sprint v1.4.0 completado (Scene Graph para instanciar assets)
- Opcional: Viewport Real (v1.5.0) para preview visual

**Complejidad**: Alta (muchos subsistemas interdependientes)

**Archivos afectados estimados**:
- `src/assets/AssetDatabase.h/cpp` (nuevo - tracking de assets)
- `src/assets/TextureImporter.h/cpp` (nuevo - carga de texturas)
- `src/assets/MeshImporter.h/cpp` (nuevo - carga de meshes)
- `src/assets/ShaderImporter.h/cpp` (nuevo - compilación HLSL)
- `src/editor/AssetBrowser.h/cpp` (nuevo - panel Asset Browser)
- `src/scene/SceneSerializer.h/cpp` (nuevo - save/load escenas)

**Referencia**: Diferido a Sprint v1.6.0 o posterior (requiere Scene Graph y Viewport)

---

### **Opción 4: Advanced Rendering Features** 🎨
**ID**: BACK-006  
**Prioridad**: Media  
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
- Sprint v1.3.0 completado ✅
- Sprint v1.4.0 recomendado (Scene Graph para lights/cameras)
- Opcional: Asset System (v1.6.0) para cargar texturas PBR

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

**Referencia**: Diferido a Sprint v1.7.0 o posterior (requiere Scene Graph)

---

### **Opción 5: Console Integration (Logs Reales)** 📋
**ID**: BACK-007  
**Prioridad**: Baja  
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
- Sprint v1.3.0 completado ✅
- Sprint v1.4.0 recomendado (Scene Graph para comandos como `spawn`)

**Complejidad**: Baja-Media (principalmente UI y refactor logging)

**Archivos afectados estimados**:
- `src/core/Log.h/cpp` (refactorizar para capturar mensajes)
- `src/editor/EditorUI.cpp` (RenderConsole con logs reales)
- `src/editor/Console.h/cpp` (nuevo - console manager, comandos)

**Referencia**: Diferido a Sprint v1.8.0 o posterior (baja prioridad)

---

### **BACK-001 - Custom Runtime UI Framework (Retained-Mode UI)**
**ID**: BACK-001  
**Prioridad**: Media  
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
- Sprint v1.3.0 completado ✅
- Text rendering system (TTF font loading)
- Input system funcional (mouse/keyboard)

**Complejidad**: Muy Alta (framework completo)

**Archivos afectados estimados**:
- `src/ui/UIManager.h/cpp` (nuevo - retained-mode UI manager)
- `src/ui/Widget.h/cpp` (nuevo - base widget class)
- `src/ui/Layout.h/cpp` (nuevo - layout algorithm)
- `src/ui/Button.h/cpp` (nuevo - button widget)
- `src/ui/Text.h/cpp` (nuevo - text widget)
- `shaders/ui_runtime.hlsl` (nuevo - shaders para runtime UI)

**Referencia**: Diferido a Sprint v1.9.0 o posterior (largo plazo)

---

### **BACK-002 - UI Theming & Styling System**
**ID**: BACK-002  
**Prioridad**: Baja  
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
- Sprint v1.3.0 completado ✅
- Custom Runtime UI completado (BACK-001)
- Font loading library integrada (FreeType o stb_truetype)

**Complejidad**: Alta

**Archivos afectados estimados**:
- `src/ui/Style.h/cpp` (nuevo - style definition & parser)
- `src/ui/Theme.h/cpp` (nuevo - theme manager)
- `src/ui/FontManager.h/cpp` (nuevo - TTF font loading)
- `src/ui/VectorRenderer.h/cpp` (nuevo - vector primitives)
- `assets/themes/dark.json` (nuevo - dark theme)
- `assets/themes/light.json` (nuevo - light theme)

**Referencia**: Diferido a Sprint v1.10.0 o posterior (largo plazo)

---

### **BACK-003 - Deferred Rendering (G-Buffer + Lighting Pass)**
**ID**: BACK-003  
**Prioridad**: Media  
**Descripción**: Implementar Deferred Rendering pipeline como alternativa a Forward Rendering. Incluye G-Buffer Pass (multiple render targets para albedo, normal, depth) y Lighting Pass (combina G-Buffer con cálculos de iluminación). Permite renderizar escenas con muchas luces (100+) de forma eficiente.

**Beneficios**:
- Optimización para escenas complejas con muchas luces
- Base para efectos avanzados (SSAO, SSR, IBL)
- Usado en Unreal Engine, CRYENGINE, y otros motores AAA

**Prerequisitos**:
- Forward Rendering funcional ✅ (Sprint v1.2.0)
- Multiple Render Targets (MRT) support
- Shaders de G-Buffer y Lighting
- Opcional: Advanced Rendering (BACK-006) para aprovechar deferred

**Complejidad**: Alta

**Archivos afectados estimados**:
- `src/renderer/DX12Renderer.h` (añadir métodos RenderDeferred, GBufferPass, LightingPass)
- `src/renderer/DX12Renderer.cpp` (implementación de passes)
- `shaders/gbuffer.hlsl` (nuevo - vertex/pixel shaders para G-Buffer)
- `shaders/lighting.hlsl` (nuevo - compute/pixel shader para lighting)

**Referencia**: Diferido a Sprint v1.11.0 o posterior (largo plazo)

---

## 📊 Resumen del Backlog (Prioridades)

| ID | Nombre | Prioridad | Complejidad | Sprint Sugerido |
|----|--------|-----------|-------------|-----------------|
| **v1.4.0** | **Scene Graph & Entity System** | ⭐ **CRÍTICA** | Media | **Siguiente inmediato** |
| BACK-004 | Viewport Real (Render Target Texture) | Alta | Alta | v1.5.0 |
| BACK-005 | Asset System & Resource Management | Alta | Alta | v1.6.0 |
| BACK-006 | Advanced Rendering Features | Media | Muy Alta | v1.7.0 |
| BACK-007 | Console Integration (Logs Reales) | Baja | Baja-Media | v1.8.0 |
| BACK-001 | Custom Runtime UI Framework | Media | Muy Alta | v1.9.0 |
| BACK-002 | UI Theming & Styling | Baja | Alta | v1.10.0 |
| BACK-003 | Deferred Rendering | Media | Alta | v1.11.0 |

**Nota**: Las prioridades y sprints sugeridos son orientativos. Se pueden reordenar según necesidades del proyecto.

---

## 🎯 Próximos Pasos Recomendados

**Después de Sprint v1.3.0** (ImGui Integration completado):

1. **Sprint v1.4.0**: Scene Graph & Entity System ← **Próximo inmediato**
2. **Sprint v1.5.0**: Viewport Real (Render Target Texture)
3. **Sprint v1.6.0**: Asset System & Resource Management
4. **Sprint v1.7.0**: Advanced Rendering Features (PBR, Shadows, Post-Processing)
5. **Sprint v1.8.0**: Console Integration (Logs Reales) + Dev Console

**Justificación**:
- Scene Graph es **prerequisito** para casi todo lo demás (Viewport, Assets, Rendering avanzado)
- Sin Scene Graph, los panels del editor siguen siendo placeholder (no funcionales al 100%)
- Con Scene Graph, el flujo de trabajo del editor se vuelve **profesional** (crear/editar/eliminar objetos reales)

---

*Nota*: El backlog se actualizará conforme se identifiquen ítems diferidos durante los sprints.

