#pragma once

#include <chrono>
#include <deque>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>
#if defined(_WIN32)
#include <windows.h>
#endif

namespace Core {

enum class LogLevel { Info, Warn, Error };

struct LogEntry {
    LogLevel level;
    std::string message;
    double timestamp; // Seconds since program start
    
    LogEntry(LogLevel lvl, const std::string& msg, double ts)
        : level(lvl), message(msg), timestamp(ts) {}
};

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

// Global log storage (ring buffer with max capacity)
inline std::deque<LogEntry>& GetLogBuffer()
{
    static std::deque<LogEntry> s_logBuffer;
    return s_logBuffer;
}

inline std::mutex& GetLogMutex()
{
    static std::mutex s_logMutex;
    return s_logMutex;
}

inline double GetElapsedSeconds()
{
    static auto s_startTime = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(now - s_startTime).count();
}

inline void Log(LogLevel level, const std::string& msg)
{
    std::lock_guard<std::mutex> lock(GetLogMutex());
    
    // Add to ring buffer (max 1000 entries)
    auto& buffer = GetLogBuffer();
    buffer.emplace_back(level, msg, GetElapsedSeconds());
    if (buffer.size() > 1000) {
        buffer.pop_front();
    }
    
    // Also emit to stdout/stderr
    if (level == LogLevel::Error)
        std::cerr << "[" << ToString(level) << "] " << msg << std::endl;
    else
        std::cout << "[" << ToString(level) << "] " << msg << std::endl;

#if defined(_WIN32)
    // Also emit to Visual Studio Output window
    std::string out = std::string("[") + ToString(level) + "] " + msg + "\n";
    int required = MultiByteToWideChar(CP_UTF8, 0, out.c_str(), -1, NULL, 0);
    if (required > 0)
    {
        std::vector<wchar_t> buf(required);
        MultiByteToWideChar(CP_UTF8, 0, out.c_str(), -1, buf.data(), required);
        OutputDebugStringW(buf.data());
    }
#endif
}

// Get all log entries (for Console panel)
inline std::vector<LogEntry> GetLogs()
{
    std::lock_guard<std::mutex> lock(GetLogMutex());
    auto& buffer = GetLogBuffer();
    return std::vector<LogEntry>(buffer.begin(), buffer.end());
}

// Convenience macros
#define CORE_LOG_INFO(msg)  ::Core::Log(::Core::LogLevel::Info,  (msg))
#define CORE_LOG_WARN(msg)  ::Core::Log(::Core::LogLevel::Warn,  (msg))
#define CORE_LOG_ERROR(msg) ::Core::Log(::Core::LogLevel::Error, (msg))

} // namespace Core
