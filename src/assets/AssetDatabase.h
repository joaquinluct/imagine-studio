#pragma once

#include <cstdint>
#include <string>
#include <vector> // v2.3.0 H5

namespace Assets {

// Asset unique identifier
using AssetID = uint64_t;

// Asset type enumeration
enum class AssetType {
    Texture,
    Mesh,
    Shader,
    Scene,
    Unknown
};

// Asset metadata structure
struct AssetMetadata {
    AssetID id;
    AssetType type;
    std::string path;
    std::string name;
    uint64_t fileSize;
    uint64_t lastModified;
};

// Asset database singleton
class AssetDatabase {
public:
    static AssetDatabase& GetInstance();
    
    // Asset registration
    void RegisterAsset(const AssetMetadata& metadata);
    void UnregisterAsset(AssetID id);
    
    // Asset queries
    bool HasAsset(AssetID id) const;
    const AssetMetadata* GetMetadata(AssetID id) const;
    
    // v2.3.0 H5: Query assets by type
    std::vector<AssetID> GetAssetsByType(AssetType type) const;
    std::vector<const AssetMetadata*> GetAllAssets() const;
    
private:
    AssetDatabase() = default;
    ~AssetDatabase() = default;
    
    // Delete copy constructor and assignment operator
    AssetDatabase(const AssetDatabase&) = delete;
    AssetDatabase& operator=(const AssetDatabase&) = delete;
};

} // namespace Assets
