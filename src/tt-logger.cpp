#include <tt-logger/tt-logger.hpp>

namespace tt {

Logger::Logger() = default;

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::initialize(const std::string& name) {
    logger_ = spdlog::get(name);
    if (!logger_) {
        logger_ = spdlog::stdout_color_mt(name);
    }
}

std::shared_ptr<spdlog::logger> Logger::getLogger() const {
    return logger_;
}

} // namespace tt 