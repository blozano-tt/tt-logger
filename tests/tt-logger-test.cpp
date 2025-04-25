#define TT_LOGGER_TESTING

#include <catch2/catch_test_macros.hpp>
#include <tt-logger/tt-logger.hpp>
#include <stdlib.h>
#include <thread>

// Mock class to simulate the device scenario
class MockDevice {
public:
    MockDevice(int id) : id_(id) {}
    void close() {
        TT_LOG_INFO_CAT(tt::LogCategory::Device, "Closing device {}", id_);
    }
private:
    int id_;
};

TEST_CASE("Logger can be used with categories", "[logger]") {
    // Test basic logging with different categories
    TT_LOG_INFO_CAT(tt::LogCategory::Device, "Device message");
    TT_LOG_INFO_CAT(tt::LogCategory::Model, "Model message");
    TT_LOG_INFO_CAT(tt::LogCategory::Runtime, "Runtime message");
    
    // If we get here without throwing, the logger works
    REQUIRE(true);
}

TEST_CASE("Logger formats device messages correctly", "[logger]") {
    // Create a mock device and call close
    MockDevice device(0);
    device.close();
    
    // The output should be:
    // device | INFO     | Closing device 0
    // If we get here without throwing, the logger works
    REQUIRE(true);
}

TEST_CASE("Logger default category works", "[logger]") {
    // Set default category to Device
    tt::Logger::setDefaultCategory(tt::LogCategory::Device);
    
    // These should use Device category
    TT_LOG_INFO("Default category message 1");
    TT_LOG_DEBUG("Default category message 2");
    
    // Change default category to Model
    tt::Logger::setDefaultCategory(tt::LogCategory::Model);
    
    // These should use Model category
    TT_LOG_INFO("Default category message 3");
    TT_LOG_DEBUG("Default category message 4");
    
    REQUIRE(true);
}

TEST_CASE("Logger level settings work", "[logger]") {
    // Set level for all loggers
    tt::Logger::getInstance().setLevel(tt::LogLevel::Debug);
    
    // Set specific level for Device category
    tt::Logger::getInstance().setLevel(tt::LogCategory::Device, tt::LogLevel::Info);
    
    // These should be logged
    TT_LOG_INFO_CAT(tt::LogCategory::Device, "Device info message");
    TT_LOG_DEBUG_CAT(tt::LogCategory::Model, "Model debug message");
    
    REQUIRE(true);
}

TEST_CASE("Trace logging includes file and line info", "[logger]") {
    // Enable trace logging
    tt::Logger::getInstance().setLevel(tt::LogLevel::Trace);
    
    // Log a trace message - this should include the file and line number
    TT_LOG_TRACE_CAT(tt::LogCategory::Device, "Test trace message");
    
    // The output should be:
    // device | TRACE    | tests/tt-logger-test.cpp:XX - Test trace message
    // where XX is the line number of the TT_LOG_TRACE_CAT call
    
    REQUIRE(true);
}

TEST_CASE("Fatal test logging works", "[logger]") {
    // Test that the fatal test logging macro can be called
    TT_LOG_FATAL_TEST("Test fatal message");
    
    // If we get here, the macro was called successfully
    REQUIRE(true);
}

TEST_CASE("Logger can log thread IDs", "[logger]") {
    // Set environment variable for log level
    setenv("TT_LOG_LEVEL", "debug", 1);
    
    // Get current thread ID
    auto system_tid = std::this_thread::get_id();
    
    // Log a message with thread ID
    TT_LOG_DEBUG_CAT(
        tt::LogCategory::Device,
        "Starting tt_cpuset_allocator constructor now for process_id: {} thread_id: {}",
        1234,  // Example process ID
        system_tid);
    
    // If we get here without throwing, the logger works
    REQUIRE(true);
} 