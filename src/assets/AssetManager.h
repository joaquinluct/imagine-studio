#pragma once

#include <string>
#include <functional>

// Forward declaration for synchronization fence
namespace Renderer { class Fence; }

namespace Assets {

class AssetManager {
public:
    AssetManager();
    ~AssetManager();

    void Initialize();
    void Shutdown();

    // Load an asset asynchronously. Callback is invoked on worker thread when done.
    void LoadAsync(const std::string& path, std::function<void(const std::string&)> callback, Renderer::Fence* signalFence = nullptr);
};

} // namespace Assets
