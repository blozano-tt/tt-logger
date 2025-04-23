#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

namespace tt {

class Logger {
public:
    static Logger& getInstance();
    void initialize(const std::string& name = "tt-logger");
    std::shared_ptr<spdlog::logger> getLogger() const;

private:
    Logger();
    std::shared_ptr<spdlog::logger> logger_;
};

// Convenience macros
#define TT_LOG_TRACE(...)    tt::Logger::getInstance().getLogger()->trace(__VA_ARGS__)
#define TT_LOG_DEBUG(...)    tt::Logger::getInstance().getLogger()->debug(__VA_ARGS__)
#define TT_LOG_INFO(...)     tt::Logger::getInstance().getLogger()->info(__VA_ARGS__)
#define TT_LOG_WARN(...)     tt::Logger::getInstance().getLogger()->warn(__VA_ARGS__)
#define TT_LOG_ERROR(...)    tt::Logger::getInstance().getLogger()->error(__VA_ARGS__)
#define TT_LOG_CRITICAL(...) tt::Logger::getInstance().getLogger()->critical(__VA_ARGS__)

} // namespace tt 