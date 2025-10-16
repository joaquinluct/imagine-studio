#pragma once

#include <iostream>
#include <mutex>
#include <string>
#include <vector>
#if defined(_WIN32)
#include <windows.h>
#endif
namespace Core {

enum class LogLevel { Info, Warn, Error };

inline const char* ToString(LogLevel level)
{
    switch (level)
    {
    case LogLevel::Info: return "INFO";
    case LogLevel::Warn: return "WARN";
    case LogLevel::Error: return "ERROR";
    default: return "LOG";
    }
}

inline void Log(LogLevel level, const std::string& msg)
{
    static std::mutex s_mutex;
    std::lock_guard<std::mutex> lock(s_mutex);
    if (level == LogLevel::Error)
        std::cerr << "[" << ToString(level) << "] " << msg << std::endl;
    else
        std::cout << "[" << ToString(level) << "] " << msg << std::endl;

#if defined(_WIN32)
    // Also emit to Visual Studio Output window for easier debugging
    std::string out = std::string("[") + ToString(level) + "] " + msg + "\n";
    // Convert UTF-8 std::string to UTF-16
    int required = MultiByteToWideChar(CP_UTF8, 0, out.c_str(), -1, NULL, 0);
    if (required > 0)
    {
        std::vector<wchar_t> buf(required);
        MultiByteToWideChar(CP_UTF8, 0, out.c_str(), -1, buf.data(), required);
        OutputDebugStringW(buf.data());
    }
#endif
}

// Convenience macros
#define CORE_LOG_INFO(msg)  ::Core::Log(::Core::LogLevel::Info,  (msg))
#define CORE_LOG_WARN(msg)  ::Core::Log(::Core::LogLevel::Warn,  (msg))
#define CORE_LOG_ERROR(msg) ::Core::Log(::Core::LogLevel::Error, (msg))

} // namespace Core
