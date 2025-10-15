#include "VFS.h"

#include <fstream>
#include <iostream>
namespace Assets {

bool VFS::Mount(const std::string& path)
{
    // Stub: accept any path
    std::cout << "VFS: Mount " << path << " (stub)\n";
    return true;
}

bool VFS::ReadFile(const std::string& path, std::vector<char>& out)
{
    std::ifstream ifs(path, std::ios::binary | std::ios::ate);
    if (!ifs) return false;
    std::streamsize size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    out.resize((size_t)size);
    ifs.read(out.data(), size);
    return true;
}

} // namespace Assets
