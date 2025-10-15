#include "VFS.h"

#include "../core/Log.h"
#include <fstream>
#include <iostream>
// Do not use <filesystem> to preserve C++14 compatibility in this build.
namespace Assets {

VFS::VFS()
{
    // Ensure default assets mount exists so LoadAsync("example_asset.dat") can find files
    Mount("assets");
}

VFS::~VFS()
{
}

bool VFS::Mount(const std::string& path)
{
    std::lock_guard<std::mutex> lk(mountsMutex_);
    // Basic normalization: remove trailing slash
    std::string p = path;
    if (!p.empty() && (p.back() == '/' || p.back() == '\\')) p.pop_back();
    mounts_.push_back(p);
    CORE_LOG_INFO(std::string("VFS: Mounted path: ") + p);
    return true;
}

bool VFS::ReadFile(const std::string& path, std::vector<char>& out)
{
    // If path is absolute or directly exists, try it first
    std::ifstream ifs(path, std::ios::binary | std::ios::ate);
    if (ifs) {
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

    // Search mounted paths
    std::lock_guard<std::mutex> lk(mountsMutex_);
    for (const auto& m : mounts_)
    {
        std::string full = m + "/" + path;
        std::ifstream ifs2(full, std::ios::binary | std::ios::ate);
        if (!ifs2) continue;
        std::streamsize size = ifs2.tellg();
        CORE_LOG_INFO(std::string("VFS::ReadFile: path=") + full + " size=" + std::to_string(static_cast<long long>(size)));
        ifs2.seekg(0, std::ios::beg);
        if (size == 0) {
            CORE_LOG_ERROR(std::string("VFS::ReadFile empty file: ") + full);
            return false;
        }
        out.resize((size_t)size);
        ifs2.read(out.data(), size);
        return true;
    }

    CORE_LOG_ERROR(std::string("VFS::ReadFile failed to open: ") + path);
    return false;
}

} // namespace Assets
