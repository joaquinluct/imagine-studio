#include "MeshImporter.h"
#include "core/Log.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace Assets {

// Thread-local error storage (thread-safe)
thread_local std::string MeshImporter::s_lastError = "";

// Helper: Check if string ends with suffix (case-insensitive)
bool MeshImporter::EndsWith(const std::string& str, const std::string& suffix)
{
    if (suffix.size() > str.size())
    {
        return false;
    }
    
    std::string strLower = str;
    std::string suffixLower = suffix;
    
    std::transform(strLower.begin(), strLower.end(), strLower.begin(), ::tolower);
    std::transform(suffixLower.begin(), suffixLower.end(), suffixLower.begin(), ::tolower);
    
    return strLower.compare(strLower.size() - suffixLower.size(), suffixLower.size(), suffixLower) == 0;
}

bool MeshImporter::IsSupportedFormat(const std::string& path)
{
    return EndsWith(path, ".obj");
}

std::string MeshImporter::GetLastError()
{
    return s_lastError;
}

bool MeshImporter::GetMeshInfo(const std::string& path, unsigned int& vertexCount, unsigned int& triangleCount)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        s_lastError = "Failed to open file: " + path;
        return false;
    }
    
    vertexCount = 0;
    triangleCount = 0;
    
    std::string line;
    while (std::getline(file, line))
    {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#')
        {
            continue;
        }
        
        // Trim leading whitespace
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos)
        {
            continue;
        }
        
        line = line.substr(start);
        
        // Count vertices
        if (line[0] == 'v' && (line.size() < 2 || line[1] == ' '))
        {
            vertexCount++;
        }
        // Count faces (triangles or quads)
        else if (line[0] == 'f' && line.size() > 1 && line[1] == ' ')
        {
            // Count spaces to determine vertex count in face
            unsigned int spaceCount = 0;
            for (size_t i = 0; i < line.size(); ++i)
            {
                if (line[i] == ' ' && i > 0 && line[i-1] != ' ')
                {
                    spaceCount++;
                }
            }
            
            // f v1 v2 v3 = 3 spaces, 1 triangle
            // f v1 v2 v3 v4 = 4 spaces, 2 triangles (quad)
            if (spaceCount >= 3)
            {
                triangleCount += (spaceCount >= 4) ? 2 : 1;
            }
        }
    }
    
    file.close();
    return true;
}

bool MeshImporter::ParseVertexPosition(const std::string& line, float& x, float& y, float& z)
{
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix; // Read "v"
    
    if (prefix != "v")
    {
        return false;
    }
    
    if (!(iss >> x >> y >> z))
    {
        return false;
    }
    
    return true;
}

bool MeshImporter::ParseVertexNormal(const std::string& line, float& nx, float& ny, float& nz)
{
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix; // Read "vn"
    
    if (prefix != "vn")
    {
        return false;
    }
    
    if (!(iss >> nx >> ny >> nz))
    {
        return false;
    }
    
    return true;
}

bool MeshImporter::ParseTextureCoord(const std::string& line, float& u, float& v)
{
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix; // Read "vt"
    
    if (prefix != "vt")
    {
        return false;
    }
    
    if (!(iss >> u >> v))
    {
        return false;
    }
    
    return true;
}

bool MeshImporter::ParseFace(const std::string& line,
                             const std::vector<float>& positions,
                             const std::vector<float>& normals,
                             const std::vector<float>& texCoords,
                             MeshData& outMesh)
{
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix; // Read "f"
    
    if (prefix != "f")
    {
        return false;
    }
    
    // Parse face vertices (3 or 4 vertices)
    std::vector<Vertex> faceVertices;
    std::string vertexStr;
    
    while (iss >> vertexStr)
    {
        // Parse vertex indices: v/vt/vn or v//vn or v/vt or v
        int posIdx = -1, texIdx = -1, normIdx = -1;
        
        // Find slashes
        size_t slash1 = vertexStr.find('/');
        size_t slash2 = vertexStr.find('/', slash1 + 1);
        
        if (slash1 == std::string::npos)
        {
            // Format: v
            posIdx = std::stoi(vertexStr);
        }
        else if (slash2 == std::string::npos)
        {
            // Format: v/vt
            posIdx = std::stoi(vertexStr.substr(0, slash1));
            texIdx = std::stoi(vertexStr.substr(slash1 + 1));
        }
        else
        {
            // Format: v/vt/vn or v//vn
            posIdx = std::stoi(vertexStr.substr(0, slash1));
            
            if (slash2 == slash1 + 1)
            {
                // Format: v//vn (no texture coords)
                normIdx = std::stoi(vertexStr.substr(slash2 + 1));
            }
            else
            {
                // Format: v/vt/vn
                texIdx = std::stoi(vertexStr.substr(slash1 + 1, slash2 - slash1 - 1));
                normIdx = std::stoi(vertexStr.substr(slash2 + 1));
            }
        }
        
        // OBJ indices are 1-based, convert to 0-based
        if (posIdx > 0) posIdx--;
        if (texIdx > 0) texIdx--;
        if (normIdx > 0) normIdx--;
        
        // Handle negative indices (relative to end)
        if (posIdx < 0) posIdx = static_cast<int>(positions.size() / 3) + posIdx;
        if (texIdx < 0) texIdx = static_cast<int>(texCoords.size() / 2) + texIdx;
        if (normIdx < 0) normIdx = static_cast<int>(normals.size() / 3) + normIdx;
        
        // Validate indices
        if (posIdx < 0 || posIdx >= static_cast<int>(positions.size() / 3))
        {
            s_lastError = "Invalid position index in face: " + std::to_string(posIdx + 1);
            return false;
        }
        
        // Create vertex
        Vertex vertex;
        
        // Position (required)
        vertex.position[0] = positions[posIdx * 3 + 0];
        vertex.position[1] = positions[posIdx * 3 + 1];
        vertex.position[2] = positions[posIdx * 3 + 2];
        
        // Normal (optional)
        if (normIdx >= 0 && normIdx < static_cast<int>(normals.size() / 3))
        {
            vertex.normal[0] = normals[normIdx * 3 + 0];
            vertex.normal[1] = normals[normIdx * 3 + 1];
            vertex.normal[2] = normals[normIdx * 3 + 2];
        }
        else
        {
            // Default normal (up)
            vertex.normal[0] = 0.0f;
            vertex.normal[1] = 1.0f;
            vertex.normal[2] = 0.0f;
        }
        
        // Texture coordinates (optional)
        if (texIdx >= 0 && texIdx < static_cast<int>(texCoords.size() / 2))
        {
            vertex.texCoord[0] = texCoords[texIdx * 2 + 0];
            vertex.texCoord[1] = texCoords[texIdx * 2 + 1];
        }
        else
        {
            // Default UV
            vertex.texCoord[0] = 0.0f;
            vertex.texCoord[1] = 0.0f;
        }
        
        faceVertices.push_back(vertex);
    }
    
    // Validate face vertex count
    if (faceVertices.size() < 3)
    {
        s_lastError = "Face has less than 3 vertices";
        return false;
    }
    
    // Triangulate face (support triangles and quads)
    unsigned int baseIndex = static_cast<unsigned int>(outMesh.vertices.size());
    
    if (faceVertices.size() == 3)
    {
        // Triangle: add 3 vertices and 3 indices
        outMesh.vertices.push_back(faceVertices[0]);
        outMesh.vertices.push_back(faceVertices[1]);
        outMesh.vertices.push_back(faceVertices[2]);
        
        outMesh.indices.push_back(baseIndex + 0);
        outMesh.indices.push_back(baseIndex + 1);
        outMesh.indices.push_back(baseIndex + 2);
    }
    else if (faceVertices.size() == 4)
    {
        // Quad: triangulate as 2 triangles (0,1,2) and (0,2,3)
        outMesh.vertices.push_back(faceVertices[0]);
        outMesh.vertices.push_back(faceVertices[1]);
        outMesh.vertices.push_back(faceVertices[2]);
        outMesh.vertices.push_back(faceVertices[3]);
        
        // Triangle 1: (0, 1, 2)
        outMesh.indices.push_back(baseIndex + 0);
        outMesh.indices.push_back(baseIndex + 1);
        outMesh.indices.push_back(baseIndex + 2);
        
        // Triangle 2: (0, 2, 3)
        outMesh.indices.push_back(baseIndex + 0);
        outMesh.indices.push_back(baseIndex + 2);
        outMesh.indices.push_back(baseIndex + 3);
    }
    else
    {
        // N-gon (n > 4): fan triangulation from first vertex
        for (size_t i = 1; i < faceVertices.size() - 1; ++i)
        {
            outMesh.vertices.push_back(faceVertices[0]);
            outMesh.vertices.push_back(faceVertices[i]);
            outMesh.vertices.push_back(faceVertices[i + 1]);
            
            unsigned int triBase = static_cast<unsigned int>(outMesh.vertices.size()) - 3;
            outMesh.indices.push_back(triBase + 0);
            outMesh.indices.push_back(triBase + 1);
            outMesh.indices.push_back(triBase + 2);
        }
    }
    
    return true;
}

MeshData MeshImporter::ImportOBJ(const std::string& path)
{
    CORE_LOG_INFO("MeshImporter: Importing OBJ file: " + path);
    
    // Check format
    if (!IsSupportedFormat(path))
    {
        s_lastError = "Unsupported file format (expected .obj): " + path;
        throw std::runtime_error(s_lastError);
    }
    
    // Open file
    std::ifstream file(path);
    if (!file.is_open())
    {
        s_lastError = "Failed to open file: " + path;
        throw std::runtime_error(s_lastError);
    }
    
    // Temporary storage for OBJ data
    std::vector<float> positions;    // v x y z (3 floats per vertex)
    std::vector<float> normals;      // vn nx ny nz (3 floats per normal)
    std::vector<float> texCoords;    // vt u v (2 floats per texcoord)
    
    MeshData mesh;
    mesh.sourcePath = path;
    
    // Extract filename from path
    size_t lastSlash = path.find_last_of("/\\");
    if (lastSlash != std::string::npos)
    {
        mesh.name = path.substr(lastSlash + 1);
        // Remove extension
        size_t dot = mesh.name.find_last_of('.');
        if (dot != std::string::npos)
        {
            mesh.name = mesh.name.substr(0, dot);
        }
    }
    else
    {
        mesh.name = path;
    }
    
    // Parse file line by line
    std::string line;
    unsigned int lineNumber = 0;
    
    while (std::getline(file, line))
    {
        lineNumber++;
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#')
        {
            continue;
        }
        
        // Trim leading whitespace
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos)
        {
            continue;
        }
        
        line = line.substr(start);
        
        // Parse line based on prefix
        if (line[0] == 'v')
        {
            if (line.size() > 1 && line[1] == ' ')
            {
                // Vertex position: v x y z
                float x, y, z;
                if (ParseVertexPosition(line, x, y, z))
                {
                    positions.push_back(x);
                    positions.push_back(y);
                    positions.push_back(z);
                }
                else
                {
                    CORE_LOG_WARN("MeshImporter: Failed to parse vertex position at line " + std::to_string(lineNumber));
                }
            }
            else if (line.size() > 1 && line[1] == 'n')
            {
                // Vertex normal: vn nx ny nz
                float nx, ny, nz;
                if (ParseVertexNormal(line, nx, ny, nz))
                {
                    normals.push_back(nx);
                    normals.push_back(ny);
                    normals.push_back(nz);
                }
                else
                {
                    CORE_LOG_WARN("MeshImporter: Failed to parse vertex normal at line " + std::to_string(lineNumber));
                }
            }
            else if (line.size() > 1 && line[1] == 't')
            {
                // Texture coordinate: vt u v
                float u, v;
                if (ParseTextureCoord(line, u, v))
                {
                    texCoords.push_back(u);
                    texCoords.push_back(v);
                }
                else
                {
                    CORE_LOG_WARN("MeshImporter: Failed to parse texture coordinate at line " + std::to_string(lineNumber));
                }
            }
        }
        else if (line[0] == 'f' && line.size() > 1 && line[1] == ' ')
        {
            // Face: f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
            if (!ParseFace(line, positions, normals, texCoords, mesh))
            {
                CORE_LOG_ERROR("MeshImporter: Failed to parse face at line " + std::to_string(lineNumber) + ": " + s_lastError);
                file.close();
                throw std::runtime_error("Failed to parse face at line " + std::to_string(lineNumber) + ": " + s_lastError);
            }
        }
        // Ignore other lines (materials, groups, etc.)
    }
    
    file.close();
    
    // Validate mesh
    if (mesh.vertices.empty())
    {
        s_lastError = "No vertices found in OBJ file: " + path;
        throw std::runtime_error(s_lastError);
    }
    
    if (mesh.indices.empty())
    {
        s_lastError = "No faces found in OBJ file: " + path;
        throw std::runtime_error(s_lastError);
    }
    
    // Calculate bounding box
    mesh.CalculateBounds();
    
    CORE_LOG_INFO("MeshImporter: Successfully imported mesh: " + mesh.name);
    CORE_LOG_INFO("  Vertices: " + std::to_string(mesh.GetVertexCount()));
    CORE_LOG_INFO("  Indices: " + std::to_string(mesh.GetIndexCount()));
    CORE_LOG_INFO("  Triangles: " + std::to_string(mesh.GetTriangleCount()));
    CORE_LOG_INFO("  Memory: " + std::to_string(mesh.GetMemorySize()) + " bytes");
    
    return mesh;
}

} // namespace Assets
