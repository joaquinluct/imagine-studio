// tests/scene_serializer_test.cpp
// Unit tests for SceneSerializer (Save/Load JSON)

#include "scene/SceneSerializer.h"
#include "scene/Scene.h"
#include "scene/Entity.h"
#include "scene/Transform.h"

#include <iostream>
#include <fstream>
#include <cmath>

// Alias para evitar conflicto Scene::Scene
namespace SceneNS = Scene;
using SceneClass = SceneNS::Scene;
using EntityClass = SceneNS::Entity;
using TransformClass = SceneNS::Transform;

// Helper: Compare floats with epsilon
bool FloatEqual(float a, float b, float epsilon = 0.0001f)
{
    return std::abs(a - b) < epsilon;
}

// Helper: Compare XMFLOAT3 with epsilon
bool Float3Equal(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b, float epsilon = 0.0001f)
{
    return FloatEqual(a.x, b.x, epsilon) &&
           FloatEqual(a.y, b.y, epsilon) &&
           FloatEqual(a.z, b.z, epsilon);
}

// ============================================
// TEST 1: SaveScene creates valid JSON file
// ============================================
void TestSaveScene()
{
    std::cout << "\n[TEST] TestSaveScene" << std::endl;
    
    unsigned int passedTests = 0;
    
    try {
        // Create test scene
        SceneClass scene("Test Scene");
        EntityClass* entity1 = scene.CreateEntity("Entity 1");
        EntityClass* entity2 = scene.CreateEntity("Entity 2");
        
        // Set transforms
        TransformClass* t1 = entity1->GetComponent<TransformClass>();
        TransformClass* t2 = entity2->GetComponent<TransformClass>();
        
        if (t1) t1->SetPosition(1.0f, 2.0f, 3.0f);
        if (t2) t2->SetPosition(4.0f, 5.0f, 6.0f);
        
        // Save scene
        const char* filepath = "assets/scenes/test_save.json";
        bool saved = SceneNS::SceneSerializer::SaveScene(&scene, filepath);
        
        // Test 1: Save successful
        if (saved)
        {
            std::cout << "  [PASS] SaveScene returned true" << std::endl;
            passedTests++;
        }
        else
        {
            std::cout << "  [ERROR] SaveScene failed: " << SceneNS::SceneSerializer::GetLastError() << std::endl;
        }
        
        // Test 2: File exists
        std::ifstream file(filepath);
        if (file.is_open())
        {
            std::cout << "  [PASS] JSON file created" << std::endl;
            passedTests++;
            file.close();
        }
        else
        {
            std::cout << "  [ERROR] JSON file not created" << std::endl;
        }
        
        // Test 3: File contains scene data
        file.open(filepath);
        if (file.is_open())
        {
            std::string content((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());
            file.close();
            
            if (content.find("Test Scene") != std::string::npos &&
                content.find("Entity 1") != std::string::npos &&
                content.find("Entity 2") != std::string::npos)
            {
                std::cout << "  [PASS] JSON contains scene data" << std::endl;
                passedTests++;
            }
            else
            {
                std::cout << "  [ERROR] JSON missing scene data" << std::endl;
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "  [ERROR] Exception: " << e.what() << std::endl;
    }
    
    std::cout << "[TEST RESULT] TestSaveScene: " << passedTests << "/3 passed" << std::endl;
}

// ============================================
// TEST 2: LoadScene reads valid JSON file
// ============================================
void TestLoadScene()
{
    std::cout << "\n[TEST] TestLoadScene" << std::endl;
    
    unsigned int passedTests = 0;
    
    try {
        // Create and save test scene first
        SceneClass originalScene("Load Test Scene");
        EntityClass* entity1 = originalScene.CreateEntity("Load Entity 1");
        EntityClass* entity2 = originalScene.CreateEntity("Load Entity 2");
        
        TransformClass* t1 = entity1->GetComponent<TransformClass>();
        TransformClass* t2 = entity2->GetComponent<TransformClass>();
        
        if (t1) t1->SetPosition(10.0f, 20.0f, 30.0f);
        if (t2) t2->SetPosition(40.0f, 50.0f, 60.0f);
        
        const char* filepath = "assets/scenes/test_load.json";
        SceneNS::SceneSerializer::SaveScene(&originalScene, filepath);
        
        // Load scene
        SceneClass* loadedScene = SceneNS::SceneSerializer::LoadScene(filepath);
        
        // Test 1: LoadScene succeeded
        if (loadedScene != nullptr)
        {
            std::cout << "  [PASS] LoadScene returned valid pointer" << std::endl;
            passedTests++;
        }
        else
        {
            std::cout << "  [ERROR] LoadScene failed: " << SceneNS::SceneSerializer::GetLastError() << std::endl;
            std::cout << "[TEST RESULT] TestLoadScene: " << passedTests << "/5 passed" << std::endl;
            return;
        }
        
        // Test 2: Scene name correct
        if (loadedScene->GetName() == "Load Test Scene")
        {
            std::cout << "  [PASS] Scene name preserved" << std::endl;
            passedTests++;
        }
        
        // Test 3: Entity count correct
        if (loadedScene->GetRootEntities().size() == 2)
        {
            std::cout << "  [PASS] Entity count correct (2)" << std::endl;
            passedTests++;
        }
        
        // Test 4: Entity names correct
        EntityClass* loadedEntity1 = loadedScene->GetEntityByName("Load Entity 1");
        EntityClass* loadedEntity2 = loadedScene->GetEntityByName("Load Entity 2");
        
        if (loadedEntity1 && loadedEntity2)
        {
            std::cout << "  [PASS] Entity names preserved" << std::endl;
            passedTests++;
        }
        
        // Test 5: Transform data preserved
        if (loadedEntity1 && loadedEntity2)
        {
            TransformClass* lt1 = loadedEntity1->GetComponent<TransformClass>();
            TransformClass* lt2 = loadedEntity2->GetComponent<TransformClass>();
            
            if (lt1 && lt2)
            {
                DirectX::XMFLOAT3 pos1 = lt1->GetPosition();
                DirectX::XMFLOAT3 pos2 = lt2->GetPosition();
                
                if (Float3Equal(pos1, DirectX::XMFLOAT3(10.0f, 20.0f, 30.0f)) &&
                    Float3Equal(pos2, DirectX::XMFLOAT3(40.0f, 50.0f, 60.0f)))
                {
                    std::cout << "  [PASS] Transform positions preserved" << std::endl;
                    passedTests++;
                }
            }
        }
        
        delete loadedScene;
    }
    catch (const std::exception& e)
    {
        std::cout << "  [ERROR] Exception: " << e.what() << std::endl;
    }
    
    std::cout << "[TEST RESULT] TestLoadScene: " << passedTests << "/5 passed" << std::endl;
}

// ============================================
// TEST 3: SaveScene with invalid scene
// ============================================
void TestSaveInvalidScene()
{
    std::cout << "\n[TEST] TestSaveInvalidScene" << std::endl;
    
    unsigned int passedTests = 0;
    
    try {
        // Test 1: Save null scene
        bool result = SceneNS::SceneSerializer::SaveScene(nullptr, "test.json");
        
        if (!result)
        {
            std::cout << "  [PASS] SaveScene(nullptr) returned false" << std::endl;
            passedTests++;
        }
        
        // Test 2: Error message set
        if (!SceneNS::SceneSerializer::GetLastError().empty())
        {
            std::cout << "  [PASS] Error message set" << std::endl;
            passedTests++;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "  [ERROR] Exception: " << e.what() << std::endl;
    }
    
    std::cout << "[TEST RESULT] TestSaveInvalidScene: " << passedTests << "/2 passed" << std::endl;
}

// ============================================
// MAIN
// ============================================
int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "SceneSerializer Unit Tests" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Create assets/scenes/ directory if it doesn't exist
    system("mkdir assets\\scenes 2>nul");
    
    // Run tests
    TestSaveScene();
    TestLoadScene();
    TestSaveInvalidScene();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "ALL TESTS COMPLETED" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}