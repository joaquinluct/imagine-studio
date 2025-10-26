# Sprint Tasks - v2.0.0 (Active)

> **Referencia**: Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias de usuario del sprint.

Este archivo contiene las tareas detalladas (bajo nivel) del sprint activo v2.0.0 - Material System (PBR).

---

## Historia 1: Material Core (H1) ? COMPLETADA

### Tarea H1.1: Crear Material.h con MaterialProperties struct
**Estado**: ? Completada  
**Archivos afectados**: `src/materials/Material.h` (creado)
**Commit**: Commit 2 (2025-01-21)

**Descripción**: Crear Material.h con MaterialProperties struct (albedo color, metallic, roughness, texture IDs).

**Estructuras requeridas**:
```cpp
struct MaterialProperties {
    float albedoColor[4];      // RGBA (default: 1,1,1,1)
    float metallic;            // 0.0-1.0 (default: 0.0)
    float roughness;           // 0.0-1.0 (default: 0.5)
    float padding[2];          // Alignment to 16 bytes
};

class Material {
public:
    Material(const std::string& name);
    
    // Texture slots
    void SetAlbedoTexture(AssetID id);
    void SetNormalTexture(AssetID id);
    void SetRoughnessTexture(AssetID id);
    void SetMetallicTexture(AssetID id);
    void SetAOTexture(AssetID id);
    
    // Properties
    void SetAlbedoColor(float r, float g, float b, float a = 1.0f);
    void SetMetallic(float value);
    void SetRoughness(float value);
    
    // Getters
    const MaterialProperties& GetProperties() const;
    AssetID GetAlbedoTexture() const;
    // ... otros getters
    
private:
    std::string m_name;
    MaterialProperties m_properties;
    AssetID m_albedoTexture = 0;
    AssetID m_normalTexture = 0;
    AssetID m_roughnessTexture = 0;
    AssetID m_metallicTexture = 0;
    AssetID m_aoTexture = 0;
};
```

---

### Tarea H1.2: Implementar Material.cpp
**Estado**: ? Completada  
**Archivos afectados**: `src/materials/Material.cpp` (creado)
**Commit**: Commit 2 (2025-01-21)

**Descripción**: Implementar constructor, setters y getters de Material.

---

### Tarea H1.3: Crear MaterialInstance class
**Estado**: ? Completada  
**Archivos afectados**: `src/materials/MaterialInstance.h/cpp` (creados)
**Commit**: Commit 2 (2025-01-21)

**Descripción**: MaterialInstance hereda de Material y comparte shader con Material base.

---

### Tarea H1.4: Crear MaterialManager singleton
**Estado**: ? Completada  
**Archivos afectados**: `src/materials/MaterialManager.h/cpp` (creados)
**Commit**: Commit 2 (2025-01-21)

**Descripción**: MaterialManager con cache de materiales y hot-reload.

---

## Historia 2: PBR Shader Pipeline (H2) ? COMPLETADA

### Tarea H2.1: Crear PBR vertex shader
**Estado**: ? Completada  
**Archivos afectados**: `assets/shaders/pbr_vs.hlsl` (creado)
**Commit**: Commit 8 (2025-01-21) - Hash `5e3e6ff`

**Descripción**: Vertex shader con transform completo (Local ? World ? Clip) y TBN matrix para normal mapping.

---

### Tarea H2.2: Crear PBR pixel shader
**Estado**: ? Completada  
**Archivos afectados**: `assets/shaders/pbr_ps.hlsl` (creado)
**Commit**: Commit 9 (2025-01-21) - Hash `10face4`

**Descripción**: Pixel shader con Cook-Torrance BRDF completo (GGX, Schlick-GGX, Fresnel-Schlick) + normal mapping + tone mapping.

---

### Tarea H2.3: Crear constant buffers
**Estado**: ? Completada  
**Archivos afectados**: `src/renderer/MaterialConstants.h` (creado)
**Commit**: Commit 10 (2025-01-21) - Hash `0b5ff10`

**Descripción**: 3 constant buffer structs (MaterialConstants, LightConstants, PerObjectConstants) con 16-byte alignment.

---

### Tarea H2.4: Descriptor heap para texturas
**Estado**: ? Completada  
**Archivos afectados**: `src/renderer/MaterialConstants.h` (actualizado)
**Commit**: Commit 11 (2025-01-21) - Hash `e20f4e6`

**Descripción**: Configuración descriptor heap (80 slots = 16 materiales × 5 texturas) con slot layout documentado.

---

### Tarea H2.5: Pipeline State Object (PSO)
**Estado**: ? Completada  
**Archivos afectados**: `src/renderer/DX12Renderer.h` (actualizado)
**Commit**: Commit 15 (2025-01-21) - Hash `d95e242`

**Descripción**: PSO PBR preparado (se integrará en H4 cuando se asignen materiales a meshes).

---

## Historia 3: Material Editor Panel (H3) ?? EN PROGRESO

### Tarea H3.1: Crear MaterialEditor.h/cpp
**Estado**: ? Completada  
**Archivos afectados**: `src/editor/MaterialEditor.h/cpp` (creados), `src/editor/EditorUI.cpp` (actualizado)
**Commit**: Commit 16 (2025-01-21) - Hash `a84ba15`

**Descripción**: Panel ImGui Material Editor con:
- Botones "New Material" y "Save Material"
- Sección "Material Properties" (albedo color picker, metallic slider, roughness slider)
- Sección "Texture Slots" (5 slots: Albedo, Normal, Roughness, Metallic, AO)
- Preview placeholder (implementación en H3.4)
- Integrado en `EditorUI::RenderAllPanels()`

**Implementación**:
- `MaterialEditor::Render()`: panel ImGui dockable completo
- `EditorUI::RenderMaterialEditor()`: llama a `MaterialEditor::Render()`
- CollapsingHeaders para secciones (DefaultOpen)
- Placeholders con logs para botones

**Validación**: ? CMake + MSBuild 0 errores, 0 warnings

---

### Tarea H3.2: Texture slots con drag & drop
**Estado**: ? Completada  
**Archivos afectados**: `src/editor/MaterialEditor.h/cpp` (actualizados)
**Commit**: Commit 17 (2025-01-21) - Hash `8655658`

**Descripción**: Implementar drag & drop en 5 texture slots desde Asset Browser

**Implementación**:
- Miembros estáticos en `MaterialEditor.h`:
  - `s_albedoTexture`, `s_normalTexture`, `s_roughnessTexture`, `s_metallicTexture`, `s_aoTexture`
- Lambda helper `RenderTextureSlot()` para DRY:
  - Button con label dinámico (None o texture filename)
  - `ImGui::BeginDragDropTarget()` + `ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM")`
  - Context menu (right-click) con "Clear Texture"
- Actualizar label de botón cuando texture es asignada
- Logs detallados: texture dropped, texture clicked, texture cleared

**Características**:
- Drag texture desde Asset Browser ? Drop en texture slot ?
- Label actualizado: "None##SlotID" ? "texture_filename.dds##SlotID" ?
- Right-click ? Context menu "Clear Texture" ?
- Logs en Console para feedback visual ?

**Validación**: ? CMake + MSBuild 0 errores, solo warnings (C4002 en CORE_LOG macros)

---

### Tarea H3.3: Property sliders
**Estado**: ? Completada (implementado en H3.1)  
**Archivos afectados**: `src/editor/MaterialEditor.cpp`

**Descripción**: Sliders para metallic, roughness, albedo color ? **Ya implementados en H3.1**.

---

### Tarea H3.4: Preview thumbnail
**Estado**: ? Completada  
**Archivos afectados**: `src/editor/MaterialEditor.cpp` (actualizado)
**Commit**: Commit 18 (2025-01-21) - Hash `e4a305d`

**Descripción**: Implementar preview thumbnail del material (visual placeholder)

**Implementación**:
- CollapsingHeader "Preview" (DefaultOpen)
- Preview box 128x128 con `ImDrawList`:
  - `AddRectFilled()` con albedo color (representa material visualmente)
  - `AddRect()` border blanco
- Material properties summary:
  - Albedo (R, G, B, A) valores con formato "%.2f"
  - Metallic valor
  - Roughness valor
- Texture count: "Textures: X/5 assigned"
- Nota amarilla sobre implementación futura (render-to-texture PBR con lighting)

**Características**:
- Preview visual actualiza color cuando albedo color cambia ?
- Summary muestra valores actuales de properties ?
- Texture count dinámico (cuenta texture paths no vacíos) ?
- Placeholder profesional (similar a Unity/Unreal material preview) ?

**Nota técnica**:
- **Placeholder visual** (no render-to-texture real)
- **Render-to-texture real** con PBR lighting se implementará en **H4.3** cuando materiales se apliquen a meshes
- Colored rectangle representa albedo color del material (suficiente para H3)

**Validación**: ? CMake + MSBuild 0 errores

**HISTORIA H3 COMPLETADA**: 4/4 tareas (100%) ?

---

## Historia 4: Material Assignment (H4) ? EN PROGRESO

### Tarea H4.1: Añadir Material* a MeshRenderer
**Estado**: ? Completada  
**Archivos afectados**: `src/components/MeshRenderer.h/cpp` (creados)
**Commit**: Commit 19 (2025-01-21) - Hash `cd9432c`

**Descripción**: Crear MeshRenderer component con soporte para Material* pointer

**Implementación**:
- Crear `src/components/MeshRenderer.h` con interfaz MeshRenderer
- Crear `src/components/MeshRenderer.cpp` con implementación
- MeshRenderer hereda de `Scene::Component`
- Miembros:
  - `std::string m_meshPath`: path to mesh asset
  - `Renderer::Material* m_material`: material pointer (owned by MaterialManager)
- Métodos:
  - `SetMesh(path)` / `GetMesh()`: mesh asset management
  - `SetMaterial(Material*)` / `GetMaterial()`: material assignment
  - `OnUpdate(deltaTime)`: vacío (rendering manejado por DX12Renderer)
  - `OnDestroy()`: limpia referencias (NO delete m_material)

**Características**:
- ? Forward declaration `Renderer::Material` (evita include circular)
- ? Ownership correcto: MaterialManager posee materials
- ? MeshRenderer solo guarda puntero (no delete en destructor)
- ? Logging con CORE_LOG_INFO cuando se asigna mesh/material
- ? Component lifecycle implementado (OnUpdate/OnDestroy)

**Validación**: ? CMake + MSBuild 0 errores

---

### Tarea H4.2: Drag & drop material en Inspector
**Estado**: ? Pendiente  
**Archivos afectados**: `src/editor/EditorUI.cpp` (Inspector section)

**Descripción**: Drag & drop material desde Asset Browser a Inspector

---

### Tarea H4.3: Apply material en rendering
**Estado**: ? Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripción**: Bind texturas del material y constant buffer en rendering pipeline

**Historia H4**: 1/3 tareas completadas (33.3%) ?

---

## Historia 5: Material Serialization & Hot-Reload (H5) ? PENDIENTE

### Tarea H5.1: SaveMaterial() y LoadMaterial()
**Estado**: ? Pendiente  
**Archivos afectados**: `src/materials/MaterialSerializer.h/cpp` (nuevos)

---

### Tarea H5.2: FileWatcher para texturas
**Estado**: ? Pendiente  
**Archivos afectados**: `src/materials/MaterialManager.cpp`

---

### Tarea H5.3: Auto-reload
**Estado**: ? Pendiente  
**Archivos afectados**: `src/materials/MaterialManager.cpp`

**Historia H5**: 0/3 tareas completadas (0%) ?

---

## ?? Resumen de Tareas

| Historia | Tarea | Título | Estado |
|----------|-------|--------|--------|
| H1 | H1.1 | Material.h | ? Completada |
| H1 | H1.2 | Material.cpp | ? Completada |
| H1 | H1.3 | MaterialInstance | ? Completada |
| H1 | H1.4 | MaterialManager | ? Completada |
| H2 | H2.1 | PBR vertex shader | ? Completada |
| H2 | H2.2 | PBR pixel shader | ? Completada |
| H2 | H2.3 | Constant buffers | ? Completada |
| H2 | H2.4 | Descriptor heap | ? Completada |
| H2 | H2.5 | PSO | ? Completada |
| H3 | H3.1 | MaterialEditor panel | ? Completada |
| H3 | H3.2 | Texture slots | ? Completada |
| H3 | H3.3 | Property sliders | ? Completada |
| H3 | H3.4 | Preview thumbnail | ? Completada |
| H4 | H4.1 | Material* en MeshRenderer | ? Completada |
| H4 | H4.2 | Drag & drop en Inspector | ? Pendiente |
| H4 | H4.3 | Apply material | ? Pendiente |
| H5 | H5.1 | SaveMaterial/LoadMaterial | ? Pendiente |
| H5 | H5.2 | FileWatcher | ? Pendiente |
| H5 | H5.3 | Auto-reload | ? Pendiente |

**Total**: 19 tareas  
**Completadas**: 14/19 (73.7%) ?  
**Pendientes**: 5/19 (26.3%) ?

**Progreso por historia**:
- ? H1: 100% (4/4) - **COMPLETADA**
- ? H2: 100% (5/5) - **COMPLETADA**
- ? H3: 100% (4/4) - **COMPLETADA** ??
- ? H4: 33.3% (1/3)
- ? H5: 0% (0/3)

---

*Última actualización*: 2025-01-21  
*Sprint*: v2.0.0 - Material System (PBR)  
*Próxima tarea*: H4.2 - Drag & drop material en Inspector
