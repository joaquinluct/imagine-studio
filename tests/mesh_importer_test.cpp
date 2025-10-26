// tests/mesh_importer_test.cpp
// Unit tests for MeshImporter (OBJ parser)

#include "assets/MeshImporter.h"
#include "core/Log.h"

#include <iostream>
#include <fstream>
#include <cmath>

using namespace Assets;

// Helper: Compare floats with epsilon
bool FloatEqual(float a, float b, float epsilon = 0.0001f)
{
    return std::abs(a - b) < epsilon;
}

// Helper: Compare float3 arrays
bool Float3Equal(const float a[3], const float b[3], float epsilon = 0.0001f)
{
    return FloatEqual(a[0], b[0], epsilon) &&
           FloatEqual(a[1], b[1], epsilon) &&
           FloatEqual(a[2], b[2], epsilon);
}

// Helper: Compare float2 arrays
bool Float2Equal(const float a[2], const float b[2], float epsilon = 0.0001f)
{
    return FloatEqual(a[0], b[0], epsilon) &&
           FloatEqual(a[1], b[1], epsilon);
}

// ============================================
// TEST 1: IsSupportedFormat
// ============================================
void TestIsSupportedFormat()
{
    std::cout << "\n[TEST] TestIsSupportedFormat" << std::endl;
    
    unsigned int passedTests = 0;
    
    try {
        // Test 1: .obj extension (lowercase)
        if (MeshImporter::IsSupportedFormat("cube.obj"))
        {
            std::cout << "  [PASS] .obj lowercase supported" << std::endl;
            passedTests++;
        }
        else
        {
            std::cout << "  [ERROR] .obj lowercase should be supported" << std::endl;
        }
        
        // Test 2: .OBJ extension (uppercase)
        if (MeshImporter::IsSupportedFormat("cube.OBJ"))
        {
            std::cout << "  [PASS] .OBJ uppercase supported" << std::endl;
            passedTests++;
        }
        else
        {
            std::cout << "  [ERROR] .OBJ uppercase should be supported" << std::endl;
        }
        
        // Test 3: .fbx extension (not supported)
        if (!MeshImporter::IsSupportedFormat("model.fbx"))
        {
            std::cout << "  [PASS] .fbx not supported (correct)" << std::endl;
            passedTests++;
        }
        else
        {
            std::cout << "  [ERROR] .fbx should NOT be supported" << std::endl;
        }
        
        // Test 4: No extension
        if (!MeshImporter::IsSupportedFormat("noextension"))
        {
            std::cout << "  [PASS] No extension not supported (correct)" << std::endl;
            passedTests++;
        }
        else
        {
            std::cout << "  [ERROR] No extension should NOT be supported" << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "  [ERROR] Exception: " << e.what() << std::endl;
    }
    
    std::cout << "[TEST RESULT] TestIsSupportedFormat: " << passedTests << "/4 passed" << std::endl;
}

// ============================================
// TEST 2: ImportInvalidFile
// ============================================
void TestImportInvalidFile()
{
    std::cout << "\n[TEST] TestImportInvalidFile" << std::endl;
    
    unsigned int passedTests = 0;
    
    try {
        // Test 1: Non-existent file should throw exception
        try {
            MeshData mesh = MeshImporter::ImportOBJ("nonexistent.obj");
            std::cout << "  [ERROR] Should throw exception for non-existent file" << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "  [PASS] Exception thrown for non-existent file: " << e.what() << std::endl;
            passedTests++;
        }
        
        // Test 2: Invalid extension should throw exception
        try {
            MeshData mesh = MeshImporter::ImportOBJ("model.fbx");
            std::cout << "  [ERROR] Should throw exception for invalid extension" << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "  [PASS] Exception thrown for invalid extension: " << e.what() << std::endl;
            passedTests++;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "  [ERROR] Unexpected exception: " << e.what() << std::endl;
    }
    
    std::cout << "[TEST RESULT] TestImportInvalidFile: " << passedTests << "/2 passed" << std::endl;
}

// ============================================
// TEST 3: GetMeshInfo
// ============================================
void TestGetMeshInfo()
{
    std::cout << "\n[TEST] TestGetMeshInfo" << std::endl;
    
    unsigned int passedTests = 0;
    
    try {
        // Create simple test OBJ file
        const char* testPath = "assets/meshes/test_triangle.obj";
        
        // Create test file with 3 vertices, 1 triangle
        std::ofstream file(testPath);
        if (file.is_open())
        {
            file << "# Test triangle mesh\n";
            file << "v 0.0 0.0 0.0\n";
            file << "v 1.0 0.0 0.0\n";
            file << "v 0.5 1.0 0.0\n";
            file << "f 1 2 3\n";
            file.close();
            
            // Test GetMeshInfo
            unsigned int vertexCount = 0;
            unsigned int triangleCount = 0;
            
            bool result = MeshImporter::GetMeshInfo(testPath, vertexCount, triangleCount);
            
            if (result && vertexCount == 3)
            {
                std::cout << "  [PASS] Vertex count correct (3)" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] Vertex count incorrect (expected 3, got " << vertexCount << ")" << std::endl;
            }
            
            if (result && triangleCount == 1)
            {
                std::cout << "  [PASS] Triangle count correct (1)" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] Triangle count incorrect (expected 1, got " << triangleCount << ")" << std::endl;
            }
            
            // Test with quad (should count as 2 triangles)
            std::ofstream file2(testPath);
            if (file2.is_open())
            {
                file2 << "# Test quad mesh\n";
                file2 << "v 0.0 0.0 0.0\n";
                file2 << "v 1.0 0.0 0.0\n";
                file2 << "v 1.0 1.0 0.0\n";
                file2 << "v 0.0 1.0 0.0\n";
                file2 << "f 1 2 3 4\n";
                file2.close();
                
                result = MeshImporter::GetMeshInfo(testPath, vertexCount, triangleCount);
                
                if (result && vertexCount == 4)
                {
                    std::cout << "  [PASS] Quad vertex count correct (4)" << std::endl;
                    passedTests++;
                }
                else
                {
                    std::cout << "  [ERROR] Quad vertex count incorrect (expected 4, got " << vertexCount << ")" << std::endl;
                }
                
                if (result && triangleCount == 2)
                {
                    std::cout << "  [PASS] Quad triangle count correct (2)" << std::endl;
                    passedTests++;
                }
                else
                {
                    std::cout << "  [ERROR] Quad triangle count incorrect (expected 2, got " << triangleCount << ")" << std::endl;
                }
            }
        }
        else
        {
            std::cout << "  [ERROR] Failed to create test OBJ file" << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "  [ERROR] Exception: " << e.what() << std::endl;
    }
    
    std::cout << "[TEST RESULT] TestGetMeshInfo: " << passedTests << "/4 passed" << std::endl;
}

// ============================================
// TEST 4: ImportOBJTriangle
// ============================================
void TestImportOBJTriangle()
{
    std::cout << "\n[TEST] TestImportOBJTriangle" << std::endl;
    
    unsigned int passedTests = 0;
    
    try {
        // Create test OBJ file with 1 triangle (positions + normals + UVs)
        const char* testPath = "assets/meshes/test_complete_triangle.obj";
        
        std::ofstream file(testPath);
        if (file.is_open())
        {
            file << "# Complete triangle with all attributes\n";
            file << "v 0.0 0.0 0.0\n";
            file << "v 1.0 0.0 0.0\n";
            file << "v 0.5 1.0 0.0\n";
            file << "vn 0.0 0.0 1.0\n";
            file << "vn 0.0 0.0 1.0\n";
            file << "vn 0.0 0.0 1.0\n";
            file << "vt 0.0 0.0\n";
            file << "vt 1.0 0.0\n";
            file << "vt 0.5 1.0\n";
            file << "f 1/1/1 2/2/2 3/3/3\n";
            file.close();
            
            // Import mesh
            MeshData mesh = MeshImporter::ImportOBJ(testPath);
            
            // Test 1: IsValid
            if (mesh.IsValid())
            {
                std::cout << "  [PASS] Mesh is valid" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] Mesh should be valid" << std::endl;
            }
            
            // Test 2: Vertex count
            if (mesh.GetVertexCount() == 3)
            {
                std::cout << "  [PASS] Vertex count correct (3)" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] Vertex count incorrect (expected 3, got " << mesh.GetVertexCount() << ")" << std::endl;
            }
            
            // Test 3: Index count
            if (mesh.GetIndexCount() == 3)
            {
                std::cout << "  [PASS] Index count correct (3)" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] Index count incorrect (expected 3, got " << mesh.GetIndexCount() << ")" << std::endl;
            }
            
            // Test 4: Triangle count
            if (mesh.GetTriangleCount() == 1)
            {
                std::cout << "  [PASS] Triangle count correct (1)" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] Triangle count incorrect (expected 1, got " << mesh.GetTriangleCount() << ")" << std::endl;
            }
            
            // Test 5: First vertex position
            float expectedPos[3] = {0.0f, 0.0f, 0.0f};
            if (Float3Equal(mesh.vertices[0].position, expectedPos))
            {
                std::cout << "  [PASS] First vertex position correct" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] First vertex position incorrect" << std::endl;
            }
            
            // Test 6: First vertex normal
            float expectedNormal[3] = {0.0f, 0.0f, 1.0f};
            if (Float3Equal(mesh.vertices[0].normal, expectedNormal))
            {
                std::cout << "  [PASS] First vertex normal correct" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] First vertex normal incorrect" << std::endl;
            }
            
            // Test 7: First vertex UV
            float expectedUV[2] = {0.0f, 0.0f};
            if (Float2Equal(mesh.vertices[0].texCoord, expectedUV))
            {
                std::cout << "  [PASS] First vertex UV correct" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] First vertex UV incorrect" << std::endl;
            }
            
            // Test 8: Mesh name
            if (mesh.name == "test_complete_triangle")
            {
                std::cout << "  [PASS] Mesh name correct" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] Mesh name incorrect (expected 'test_complete_triangle', got '" << mesh.name << "')" << std::endl;
            }
        }
        else
        {
            std::cout << "  [ERROR] Failed to create test OBJ file" << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "  [ERROR] Exception: " << e.what() << std::endl;
    }
    
    std::cout << "[TEST RESULT] TestImportOBJTriangle: " << passedTests << "/8 passed" << std::endl;
}

// ============================================
// TEST 5: ImportOBJQuad
// ============================================
void TestImportOBJQuad()
{
    std::cout << "\n[TEST] TestImportOBJQuad" << std::endl;
    
    unsigned int passedTests = 0;
    
    try {
        // Create test OBJ file with 1 quad (should triangulate to 2 triangles)
        const char* testPath = "assets/meshes/test_quad.obj";
        
        std::ofstream file(testPath);
        if (file.is_open())
        {
            file << "# Quad mesh\n";
            file << "v 0.0 0.0 0.0\n";
            file << "v 1.0 0.0 0.0\n";
            file << "v 1.0 1.0 0.0\n";
            file << "v 0.0 1.0 0.0\n";
            file << "f 1 2 3 4\n";
            file.close();
            
            // Import mesh
            MeshData mesh = MeshImporter::ImportOBJ(testPath);
            
            // Test 1: Vertex count (quad ? 4 vertices for 2 triangles)
            if (mesh.GetVertexCount() == 4)
            {
                std::cout << "  [PASS] Quad vertex count correct (4)" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] Quad vertex count incorrect (expected 4, got " << mesh.GetVertexCount() << ")" << std::endl;
            }
            
            // Test 2: Index count (quad ? 6 indices = 2 triangles * 3 indices)
            if (mesh.GetIndexCount() == 6)
            {
                std::cout << "  [PASS] Quad index count correct (6)" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] Quad index count incorrect (expected 6, got " << mesh.GetIndexCount() << ")" << std::endl;
            }
            
            // Test 3: Triangle count
            if (mesh.GetTriangleCount() == 2)
            {
                std::cout << "  [PASS] Quad triangle count correct (2)" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] Quad triangle count incorrect (expected 2, got " << mesh.GetTriangleCount() << ")" << std::endl;
            }
            
            // Test 4: First triangle indices (0, 1, 2)
            if (mesh.indices[0] == 0 && mesh.indices[1] == 1 && mesh.indices[2] == 2)
            {
                std::cout << "  [PASS] First triangle indices correct (0,1,2)" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] First triangle indices incorrect" << std::endl;
            }
            
            // Test 5: Second triangle indices (0, 2, 3)
            if (mesh.indices[3] == 0 && mesh.indices[4] == 2 && mesh.indices[5] == 3)
            {
                std::cout << "  [PASS] Second triangle indices correct (0,2,3)" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] Second triangle indices incorrect" << std::endl;
            }
        }
        else
        {
            std::cout << "  [ERROR] Failed to create test OBJ file" << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "  [ERROR] Exception: " << e.what() << std::endl;
    }
    
    std::cout << "[TEST RESULT] TestImportOBJQuad: " << passedTests << "/5 passed" << std::endl;
}

// ============================================
// TEST 6: BoundingBox Calculation
// ============================================
void TestBoundingBox()
{
    std::cout << "\n[TEST] TestBoundingBox" << std::endl;
    
    unsigned int passedTests = 0;
    
    try {
        // Create test OBJ file with known bounds
        const char* testPath = "assets/meshes/test_bounds.obj";
        
        std::ofstream file(testPath);
        if (file.is_open())
        {
            file << "# Mesh with known bounds\n";
            file << "v -1.0 -2.0 -3.0\n";  // Min corner
            file << "v 4.0 5.0 6.0\n";      // Max corner
            file << "v 0.0 0.0 0.0\n";
            file << "f 1 2 3\n";
            file.close();
            
            // Import mesh
            MeshData mesh = MeshImporter::ImportOBJ(testPath);
            
            // Test 1: Min bounds
            float expectedMin[3] = {-1.0f, -2.0f, -3.0f};
            if (Float3Equal(mesh.bounds.min, expectedMin))
            {
                std::cout << "  [PASS] Min bounds correct (-1, -2, -3)" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] Min bounds incorrect (expected (-1,-2,-3), got (" 
                          << mesh.bounds.min[0] << "," << mesh.bounds.min[1] << "," << mesh.bounds.min[2] << "))" << std::endl;
            }
            
            // Test 2: Max bounds
            float expectedMax[3] = {4.0f, 5.0f, 6.0f};
            if (Float3Equal(mesh.bounds.max, expectedMax))
            {
                std::cout << "  [PASS] Max bounds correct (4, 5, 6)" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] Max bounds incorrect (expected (4,5,6), got (" 
                          << mesh.bounds.max[0] << "," << mesh.bounds.max[1] << "," << mesh.bounds.max[2] << "))" << std::endl;
            }
            
            // Test 3: Center
            float centerX, centerY, centerZ;
            mesh.bounds.GetCenter(centerX, centerY, centerZ);
            float expectedCenter[3] = {1.5f, 1.5f, 1.5f};  // ((4 + -1) / 2, (5 + -2) / 2, (6 + -3) / 2)
            if (FloatEqual(centerX, expectedCenter[0]) && FloatEqual(centerY, expectedCenter[1]) && FloatEqual(centerZ, expectedCenter[2]))
            {
                std::cout << "  [PASS] Center correct (1.5, 1.5, 1.5)" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] Center incorrect (expected (1.5,1.5,1.5), got (" 
                          << centerX << "," << centerY << "," << centerZ << "))" << std::endl;
            }
            
            // Test 4: Size
            float width, height, depth;
            mesh.bounds.GetSize(width, height, depth);
            float expectedSize[3] = {5.0f, 7.0f, 9.0f};  // (4 - (-1), 5 - (-2), 6 - (-3))
            if (FloatEqual(width, expectedSize[0]) && FloatEqual(height, expectedSize[1]) && FloatEqual(depth, expectedSize[2]))
            {
                std::cout << "  [PASS] Size correct (5, 7, 9)" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] Size incorrect (expected (5,7,9), got (" 
                          << width << "," << height << "," << depth << "))" << std::endl;
            }
        }
        else
        {
            std::cout << "  [ERROR] Failed to create test OBJ file" << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "  [ERROR] Exception: " << e.what() << std::endl;
    }
    
    std::cout << "[TEST RESULT] TestBoundingBox: " << passedTests << "/4 passed" << std::endl;
}

// ============================================
// MAIN
// ============================================
int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "MeshImporter Unit Tests" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Create assets/meshes/ directory if it doesn't exist
    system("mkdir assets\\meshes 2>nul");
    
    // Run tests
    TestIsSupportedFormat();
    TestImportInvalidFile();
    TestGetMeshInfo();
    TestImportOBJTriangle();
    TestImportOBJQuad();
    TestBoundingBox();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "ALL MESH IMPORTER TESTS COMPLETED" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}
