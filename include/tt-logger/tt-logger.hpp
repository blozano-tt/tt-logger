#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <fmt/std.h>

#include <array>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

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

    // Get stack trace as string
    static std::string getStackTrace();

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
#define TT_LOG_TRACE(...)    TT_LOG_IMPL(trace, tt::Logger::getDefaultCategory(), fmt::format(__VA_ARGS__) + fmt::format(" [{}:{}]", __FILE__, __LINE__))
#define TT_LOG_DEBUG(...)    TT_LOG_IMPL(debug, tt::Logger::getDefaultCategory(), __VA_ARGS__)
#endif
#define TT_LOG_INFO(...)     TT_LOG_IMPL(info, tt::Logger::getDefaultCategory(), __VA_ARGS__)
#define TT_LOG_WARNING(...)  TT_LOG_IMPL(warn, tt::Logger::getDefaultCategory(), __VA_ARGS__)
#define TT_LOG_ERROR(...)    TT_LOG_IMPL(error, tt::Logger::getDefaultCategory(), __VA_ARGS__)
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define TT_LOG_CRITICAL(...) TT_LOG_IMPL(critical, tt::Logger::getDefaultCategory(), fmt::format(__VA_ARGS__) + fmt::format(" [{}:{}]", __FILE__, __LINE__))

// Convenience macros with explicit category
#ifdef NDEBUG
#define TT_LOG_TRACE_CAT(category, ...)    ((void)0)
#define TT_LOG_DEBUG_CAT(category, ...)    ((void)0)
#else
#define TT_LOG_TRACE_CAT(category, ...)    TT_LOG_IMPL(trace, category, __VA_ARGS__ " [{}:{}]", __FILE__, __LINE__)
#define TT_LOG_DEBUG_CAT(category, ...)    TT_LOG_IMPL(debug, category, __VA_ARGS__)
#endif
#define TT_LOG_INFO_CAT(category, ...)     TT_LOG_IMPL(info, category, __VA_ARGS__)
#define TT_LOG_WARNING_CAT(category, ...)  TT_LOG_IMPL(warn, category, __VA_ARGS__)
#define TT_LOG_ERROR_CAT(category, ...)    TT_LOG_IMPL(error, category, __VA_ARGS__)
#define TT_LOG_CRITICAL_CAT(category, ...) TT_LOG_IMPL(critical, category, __VA_ARGS__ " [{}:{}]", __FILE__, __LINE__)

// Fatal logging macros that terminate the program
#define TT_LOG_FATAL(...) \
    do { \
        TT_LOG_CRITICAL(__VA_ARGS__); \
        TT_LOG_CRITICAL(tt::Logger::getStackTrace()); \
        std::exit(EXIT_FAILURE); \
    } while (0)

#define TT_LOG_FATAL_CAT(category, ...) \
    do { \
        TT_LOG_CRITICAL_CAT(category, __VA_ARGS__); \
        TT_LOG_CRITICAL_CAT(category, "{}", tt::Logger::getStackTrace()); \
        std::exit(EXIT_FAILURE); \
    } while (0)

// Test-specific macro that mimics TT_LOG_FATAL but doesn't exit
// Only defined when TT_LOGGER_TESTING is defined
#ifdef TT_LOGGER_TESTING
#define TT_LOG_FATAL_TEST(...) \
    do { \
        TT_LOG_CRITICAL(__VA_ARGS__); \
        TT_LOG_CRITICAL(tt::Logger::getStackTrace()); \
    } while (0)

#define TT_LOG_FATAL_TEST_CAT(category, ...) \
    do { \
        TT_LOG_CRITICAL_CAT(category, __VA_ARGS__); \
        TT_LOG_CRITICAL_CAT(category, "{}", tt::Logger::getStackTrace()); \
    } while (0)
#endif

// Throw exception with logging
#define TT_THROW(...) \
    do { \
        std::string msg = fmt::format(__VA_ARGS__); \
        TT_LOG_CRITICAL("{}", msg); \
        TT_LOG_CRITICAL("{}", tt::Logger::getStackTrace()); \
        throw std::runtime_error(msg); \
    } while (0)

#define TT_THROW_CAT(category, ...) \
    do { \
        std::string msg = fmt::format(__VA_ARGS__); \
        TT_LOG_CRITICAL_CAT(category, "{}", msg); \
        TT_LOG_CRITICAL_CAT(category, "{}", tt::Logger::getStackTrace()); \
        throw std::runtime_error(msg); \
    } while (0)

} // namespace tt

// Global namespace aliases for log categories
constexpr tt::LogCategory LogAlways = tt::LogCategory::Always;
constexpr tt::LogCategory LogDevice = tt::LogCategory::Device;
constexpr tt::LogCategory LogModel = tt::LogCategory::Model;
constexpr tt::LogCategory LogRuntime = tt::LogCategory::Runtime;
constexpr tt::LogCategory LogLoader = tt::LogCategory::Loader;
constexpr tt::LogCategory LogIO = tt::LogCategory::IO;
constexpr tt::LogCategory LogCompile = tt::LogCategory::Compile;
constexpr tt::LogCategory LogBuild = tt::LogCategory::Build;
constexpr tt::LogCategory LogVerification = tt::LogCategory::Verification;
constexpr tt::LogCategory LogGolden = tt::LogCategory::Golden;
constexpr tt::LogCategory LogOperation = tt::LogCategory::Operation;
constexpr tt::LogCategory LogHLK = tt::LogCategory::HLK;
constexpr tt::LogCategory LogGraph = tt::LogCategory::Graph;
constexpr tt::LogCategory LogDispatch = tt::LogCategory::Dispatch;
constexpr tt::LogCategory LogFabric = tt::LogCategory::Fabric;
constexpr tt::LogCategory LogMetal = tt::LogCategory::Metal;
constexpr tt::LogCategory LogSiliconDriver = tt::LogCategory::SiliconDriver;
constexpr tt::LogCategory LogEmulationDriver = tt::LogCategory::EmulationDriver;
constexpr tt::LogCategory LogCustom = tt::LogCategory::Custom;

// Assertion macros
#ifdef NDEBUG
#define TT_ASSERT(condition, ...)
#else
#define TT_ASSERT(condition, ...) \
    do { \
        if (not(condition)) [[unlikely]] \
            TT_LOG_FATAL("Assertion failed: {}", #condition, ##__VA_ARGS__); \
    } while (0)
#endif

#define TT_FATAL(condition, ...) \
    do { \
        if (not(condition)) [[unlikely]] \
            TT_LOG_FATAL("Fatal error: {}", #condition, ##__VA_ARGS__); \
    } while (0)
