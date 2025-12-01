#include "FileWatcher.h"
#include "../core/Log.h"

#if defined(_WIN32) && defined(_MSC_VER)
#include <windows.h>
#else
#include <sys/stat.h>
#endif

namespace Assets {

FileWatcher::FileWatcher()
{
    CORE_LOG_INFO("FileWatcher initialized");
}

FileWatcher::~FileWatcher()
{
    Clear();
}

void FileWatcher::WatchFile(const std::string& filepath, FileChangedCallback callback)
{
    if (filepath.empty() || !callback)
    {
        CORE_LOG_ERROR("FileWatcher::WatchFile: Invalid filepath or callback");
        return;
    }
    
    WatchedFile watchedFile;
    watchedFile.filepath = filepath;
    watchedFile.callback = callback;
    watchedFile.lastModified = GetLastModifiedTime(filepath);
    
    m_watchedFiles[filepath] = watchedFile;
    
    CORE_LOG_INFO("FileWatcher: Now watching file: " + filepath);
}

void FileWatcher::UnwatchFile(const std::string& filepath)
{
    auto it = m_watchedFiles.find(filepath);
    if (it != m_watchedFiles.end())
    {
        m_watchedFiles.erase(it);
        CORE_LOG_INFO("FileWatcher: Stopped watching file: " + filepath);
    }
}

void FileWatcher::Clear()
{
    m_watchedFiles.clear();
    CORE_LOG_INFO("FileWatcher: Cleared all watched files");
}

void FileWatcher::Update()
{
    // Poll all watched files for changes
    for (auto& pair : m_watchedFiles)
    {
        WatchedFile& watchedFile = pair.second;
        
        // Get current modified time
        auto currentModified = GetLastModifiedTime(watchedFile.filepath);
        
        // Check if file was modified since last check
        if (currentModified > watchedFile.lastModified)
        {
            CORE_LOG_INFO("FileWatcher: File modified detected: " + watchedFile.filepath);
            
            // Update last modified time
            watchedFile.lastModified = currentModified;
            
            // Trigger callback
            if (watchedFile.callback)
            {
                watchedFile.callback(watchedFile.filepath);
            }
        }
    }
}

std::chrono::system_clock::time_point FileWatcher::GetLastModifiedTime(const std::string& filepath)
{
#if defined(_WIN32) && defined(_MSC_VER)
    // Windows: Use GetFileAttributesEx
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    if (GetFileAttributesExA(filepath.c_str(), GetFileExInfoStandard, &fileInfo))
    {
        // Convert FILETIME to time_point
        ULARGE_INTEGER ull;
        ull.LowPart = fileInfo.ftLastWriteTime.dwLowDateTime;
        ull.HighPart = fileInfo.ftLastWriteTime.dwHighDateTime;
        
        // FILETIME is in 100-nanosecond intervals since Jan 1, 1601
        // Convert to system_clock time_point
        auto duration = std::chrono::duration<int64_t, std::ratio<1, 10000000>>(ull.QuadPart);
        
        // Jan 1, 1601 to Jan 1, 1970 is 11644473600 seconds
        auto epoch = std::chrono::system_clock::from_time_t(0);
        auto fileTime = epoch + duration - std::chrono::seconds(11644473600);
        
        return fileTime;
    }
    
    // File not found or error - return epoch
    return std::chrono::system_clock::from_time_t(0);
#else
    // Unix: Use stat
    struct stat fileInfo;
    if (stat(filepath.c_str(), &fileInfo) == 0)
    {
        return std::chrono::system_clock::from_time_t(fileInfo.st_mtime);
    }
    
    // File not found or error - return epoch
    return std::chrono::system_clock::from_time_t(0);
#endif
}

} // namespace Assets
