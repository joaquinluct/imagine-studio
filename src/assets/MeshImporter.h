#pragma once

#include "MeshData.h"
#include <string>

namespace Assets {

// MeshImporter - Import mesh files (OBJ format) to MeshData
// Similar to TextureImporter, this is a static utility class
class MeshImporter {
public:
    // Import mesh from OBJ file
    // Returns MeshData with vertices, indices, normals, UVs
    // Throws std::runtime_error on failure
    static MeshData ImportOBJ(const std::string& path);
    
    // Check if file format is supported for mesh import
    // Supported formats: .obj
    static bool IsSupportedFormat(const std::string& path);
    
    // Get mesh info without loading full data (fast)
    // Returns true if file exists and is valid OBJ
    // vertexCount, triangleCount are output parameters
    static bool GetMeshInfo(const std::string& path, unsigned int& vertexCount, unsigned int& triangleCount);
    
    // Get last error message (if ImportOBJ failed)
    static std::string GetLastError();

private:
    // Private constructor (static utility class)
    MeshImporter() = delete;
    
    // Helper: Parse vertex position line (v x y z)
    static bool ParseVertexPosition(const std::string& line, float& x, float& y, float& z);
    
    // Helper: Parse vertex normal line (vn nx ny nz)
    static bool ParseVertexNormal(const std::string& line, float& nx, float& ny, float& nz);
    
    // Helper: Parse texture coordinate line (vt u v)
    static bool ParseTextureCoord(const std::string& line, float& u, float& v);
    
    // Helper: Parse face line (f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3)
    // OBJ faces can be triangles or quads, we triangulate quads
    static bool ParseFace(const std::string& line, 
                         const std::vector<float>& positions,
                         const std::vector<float>& normals,
                         const std::vector<float>& texCoords,
                         MeshData& outMesh);
    
    // Helper: Check if string ends with suffix (case-insensitive)
    static bool EndsWith(const std::string& str, const std::string& suffix);
    
    // Last error message (thread-local for thread safety)
    static thread_local std::string s_lastError;
};

} // namespace Assets
