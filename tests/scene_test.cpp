#include "../src/scene/Scene.h"
#include "../src/scene/Transform.h"
#include <cassert>
#include <iostream>
#include <cmath>

// Helper function to compare floats with epsilon
bool FloatEqual(float a, float b, float epsilon = 0.0001f)
{
    return std::abs(a - b) < epsilon;
}

// Helper function to compare XMFLOAT3
bool Float3Equal(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b, float epsilon = 0.0001f)
{
    return FloatEqual(a.x, b.x, epsilon) &&
           FloatEqual(a.y, b.y, epsilon) &&
           FloatEqual(a.z, b.z, epsilon);
}

void TestSceneBasics()
{
    std::cout << "[TEST] TestSceneBasics - START" << std::endl;
    
    Scene::Scene scene("Test Scene");
    
    // Test 1: Scene name
    assert(scene.GetName() == "Test Scene");
    std::cout << "  [PASS] Scene name is correct" << std::endl;
    
    // Test 2: Initial state (no entities)
    assert(scene.GetRootEntities().size() == 0);
    std::cout << "  [PASS] Initial scene has no entities" << std::endl;
    
    // Test 3: CreateEntity
    Scene::Entity* e1 = scene.CreateEntity("Player");
    assert(e1 != nullptr);
    assert(e1->GetName() == "Player");
    assert(scene.GetRootEntities().size() == 1);
    std::cout << "  [PASS] CreateEntity creates entity with Transform" << std::endl;
    
    // Test 4: Entity has Transform component by default
    Scene::Transform* t1 = e1->GetComponent<Scene::Transform>();
    assert(t1 != nullptr);
    std::cout << "  [PASS] Created entity has Transform component by default" << std::endl;
    
    // Test 5: Create multiple entities
    Scene::Entity* e2 = scene.CreateEntity("Enemy");
    Scene::Entity* e3 = scene.CreateEntity("Camera");
    assert(scene.GetRootEntities().size() == 3);
    std::cout << "  [PASS] CreateEntity adds to root entities" << std::endl;
    
    // Test 6: GetEntity by ID
    Scene::Entity* found = scene.GetEntity(e1->GetID());
    assert(found == e1);
    std::cout << "  [PASS] GetEntity(id) works correctly" << std::endl;
    
    // Test 7: GetEntityByName
    Scene::Entity* foundByName = scene.GetEntityByName("Enemy");
    assert(foundByName == e2);
    std::cout << "  [PASS] GetEntityByName works correctly" << std::endl;
    
    std::cout << "[TEST] TestSceneBasics - PASSED" << std::endl;
}

void TestSceneSelection()
{
    std::cout << "[TEST] TestSceneSelection - START" << std::endl;
    
    Scene::Scene scene("Selection Test");
    
    Scene::Entity* e1 = scene.CreateEntity("Entity1");
    Scene::Entity* e2 = scene.CreateEntity("Entity2");
    
    // Test 1: Initial selection (nullptr)
    assert(scene.GetSelectedEntity() == nullptr);
    std::cout << "  [PASS] Initial selection is nullptr" << std::endl;
    
    // Test 2: SetSelectedEntity
    scene.SetSelectedEntity(e1->GetID());
    assert(scene.GetSelectedEntity() == e1);
    std::cout << "  [PASS] SetSelectedEntity works correctly" << std::endl;
    
    // Test 3: Change selection
    scene.SetSelectedEntity(e2->GetID());
    assert(scene.GetSelectedEntity() == e2);
    std::cout << "  [PASS] Changing selection works" << std::endl;
    
    // Test 4: Select invalid ID (should set to nullptr)
    scene.SetSelectedEntity(9999);
    assert(scene.GetSelectedEntity() == nullptr);
    std::cout << "  [PASS] Selecting invalid ID sets selection to nullptr" << std::endl;
    
    std::cout << "[TEST] TestSceneSelection - PASSED" << std::endl;
}

void TestSceneDestroy()
{
    std::cout << "[TEST] TestSceneDestroy - START" << std::endl;
    
    Scene::Scene scene("Destroy Test");
    
    Scene::Entity* e1 = scene.CreateEntity("Entity1");
    Scene::Entity* e2 = scene.CreateEntity("Entity2");
    Scene::Entity* e3 = scene.CreateEntity("Entity3");
    
    Scene::EntityID id1 = e1->GetID();
    Scene::EntityID id2 = e2->GetID();
    
    // Test 1: Select entity then destroy it
    scene.SetSelectedEntity(id1);
    assert(scene.GetSelectedEntity() == e1);
    
    scene.DestroyEntity(id1);
    
    // Selection should be cleared
    assert(scene.GetSelectedEntity() == nullptr);
    assert(scene.GetRootEntities().size() == 2);
    std::cout << "  [PASS] DestroyEntity clears selection if selected entity is destroyed" << std::endl;
    
    // Test 2: GetEntity on destroyed entity returns nullptr
    assert(scene.GetEntity(id1) == nullptr);
    std::cout << "  [PASS] GetEntity on destroyed entity returns nullptr" << std::endl;
    
    // Test 3: Destroy middle entity
    scene.DestroyEntity(id2);
    assert(scene.GetRootEntities().size() == 1);
    assert(scene.GetRootEntities()[0] == e3);
    std::cout << "  [PASS] Destroying middle entity preserves others" << std::endl;
    
    std::cout << "[TEST] TestSceneDestroy - PASSED" << std::endl;
}

void TestSceneUpdate()
{
    std::cout << "[TEST] TestSceneUpdate - START" << std::endl;
    
    Scene::Scene scene("Update Test");
    
    Scene::Entity* e1 = scene.CreateEntity("Entity1");
    Scene::Entity* e2 = scene.CreateEntity("Entity2");
    
    // Test 1: Update scene (should not crash)
    scene.Update(0.016f); // 60 FPS
    std::cout << "  [PASS] Scene::Update() executes without errors" << std::endl;
    
    // Test 2: UpdateTransforms (should not crash)
    scene.UpdateTransforms();
    std::cout << "  [PASS] Scene::UpdateTransforms() executes without errors" << std::endl;
    
    std::cout << "[TEST] TestSceneUpdate - PASSED" << std::endl;
}

void TestSceneHierarchy()
{
    std::cout << "[TEST] TestSceneHierarchy - START" << std::endl;
    
    Scene::Scene scene("Hierarchy Test");
    
    // Create parent and child
    Scene::Entity* parent = scene.CreateEntity("Parent");
    Scene::Entity* child = scene.CreateEntity("Child");
    
    Scene::Transform* parentTransform = parent->GetComponent<Scene::Transform>();
    Scene::Transform* childTransform = child->GetComponent<Scene::Transform>();
    
    // Test 1: Both are root entities initially
    assert(scene.GetRootEntities().size() == 2);
    std::cout << "  [PASS] Parent and child are both root entities initially" << std::endl;
    
    // Test 2: Set parent-child relationship
    childTransform->SetParent(parent);
    
    // Child should still be in root entities (we don't automatically remove it)
    // NOTE: In a full implementation, we'd remove child from rootEntities
    // For now, Scene just manages entities, hierarchy is Transform's responsibility
    
    // Test 3: Transform hierarchy works
    parentTransform->SetPosition(10.0f, 0.0f, 0.0f);
    childTransform->SetPosition(5.0f, 0.0f, 0.0f);
    
    scene.UpdateTransforms();
    
    DirectX::XMMATRIX childWorld = childTransform->GetWorldMatrix();
    DirectX::XMFLOAT4X4 mat;
    DirectX::XMStoreFloat4x4(&mat, childWorld);
    
    // Child world position should be (15, 0, 0)
    assert(FloatEqual(mat._41, 15.0f));
    std::cout << "  [PASS] Transform hierarchy propagates correctly after UpdateTransforms()" << std::endl;
    
    std::cout << "[TEST] TestSceneHierarchy - PASSED" << std::endl;
}

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "Scene Unit Tests" << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        TestSceneBasics();
        TestSceneSelection();
        TestSceneDestroy();
        TestSceneUpdate();
        TestSceneHierarchy();
        
        std::cout << "========================================" << std::endl;
        std::cout << "ALL TESTS PASSED!" << std::endl;
        std::cout << "========================================" << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "[ERROR] Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "[ERROR] Test failed with unknown exception" << std::endl;
        return 1;
    }
}
