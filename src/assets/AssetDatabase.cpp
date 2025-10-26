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

} // namespace Assets
