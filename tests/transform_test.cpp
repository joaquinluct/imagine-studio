#include "../src/scene/Transform.h"
#include "../src/scene/Entity.h"
#include "../src/scene/EntityManager.h"
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

// Helper function to extract translation from matrix
DirectX::XMFLOAT3 ExtractTranslation(const DirectX::XMMATRIX& matrix)
{
    DirectX::XMFLOAT4X4 mat;
    DirectX::XMStoreFloat4x4(&mat, matrix);
    return DirectX::XMFLOAT3(mat._41, mat._42, mat._43);
}

void TestTransformDefaultValues()
{
    std::cout << "[TEST] TestTransformDefaultValues - START" << std::endl;
    
    Scene::Transform transform;
    
    // Test default position (0,0,0)
    DirectX::XMFLOAT3 pos = transform.GetPosition();
    assert(Float3Equal(pos, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)));
    std::cout << "  [PASS] Default position is (0,0,0)" << std::endl;
    
    // Test default rotation (0,0,0)
    DirectX::XMFLOAT3 rot = transform.GetRotation();
    assert(Float3Equal(rot, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)));
    std::cout << "  [PASS] Default rotation is (0,0,0)" << std::endl;
    
    // Test default scale (1,1,1)
    DirectX::XMFLOAT3 scale = transform.GetScale();
    assert(Float3Equal(scale, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)));
    std::cout << "  [PASS] Default scale is (1,1,1)" << std::endl;
    
    std::cout << "[TEST] TestTransformDefaultValues - PASSED" << std::endl;
}

void TestTransformSettersGetters()
{
    std::cout << "[TEST] TestTransformSettersGetters - START" << std::endl;
    
    Scene::Transform transform;
    
    // Test SetPosition (XMFLOAT3)
    transform.SetPosition(DirectX::XMFLOAT3(1.0f, 2.0f, 3.0f));
    DirectX::XMFLOAT3 pos = transform.GetPosition();
    assert(Float3Equal(pos, DirectX::XMFLOAT3(1.0f, 2.0f, 3.0f)));
    std::cout << "  [PASS] SetPosition(XMFLOAT3) works correctly" << std::endl;
    
    // Test SetPosition (x,y,z)
    transform.SetPosition(4.0f, 5.0f, 6.0f);
    pos = transform.GetPosition();
    assert(Float3Equal(pos, DirectX::XMFLOAT3(4.0f, 5.0f, 6.0f)));
    std::cout << "  [PASS] SetPosition(x,y,z) works correctly" << std::endl;
    
    // Test SetRotation (XMFLOAT3)
    transform.SetRotation(DirectX::XMFLOAT3(0.1f, 0.2f, 0.3f));
    DirectX::XMFLOAT3 rot = transform.GetRotation();
    assert(Float3Equal(rot, DirectX::XMFLOAT3(0.1f, 0.2f, 0.3f)));
    std::cout << "  [PASS] SetRotation(XMFLOAT3) works correctly" << std::endl;
    
    // Test SetRotation (x,y,z)
    transform.SetRotation(0.4f, 0.5f, 0.6f);
    rot = transform.GetRotation();
    assert(Float3Equal(rot, DirectX::XMFLOAT3(0.4f, 0.5f, 0.6f)));
    std::cout << "  [PASS] SetRotation(x,y,z) works correctly" << std::endl;
    
    // Test SetScale (XMFLOAT3)
    transform.SetScale(DirectX::XMFLOAT3(2.0f, 3.0f, 4.0f));
    DirectX::XMFLOAT3 scale = transform.GetScale();
    assert(Float3Equal(scale, DirectX::XMFLOAT3(2.0f, 3.0f, 4.0f)));
    std::cout << "  [PASS] SetScale(XMFLOAT3) works correctly" << std::endl;
    
    // Test SetScale (x,y,z)
    transform.SetScale(5.0f, 6.0f, 7.0f);
    scale = transform.GetScale();
    assert(Float3Equal(scale, DirectX::XMFLOAT3(5.0f, 6.0f, 7.0f)));
    std::cout << "  [PASS] SetScale(x,y,z) works correctly" << std::endl;
    
    // Test SetUniformScale
    transform.SetUniformScale(2.5f);
    scale = transform.GetScale();
    assert(Float3Equal(scale, DirectX::XMFLOAT3(2.5f, 2.5f, 2.5f)));
    std::cout << "  [PASS] SetUniformScale works correctly" << std::endl;
    
    std::cout << "[TEST] TestTransformSettersGetters - PASSED" << std::endl;
}

void TestTransformLocalMatrix()
{
    std::cout << "[TEST] TestTransformLocalMatrix - START" << std::endl;
    
    // Test 1: Identity matrix (default transform)
    {
        Scene::Transform transform;
        DirectX::XMMATRIX localMatrix = transform.GetLocalMatrix();
        
        DirectX::XMFLOAT4X4 mat;
        DirectX::XMStoreFloat4x4(&mat, localMatrix);
        
        // Check if it's identity (diagonal = 1, rest = 0)
        assert(FloatEqual(mat._11, 1.0f) && FloatEqual(mat._22, 1.0f) && 
               FloatEqual(mat._33, 1.0f) && FloatEqual(mat._44, 1.0f));
        std::cout << "  [PASS] Default transform produces identity matrix" << std::endl;
    }
    
    // Test 2: Translation only
    {
        Scene::Transform transform;
        transform.SetPosition(10.0f, 20.0f, 30.0f);
        
        DirectX::XMMATRIX localMatrix = transform.GetLocalMatrix();
        DirectX::XMFLOAT3 translation = ExtractTranslation(localMatrix);
        
        assert(Float3Equal(translation, DirectX::XMFLOAT3(10.0f, 20.0f, 30.0f)));
        std::cout << "  [PASS] Translation is correctly encoded in matrix" << std::endl;
    }
    
    // Test 3: Scale only
    {
        Scene::Transform transform;
        transform.SetScale(2.0f, 3.0f, 4.0f);
        
        DirectX::XMMATRIX localMatrix = transform.GetLocalMatrix();
        DirectX::XMFLOAT4X4 mat;
        DirectX::XMStoreFloat4x4(&mat, localMatrix);
        
        // Check scale on diagonal
        assert(FloatEqual(mat._11, 2.0f));
        assert(FloatEqual(mat._22, 3.0f));
        assert(FloatEqual(mat._33, 4.0f));
        std::cout << "  [PASS] Scale is correctly encoded in matrix" << std::endl;
    }
    
    // Test 4: Combined TRS
    {
        Scene::Transform transform;
        transform.SetPosition(1.0f, 2.0f, 3.0f);
        transform.SetRotation(0.0f, 0.0f, 0.0f); // No rotation for simplicity
        transform.SetScale(2.0f, 2.0f, 2.0f);
        
        DirectX::XMMATRIX localMatrix = transform.GetLocalMatrix();
        DirectX::XMFLOAT3 translation = ExtractTranslation(localMatrix);
        
        // Translation should still be (1,2,3)
        assert(Float3Equal(translation, DirectX::XMFLOAT3(1.0f, 2.0f, 3.0f)));
        std::cout << "  [PASS] Combined TRS matrix is computed correctly" << std::endl;
    }
    
    std::cout << "[TEST] TestTransformLocalMatrix - PASSED" << std::endl;
}

void TestTransformWorldMatrix()
{
    std::cout << "[TEST] TestTransformWorldMatrix - START" << std::endl;
    
    Scene::Transform transform;
    transform.SetPosition(5.0f, 10.0f, 15.0f);
    
    // For now, GetWorldMatrix() == GetLocalMatrix() (no parent)
    DirectX::XMMATRIX localMatrix = transform.GetLocalMatrix();
    DirectX::XMMATRIX worldMatrix = transform.GetWorldMatrix();
    
    DirectX::XMFLOAT4X4 local, world;
    DirectX::XMStoreFloat4x4(&local, localMatrix);
    DirectX::XMStoreFloat4x4(&world, worldMatrix);
    
    // Matrices should be identical
    bool matricesEqual = true;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (!FloatEqual(local.m[i][j], world.m[i][j])) {
                matricesEqual = false;
                break;
            }
        }
    }
    
    assert(matricesEqual);
    std::cout << "  [PASS] GetWorldMatrix() equals GetLocalMatrix() (no parent)" << std::endl;
    
    std::cout << "[TEST] TestTransformWorldMatrix - PASSED" << std::endl;
}

void TestParentChildHierarchy()
{
    std::cout << "[TEST] TestParentChildHierarchy - START" << std::endl;
    
    // Create EntityManager and entities
    Scene::EntityManager mgr;
    Scene::Entity* parentEntity = mgr.CreateEntity("Parent");
    Scene::Entity* childEntity = mgr.CreateEntity("Child");
    
    // Add Transform components
    Scene::Transform* parentTransform = parentEntity->AddComponent<Scene::Transform>();
    Scene::Transform* childTransform = childEntity->AddComponent<Scene::Transform>();
    
    // Test 1: SetParent and GetParent
    {
        childTransform->SetParent(parentEntity);
        assert(childTransform->GetParent() == parentEntity);
        std::cout << "  [PASS] SetParent() and GetParent() work correctly" << std::endl;
    }
    
    // Test 2: Child inherits parent position (simple case)
    {
        parentTransform->SetPosition(10.0f, 0.0f, 0.0f);
        childTransform->SetPosition(5.0f, 0.0f, 0.0f);
        
        DirectX::XMMATRIX childWorld = childTransform->GetWorldMatrix();
        DirectX::XMFLOAT3 childWorldPos = ExtractTranslation(childWorld);
        
        // Child world position should be parent(10,0,0) + child_local(5,0,0) = (15,0,0)
        assert(Float3Equal(childWorldPos, DirectX::XMFLOAT3(15.0f, 0.0f, 0.0f)));
        std::cout << "  [PASS] Child inherits parent position correctly" << std::endl;
    }
    
    // Test 3: Move parent ? child moves automatically
    {
        parentTransform->SetPosition(20.0f, 0.0f, 0.0f);
        
        DirectX::XMMATRIX childWorld = childTransform->GetWorldMatrix();
        DirectX::XMFLOAT3 childWorldPos = ExtractTranslation(childWorld);
        
        // Child world position should be parent(20,0,0) + child_local(5,0,0) = (25,0,0)
        assert(Float3Equal(childWorldPos, DirectX::XMFLOAT3(25.0f, 0.0f, 0.0f)));
        std::cout << "  [PASS] Moving parent moves child automatically" << std::endl;
    }
    
    // Test 4: Remove parent (SetParent(nullptr))
    {
        childTransform->SetParent(nullptr);
        assert(childTransform->GetParent() == nullptr);
        
        DirectX::XMMATRIX childWorld = childTransform->GetWorldMatrix();
        DirectX::XMFLOAT3 childWorldPos = ExtractTranslation(childWorld);
        
        // Child world position should be child_local(5,0,0) only
        assert(Float3Equal(childWorldPos, DirectX::XMFLOAT3(5.0f, 0.0f, 0.0f)));
        std::cout << "  [PASS] Removing parent (SetParent(nullptr)) works correctly" << std::endl;
    }
    
    // Test 5: Multi-level hierarchy (grandparent ? parent ? child)
    {
        Scene::Entity* grandparentEntity = mgr.CreateEntity("Grandparent");
        Scene::Transform* grandparentTransform = grandparentEntity->AddComponent<Scene::Transform>();
        
        grandparentTransform->SetPosition(100.0f, 0.0f, 0.0f);
        parentTransform->SetPosition(10.0f, 0.0f, 0.0f);
        childTransform->SetPosition(1.0f, 0.0f, 0.0f);
        
        parentTransform->SetParent(grandparentEntity);
        childTransform->SetParent(parentEntity);
        
        DirectX::XMMATRIX childWorld = childTransform->GetWorldMatrix();
        DirectX::XMFLOAT3 childWorldPos = ExtractTranslation(childWorld);
        
        // Child world position should be grandparent(100) + parent(10) + child(1) = (111,0,0)
        assert(Float3Equal(childWorldPos, DirectX::XMFLOAT3(111.0f, 0.0f, 0.0f)));
        std::cout << "  [PASS] Multi-level hierarchy (3 levels) works correctly" << std::endl;
    }
    
    std::cout << "[TEST] TestParentChildHierarchy - PASSED" << std::endl;
}

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "Transform Component Unit Tests" << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        TestTransformDefaultValues();
        TestTransformSettersGetters();
        TestTransformLocalMatrix();
        TestTransformWorldMatrix();
        TestParentChildHierarchy();
        
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
