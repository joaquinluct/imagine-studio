#pragma once

#include <string>
#include <iostream>
#include <mutex>

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
}

// Convenience macros
#define CORE_LOG_INFO(msg)  ::Core::Log(::Core::LogLevel::Info,  (msg))
#define CORE_LOG_WARN(msg)  ::Core::Log(::Core::LogLevel::Warn,  (msg))
#define CORE_LOG_ERROR(msg) ::Core::Log(::Core::LogLevel::Error, (msg))

} // namespace Core
