#pragma once

#include <string>
#include <vector>

namespace Assets {

// Vertex structure for mesh data (position, normal, UV)
struct Vertex {
    float position[3];  // x, y, z
    float normal[3];    // nx, ny, nz
    float texCoord[2];  // u, v
    
    Vertex()
        : position{0.0f, 0.0f, 0.0f}
        , normal{0.0f, 1.0f, 0.0f}
        , texCoord{0.0f, 0.0f}
    {
    }
    
    Vertex(float px, float py, float pz, float nx, float ny, float nz, float u, float v)
        : position{px, py, pz}
        , normal{nx, ny, nz}
        , texCoord{u, v}
    {
    }
};

// Axis-Aligned Bounding Box (AABB) for frustum culling
struct BoundingBox {
    float min[3];  // Min corner (x, y, z)
    float max[3];  // Max corner (x, y, z)
    
    BoundingBox()
        : min{0.0f, 0.0f, 0.0f}
        , max{0.0f, 0.0f, 0.0f}
    {
    }
    
    BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
        : min{minX, minY, minZ}
        , max{maxX, maxY, maxZ}
    {
    }
    
    // Calculate center of bounding box
    void GetCenter(float& x, float& y, float& z) const
    {
        x = (min[0] + max[0]) * 0.5f;
        y = (min[1] + max[1]) * 0.5f;
        z = (min[2] + max[2]) * 0.5f;
    }
    
    // Calculate size of bounding box
    void GetSize(float& width, float& height, float& depth) const
    {
        width = max[0] - min[0];
        height = max[1] - min[1];
        depth = max[2] - min[2];
    }
};

// MeshData structure containing all mesh information
struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    BoundingBox bounds;
    std::string name;
    std::string sourcePath;
    
    MeshData() = default;
    
    MeshData(const std::string& meshName)
        : name(meshName)
    {
    }
    
    // Check if mesh data is valid
    bool IsValid() const
    {
        return !vertices.empty() && !indices.empty();
    }
    
    // Get vertex count
    unsigned int GetVertexCount() const
    {
        return static_cast<unsigned int>(vertices.size());
    }
    
    // Get index count
    unsigned int GetIndexCount() const
    {
        return static_cast<unsigned int>(indices.size());
    }
    
    // Get triangle count (indices / 3)
    unsigned int GetTriangleCount() const
    {
        return GetIndexCount() / 3;
    }
    
    // Calculate memory size in bytes
    unsigned int GetMemorySize() const
    {
        unsigned int vertexSize = GetVertexCount() * sizeof(Vertex);
        unsigned int indexSize = GetIndexCount() * sizeof(unsigned int);
        return vertexSize + indexSize;
    }
    
    // Calculate bounding box from vertices
    void CalculateBounds()
    {
        if (vertices.empty())
        {
            bounds = BoundingBox();
            return;
        }
        
        // Initialize with first vertex
        bounds.min[0] = bounds.max[0] = vertices[0].position[0];
        bounds.min[1] = bounds.max[1] = vertices[0].position[1];
        bounds.min[2] = bounds.max[2] = vertices[0].position[2];
        
        // Expand to include all vertices
        for (size_t i = 1; i < vertices.size(); ++i)
        {
            const Vertex& v = vertices[i];
            
            // Update min
            if (v.position[0] < bounds.min[0]) bounds.min[0] = v.position[0];
            if (v.position[1] < bounds.min[1]) bounds.min[1] = v.position[1];
            if (v.position[2] < bounds.min[2]) bounds.min[2] = v.position[2];
            
            // Update max
            if (v.position[0] > bounds.max[0]) bounds.max[0] = v.position[0];
            if (v.position[1] > bounds.max[1]) bounds.max[1] = v.position[1];
            if (v.position[2] > bounds.max[2]) bounds.max[2] = v.position[2];
        }
    }
    
    // Clear all mesh data
    void Clear()
    {
        vertices.clear();
        indices.clear();
        bounds = BoundingBox();
        name.clear();
        sourcePath.clear();
    }
};

} // namespace Assets
