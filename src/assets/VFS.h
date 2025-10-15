#pragma once

#include <string>
#include <vector>
#include <mutex>

namespace Assets {

class VFS {
public:
    VFS();
    ~VFS();

    // Mount a path so ReadFile will search it
    bool Mount(const std::string& path);

    // Read a file into buffer (returns false if not found)
    bool ReadFile(const std::string& path, std::vector<char>& out);

private:
    std::vector<std::string> mounts_;
    std::mutex mountsMutex_;
};

} // namespace Assets
