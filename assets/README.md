# Assets Directory

This directory contains all game assets organized by type.

## Structure

```
assets/
??? textures/     # PNG, JPG, DDS texture files
??? meshes/       # OBJ, FBX mesh files
??? shaders/      # HLSL shader source files
??? scenes/       # JSON scene definition files
```

## File Naming Conventions

### Textures
- Use lowercase with underscores: `brick_diffuse.png`
- Suffix conventions:
  - `_diffuse` - Base color/albedo
  - `_normal` - Normal map
  - `_roughness` - Roughness map
  - `_metallic` - Metallic map
  - `_ao` - Ambient occlusion

### Meshes
- Use PascalCase: `CrateLarge.obj`
- Include LOD suffix if applicable: `Tree_LOD0.obj`, `Tree_LOD1.obj`

### Shaders
- Use lowercase with underscores: `pbr_standard.hlsl`
- Prefix with shader type:
  - `vs_` - Vertex shader
  - `ps_` - Pixel shader
  - `cs_` - Compute shader

### Scenes
- Use lowercase with underscores: `level_01.json`
- Prefix with category:
  - `level_` - Game levels
  - `test_` - Test scenes
  - `demo_` - Demo scenes

## Supported Formats

| Asset Type | Formats | Import Status |
|------------|---------|---------------|
| Textures   | PNG, JPG | ?? In Progress (H2) |
| Meshes     | OBJ | ?? In Progress (H3) |
| Shaders    | HLSL | ? Planned |
| Scenes     | JSON | ? Planned (H5) |

## Usage

Assets are automatically detected by the `AssetDatabase` when placed in these folders. Use the Asset Browser panel in the editor to import and manage assets.

---

*Last updated*: 2025-01-21  
*Sprint*: v1.9.0 - Asset System & Resource Management
