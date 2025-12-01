#include "AssetDatabase.h"
#include <unordered_map>
#include <mutex>

namespace Assets {

// Internal storage for assets
static std::unordered_map<AssetID, AssetMetadata> s_assetRegistry;
static std::mutex s_registryMutex;

AssetDatabase& AssetDatabase::GetInstance() {
    static AssetDatabase instance;
    return instance;
}

void AssetDatabase::RegisterAsset(const AssetMetadata& metadata) {
    std::lock_guard<std::mutex> lock(s_registryMutex);
    s_assetRegistry[metadata.id] = metadata;
}

void AssetDatabase::UnregisterAsset(AssetID id) {
    std::lock_guard<std::mutex> lock(s_registryMutex);
    auto it = s_assetRegistry.find(id);
    if (it != s_assetRegistry.end()) {
        s_assetRegistry.erase(it);
    }
}

bool AssetDatabase::HasAsset(AssetID id) const {
    std::lock_guard<std::mutex> lock(s_registryMutex);
    return s_assetRegistry.find(id) != s_assetRegistry.end();
}

const AssetMetadata* AssetDatabase::GetMetadata(AssetID id) const {
    std::lock_guard<std::mutex> lock(s_registryMutex);
    auto it = s_assetRegistry.find(id);
    if (it != s_assetRegistry.end()) {
        return &it->second;
    }
    return nullptr;
}

// v2.3.0 H5: Get all assets of a specific type
std::vector<AssetID> AssetDatabase::GetAssetsByType(AssetType type) const {
    std::lock_guard<std::mutex> lock(s_registryMutex);
    std::vector<AssetID> result;
    
    for (const auto& pair : s_assetRegistry) {
        if (pair.second.type == type) {
            result.push_back(pair.first);
        }
    }
    
    return result;
}

// v2.3.0 H5: Get all assets metadata
std::vector<const AssetMetadata*> AssetDatabase::GetAllAssets() const {
    std::lock_guard<std::mutex> lock(s_registryMutex);
    std::vector<const AssetMetadata*> result;
    
    for (const auto& pair : s_assetRegistry) {
        result.push_back(&pair.second);
    }
    
    return result;
}

} // namespace Assets
