#include <catch2/catch_test_macros.hpp>
#include <tt-logger/tt-logger.hpp>

TEST_CASE("Logger can be initialized and used", "[logger]") {
    tt::Logger::getInstance().initialize("test-logger");
    TT_LOG_INFO("Hello, world!");
    // If we get here without throwing, the logger works
    REQUIRE(true);
} 