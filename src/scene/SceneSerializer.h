#pragma once

#include <string>

// Forward declarations
namespace Scene {
    class Scene;
}

namespace Scene {

/**
 * @brief SceneSerializer - Save/Load scenes to/from JSON files
 * 
 * **Version**: v1.9.0 H5.2 - Scene Serialization
 * 
 * Serializes complete scene graph to JSON format:
 * - Entities (ID, name)
 * - Transform components (position, rotation, scale)
 * - Parent-child hierarchy
 * - Asset references (future: textures, meshes)
 * 
 * JSON Structure:
 * {
 *   "scene": {
 *     "name": "MyScene",
 *     "entities": [
 *       {
 *         "id": 1,
 *         "name": "Main Camera",
 *         "transform": {
 *           "position": [0, 0, -10],
 *           "rotation": [0, 0, 0],
 *           "scale": [1, 1, 1]
 *         }
 *       }
 *     ]
 *   }
 * }
 * 
 * Part of Sprint v1.9.0 - Asset System & Resource Management
 */
class SceneSerializer {
public:
    /**
     * @brief Save scene to JSON file
     * @param scene Scene to serialize
     * @param filepath Output file path (e.g., "assets/scenes/my_scene.json")
     * @return true if save successful, false otherwise
     */
    static bool SaveScene(const Scene* scene, const std::string& filepath);

    /**
     * @brief Load scene from JSON file
     * @param filepath Input file path
     * @return Loaded scene (caller owns), nullptr if load failed
     */
    static Scene* LoadScene(const std::string& filepath);

    /**
     * @brief Get last error message
     * @return Error string from last SaveScene/LoadScene failure
     */
    static const std::string& GetLastError();

private:
    static std::string s_lastError;  ///< Thread-local would be better for multi-threading
};

} // namespace Scene
