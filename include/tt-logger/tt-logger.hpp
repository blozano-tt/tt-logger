#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>
#include <unordered_map>
#include <string>
#include <mutex>
#include <array>

namespace tt {

enum class LogCategory {
    Always,
    Device,
    Model,
    Runtime,
    Loader,
    IO,
    Compile,
    Build,
    Verification,
    Golden,
    Operation,
    HLK,
    Graph,
    Dispatch,
    Fabric,
    Metal,
    SiliconDriver,
    EmulationDriver,
    Custom,  // Special category for dynamic categories
    Count    // Keep track of the number of categories
};

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Critical,
    Off
};

class Logger {
public:
    static Logger& getInstance();

    // Set default category for logging
    static void setDefaultCategory(LogCategory category) {
        getInstance().default_category_ = category;
    }

    // Get default category
    static LogCategory getDefaultCategory() {
        return getInstance().default_category_;
    }

    // Get logger for a custom category
    std::shared_ptr<spdlog::logger> getOrCreateLogger(LogCategory category);

    // Configure logging level for all loggers
    void setLevel(LogLevel level);

    // Configure logging level for specific logger
    void setLevel(LogCategory category, LogLevel level);

private:
    Logger() : default_category_(LogCategory::Custom) {}
    std::unordered_map<LogCategory, std::shared_ptr<spdlog::logger>> loggers_;
    std::mutex mutex_;
    LogCategory default_category_;
};

// Helper macro to handle both with and without category
#define TT_LOG_IMPL(level, category, ...) \
    tt::Logger::getInstance().getOrCreateLogger(category)->level(__VA_ARGS__)

// Convenience macros for using a default category
#ifdef NDEBUG
#define TT_LOG_TRACE(...)    ((void)0)
#define TT_LOG_DEBUG(...)    ((void)0)
#else
#define TT_LOG_TRACE(...)    TT_LOG_IMPL(trace, tt::Logger::getDefaultCategory(), __VA_ARGS__ " [{}:{}]", __FILE__, __LINE__)
#define TT_LOG_DEBUG(...)    TT_LOG_IMPL(debug, tt::Logger::getDefaultCategory(), __VA_ARGS__)
#endif
#define TT_LOG_INFO(...)     TT_LOG_IMPL(info, tt::Logger::getDefaultCategory(), __VA_ARGS__)
#define TT_LOG_WARNING(...)  TT_LOG_IMPL(warn, tt::Logger::getDefaultCategory(), __VA_ARGS__)
#define TT_LOG_ERROR(...)    TT_LOG_IMPL(error, tt::Logger::getDefaultCategory(), __VA_ARGS__)
#define TT_LOG_CRITICAL(...) TT_LOG_IMPL(critical, tt::Logger::getDefaultCategory(), __VA_ARGS__)

// Convenience macros with explicit category
#ifdef NDEBUG
#define TT_LOG_TRACE_CAT(category, ...)    ((void)0)
#define TT_LOG_DEBUG_CAT(category, ...)    ((void)0)
#else
#define TT_LOG_TRACE_CAT(category, ...)    TT_LOG_IMPL(trace, category, __VA_ARGS__ " [{}:{}]", __FILE__, __LINE__)
#define TT_LOG_DEBUG_CAT(category, ...)    TT_LOG_IMPL(debug, category, __VA_ARGS__)
#endif
#define TT_LOG_INFO_CAT(category, ...)     TT_LOG_IMPL(info, category, __VA_ARGS__)
#define TT_LOG_WARN_CAT(category, ...)     TT_LOG_IMPL(warn, category, __VA_ARGS__)
#define TT_LOG_ERROR_CAT(category, ...)    TT_LOG_IMPL(error, category, __VA_ARGS__)
#define TT_LOG_CRITICAL_CAT(category, ...) TT_LOG_IMPL(critical, category, __VA_ARGS__)

} // namespace tt 