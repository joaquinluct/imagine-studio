#include "../src/scene/Entity.h"
#include "../src/scene/EntityManager.h"
#include <cassert>
#include <iostream>

void TestEntityCreation()
{
    std::cout << "[TEST] TestEntityCreation - START" << std::endl;
    
    Scene::EntityManager mgr;
    
    // Test 1: Create entity
    Scene::Entity* e1 = mgr.CreateEntity("Player");
    assert(e1 != nullptr);
    assert(e1->GetName() == "Player");
    assert(e1->GetID() == 1);
    std::cout << "  [PASS] Create entity with name 'Player' (ID=1)" << std::endl;
    
    // Test 2: Create multiple entities
    Scene::Entity* e2 = mgr.CreateEntity("Enemy");
    assert(e2 != nullptr);
    assert(e2->GetID() == 2);
    assert(e2->GetName() == "Enemy");
    std::cout << "  [PASS] Create second entity 'Enemy' (ID=2)" << std::endl;
    
    Scene::Entity* e3 = mgr.CreateEntity("Camera");
    assert(e3 != nullptr);
    assert(e3->GetID() == 3);
    std::cout << "  [PASS] Create third entity 'Camera' (ID=3)" << std::endl;
    
    // Test 3: Get entity by ID
    Scene::Entity* found1 = mgr.GetEntity(1);
    assert(found1 == e1);
    assert(found1->GetName() == "Player");
    std::cout << "  [PASS] GetEntity(1) returns correct entity" << std::endl;
    
    Scene::Entity* found2 = mgr.GetEntity(2);
    assert(found2 == e2);
    std::cout << "  [PASS] GetEntity(2) returns correct entity" << std::endl;
    
    // Test 4: GetEntityCount
    assert(mgr.GetEntityCount() == 3);
    std::cout << "  [PASS] GetEntityCount() returns 3" << std::endl;
    
    // Test 5: Destroy entity
    mgr.DestroyEntity(1);
    assert(mgr.GetEntity(1) == nullptr);
    assert(mgr.GetEntityCount() == 2);
    std::cout << "  [PASS] DestroyEntity(1) removes entity" << std::endl;
    
    // Test 6: Get non-existent entity
    Scene::Entity* notFound = mgr.GetEntity(999);
    assert(notFound == nullptr);
    std::cout << "  [PASS] GetEntity(999) returns nullptr" << std::endl;
    
    // Test 7: Clear all entities
    mgr.Clear();
    assert(mgr.GetEntityCount() == 0);
    assert(mgr.GetEntity(2) == nullptr);
    assert(mgr.GetEntity(3) == nullptr);
    std::cout << "  [PASS] Clear() removes all entities" << std::endl;
    
    std::cout << "[TEST] TestEntityCreation - PASSED" << std::endl;
}

void TestEntityManager()
{
    std::cout << "[TEST] TestEntityManager - START" << std::endl;
    
    Scene::EntityManager mgr;
    
    // Test incremental IDs
    Scene::Entity* e1 = mgr.CreateEntity("Entity1");
    Scene::Entity* e2 = mgr.CreateEntity("Entity2");
    Scene::Entity* e3 = mgr.CreateEntity("Entity3");
    
    assert(e1->GetID() == 1);
    assert(e2->GetID() == 2);
    assert(e3->GetID() == 3);
    std::cout << "  [PASS] IDs are incremental (1, 2, 3)" << std::endl;
    
    // Test destroy middle entity
    mgr.DestroyEntity(2);
    assert(mgr.GetEntity(2) == nullptr);
    assert(mgr.GetEntity(1) != nullptr);
    assert(mgr.GetEntity(3) != nullptr);
    assert(mgr.GetEntityCount() == 2);
    std::cout << "  [PASS] Destroy middle entity preserves others" << std::endl;
    
    // Test create after destroy (ID continues incrementing)
    Scene::Entity* e4 = mgr.CreateEntity("Entity4");
    assert(e4->GetID() == 4); // ID continues from 3, not reusing 2
    std::cout << "  [PASS] New entity gets ID=4 (continues sequence)" << std::endl;
    
    std::cout << "[TEST] TestEntityManager - PASSED" << std::endl;
}

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "Entity System Unit Tests" << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        TestEntityCreation();
        TestEntityManager();
        
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
