#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <chrono>

namespace Assets {

// v2.1.0 H2.2: Simple file watcher using polling (Windows)
// Detects file modifications by comparing last write time
class FileWatcher {
public:
    using FileChangedCallback = std::function<void(const std::string&)>;
    
    FileWatcher();
    ~FileWatcher();
    
    // Watch a file for changes
    // Callback is called when file modification is detected
    void WatchFile(const std::string& filepath, FileChangedCallback callback);
    
    // Stop watching a file
    void UnwatchFile(const std::string& filepath);
    
    // Clear all watched files
    void Clear();
    
    // Update (should be called every frame)
    // Checks all watched files and triggers callbacks if modified
    void Update();

private:
    struct WatchedFile {
        std::string filepath;
        FileChangedCallback callback;
        std::chrono::system_clock::time_point lastModified;
    };
    
    std::unordered_map<std::string, WatchedFile> m_watchedFiles;
    
    // Helper: Get last modified time of file
    std::chrono::system_clock::time_point GetLastModifiedTime(const std::string& filepath);
};

} // namespace Assets
