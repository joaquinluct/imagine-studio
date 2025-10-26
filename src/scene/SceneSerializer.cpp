#include "SceneSerializer.h"
#include "Scene.h"
#include "Entity.h"
#include "Transform.h"
#include "core/Log.h"

#include "../external/json/json.hpp"  // Relative path from src/
#include <fstream>

using json = nlohmann::json;

namespace Scene {

// Static member initialization
std::string SceneSerializer::s_lastError = "";

bool SceneSerializer::SaveScene(const Scene* scene, const std::string& filepath)
{
    if (!scene) {
        s_lastError = "Scene is null";
        CORE_LOG_ERROR("SceneSerializer: Cannot save null scene");
        return false;
    }

    try {
        // Create JSON root
        json j;
        j["scene"]["name"] = scene->GetName();

        // Serialize all root entities
        json entitiesArray = json::array();

        for (const Entity* entity : scene->GetRootEntities()) {
            json entityJson;

            // Entity base data
            entityJson["id"] = entity->GetID();
            entityJson["name"] = entity->GetName();

            // Transform component (cast away const - GetComponent() is not const-correct)
            Entity* mutableEntity = const_cast<Entity*>(entity);
            Transform* transform = mutableEntity->GetComponent<Transform>();
            if (transform) {
                json transformJson;

                // Position
                DirectX::XMFLOAT3 pos = transform->GetPosition();
                transformJson["position"] = {pos.x, pos.y, pos.z};

                // Rotation (Euler angles in degrees for readability)
                DirectX::XMFLOAT3 rot = transform->GetRotation();
                transformJson["rotation"] = {
                    DirectX::XMConvertToDegrees(rot.x),
                    DirectX::XMConvertToDegrees(rot.y),
                    DirectX::XMConvertToDegrees(rot.z)
                };

                // Scale
                DirectX::XMFLOAT3 scale = transform->GetScale();
                transformJson["scale"] = {scale.x, scale.y, scale.z};

                entityJson["transform"] = transformJson;
            }

            // TODO (future): Serialize other components (Camera, MeshRenderer, etc.)

            entitiesArray.push_back(entityJson);
        }

        j["scene"]["entities"] = entitiesArray;

        // Write to file
        std::ofstream file(filepath);
        if (!file.is_open()) {
            s_lastError = "Failed to open file for writing: " + filepath;
            CORE_LOG_ERROR("SceneSerializer: %s", s_lastError.c_str());
            return false;
        }

        // Pretty print with 2-space indent
        file << j.dump(2);
        file.close();

        CORE_LOG_INFO("SceneSerializer: Scene saved successfully: %s", filepath.c_str());
        CORE_LOG_INFO("  Entities: %zu", entitiesArray.size());

        return true;
    }
    catch (const std::exception& e) {
        s_lastError = std::string("Exception during save: ") + e.what();
        CORE_LOG_ERROR("SceneSerializer: %s", s_lastError.c_str());
        return false;
    }
}

Scene* SceneSerializer::LoadScene(const std::string& filepath)
{
    try {
        // Read file
        std::ifstream file(filepath);
        if (!file.is_open()) {
            s_lastError = "Failed to open file for reading: " + filepath;
            CORE_LOG_ERROR("SceneSerializer: %s", s_lastError.c_str());
            return nullptr;
        }

        // Parse JSON
        json j;
        file >> j;
        file.close();

        // Validate structure
        if (!j.contains("scene") || !j["scene"].contains("name") || !j["scene"].contains("entities")) {
            s_lastError = "Invalid JSON structure: missing 'scene', 'name' or 'entities'";
            CORE_LOG_ERROR("SceneSerializer: %s", s_lastError.c_str());
            return nullptr;
        }

        // Create scene
        std::string sceneName = j["scene"]["name"];
        Scene* scene = new Scene(sceneName);

        CORE_LOG_INFO("SceneSerializer: Loading scene: %s", sceneName.c_str());

        // Load entities
        const json& entitiesArray = j["scene"]["entities"];
        unsigned int loadedEntities = 0;

        for (const auto& entityJson : entitiesArray) {
            // Validate entity structure
            if (!entityJson.contains("id") || !entityJson.contains("name")) {
                // Skip entity with missing data (no log, silent skip)
                continue;
            }

            // Create entity
            std::string entityName = entityJson["name"];
            Entity* entity = scene->CreateEntity(entityName);

            // Load transform
            if (entityJson.contains("transform")) {
                const json& transformJson = entityJson["transform"];

                Transform* transform = entity->GetComponent<Transform>();
                if (transform) {
                    // Position
                    if (transformJson.contains("position") && transformJson["position"].size() == 3) {
                        DirectX::XMFLOAT3 pos(
                            transformJson["position"][0],
                            transformJson["position"][1],
                            transformJson["position"][2]
                        );
                        transform->SetPosition(pos);
                    }

                    // Rotation (convert degrees to radians)
                    if (transformJson.contains("rotation") && transformJson["rotation"].size() == 3) {
                        float rotX = DirectX::XMConvertToRadians(transformJson["rotation"][0].get<float>());
                        float rotY = DirectX::XMConvertToRadians(transformJson["rotation"][1].get<float>());
                        float rotZ = DirectX::XMConvertToRadians(transformJson["rotation"][2].get<float>());
                        transform->SetRotation(rotX, rotY, rotZ);
                    }

                    // Scale
                    if (transformJson.contains("scale") && transformJson["scale"].size() == 3) {
                        DirectX::XMFLOAT3 scale(
                            transformJson["scale"][0],
                            transformJson["scale"][1],
                            transformJson["scale"][2]
                        );
                        transform->SetScale(scale);
                    }
                }
            }

            // TODO (future): Load other components

            loadedEntities++;
        }

        CORE_LOG_INFO("SceneSerializer: Scene loaded successfully");
        CORE_LOG_INFO("  Entities loaded: %u", loadedEntities);

        return scene;
    }
    catch (const std::exception& e) {
        s_lastError = std::string("Exception during load: ") + e.what();
        CORE_LOG_ERROR("SceneSerializer: %s", s_lastError.c_str());
        return nullptr;
    }
}

const std::string& SceneSerializer::GetLastError()
{
    return s_lastError;
}

} // namespace Scene
