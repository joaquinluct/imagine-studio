# Sprint Tasks - v2.0.0 (Active)

> **Referencia**: Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias de usuario del sprint.

Este archivo contiene las tareas detalladas (bajo nivel) del sprint activo v2.0.0 - Material System (PBR).

---

## Historia 1: Material Core (H1) ? EN PROGRESO

### Tarea H1.1: Crear Material.h con MaterialProperties struct
**Estado**: ? Pendiente  
**Archivos afectados**: `src/materials/Material.h` (nuevo)

**Descripci�n**: Crear Material.h con MaterialProperties struct (albedo color, metallic, roughness, texture IDs).

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
**Estado**: ? Pendiente  
**Archivos afectados**: `src/materials/Material.cpp` (nuevo)

**Descripci�n**: Implementar constructor, setters y getters de Material.

---

### Tarea H1.3: Crear MaterialInstance class
**Estado**: ? Pendiente  
**Archivos afectados**: `src/materials/MaterialInstance.h/cpp` (nuevos)

**Descripci�n**: MaterialInstance hereda de Material y comparte shader con Material base.

---

### Tarea H1.4: Crear MaterialManager singleton
**Estado**: ? Pendiente  
**Archivos afectados**: `src/materials/MaterialManager.h/cpp` (nuevos)

**Descripci�n**: MaterialManager con cache de materiales y hot-reload.

---

## Historia 2: PBR Shader Pipeline (H2) ? PENDIENTE

### Tarea H2.1: Crear PBR vertex shader
**Estado**: ? Pendiente  
**Archivos afectados**: `assets/shaders/pbr_vs.hlsl` (nuevo)

**Descripci�n**: Vertex shader con transform y TBN matrix para normal mapping.

---

### Tarea H2.2: Crear PBR pixel shader
**Estado**: ? Pendiente  
**Archivos afectados**: `assets/shaders/pbr_ps.hlsl` (nuevo)

**Descripci�n**: Pixel shader con Cook-Torrance BRDF y IBL opcional.

---

### Tarea H2.3: Crear constant buffers
**Estado**: ? Pendiente  
**Archivos afectados**: `src/renderer/MaterialConstants.h` (nuevo)

**Descripci�n**: Material CB y Light CB para shaders.

---

### Tarea H2.4: Descriptor heap para texturas
**Estado**: ? Pendiente  
**Archivos afectados**: `src/renderer/DX12ResourceManager.cpp`

**Descripci�n**: Descriptor heap con SRVs para texturas PBR.

---

### Tarea H2.5: Pipeline State Object (PSO)
**Estado**: ? Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripci�n**: PSO para PBR rendering con shaders compilados.

---

## Historia 3: Material Editor Panel (H3) ? PENDIENTE

### Tarea H3.1: Crear MaterialEditor.h/cpp
**Estado**: ? Pendiente  
**Archivos afectados**: `src/editor/MaterialEditor.h/cpp` (nuevos)

**Descripci�n**: Panel ImGui para editar materiales.

---

### Tarea H3.2: Texture slots con drag & drop
**Estado**: ? Pendiente  
**Archivos afectados**: `src/editor/MaterialEditor.cpp`

**Descripci�n**: 5 texture slots con drag & drop desde Asset Browser.

---

### Tarea H3.3: Property sliders
**Estado**: ? Pendiente  
**Archivos afectados**: `src/editor/MaterialEditor.cpp`

**Descripci�n**: Sliders para metallic, roughness, albedo color.

---

### Tarea H3.4: Preview thumbnail
**Estado**: ? Pendiente  
**Archivos afectados**: `src/editor/MaterialEditor.cpp`

**Descripci�n**: Preview del material (render to texture).

---

## Historia 4: Material Assignment (H4) ? PENDIENTE

### Tarea H4.1: A�adir Material* a MeshRenderer
**Estado**: ? Pendiente  
**Archivos afectados**: `src/components/MeshRenderer.h/cpp`

**Descripci�n**: MeshRenderer almacena puntero a Material.

---

### Tarea H4.2: Drag & drop material en Inspector
**Estado**: ? Pendiente  
**Archivos afectados**: `src/editor/Inspector.cpp`

**Descripci�n**: Drag & drop material desde Asset Browser a Inspector.

---

### Tarea H4.3: Apply material en rendering
**Estado**: ? Pendiente  
**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Descripci�n**: Bind texturas y constant buffer del material.

---

## Historia 5: Material Serialization & Hot-Reload (H5) ? PENDIENTE

### Tarea H5.1: SaveMaterial() y LoadMaterial()
**Estado**: ? Pendiente  
**Archivos afectados**: `src/materials/MaterialSerializer.h/cpp` (nuevos)

**Descripci�n**: Serializaci�n JSON de materiales (.mat files).

---

### Tarea H5.2: FileWatcher para texturas
**Estado**: ? Pendiente  
**Archivos afectados**: `src/materials/MaterialManager.cpp`

**Descripci�n**: Detectar cambios en archivos de textura.

---

### Tarea H5.3: Auto-reload
**Estado**: ? Pendiente  
**Archivos afectados**: `src/materials/MaterialManager.cpp`

**Descripci�n**: Recargar material al cambiar archivo en disco.

---

## ?? Resumen de Tareas

| Historia | Tarea | T�tulo | Estado |
|----------|-------|--------|--------|
| H1 | H1.1 | Material.h | ? Pendiente |
| H1 | H1.2 | Material.cpp | ? Pendiente |
| H1 | H1.3 | MaterialInstance | ? Pendiente |
| H1 | H1.4 | MaterialManager | ? Pendiente |
| H2 | H2.1 | PBR vertex shader | ? Pendiente |
| H2 | H2.2 | PBR pixel shader | ? Pendiente |
| H2 | H2.3 | Constant buffers | ? Pendiente |
| H2 | H2.4 | Descriptor heap | ? Pendiente |
| H2 | H2.5 | PSO | ? Pendiente |
| H3 | H3.1 | MaterialEditor panel | ? Pendiente |
| H3 | H3.2 | Texture slots | ? Pendiente |
| H3 | H3.3 | Property sliders | ? Pendiente |
| H3 | H3.4 | Preview thumbnail | ? Pendiente |
| H4 | H4.1 | Material* en MeshRenderer | ? Pendiente |
| H4 | H4.2 | Drag & drop en Inspector | ? Pendiente |
| H4 | H4.3 | Apply material | ? Pendiente |
| H5 | H5.1 | SaveMaterial/LoadMaterial | ? Pendiente |
| H5 | H5.2 | FileWatcher | ? Pendiente |
| H5 | H5.3 | Auto-reload | ? Pendiente |

**Total**: 19 tareas  
**Completadas**: 0/19 (0%) ?  
**Pendientes**: 19/19 (100%)

---

*�ltima actualizaci�n*: 2025-01-21  
*Sprint*: v2.0.0 - Material System (PBR)
