# Sprint Tasks - v1.9.0 (Active)

> **Referencia**: Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias de usuario del sprint.

Este archivo contiene las tareas detalladas (bajo nivel) del sprint activo v1.9.0 - Asset System & Resource Management.

---

## Historia 1: Asset Database Core (H1)

### Tarea H1.1: Crear AssetDatabase.h con estructuras básicas
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/assets/AssetDatabase.h` (nuevo)

**Descripción**: Crear AssetDatabase.h con AssetID, AssetType, AssetMetadata y AssetDatabase class.

**Cambios**:
```cpp
#pragma once
#include <string>
#include <unordered_map>
#include <cstdint>

namespace Assets {

using AssetID = uint64_t;

enum class AssetType {
    Unknown,
    Texture,
    Mesh,
    Shader,
    Scene
};

struct AssetMetadata {
    AssetID id;
    std::string path;
    std::string name;
    AssetType type;
    uint64_t size;
    uint64_t lastModified;
    std::vector<AssetID> dependencies;
};

class AssetDatabase {
public:
    static AssetDatabase& Instance();
    
    AssetID ImportAsset(const std::string& path);
    AssetMetadata* GetAsset(AssetID id);
    AssetID GetAssetByPath(const std::string& path);
    void RefreshAssets();
    
    const std::unordered_map<AssetID, AssetMetadata>& GetAllAssets() const;
    
private:
    AssetDatabase() = default;
    AssetID GenerateID();
    AssetType DetectType(const std::string& path);
    
    std::unordered_map<AssetID, AssetMetadata> m_assets;
    std::unordered_map<std::string, AssetID> m_pathToID;
    AssetID m_nextID = 1;
};

} // namespace Assets
```

---

### Tarea H1.2: Implementar AssetDatabase.cpp
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/assets/AssetDatabase.cpp` (nuevo)

**Descripción**: Implementar métodos de AssetDatabase (ImportAsset, GetAsset, RefreshAssets).

---

### Tarea H1.3: Crear asset folder structure
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `assets/` (carpetas nuevas)

**Descripción**: Crear estructura de carpetas para assets:
```
assets/
├── textures/
├── meshes/
├── shaders/
└── scenes/
```

---

### Tarea H1.4: Testing AssetDatabase
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `tests/asset_database_test.cpp` (nuevo)

**Descripción**: Tests unitarios para AssetDatabase (ImportAsset, GetAsset, DetectType).

---

## Historia 2: Texture Importer (H2)

### Tarea H2.1: Integrar stb_image library
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `external/stb/` (nuevo), `CMakeLists.txt`

**Descripción**: Descargar stb_image.h y añadir a external/ para cargar PNG/JPG.

---

### Tarea H2.2: Crear TextureImporter.h/cpp
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/assets/TextureImporter.h/cpp` (nuevos)

**Descripción**: Implementar TextureImporter con ImportTexture(path) usando stb_image.

---

### Tarea H2.3: Crear DX12 Texture2D desde pixel data
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/renderer/DX12ResourceManager.h/cpp`

**Descripción**: Añadir CreateTexture2DFromData() en DX12ResourceManager para upload de pixels.

---

### Tarea H2.4: Testing TextureImporter
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `tests/texture_importer_test.cpp` (nuevo), `assets/textures/test.png`

**Descripción**: Tests con textura PNG real (importar, verificar dimensiones, formato).

---

## Historia 3: Mesh Importer (H3)

### Tarea H3.1: Crear MeshImporter.h con MeshData struct
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/assets/MeshImporter.h` (nuevo)

**Descripción**: Definir MeshData struct (vertices, indices, bounds) y MeshImporter class.

---

### Tarea H3.2: Implementar OBJ parser
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/assets/MeshImporter.cpp` (nuevo)

**Descripción**: Parser de formato OBJ (v, vn, vt, f lines) → MeshData.

---

### Tarea H3.3: Crear Vertex/Index buffers en DX12
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/renderer/DX12ResourceManager.h/cpp`

**Descripción**: Añadir CreateMeshBuffers(MeshData) en DX12ResourceManager.

---

### Tarea H3.4: Testing MeshImporter
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `tests/mesh_importer_test.cpp` (nuevo), `assets/meshes/cube.obj`

**Descripción**: Tests con mesh OBJ simple (cube.obj) → verificar vertices, indices.

---

## Historia 4: Asset Browser Panel (H4)

### Tarea H4.1: Crear AssetBrowser.h/cpp
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/editor/AssetBrowser.h/cpp` (nuevos)

**Descripción**: Crear panel Asset Browser con ImGui (navegación de carpetas).

---

### Tarea H4.2: Implementar thumbnails para texturas
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/editor/AssetBrowser.cpp`

**Descripción**: Mostrar preview de texturas con ImGui::Image().

---

### Tarea H4.3: Drag & drop de assets
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/editor/AssetBrowser.cpp`, `src/editor/EditorUI.cpp`

**Descripción**: ImGui drag & drop de assets al Viewport/Inspector.

---

### Tarea H4.4: Context menu (Reimport, Delete)
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/editor/AssetBrowser.cpp`

**Descripción**: Right-click en asset muestra context menu con opciones.

---

## Historia 5: Scene Serialization (H5)

### Tarea H5.1: Integrar JSON library (nlohmann/json)
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `external/json/` (nuevo), `CMakeLists.txt`

**Descripción**: Añadir nlohmann/json (single-header) para serialization.

---

### Tarea H5.2: Crear SceneSerializer.h/cpp
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/scene/SceneSerializer.h/cpp` (nuevos)

**Descripción**: Implementar SaveScene(scene, path) y LoadScene(path).

---

### Tarea H5.3: File → Save/Load Scene en editor
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `src/editor/EditorUI.cpp`, `src/main.cpp`

**Descripción**: Menu File con Save Scene (Ctrl+S) y Load Scene (Ctrl+O).

---

### Tarea H5.4: Testing SceneSerializer
**Estado**: ⏳ Pendiente  
**Archivos afectados**: `tests/scene_serializer_test.cpp` (nuevo)

**Descripción**: Tests de save/load (crear scene, guardar JSON, cargar, verificar entities).

---

## 📊 Resumen de Tareas

| Historia | Tarea | Título | Estado |
|----------|-------|--------|--------|
| H1 | H1.1 | AssetDatabase.h | ⏳ Pendiente |
| H1 | H1.2 | AssetDatabase.cpp | ⏳ Pendiente |
| H1 | H1.3 | Asset folders | ⏳ Pendiente |
| H1 | H1.4 | Testing AssetDatabase | ⏳ Pendiente |
| H2 | H2.1 | Integrar stb_image | ⏳ Pendiente |
| H2 | H2.2 | TextureImporter | ⏳ Pendiente |
| H2 | H2.3 | DX12 Texture2D | ⏳ Pendiente |
| H2 | H2.4 | Testing TextureImporter | ⏳ Pendiente |
| H3 | H3.1 | MeshImporter.h | ⏳ Pendiente |
| H3 | H3.2 | OBJ parser | ⏳ Pendiente |
| H3 | H3.3 | DX12 buffers | ⏳ Pendiente |
| H3 | H3.4 | Testing MeshImporter | ⏳ Pendiente |
| H4 | H4.1 | AssetBrowser panel | ⏳ Pendiente |
| H4 | H4.2 | Thumbnails | ⏳ Pendiente |
| H4 | H4.3 | Drag & drop | ⏳ Pendiente |
| H4 | H4.4 | Context menu | ⏳ Pendiente |
| H5 | H5.1 | Integrar JSON | ⏳ Pendiente |
| H5 | H5.2 | SceneSerializer | ⏳ Pendiente |
| H5 | H5.3 | File menu | ⏳ Pendiente |
| H5 | H5.4 | Testing Serializer | ⏳ Pendiente |

**Total**: 20 tareas (0 completadas, 20 pendientes)

---

*Última actualización*: 2025-01-18  
*Sprint*: v1.9.0 - Asset System & Resource Management
