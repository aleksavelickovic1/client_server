#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <sstream>
#include <iostream>
#include <chrono>
#include <ctime>

enum class LogLevel { DEBUG, INFO, WARN, ERROR };

inline std::string to_string(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
    }
    return "UNKNOWN";
}

class Logger {
public:

    static Logger& instance() {
        static Logger logger;
        return logger;
    }

    void setLogFile(const std::string& path) {
        std::lock_guard<std::mutex> lock(mutex_);
        fileStream_.open(path, std::ios::app);
            if (!fileStream_.is_open()) {
                 std::cerr << "Failed to open log file: " << path << std::endl;
            }
    }

    void setMinLevel(LogLevel level) {
        minLevel_ = level;
    }
    
    void setConsoleLogging(bool enabled) {
        std::lock_guard<std::mutex> lock(mutex_);
        logToConsole_ = enabled;
    }

    void log(LogLevel level, const std::string& component,
             const std::string& message,
             const char* file, int line)
    {
        if (level < minLevel_) return;

        std::string entry = format(level, component, message, file, line);

        std::lock_guard<std::mutex> lock(mutex_);

        
        if (logToConsole_) {
            std::cerr << entry << "\n";
        }

        if (fileStream_.is_open()) {
            fileStream_ << entry << "\n";
            fileStream_.flush(); 
        }
    }

    Logger(const Logger&)            = delete;
    Logger& operator=(const Logger&) = delete;

private:

    Logger() = default;
    bool logToConsole_ = true;
    static std::string timestamp() {
        auto now  = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        char buf[20];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
        return buf;
    }

    static std::string format(LogLevel level, const std::string& component,
                               const std::string& message,
                               const char* file, int line)
    {
        std::ostringstream oss;
        oss << "[" << timestamp() << "]"
            << "[" << to_string(level) << "]"
            << "[" << component << "] "
            << message
            << "  (" << file << ":" << line << ")";
        return oss.str();
    }

    std::mutex   mutex_;
    std::ofstream fileStream_;
    LogLevel     minLevel_ = LogLevel::DEBUG;
};


#define LOG_DEBUG(component, msg) do { \
    std::ostringstream _oss; _oss << msg; \
    Logger::instance().log(LogLevel::DEBUG, component, _oss.str(), __FILE__, __LINE__); \
} while(0)

#define LOG_INFO(component, msg) do { \
    std::ostringstream _oss; _oss << msg; \
    Logger::instance().log(LogLevel::INFO,  component, _oss.str(), __FILE__, __LINE__); \
} while(0)

#define LOG_WARN(component, msg) do { \
    std::ostringstream _oss; _oss << msg; \
    Logger::instance().log(LogLevel::WARN,  component, _oss.str(), __FILE__, __LINE__); \
} while(0)

#define LOG_ERROR(component, msg) do { \
    std::ostringstream _oss; _oss << msg; \
    Logger::instance().log(LogLevel::ERROR, component, _oss.str(), __FILE__, __LINE__); \
} while(0)
