# Sprint v2.0.0 - Material System (PBR)

> **Referencia**: Ver [`docs/MAIN.md`](MAIN.md) para los pilares fundamentales del proyecto.

---

## ?? Objetivo del Sprint

**Meta**: Implementar un **Material System PBR** (Physically Based Rendering) completo que permita asignar texturas y propiedades físicas a meshes. El sistema debe soportar texturas PBR estándar (Albedo, Normal, Roughness, Metallic, AO) y permitir edición en tiempo real desde el Editor.

**Resultado esperado**: Al ejecutar la aplicación, el usuario podrá:
1. Crear materiales desde el Asset Browser
2. Asignar texturas PBR (drag & drop desde Asset Browser)
3. Editar propiedades en Material Editor panel
4. Ver preview en tiempo real en Viewport
5. Aplicar materiales a entities con MeshRenderer
6. Hot-reload de materiales sin recompilar

---

## ?? Criterios de Aceptación

1. ? Material class con propiedades PBR (albedo color, metallic, roughness)
2. ? Soporte texturas: Albedo, Normal, Roughness, Metallic, AO
3. ? Material Editor panel con drag & drop de texturas
4. ? Preview real-time en Viewport (shaders PBR)
5. ? MaterialInstance para crear variantes sin duplicar shaders
6. ? Hot-reload al cambiar texturas en disco
7. ? Serialización JSON de materiales (.mat files)
8. ? Compilación limpia (0 errores, 0 warnings)

---

## ?? Historias de Usuario

### **H1: Material Core** (4 tareas) - ~60 min
**Objetivo**: Crear clases base del sistema de materiales

- H1.1: Material.h con MaterialProperties struct (albedo, metallic, roughness, textureIDs)
- H1.2: Material.cpp con constructor, setters, getters
- H1.3: MaterialInstance class para variantes (hereda de Material)
- H1.4: MaterialManager singleton para tracking y hot-reload

**Criterios**:
- Material almacena 5 texture slots (albedo, normal, roughness, metallic, AO)
- MaterialProperties con valores default (albedo blanco, metallic 0.0, roughness 0.5)
- MaterialInstance comparte shader con Material base
- MaterialManager con cache de materiales cargados

---

### **H2: PBR Shader Pipeline** (5 tareas) - ~90 min
**Objetivo**: Implementar shaders PBR en DirectX 12

- H2.1: PBR vertex shader (transform, TBN matrix para normal mapping)
- H2.2: PBR pixel shader (Cook-Torrance BRDF, IBL opcional)
- H2.3: Constant buffers (Material CB, Light CB)
- H2.4: Descriptor heap para texturas (SRV)
- H2.5: PSO (Pipeline State Object) para PBR rendering

**Criterios**:
- Lighting model: Cook-Torrance specular + Lambert diffuse
- Normal mapping funcional (tangent space)
- Soporte mínimo 1 directional light
- Fallback a valores default si textura no presente
- Performance: <1ms render time para 1000 objetos

---

### **H3: Material Editor Panel** (4 tareas) - ~60 min
**Objetivo**: Panel ImGui para editar materiales

- H3.1: MaterialEditor.h/cpp con panel ImGui
- H3.2: Texture slots con drag & drop (Asset Browser ? Material Editor)
- H3.3: Property sliders (metallic, roughness, albedo color)
- H3.4: Preview thumbnail del material (render a texture)

**Criterios**:
- 5 texture slots visibles (cada uno 64x64 thumbnail)
- Drag & drop desde Asset Browser funcional
- Sliders 0.0-1.0 para metallic/roughness
- Color picker para albedo tint
- Preview actualizado en real-time

---

### **H4: Material Assignment** (3 tareas) - ~45 min
**Objetivo**: Conectar materiales con Entity System

- H4.1: Añadir Material* field a MeshRenderer component
- H4.2: Drag & drop material desde Asset Browser a Inspector
- H4.3: Apply material en rendering pipeline (bind textures + CB)

**Criterios**:
- MeshRenderer almacena puntero a Material
- Inspector muestra material asignado (nombre + preview)
- Drag & drop actualiza material del entity
- Rendering usa texturas del material asignado

---

### **H5: Material Serialization & Hot-Reload** (3 tareas) - ~45 min
**Objetivo**: Guardar materiales y hot-reload automático

- H5.1: SaveMaterial() y LoadMaterial() en JSON (.mat files)
- H5.2: FileWatcher para detectar cambios en texturas
- H5.3: Auto-reload al cambiar archivo en disco

**Criterios**:
- .mat files en assets/materials/
- Formato JSON con texture paths y properties
- Hot-reload en <100ms
- Log en Console panel: "Material reloaded: brick.mat"

---

## ?? Assets Disponibles

**Texturas PBR reales** (desde DirectX-Voxelado):
```
C:\Users\joaqu\source\repos\DirectX-Voxelado\Assets\Textures\
??? Brick/
?   ??? Poliigon_BrickWallReclaimed_8320_BaseColor.jpg
?   ??? Poliigon_BrickWallReclaimed_8320_Normal.png
?   ??? Poliigon_BrickWallReclaimed_8320_Roughness.jpg
?   ??? Poliigon_BrickWallReclaimed_8320_Metallic.jpg
?   ??? Poliigon_BrickWallReclaimed_8320_AmbientOcclusion.jpg
??? Dirt/, Grass/, Rock/, Sand/, Snow/ (similar estructura)
??? DefaultSkybox/
```

**Copiar a**: `assets/textures/pbr/` durante H1.1

---

## ?? Progreso Sprint

**Total tareas**: 19  
**Completadas**: 0/19 (0%)  
**Historias**: 5 (H1, H2, H3, H4, H5)

```
+--------------------------------------------------------------------+
 0.0%?
+--------------------------------------------------------------------+
```

---

**Estado**: ?? **EN PROGRESO**  
**Versión**: v2.0.0  
**Fecha inicio**: 2025-01-21  
**Duración estimada**: ~5 horas  
**Calificación AAA objetivo**: 9.5/10 ???

---

## ?? Referencias Técnicas

**Cook-Torrance BRDF**:
```hlsl
// Specular term (microfacet BRDF)
float D = DistributionGGX(NdotH, roughness);
float G = GeometrySmith(NdotV, NdotL, roughness);
float3 F = FresnelSchlick(VdotH, F0);

float3 specular = (D * G * F) / (4.0 * NdotV * NdotL + 0.001);
```

**Normal Mapping** (tangent space):
```hlsl
float3 tangentNormal = NormalTexture.Sample(sampler, uv).xyz * 2.0 - 1.0;
float3 N = normalize(TBN * tangentNormal);
```

---

**Sprint anterior**: [v1.9.1 - Console Integration](sprints/sprint_v1.9.1/sprint_v1.9.1.md) ? (CERRADO 100%)
