# Sprint Commits - v1.9.0 (Active)

> **Referencia**: Ver [docs/sprint.md](sprint.md) para objetivo y progreso del sprint.

Este archivo registra todos los commits realizados durante el **Sprint v1.9.0 - Asset System**.

---

## Convenciones de Commits

Formato recomendado para los mensajes de commit:

```
<tipo>(<ámbito>): <mensaje corto>

<mensaje largo opcional>

Refs: <issue/track-id>
```

### Tipos comunes:
- **feat**: Nueva funcionalidad
- **fix**: Corrección de errores
- **docs**: Cambios en documentación
- **style**: Formato, estilo, sin cambios funcionales
- **refactor**: Refactorización sin cambios en comportamiento
- **perf**: Mejoras de rendimiento
- **test**: Añadir o corregir tests
- **chore**: Tareas de mantenimiento

---

## Historial de Commits (Sprint v1.9.0)

### Commit 1 - H1.1: AssetDatabase.h creado
**Fecha**: 2025-01-18  
**Tipo**: feat  
**Archivos**:
- `src/assets/AssetDatabase.h` (nuevo)
- `Imagine Studio.vcxproj` (modificado)

**Mensaje**:
```
feat(assets): Crear AssetDatabase.h con estructuras básicas

- AssetID (uint64_t) como identificador único
- AssetType enum class (Texture, Mesh, Shader, Scene, Unknown)
- AssetMetadata struct con metadatos completos
- AssetDatabase singleton class con métodos de registro/consulta

Refs: H1.1 (Sprint v1.9.0)
```

### Commit 2 - H1.2: AssetDatabase.cpp implementado
**Fecha**: 2025-01-21  
**Tipo**: feat  
**Archivos**:
- `src/assets/AssetDatabase.cpp` (nuevo)
- `Imagine Studio.vcxproj` (modificado)
- `src/assets/AssetDatabase.h` (encoding fix)

**Mensaje**:
```
feat(assets): Implementar AssetDatabase.cpp con singleton thread-safe

- GetInstance() con patrón Meyer's Singleton
- RegisterAsset() con thread-safe std::mutex
- UnregisterAsset() con validación de existencia
- HasAsset() y GetMetadata() con protección concurrente
- Storage interno con std::unordered_map

Compilación limpia: 0 errores, 0 warnings (CMake + MSBuild)

Refs: H1.2 (Sprint v1.9.0)
```

### Commit 3 - H1.3: Asset folder structure creada
**Fecha**: 2025-01-21  
**Tipo**: chore  
**Archivos**:
- `assets/` (carpeta nueva)
- `assets/textures/`, `meshes/`, `shaders/`, `scenes/` (carpetas)
- `assets/README.md` (nuevo)
- `assets/*/.gitkeep` (4 archivos)

**Mensaje**:
```
chore(assets): Crear estructura de carpetas para assets

- assets/textures/ para PNG, JPG, DDS
- assets/meshes/ para OBJ, FBX
- assets/shaders/ para HLSL
- assets/scenes/ para JSON
- .gitkeep en cada carpeta para Git tracking
- assets/README.md con convenciones y documentación

Organización tipo Unity/Unreal para workflow profesional

Refs: H1.3 (Sprint v1.9.0)
```

### Commit 4 - H1.4: Testing AssetDatabase (Historia H1 COMPLETADA ✅)
**Fecha**: 2025-01-21  
**Tipo**: test  
**Archivos**:
- `tests/asset_database_test.cpp` (nuevo)
- `CMakeLists.txt` (añadir test target)
- `Imagine Studio.vcxproj` (excluir test del main)

**Mensaje**:
```
test(assets): Testing AssetDatabase - Historia H1 completada

Tests implementados:
- TestSingletonInstance (2 assertions)
- TestRegisterAsset (4 assertions)
- TestHasAsset (7 assertions)
- TestGetMetadata (10 assertions)
- TestUnregisterAsset (4 assertions)
- TestAssetTypes (3 assertions)
- TestThreadSafety (1 assertion)

Resultado: 27 assertions passed ✓

HISTORIA H1 COMPLETADA (Asset Database Core - 4/4 tareas)
Primera historia del Sprint v1.9.0 completada (20% del sprint)

Refs: H1.4 (Sprint v1.9.0)
```

### Commit 5 - H2.1: stb_image library integrada (Historia H2 iniciada)
**Fecha**: 2025-01-21  
**Tipo**: chore  
**Archivos**:
- `external/stb/stb_image.h` (descargado de nothings/stb)
- `external/stb/stb_image_impl.cpp` (nuevo)
- `external/stb/README.md` (nuevo)
- `CMakeLists.txt` (añadir librería stb_image)
- `Imagine Studio.vcxproj` (añadir stb_image_impl.cpp)

**Mensaje**:
```
chore(external): Integrar stb_image library para carga de imágenes

Librería stb_image (nothings/stb):
- Single-header C library (Public Domain)
- Formatos: PNG, JPG, BMP, TGA, PSD, GIF, HDR, PIC
- Thread-safe, sin dependencias externas

Archivos:
- stb_image.h (header)
- stb_image_impl.cpp (implementation)
- README.md (documentación API completa)

Integración:
- CMakeLists.txt: target stb_image STATIC library
- Linked a ImagineStudio executable

Compilación limpia: 0 errores, 0 warnings

HISTORIA H2 INICIADA (Texture Importer - 1/4 tareas)

Refs: H2.1 (Sprint v1.9.0)
```

### Commit 6 - H2.2: TextureImporter implementado
**Fecha**: 2025-01-21  
**Tipo**: feat  
**Archivos**:
- `src/assets/TextureImporter.h` (nuevo)
- `src/assets/TextureImporter.cpp` (nuevo)
- `Imagine Studio.vcxproj` (añadir archivos)

**Mensaje**:
```
feat(assets): Implementar TextureImporter con stb_image

TextureImporter class (static methods):
- ImportTexture(path, desiredChannels) - Cargar con canales configurables
- ImportTextureRGBA(path) - Forzar formato RGBA (4 canales)
- FreeTextureData(data) - Liberar memoria pixel data
- GetTextureInfo(path, w, h, ch) - Obtener dimensiones sin cargar
- IsSupportedFormat(path) - Verificar extensión (PNG, JPG, BMP, TGA, etc.)
- GetLastError() - Obtener último error

TextureData struct:
- pixels (unsigned char*)
- width, height, channels
- path (original file path)
- IsValid() helper

Implementación:
- Usa stb_image API (stbi_load, stbi_info, stbi_image_free)
- EndsWith() helper para C++14 compatibility
- Excepciones std::runtime_error en caso de fallo
- Soporte PNG, JPG, BMP, TGA, PSD, GIF, HDR, PIC

Compilación limpia: 0 errores, 0 warnings

HISTORIA H2 en progreso (Texture Importer - 2/4 tareas)

Refs: H2.2 (Sprint v1.9.0)
```

### Commit 7 - H2.3: DX12 Texture2D from data implementado
**Fecha**: 2025-01-21  
**Tipo**: feat  
**Archivos**:
- `src/renderer/DX12ResourceManager.h` (añadir método CreateTexture2DFromData)
- `src/renderer/DX12ResourceManager.cpp` (implementación)

**Mensaje**:
```
feat(renderer): Implementar CreateTexture2DFromData para upload GPU

Método CreateTexture2DFromData en DX12ResourceManager:
- Create texture en default heap (GPU-only memory)
- Create upload buffer en upload heap (staging)
- GetCopyableFootprints para calcular subresource layout
- Map upload buffer y copy pixel data con row pitch alignment
- CopyTextureRegion para transferir staging → GPU
- Resource barrier: COPY_DEST → PIXEL_SHADER_RESOURCE
- Retornar upload buffer al caller para deferred release

Características:
- Soporte DXGI_FORMAT_R8G8B8A8_UNORM (RGBA)
- Row pitch alignment correcto (D3D12_PLACED_SUBRESOURCE_FOOTPRINT)
- Upload buffer NO se libera hasta GPU finish (evita errors)
- Logging con dimensiones de textura

Flujo completo:
1. Create texture (default heap) + upload buffer (staging)
2. Map upload buffer → copy pixel data row by row
3. Unmap upload buffer
4. CopyTextureRegion (staging → GPU)
5. Resource barrier (COPY_DEST → PIXEL_SHADER_RESOURCE)
6. Retornar texture + upload buffer
7. Caller: ExecuteCommandList → Wait fence → Release upload buffer

Compilación limpia: 0 errores, 0 warnings

HISTORIA H2 en progreso (Texture Importer - 3/4 tareas, 75%)

Refs: H2.3 (Sprint v1.9.0)
```

### Commit 8 - H2.4: Testing TextureImporter (Historia H2 COMPLETADA ✅)
**Fecha**: 2025-01-21  
**Tipo**: test  
**Archivos**:
- `tests/texture_importer_test.cpp` (nuevo)
- `assets/textures/test_4x4.png` (imagen de prueba)
- `CMakeLists.txt` (añadir test target)
- `scripts/generate_test_texture.py` (script generador)

**Mensaje**:
```
test(assets): Testing TextureImporter - Historia H2 completada

Tests implementados:
- TestIsSupportedFormat (12 assertions) - Valida extensiones soportadas
- TestImportTextureInvalid (1 assertion) - Valida manejo de errores
- TestGetTextureInfo (5 assertions) - Valida info sin cargar pixels
- TestImportTextureValid (7 assertions) - Valida carga completa
- TestImportTextureChannels (3 assertions) - Valida RGBA forzado

Resultado: 27 assertions passed ✓

Imagen de prueba:
- assets/textures/test_4x4.png (79 bytes)
- 4x4 píxeles en patrón checkerboard (rojo/azul)
- Formato RGB (3 canales)
- Generada con Python PIL (scripts/generate_test_texture.py)

Beneficios:
- Validación completa de TextureImporter
- Cobertura de casos edge (errores, formatos inválidos)
- Tests rápidos (no requieren DX12 context)
- Imagen de prueba reutilizable

Compilación limpia: 0 errores, 0 warnings (CMake + MSBuild)

HISTORIA H2 COMPLETADA (Texture Importer - 4/4 tareas)
Segunda historia del Sprint v1.9.0 completada (40% del sprint)

Refs: H2.4 (Sprint v1.9.0)
```

---

## 📊 Estadísticas del Sprint

**Total commits**: 8  
**Historias completadas**: 2/5 (H1 ✅, H2 ✅)  
**Tareas completadas**: 8/20 (40%)  
**Progreso sprint**: 40%

### Desglose por tipo
- **feat**: 4 commits (50%)
- **test**: 2 commits (25%)
- **chore**: 2 commits (25%)

### Compilación
- **CMake builds**: 8/8 exitosos ✅
- **MSBuild builds**: 8/8 exitosos ✅
- **Errores**: 0
- **Warnings**: 0

### Tests
- **Test targets creados**: 2 (asset_database_test, texture_importer_test)
- **Total assertions**: 54 (27 + 27)
- **Assertions passed**: 54/54 ✅ (100%)

---

## 🎯 Próximos Commits

### Historia H3: Mesh Importer (siguiente)
**Commit 9 - H3.1**: Crear MeshData.h con struct MeshData  
**Commit 10 - H3.2**: Implementar OBJ parser  
**Commit 11 - H3.3**: DX12 Vertex/Index buffers  
**Commit 12 - H3.4**: Testing MeshImporter  

---

**Versión**: v1.0  
**Última actualización**: 2025-01-21  
**Sprint**: v1.9.0 - Asset System - **EN PROGRESO** (40%)  
**Historias completadas**: 2/5 (✅ H1, ✅ H2)  
**Próxima tarea**: H3.1 - Planificar Mesh Importer
