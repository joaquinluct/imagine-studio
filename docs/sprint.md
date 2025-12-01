# Sprint v2.1.0 - Completar Material System (PBR)

> **Referencia**: Ver [`docs/MAIN.md`](MAIN.md) para los pilares fundamentales del proyecto.

---

## 🎯 Objetivo del Sprint

**Meta**: Completar el **Material System PBR** con rendering visual de texturas reales y hot-reload automático. Este sprint continúa el trabajo de v2.0.0 (H4.3 + H5 diferidas).

**Resultado esperado**: Al ejecutar la aplicación, el usuario verá:
1. Meshes renderizadas con texturas PBR reales (Brick)
2. Material Editor con preview real (render-to-texture)
3. Hot-reload automático al cambiar texturas en disco
4. Serialización JSON de materiales (.mat files)

---

## 📋 Criterios de Aceptación

1. ✅ Texturas PBR cargadas desde disco (JPG/PNG con STB)
2. ✅ 80 SRVs creados (16 materiales × 5 texturas)
3. ✅ Root signature PBR con texture slots (t0-t4)
4. ✅ PSO PBR completo (bind shaders, textures, constant buffers)
5. ✅ Meshes renderizadas con texturas PBR visibles en Viewport
6. ✅ SaveMaterial/LoadMaterial JSON funcional
7. ✅ FileWatcher detecta cambios en texturas
8. ✅ Hot-reload actualiza material sin reiniciar
9. ✅ Compilación limpia (0 errores, 0 warnings)

---

## 📊 Historias de Usuario

### **H1: Apply Material en Rendering** (6 tareas) - ~2.5 horas
**Objetivo**: Cargar texturas PBR y renderizar meshes con material visual

- H1.1: Crear TextureLoader con STB (cargar JPG/PNG desde disco)
- H1.2: Cargar texturas de Brick (5 texturas: Albedo, Normal, Roughness, Metallic, AO)
- H1.3: Crear 80 SRVs en descriptor heap (16 materiales × 5 texturas)
- H1.4: Actualizar root signature PBR (añadir texture slots t0-t4)
- H1.5: Crear PSO PBR completo (bind shaders compilados, root signature, input layout)
- H1.6: Bind texturas en rendering (SetGraphicsRootDescriptorTable, draw call)

**Criterios**:
- Texturas cargadas correctamente (log confirma: "Loaded texture: Brick_Albedo.jpg")
- SRVs creados sin errores (80 descriptors en heap)
- PSO PBR usa shaders pbr_vs.hlsl + pbr_ps.hlsl
- Meshes muestran texturas PBR en Viewport (visual confirmation)

---

### **H2: Serialization & Hot-Reload** (3 tareas) - ~1.5 horas
**Objetivo**: Guardar materiales en JSON y hot-reload automático

- H2.1: SaveMaterial/LoadMaterial JSON (.mat files en assets/materials/)
- H2.2: FileWatcher para detectar cambios en texturas (polling cada 1s)
- H2.3: Hot-reload al cambiar archivo (reload texture + recreate SRV)

**Criterios**:
- .mat files guardados correctamente (JSON válido)
- FileWatcher detecta cambios en <1s
- Hot-reload actualiza material en <100ms
- Log en Console: "Material reloaded: brick.mat"

---

## 📊 Progreso Sprint

**Total tareas**: 9  
**Completadas**: 6/9 (66.7%)  
**Historias**: 2 (H1 ✅, H2)

```
+--------------------------------------------------------------------+
████████████████████████████████████████████████████████████████████⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛ 66.7%
+--------------------------------------------------------------------+
```

---

**Estado**: 🚀 **EN PROGRESO**  
**Versión**: v2.1.0  
**Fecha inicio**: 2025-01-22  
**Duración estimada**: ~4 horas  
**Calificación AAA objetivo**: 9.5/10 ⭐⭐⭐

---

## 🔗 Referencias Técnicas

**STB Image Loader**:
```cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char* pixels = stbi_load("texture.jpg", &width, &height, &channels, STBI_rgb_alpha);
// ... create DX12 texture
stbi_image_free(pixels);
```

**Root Signature con Textures**:
```cpp
// Root parameter 0: MVP matrix (root constants)
// Root parameter 1: Material CB (register b1)
// Root parameter 2: Light CB (register b2)
// Root parameter 3: Texture table (5 SRVs - t0 to t4)
D3D12_DESCRIPTOR_RANGE srvRange = {};
srvRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
srvRange.NumDescriptors = 5;
srvRange.BaseShaderRegister = 0; // t0-t4
```

---

**Sprint anterior**: [v2.0.0 - Material System (PBR)](sprints/sprint_v2.0.0.md) ✅ (CERRADO 78.9%)
