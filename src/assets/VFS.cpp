#include "VFS.h"

#include "../core/Log.h"
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
    if (!ifs) {
        CORE_LOG_ERROR(std::string("VFS::ReadFile failed to open: ") + path);
        return false;
    }
    std::streamsize size = ifs.tellg();
    CORE_LOG_INFO(std::string("VFS::ReadFile: path=") + path + " size=" + std::to_string(static_cast<long long>(size)));
    ifs.seekg(0, std::ios::beg);
    if (size == 0) {
        CORE_LOG_ERROR(std::string("VFS::ReadFile empty file: ") + path);
        return false;
    }
    out.resize((size_t)size);
    ifs.read(out.data(), size);
    return true;
}

} // namespace Assets
