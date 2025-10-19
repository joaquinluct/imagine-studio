# Sprint Histories - v1.9.0 (Active)

> **Referencia**: Ver [`docs/sprint_tasks.md`](sprint_tasks.md) para tareas detalladas del sprint.

Este archivo contiene las historias de usuario (alto nivel) del sprint activo v1.9.0 - Asset System & Resource Management.

---

## Historia 1: Asset Database Core (H1)

### H1 - Asset Database Core
**ID**: H1  
**Prioridad**: 🔴 Crítica  
**Estado**: ⏳ Pendiente  

**Descripción**: Implementar Asset Database para tracking de assets en disco con metadatos, dependencies y hot-reload.

**Criterios de aceptación**:
- [ ] AssetDatabase class con tracking de assets (path → AssetID)
- [ ] AssetMetadata struct (type, size, lastModified, dependencies)
- [ ] ImportAsset() detecta tipo automáticamente (extensión)
- [ ] RefreshAssets() detecta cambios en disco (FileSystemWatcher)
- [ ] GetAsset() retorna metadata por AssetID
- [ ] Soporte para carpetas recursivas (assets/textures/, assets/meshes/)

**Tareas derivadas**: H1.1, H1.2, H1.3, H1.4

---

## Historia 2: Texture Importer (H2)

### H2 - Texture Importer
**ID**: H2  
**Prioridad**: 🔴 Crítica  
**Estado**: ⏳ Pendiente  

**Descripción**: Implementar TextureImporter para cargar PNG/JPG desde disco y convertir a DX12 Texture con mipmaps.

**Criterios de aceptación**:
- [ ] TextureImporter class con ImportTexture(path)
- [ ] Soporte PNG/JPG con stb_image (single-header library)
- [ ] CreateTexture2D() crea ID3D12Resource con formato correcto
- [ ] Upload de datos pixel a GPU (staging buffer)
- [ ] GenerateMipmaps() opcional para texturas
- [ ] Texturas usables en shaders (SRV descriptor)

**Tareas derivadas**: H2.1, H2.2, H2.3, H2.4

---

## Historia 3: Mesh Importer (H3)

### H3 - Mesh Importer
**ID**: H3  
**Prioridad**: 🟡 Alta  
**Estado**: ⏳ Pendiente  

**Descripción**: Implementar MeshImporter para cargar OBJ desde disco y convertir a Vertex/Index buffers.

**Criterios de aceptación**:
- [ ] MeshImporter class con ImportOBJ(path)
- [ ] Parser de formato OBJ (vertices, normals, UVs, faces)
- [ ] MeshData struct (vertices, indices, bounds)
- [ ] CreateVertexBuffer() y CreateIndexBuffer() en DX12
- [ ] Meshes renderizables con DrawIndexedInstanced()
- [ ] Cálculo de bounding box (AABB) para culling

**Tareas derivadas**: H3.1, H3.2, H3.3, H3.4

---

## Historia 4: Asset Browser Panel (H4)

### H4 - Asset Browser Panel
**ID**: H4  
**Prioridad**: 🟡 Alta  
**Estado**: ⏳ Pendiente  

**Descripción**: Implementar Asset Browser panel en editor para navegar, preview y usar assets.

**Criterios de aceptación**:
- [ ] AssetBrowser panel con ImGui (navegación de carpetas)
- [ ] Thumbnails para texturas (preview)
- [ ] Íconos para meshes/shaders (tipo de asset)
- [ ] Double-click abre asset en Inspector
- [ ] Drag & drop de assets al Viewport
- [ ] Context menu (Reimport, Delete, Properties)

**Tareas derivadas**: H4.1, H4.2, H4.3, H4.4

---

## Historia 5: Scene Serialization (H5)

### H5 - Scene Serialization
**ID**: H5  
**Prioridad**: 🟡 Alta  
**Estado**: ⏳ Pendiente  

**Descripción**: Implementar SceneSerializer para save/load escenas completas en formato JSON.

**Criterios de aceptación**:
- [ ] SceneSerializer class con SaveScene() y LoadScene()
- [ ] Formato JSON con entities, transforms, components
- [ ] Referencias a assets (AssetID, no paths absolutos)
- [ ] File → Save Scene / Load Scene en editor
- [ ] Preserva jerarquía padre-hijo
- [ ] Extensible para custom components

**Tareas derivadas**: H5.1, H5.2, H5.3, H5.4

---

## 📊 Resumen de Historias

| ID | Historia | Tareas | Completadas | Estado |
|----|----------|--------|-------------|--------|
| H1 | Asset Database Core | 4 | 0 | ⏳ Pendiente |
| H2 | Texture Importer | 4 | 0 | ⏳ Pendiente |
| H3 | Mesh Importer | 4 | 0 | ⏳ Pendiente |
| H4 | Asset Browser Panel | 4 | 0 | ⏳ Pendiente |
| H5 | Scene Serialization | 4 | 0 | ⏳ Pendiente |

**Total historias**: 5 (0 completadas, 5 pendientes)

---

*Última actualización*: 2025-01-18  
*Sprint*: v1.9.0 - Asset System & Resource Management
