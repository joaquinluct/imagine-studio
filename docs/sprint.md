# Sprint v1.9.0 - Asset System & Resource Management

> **Estado**: 🟢 En planificación  
> **Fecha inicio**: 2025-01-18  
> **Fecha fin estimada**: 2025-02-01

---

## 🎯 Objetivo del Sprint

**Meta**: Implementar Asset System completo para cargar contenido real (texturas, meshes, shaders) desde disco, con Asset Browser panel en editor y flujo de trabajo profesional (importar → usar → guardar).

**Resultado esperado**: Al finalizar el sprint, el editor tendrá:
- Asset Database funcional con tracking de archivos
- Importers para Textures, Meshes y Shaders
- Asset Browser panel para navegar contenido
- Drag & drop de assets al Viewport/Inspector
- Scene serialization (save/load JSON)

---

## 📋 Historias de Usuario

Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias detalladas.

| ID | Historia | Prioridad | Estado |
|----|----------|-----------|--------|
| H1 | Asset Database Core | 🔴 Crítica | ⏳ Pendiente |
| H2 | Texture Importer | 🔴 Crítica | ⏳ Pendiente |
| H3 | Mesh Importer | 🟡 Alta | ⏳ Pendiente |
| H4 | Asset Browser Panel | 🟡 Alta | ⏳ Pendiente |
| H5 | Scene Serialization | 🟡 Alta | ⏳ Pendiente |

---

## 📊 Métricas de Éxito

| Métrica | Objetivo |
|---------|----------|
| **Formatos soportados** | PNG, JPG (texturas), OBJ (meshes), HLSL (shaders) |
| **Asset Browser** | Navegación de carpetas funcional |
| **Drag & drop** | Texturas y meshes al Viewport |
| **Serialization** | Save/load escenas completas |
| **Performance** | Carga de assets sin bloqueo UI (<100ms) |
| **Compilación limpia** | 0 errores, 0 warnings |

---

## 📈 Progreso

**Historias completadas**: 0/5 (0%)  
**Tareas completadas**: 0/20 (0%)

```
┌────────────────────────────────────────────────────────────────────┐
│⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛ 0.0%│
└────────────────────────────────────────────────────────────────────┘
```

---

## 🏗️ Arquitectura Propuesta

```cpp
// Asset Database (tracking de assets en disco)
class AssetDatabase {
    std::unordered_map<AssetID, AssetMetadata> m_assets;
    std::unordered_map<std::string, AssetID> m_pathToID;
    
    AssetID ImportAsset(const std::string& path);
    AssetMetadata* GetAsset(AssetID id);
    void RefreshAssets(); // Detecta cambios en disco
};

// Asset Importers (PNG/JPG → DX12 Texture)
class TextureImporter {
    ID3D12Resource* ImportTexture(const std::string& path);
    void GenerateMipmaps(ID3D12Resource* texture);
};

class MeshImporter {
    MeshData ImportOBJ(const std::string& path);
};

// Asset Browser Panel (UI para navegar assets)
class AssetBrowser {
    void RenderAssetBrowser();
    void OnAssetSelected(AssetID id);
    void OnAssetDoubleClick(AssetID id);
};

// Scene Serialization (save/load JSON)
class SceneSerializer {
    void SaveScene(Scene* scene, const std::string& path);
    Scene* LoadScene(const std::string& path);
};
```

---

## 🎯 Comparación con v1.8.0

| Aspecto | v1.8.0 | v1.9.0 (Este Sprint) |
|---------|--------|----------------------|
| **Assets** | Hardcoded (quad geometry) | Importados desde disco |
| **Texturas** | No disponibles | PNG/JPG → DX12 Texture |
| **Meshes** | Quad hardcoded | OBJ → Vertex/Index buffers |
| **Asset Browser** | No disponible | Panel funcional con preview |
| **Serialization** | No disponible | Save/load escenas JSON |
| **Workflow** | Manual (código) | Profesional (importar → usar) |

---

## 📚 Referencias

- **Prerequisitos**: Sprint v1.8.0 completado ✅
- **Backlog**: BACK-005 (Asset System & Resource Management)
- **Inspiración**: Unity Asset Database, Unreal Asset Manager

---

*Última actualización*: 2025-01-18  
*Sprint*: v1.9.0 - Asset System & Resource Management - 🟢 **EN PLANIFICACIÓN**
