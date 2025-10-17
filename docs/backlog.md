# Backlog (ba�l)

> **Referencia**: Ver [`docs/MAIN.md`](MAIN.md) para los pilares fundamentales del proyecto.

Este fichero contendr�, de forma persistente, todos los �tems que NO est�n en el sprint activo y que quedan para futuras iteraciones.

Por ahora est� vac�o y se usar� como dep�sito para historias/tareas descartadas temporalmente o pendientes de priorizaci�n.

---

## Formato

Lista de elementos con:
- **ID**: Identificador �nico
- **Prioridad**: Baja/Media/Alta
- **Descripci�n**: Breve descripci�n del �tem
- **Referencia**: Enlace a historia/tarea en `docs/sprint_histories.md` o `docs/sprint_tasks.md` si procede

---

## Ítems en Backlog

### **BACK-001 - Custom Runtime UI Framework (Retained-Mode UI)**
**ID**: BACK-001  
**Prioridad**: Alta  
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

**Prerrequisitos**:
- Sprint v1.3.0 completado (ImGui para Editor)
- Text rendering system (TTF font loading)
- Input system funcional (mouse/keyboard)

**Archivos afectados estimados**:
- `src/ui/UIManager.h/cpp` (nuevo - retained-mode UI manager)
- `src/ui/Widget.h/cpp` (nuevo - base widget class)
- `src/ui/Layout.h/cpp` (nuevo - layout algorithm)
- `src/ui/Button.h/cpp` (nuevo - button widget)
- `src/ui/Text.h/cpp` (nuevo - text widget)
- `shaders/ui_runtime.hlsl` (nuevo - shaders para runtime UI)

**Referencia**: Implementar en Sprint v1.4.0 después de ImGui (Editor UI)

---

### **BACK-002 - UI Theming & Styling System**
**ID**: BACK-002  
**Prioridad**: Media  
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

**Prerrequisitos**:
- Sprint v1.3.0 completado (ImGui para Editor)
- Sprint v1.4.0 completado (Custom Runtime UI)
- Font loading library integrada (FreeType o stb_truetype)

**Archivos afectados estimados**:
- `src/ui/Style.h/cpp` (nuevo - style definition & parser)
- `src/ui/Theme.h/cpp` (nuevo - theme manager)
- `src/ui/FontManager.h/cpp` (nuevo - TTF font loading)
- `src/ui/VectorRenderer.h/cpp` (nuevo - vector primitives)
- `assets/themes/dark.json` (nuevo - dark theme)
- `assets/themes/light.json` (nuevo - light theme)

**Referencia**: Implementar en Sprint v1.5.0 después de Runtime UI

---

### **BACK-003 - Deferred Rendering (G-Buffer + Lighting Pass)**
**ID**: BACK-003  
**Prioridad**: Media  
**Descripción**: Implementar Deferred Rendering pipeline como alternativa a Forward Rendering. Incluye G-Buffer Pass (multiple render targets para albedo, normal, depth) y Lighting Pass (combina G-Buffer con cálculos de iluminación). Permite renderizar escenas con muchas luces (100+) de forma eficiente.

**Beneficios**:
- Optimización para escenas complejas con muchas luces
- Base para efectos avanzados (SSAO, SSR, IBL)
- Usado en Unreal Engine, CRYENGINE, y otros motores AAA

**Prerrequisitos**:
- Forward Rendering funcional (Sprint v1.2.0)
- Multiple Render Targets (MRT) support
- Shaders de G-Buffer y Lighting

**Archivos afectados estimados**:
- `src/renderer/DX12Renderer.h` (añadir métodos RenderDeferred, GBufferPass, LightingPass)
- `src/renderer/DX12Renderer.cpp` (implementación de passes)
- `shaders/gbuffer.hlsl` (nuevo - vertex/pixel shaders para G-Buffer)
- `shaders/lighting.hlsl` (nuevo - compute/pixel shader para lighting)

**Referencia**: Esta feature se puede implementar después de Sprint v1.2.0 como evolución natural del renderer. Ver docs/TEMPLATE.md para comparación Forward vs Deferred.

---

*Nota*: El backlog se actualizará conforme se identifiquen ítems diferidos durante los sprints.

