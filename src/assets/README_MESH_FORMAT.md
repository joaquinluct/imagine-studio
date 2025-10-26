# MeshImporter - OBJ Format Support

## Overview

MeshImporter loads 3D mesh files in **Wavefront OBJ** format (.obj) and converts them to `MeshData` structures suitable for GPU rendering.

## Supported OBJ Features

### Vertex Data
- **Positions** (`v x y z`) - 3D coordinates
- **Normals** (`vn nx ny nz`) - Surface normals for lighting
- **Texture Coordinates** (`vt u v`) - UV mapping for textures

### Faces
- **Triangles** (`f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3`)
- **Quads** (`f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 v4/vt4/vn4`) - Automatically triangulated
- **Index Formats**:
  - `f v1 v2 v3` (positions only)
  - `f v1/vt1 v2/vt2 v3/vt3` (positions + UVs)
  - `f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3` (positions + UVs + normals)
  - `f v1//vn1 v2//vn2 v3//vn3` (positions + normals, no UVs)

## NOT Supported (Future)
- Materials (`.mtl` files) - Will be in H4 (Asset Browser)
- Multiple objects in one file
- Smooth shading groups (`s`)
- Free-form curves/surfaces

## Example OBJ File (Cube)

```obj
# Cube mesh (8 vertices, 12 triangles)
v -1.0 -1.0  1.0  # Front bottom-left
v  1.0 -1.0  1.0  # Front bottom-right
v  1.0  1.0  1.0  # Front top-right
v -1.0  1.0  1.0  # Front top-left
v -1.0 -1.0 -1.0  # Back bottom-left
v  1.0 -1.0 -1.0  # Back bottom-right
v  1.0  1.0 -1.0  # Back top-right
v -1.0  1.0 -1.0  # Back top-left

# Normals
vn  0.0  0.0  1.0  # Front
vn  0.0  0.0 -1.0  # Back
vn  1.0  0.0  0.0  # Right
vn -1.0  0.0  0.0  # Left
vn  0.0  1.0  0.0  # Top
vn  0.0 -1.0  0.0  # Bottom

# Texture coordinates
vt 0.0 0.0
vt 1.0 0.0
vt 1.0 1.0
vt 0.0 1.0

# Faces (front)
f 1/1/1 2/2/1 3/3/1
f 1/1/1 3/3/1 4/4/1

# ... (more faces for other sides)
```

## Usage

```cpp
#include "assets/MeshImporter.h"

// Import mesh from OBJ file
MeshData mesh = MeshImporter::ImportOBJ("assets/meshes/cube.obj");

// Check if valid
if (mesh.IsValid())
{
    // Access mesh data
    unsigned int vertexCount = mesh.GetVertexCount();
    unsigned int triangleCount = mesh.GetTriangleCount();
    
    // Calculate bounding box for culling
    mesh.CalculateBounds();
    
    // Use mesh data for GPU upload (H3.3 - DX12 buffers)
    // ...
}
```

## Error Handling

```cpp
try
{
    MeshData mesh = MeshImporter::ImportOBJ("invalid.obj");
}
catch (const std::runtime_error& e)
{
    // Get detailed error
    std::string error = MeshImporter::GetLastError();
}
```

## Performance Notes

- **GetMeshInfo()** is fast - only counts vertices/triangles without loading full data
- **ImportOBJ()** loads complete mesh - may take time for large files
- **CalculateBounds()** is O(n) - call once after loading
- OBJ parser is single-threaded (multi-threading in future sprints)

## Coordinate System

- **Right-handed** coordinate system (OpenGL/Unity style)
- **Y-up** axis (positive Y is "up")
- **Z-forward** axis (negative Z is "into screen")
- DirectX 12 uses left-handed, so matrices must account for this

## Memory Layout

```cpp
struct Vertex {
    float position[3];  // 12 bytes (x, y, z)
    float normal[3];    // 12 bytes (nx, ny, nz)
    float texCoord[2];  // 8 bytes (u, v)
};  // Total: 32 bytes per vertex

// Indices: 4 bytes per index (unsigned int)
```

## Integration with Asset System

1. **H3.1** (this task): Define MeshData structure ?
2. **H3.2**: Implement OBJ parser ? MeshData
3. **H3.3**: DX12 vertex/index buffer creation
4. **H3.4**: Unit tests with cube.obj

---

**Created**: 2025-01-21  
**Sprint**: v1.9.0 - Asset System  
**Task**: H3.1 - Planificar Mesh Importer
