# Sprint v1.9.0 - Asset System & Resource Management

> **Estado**: 🟢 En progreso  
> **Fecha inicio**: 2025-01-18  
> **Fecha fin estimada**: 2025-02-01

---

## 🎯 Objetivo del Sprint

**Meta**: Implementar Asset System completo para cargar contenido real (texturas, meshes, shaders) desde disco, con Asset Browser panel en editor y flujo de trabajo profesional (importar → usar → guardar).

**Resultado esperado**: Al finalizar el sprint, el editor tendrá:
- Asset Database funcional con tracking de archivos ✅
- Importers para Textures ✅, Meshes y Shaders
- Asset Browser panel para navegar contenido
- Drag & drop de assets al Viewport/Inspector
- Scene serialization (save/load JSON)

---

## 📋 Historias de Usuario

Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias detalladas.

| ID | Historia | Prioridad | Estado |
|----|----------|-----------|--------|
| H1 | Asset Database Core | 🔴 Crítica | ✅ **Completada** |
| H2 | Texture Importer | 🔴 Crítica | ✅ **Completada** |
| H3 | Mesh Importer | 🟡 Alta | ⏳ Pendiente |
| H4 | Asset Browser Panel | 🟡 Alta | ⏳ Pendiente |
| H5 | Scene Serialization | 🟡 Alta | ⏳ Pendiente |

---

## 📊 Métricas de Éxito

| Métrica | Objetivo | Estado |
|---------|----------|--------|
| **Formatos soportados (texturas)** | PNG, JPG | ✅ Completado |
| **Formatos soportados (meshes)** | OBJ | ⏳ Pendiente |
| **Formatos soportados (shaders)** | HLSL | ⏳ Pendiente |
| **Asset Browser** | Navegación de carpetas funcional | ⏳ Pendiente |
| **Drag & drop** | Texturas y meshes al Viewport | ⏳ Pendiente |
| **Serialization** | Save/load escenas completas | ⏳ Pendiente |
| **Performance** | Carga de assets sin bloqueo UI (<100ms) | ⏳ Pendiente |
| **Compilación limpia** | 0 errores, 0 warnings | ✅ Completado |

---

## 📈 Progreso

**Historias completadas**: 2/5 (40%)  
**Tareas completadas**: 8/20 (40%)

```
┌────────────────────────────────────────────────────────────────────┐
│🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛ 40.0%│
└────────────────────────────────────────────────────────────────────┘
```

### Desglose por Historia

| Historia | Tareas | Completadas | Progreso |
|----------|--------|-------------|----------|
| **H1: Asset Database Core** | 4 | 4/4 | ✅ 100% |
| **H2: Texture Importer** | 4 | 4/4 | ✅ 100% |
| **H3: Mesh Importer** | 4 | 0/4 | ⏳ 0% |
| **H4: Asset Browser Panel** | 4 | 0/4 | ⏳ 0% |
| **H5: Scene Serialization** | 4 | 0/4 | ⏳ 0% |

---

## 🏗️ Arquitectura Propuesta

```cpp
// Asset Database (tracking de assets en disco) ✅ COMPLETADO
class AssetDatabase {
    std::unordered_map<AssetID, AssetMetadata> m_assets;
    std::unordered_map<std::string, AssetID> m_pathToID;
    
    AssetID ImportAsset(const std::string& path);
    AssetMetadata* GetAsset(AssetID id);
    void RefreshAssets(); // Detecta cambios en disco
};

// Asset Importers (PNG/JPG → DX12 Texture) ✅ COMPLETADO
class TextureImporter {
    TextureData ImportTexture(const std::string& path);
    TextureData ImportTextureRGBA(const std::string& path);
    bool GetTextureInfo(const std::string& path, int& w, int& h, int& ch);
    bool IsSupportedFormat(const std::string& path);
};

// DX12 Texture Upload ✅ COMPLETADO
class DX12ResourceManager {
    ID3D12Resource* CreateTexture2DFromData(
        unsigned char* data, int width, int height,
        ID3D12Resource** uploadBuffer);
};

// Mesh Importer (OBJ → buffers) ⏳ PENDIENTE
class MeshImporter {
    MeshData ImportOBJ(const std::string& path);
};

// Asset Browser Panel (UI para navegar assets) ⏳ PENDIENTE
class AssetBrowser {
    void RenderAssetBrowser();
    void OnAssetSelected(AssetID id);
    void OnAssetDoubleClick(AssetID id);
};

// Scene Serialization (save/load JSON) ⏳ PENDIENTE
class SceneSerializer {
    void SaveScene(Scene* scene, const std::string& path);
    Scene* LoadScene(const std::string& path);
};
```

---

## 🎯 Comparación con v1.8.0

| Aspecto | v1.8.0 | v1.9.0 (Este Sprint) |
|---------|--------|----------------------|
| **Assets** | Hardcoded (quad geometry) | ✅ Tracking con AssetDatabase |
| **Texturas** | No disponibles | ✅ PNG/JPG → DX12 Texture |
| **Meshes** | Quad hardcoded | ⏳ OBJ → Vertex/Index buffers |
| **Asset Browser** | No disponible | ⏳ Panel funcional con preview |
| **Serialization** | No disponible | ⏳ Save/load escenas JSON |
| **Workflow** | Manual (código) | ⏳ Profesional (importar → usar) |

---

## ✅ Logros del Sprint (hasta ahora)

### Historia H1: Asset Database Core ✅
- AssetDatabase singleton thread-safe implementado
- AssetID, AssetType, AssetMetadata structures
- Métodos: Register, Unregister, HasAsset, GetMetadata
- Asset folder structure creada (textures/, meshes/, shaders/, scenes/)
- Tests unitarios: 27 assertions passed ✓

### Historia H2: Texture Importer ✅
- stb_image library integrada (PNG, JPG, BMP, TGA, PSD, GIF, HDR, PIC)
- TextureImporter class con métodos ImportTexture, GetTextureInfo, IsSupportedFormat
- DX12ResourceManager::CreateTexture2DFromData() implementado
- Upload buffer, row pitch alignment, resource barriers
- Tests unitarios: 27 assertions passed ✓
- Imagen de prueba creada: `assets/textures/test_4x4.png`

---

## 📚 Referencias

- **Prerequisitos**: Sprint v1.8.0 completado ✅
- **Backlog**: BACK-005 (Asset System & Resource Management)
- **Inspiración**: Unity Asset Database, Unreal Asset Manager

---

*Última actualización*: 2025-01-21  
*Sprint*: v1.9.0 - Asset System & Resource Management - 🟢 **EN PROGRESO (40%)**
