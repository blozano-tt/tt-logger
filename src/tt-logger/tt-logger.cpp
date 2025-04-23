#include "tt-logger/tt-logger.hpp"
#include <cstdlib>
#include <cstring>

namespace tt {
namespace {

// Convert enum to string - private implementation detail
const char* to_string(LogCategory category) {
    static const std::array<const char*, static_cast<size_t>(LogCategory::Count)> category_names = {
        "Always",
        "Device",
        "Model",
        "Runtime",
        "Loader",
        "IO",
        "Compile",
        "Build",
        "Verification",
        "Golden",
        "Operation",
        "HLK",
        "Graph",
        "Dispatch",
        "Fabric",
        "Metal",
        "SiliconDriver",
        "EmulationDriver",
        "Custom"
    };
    return category_names[static_cast<size_t>(category)];
}

// Convert string to LogLevel
LogLevel string_to_level(const char* str) {
    if (str == nullptr) return LogLevel::Info;
    if (strcasecmp(str, "trace") == 0) return LogLevel::Trace;
    if (strcasecmp(str, "debug") == 0) return LogLevel::Debug;
    if (strcasecmp(str, "info") == 0) return LogLevel::Info;
    if (strcasecmp(str, "warning") == 0) return LogLevel::Warning;
    if (strcasecmp(str, "error") == 0) return LogLevel::Error;
    if (strcasecmp(str, "critical") == 0) return LogLevel::Critical;
    if (strcasecmp(str, "off") == 0) return LogLevel::Off;
    return LogLevel::Info;
}

// Convert our LogLevel to spdlog's level enum
spdlog::level::level_enum to_spdlog_level(LogLevel level) {
    switch (level) {
        case LogLevel::Trace:    return spdlog::level::trace;
        case LogLevel::Debug:    return spdlog::level::debug;
        case LogLevel::Info:     return spdlog::level::info;
        case LogLevel::Warning:  return spdlog::level::warn;
        case LogLevel::Error:    return spdlog::level::err;
        case LogLevel::Critical: return spdlog::level::critical;
        case LogLevel::Off:      return spdlog::level::off;
        default:                 return spdlog::level::info;
    }
}

} // anonymous namespace

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

// Get logger for a custom category
std::shared_ptr<spdlog::logger> Logger::getOrCreateLogger(LogCategory category) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (loggers_.count(category)) {
        return loggers_[category];
    }
    auto logger = spdlog::stdout_color_mt(to_string(category));
    // \033[36m is cyan for timestamp
    // \033[35m is magenta for logger name
    // \033[0m resets the color
    logger->set_pattern("\033[36m%Y-%m-%d %H:%M:%S.%e\033[0m | \033[35m%15n\033[0m | %^%-8l%$ | %v");
    
    // Set initial level from environment variable if present
    const char* env_level = std::getenv("TT_LOG_LEVEL");
    if (env_level != nullptr) {
        logger->set_level(to_spdlog_level(string_to_level(env_level)));
    }
    
    loggers_[category] = logger;
    return logger;
}

// Configure logging level for all loggers
void Logger::setLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto spdlog_level = to_spdlog_level(level);
    for (auto& [_, logger] : loggers_) {
        logger->set_level(spdlog_level);
    }
}

// Configure logging level for specific logger
void Logger::setLevel(LogCategory category, LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (loggers_.count(category)) {
        loggers_[category]->set_level(to_spdlog_level(level));
    }
}

} // namespace tt 