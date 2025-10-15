#pragma once

#include <string>
#include <vector>
namespace Assets {

class VFS {
public:
    VFS() = default;
    ~VFS() = default;

    // Mount a path (stubbed)
    bool Mount(const std::string& path);

    // Read a file into buffer (returns false if not found)
    bool ReadFile(const std::string& path, std::vector<char>& out);
};

} // namespace Assets
