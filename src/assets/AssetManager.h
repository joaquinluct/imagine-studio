#pragma once

#include <string>
#include <functional>

namespace Assets {

class AssetManager {
public:
    AssetManager();
    ~AssetManager();

    void Initialize();
    void Shutdown();

    // Load an asset asynchronously. Callback is invoked on worker thread when done.
    void LoadAsync(const std::string& path, std::function<void(const std::string&)> callback);
};

} // namespace Assets
