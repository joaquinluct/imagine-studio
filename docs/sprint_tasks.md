# Sprint Tasks - v1.9.0 (Active)

> **Referencia**: Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias de usuario del sprint.

Este archivo contiene las tareas detalladas (bajo nivel) del sprint activo v1.9.0 - Asset System & Resource Management.

---

## Historia 1: Asset Database Core (H1) ? COMPLETADA

### Tarea H1.1: Crear AssetDatabase.h con estructuras b�sicas
**Estado**: ? Completada  
**Fecha**: 2025-01-18  
**Archivos afectados**: `src/assets/AssetDatabase.h` (nuevo)

**Descripci�n**: Crear AssetDatabase.h con AssetID, AssetType, AssetMetadata y AssetDatabase class.

**Cambios realizados**:
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
**Estado**: ? Completada  
**Fecha**: 2025-01-21  
**Archivos afectados**: `src/assets/AssetDatabase.cpp` (nuevo)

**Descripci�n**: Implementar m�todos de AssetDatabase (ImportAsset, GetAsset, RefreshAssets).

**Implementaci�n**: Singleton thread-safe con m�todos Register/Unregister/HasAsset/GetMetadata.

---

### Tarea H1.3: Crear asset folder structure
**Estado**: ? Completada  
**Fecha**: 2025-01-21  
**Archivos afectados**: `assets/` (carpetas nuevas)

**Descripci�n**: Crear estructura de carpetas para assets:
```
assets/
??? textures/
??? meshes/
??? shaders/
??? scenes/
```

**Implementaci�n**: Estructura completa creada con README.md y .gitkeep files.

---

### Tarea H1.4: Testing AssetDatabase
**Estado**: ? Completada  
**Fecha**: 2025-01-21  
**Archivos afectados**: `tests/asset_database_test.cpp` (nuevo)

**Descripci�n**: Tests unitarios para AssetDatabase (ImportAsset, GetAsset, DetectType).

**Resultado**: 27 assertions passed ?

---

## Historia 2: Texture Importer (H2) ? COMPLETADA

### Tarea H2.1: Integrar stb_image library
**Estado**: ? Completada  
**Fecha**: 2025-01-21  
**Archivos afectados**: `external/stb/` (nuevo), `CMakeLists.txt`

**Descripci�n**: Descargar stb_image.h y a�adir a external/ para cargar PNG/JPG.

**Implementaci�n**: stb_image integrado con target CMake, soporte PNG/JPG/BMP/TGA/PSD/GIF/HDR/PIC.

---

### Tarea H2.2: Crear TextureImporter.h/cpp
**Estado**: ? Completada  
**Fecha**: 2025-01-21  
**Archivos afectados**: `src/assets/TextureImporter.h/cpp` (nuevos)

**Descripci�n**: Implementar TextureImporter con ImportTexture(path) usando stb_image.

**Implementaci�n**: TextureImporter class con m�todos ImportTexture, ImportTextureRGBA, GetTextureInfo, IsSupportedFormat.

---

### Tarea H2.3: Crear DX12 Texture2D desde pixel data
**Estado**: ? Completada  
**Fecha**: 2025-01-21  
**Archivos afectados**: `src/renderer/DX12ResourceManager.h/cpp`

**Descripci�n**: A�adir CreateTexture2DFromData() en DX12ResourceManager para upload de pixels.

**Implementaci�n**: CreateTexture2DFromData con staging buffer, row pitch alignment, resource barrier.

---

### Tarea H2.4: Testing TextureImporter
**Estado**: ? Completada  
**Fecha**: 2025-01-21  
**Archivos afectados**: `tests/texture_importer_test.cpp` (nuevo), `assets/textures/test_4x4.png`

**Descripci�n**: Tests con textura PNG real (importar, verificar dimensiones, formato).

**Resultado**: 27 assertions passed ? (5 test suites: IsSupportedFormat, ImportInvalid, GetInfo, ImportValid, Channels)

---

## Historia 3: Mesh Importer (H3)

### Tarea H3.1: Crear MeshImporter.h con MeshData struct
**Estado**: ? Completada  
**Fecha**: 2025-01-21  
**Archivos afectados**: `src/assets/MeshData.h`, `src/assets/MeshImporter.h`, `src/assets/README_MESH_FORMAT.md` (nuevos)

**Descripci�n**: Definir MeshData struct (vertices, indices, bounds) y MeshImporter class.

**Implementaci�n**:
- `MeshData.h`: Estructuras Vertex, BoundingBox, MeshData completas
- `MeshImporter.h`: API para importar OBJ con m�todos ImportOBJ, IsSupportedFormat, GetMeshInfo
- README_MESH_FORMAT.md: Documentaci�n completa del formato OBJ soportado

**Estructuras creadas**:
```cpp
struct Vertex {
    float position[3];  // x, y, z
    float normal[3];    // nx, ny, nz
    float texCoord[2];  // u, v
};

struct BoundingBox {
    float min[3];  // Min corner
    float max[3];  // Max corner
    void GetCenter(...);
    void GetSize(...);
};

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    BoundingBox bounds;
    std::string name;
    std::string sourcePath;
    
    bool IsValid() const;
    unsigned int GetVertexCount() const;
    unsigned int GetIndexCount() const;
    unsigned int GetTriangleCount() const;
    unsigned int GetMemorySize() const;
    void CalculateBounds();
};
```

**API MeshImporter**:
- `ImportOBJ(path)` - Cargar mesh completo desde archivo OBJ
- `IsSupportedFormat(path)` - Verificar extensi�n .obj
- `GetMeshInfo(path, vertexCount, triangleCount)` - Info r�pida sin cargar
- `GetLastError()` - �ltimo error si ImportOBJ falla

**Formato OBJ soportado**:
- Vertex positions (v x y z)
- Vertex normals (vn nx ny nz)
- Texture coordinates (vt u v)
- Triangles y quads (f v1/vt1/vn1 ...)

**Compilaci�n**: 0 errores, 0 warnings (CMake + MSBuild) ?

---

### Tarea H3.2: Implementar OBJ parser
**Estado**: ? Completada  
**Fecha**: 2025-01-21  
**Archivos afectados**: `src/assets/MeshImporter.cpp` (nuevo)

**Descripci�n**: Parser de formato OBJ (v, vn, vt, f lines) ? MeshData.

**Implementaci�n completa** (~450 l�neas):
- Parser robusto con validaci�n de errores
- Soporte completo de formatos OBJ est�ndar
- Triangulaci�n autom�tica de quads y n-gons
- Thread-safe error handling

**Funcionalidades**:
1. **ImportOBJ(path)** - Parser completo de archivos OBJ
   - Lee l�neas v (positions), vn (normals), vt (texcoords), f (faces)
   - Convierte �ndices 1-based (OBJ) a 0-based (C++)
   - Soporte �ndices negativos (relativos al final)
   - Validaci�n de �ndices (rango v�lido)

2. **Triangulaci�n autom�tica**:
   - Triangles (3 v�rtices): 1 tri�ngulo
   - Quads (4 v�rtices): 2 tri�ngulos (fan triangulation)
   - N-gons (>4 v�rtices): N-2 tri�ngulos (fan desde primer v�rtice)

3. **Formatos de �ndices soportados**:
   - `f v1 v2 v3` - Solo posiciones
   - `f v1/vt1 v2/vt2 v3/vt3` - Posiciones + UVs
   - `f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3` - Completo
   - `f v1//vn1 v2//vn2 v3//vn3` - Sin UVs

4. **Validaci�n y error handling**:
   - Verificar formato .obj
   - Validar apertura de archivo
   - Validar �ndices de posiciones/normals/UVs
   - Error messages detallados con n�mero de l�nea
   - Exceptions std::runtime_error en caso de fallo

5. **Logging autom�tico**:
   - Informaci�n de importaci�n (nombre, path)
   - Estad�sticas (vertices, indices, triangles, memory)
   - Warnings para l�neas no parseables
   - Errors para faces inv�lidas

**Caracter�sticas t�cnicas**:
- **Thread-safe**: `thread_local s_lastError`
- **Default values**: Normals (0,1,0), UVs (0,0) si no presentes
- **Bounding box**: Calculado autom�ticamente con CalculateBounds()
- **Memory efficient**: Un solo pass, no realloc innecesarios

**Ejemplo de output log**:
```
MeshImporter: Importing OBJ file: assets/meshes/cube.obj
MeshImporter: Successfully imported mesh: cube
  Vertices: 24
  Indices: 36
  Triangles: 12
  Memory: 912 bytes
```

**Compilaci�n**: 0 errores, 0 warnings (CMake + MSBuild) ?

---

### Tarea H3.3: Crear Vertex/Index buffers en DX12
**Estado**: ? Completada  
**Fecha**: 2025-01-21  
**Archivos afectados**: `src/renderer/DX12ResourceManager.h/cpp`

**Descripci�n**: A�adir CreateMeshBuffers(MeshData) en DX12ResourceManager.

**Implementaci�n completa** (~220 l�neas):
- Estructura `MeshBuffers` con vertex/index buffers + views
- M�todo `CreateMeshBuffers(mesh, commandList, uploadBuffers)`
- Upload autom�tico a GPU (staging buffers)
- Resource barriers (COPY_DEST ? VERTEX_BUFFER/INDEX_BUFFER)

**Funcionalidades**:
1. **Estructura MeshBuffers**:
   - `ID3D12Resource* vertexBuffer` - Vertex buffer en GPU
   - `ID3D12Resource* indexBuffer` - Index buffer en GPU
   - `D3D12_VERTEX_BUFFER_VIEW vertexBufferView` - View para binding
   - `D3D12_INDEX_BUFFER_VIEW indexBufferView` - View para binding
   - `unsigned int vertexCount` - N�mero de v�rtices
   - `unsigned int indexCount` - N�mero de �ndices
   - `bool IsValid()` - Verificar si buffers creados correctamente

2. **CreateMeshBuffers(mesh, commandList, uploadBuffers)**:
   - **Input**: MeshData (desde MeshImporter), command list, array upload buffers[2]
   - **Output**: MeshBuffers struct con buffers + views listos para rendering
   
3. **Proceso de upload**:
   - Crear vertex buffer en default heap (GPU-only, optimal)
   - Crear index buffer en default heap
   - Crear vertex upload buffer (staging)
   - Crear index upload buffer (staging)
   - Map upload buffers ? copy data ? unmap
   - CopyBufferRegion (upload ? default heap)
   - Resource barriers (COPY_DEST ? VERTEX/INDEX state)
   - Return upload buffers para deferred release

4. **Buffer views**:
   - Vertex buffer view: GPU address, stride (sizeof(Vertex)=32 bytes), size
   - Index buffer view: GPU address, format (R32_UINT), size

5. **Logging detallado**:
   ```
   DX12ResourceManager: Creating mesh buffers for cube
   DX12ResourceManager: Mesh buffers created for cube
     Vertex buffer: 24 vertices (768 bytes)
     Index buffer: 36 indices (144 bytes)
     Triangles: 12
     Upload buffers returned to caller (manual management)
   ```

**Caracter�sticas t�cnicas**:
- **Memory layout**: Vertex buffer + Index buffer separados (�ptimo DX12)
- **Stride**: 32 bytes por v�rtice (pos 12 + normal 12 + UV 8)
- **Index format**: R32_UINT (32-bit unsigned int)
- **Upload pattern**: Mismo que CreateTexture2DFromData (deferred release)
- **Barriers batching**: 2 barriers en una sola llamada (efficient)

**Beneficio**: MeshData (CPU) ? GPU buffers listos para DrawIndexedInstanced()

**Compilaci�n**: 0 errores, 0 warnings (CMake + MSBuild) ?

---

### Tarea H3.4: Testing MeshImporter
**Estado**: ? Pendiente  
**Archivos afectados**: `tests/mesh_importer_test.cpp` (nuevo), `assets/meshes/cube.obj`

**Descripci�n**: Tests con mesh OBJ simple (cube.obj) ? verificar vertices, indices.

---

## Historia 4: Asset Browser Panel (H4)

### Tarea H4.1: Crear AssetBrowser.h/cpp
**Estado**: ? Pendiente  
**Archivos afectados**: `src/editor/AssetBrowser.h/cpp` (nuevos)

**Descripci�n**: Crear panel Asset Browser con ImGui (navegaci�n de carpetas).

---

### Tarea H4.2: Implementar thumbnails para texturas
**Estado**: ? Pendiente  
**Archivos afectados**: `src/editor/AssetBrowser.cpp`

**Descripci�n**: Mostrar preview de texturas con ImGui::Image().

---

### Tarea H4.3: Drag & drop de assets
**Estado**: ? Pendiente  
**Archivos afectados**: `src/editor/AssetBrowser.cpp`, `src/editor/EditorUI.cpp`

**Descripci�n**: ImGui drag & drop de assets al Viewport/Inspector.

---

### Tarea H4.4: Context menu (Reimport, Delete)
**Estado**: ? Pendiente  
**Archivos afectados**: `src/editor/AssetBrowser.cpp`

**Descripci�n**: Right-click en asset muestra context menu con opciones.

---

## Historia 5: Scene Serialization (H5)

### Tarea H5.1: Integrar JSON library (nlohmann/json)
**Estado**: ? Pendiente  
**Archivos afectados**: `external/json/` (nuevo), `CMakeLists.txt`

**Descripci�n**: A�adir nlohmann/json (single-header) para serialization.

---

### Tarea H5.2: Crear SceneSerializer.h/cpp
**Estado**: ? Pendiente  
**Archivos afectados**: `src/scene/SceneSerializer.h/cpp` (nuevos)

**Descripci�n**: Implementar SaveScene(scene, path) y LoadScene(path).

---

### Tarea H5.3: File ? Save/Load Scene en editor
**Estado**: ? Pendiente  
**Archivos afectados**: `src/editor/EditorUI.cpp`, `src/main.cpp`

**Descripci�n**: Menu File con Save Scene (Ctrl+S) y Load Scene (Ctrl+O).

---

### Tarea H5.4: Testing SceneSerializer
**Estado**: ? Pendiente  
**Archivos afectados**: `tests/scene_serializer_test.cpp` (nuevo)

**Descripci�n**: Tests de save/load (crear scene, guardar JSON, cargar, verificar entities).

---

## ?? Resumen de Tareas

| Historia | Tarea | T�tulo | Estado |
|----------|-------|--------|--------|
| H1 | H1.1 | AssetDatabase.h | ? Completada |
| H1 | H1.2 | AssetDatabase.cpp | ? Completada |
| H1 | H1.3 | Asset folders | ? Completada |
| H1 | H1.4 | Testing AssetDatabase | ? Completada |
| H2 | H2.1 | Integrar stb_image | ? Completada |
| H2 | H2.2 | TextureImporter | ? Completada |
| H2 | H2.3 | DX12 Texture2D | ? Completada |
| H2 | H2.4 | Testing TextureImporter | ? Completada |
| H3 | H3.1 | MeshImporter.h | ? Completada |
| H3 | H3.2 | OBJ parser | ? Completada |
| H3 | H3.3 | DX12 buffers | ? Completada |
| H3 | H3.4 | Testing MeshImporter | ? Pendiente |
| H4 | H4.1 | AssetBrowser panel | ? Pendiente |
| H4 | H4.2 | Thumbnails | ? Pendiente |
| H4 | H4.3 | Drag & drop | ? Pendiente |
| H4 | H4.4 | Context menu | ? Pendiente |
| H5 | H5.1 | Integrar JSON | ? Pendiente |
| H5 | H5.2 | SceneSerializer | ? Pendiente |
| H5 | H5.3 | File menu | ? Pendiente |
| H5 | H5.4 | Testing Serializer | ? Pendiente |

**Total**: 20 tareas  
**Completadas**: 11/20 (55%)  
**Pendientes**: 9/20 (45%)

**Historias completadas**: 2/5 (40%) - ? H1, ? H2  
**Historias en progreso**: 1/5 (20%) - ? H3 (3/4 tareas, 75%)

---

*�ltima actualizaci�n*: 2025-01-21  
*Sprint*: v1.9.0 - Asset System & Resource Management
