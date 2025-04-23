# tt-logger

A simple logging library built on top of spdlog.

## Features

- Singleton-based logger instance
- Thread-safe logging with double-checked locking
- Colored console output
- Multiple log levels (TRACE, DEBUG, INFO, WARN, ERROR, CRITICAL)
- Category-based logging (Device, Model, Runtime, etc.)
- Easy to use macros for logging
- File and line number tracking for trace logs

## Dependencies

- spdlog (automatically managed via CPM)
- Catch2 (for tests)

## Building

```bash
# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build .

# Run tests
ctest

# Install (optional)
cmake --install .
```

## Usage

```cpp
#include <tt-logger/tt-logger.hpp>

int main() {
    // Get logger instance
    auto& logger = tt::Logger::getInstance();
    
    // Set global log level
    logger.setLevel(tt::LogLevel::Debug);
    
    // Set specific category level
    logger.setLevel(tt::LogCategory::Device, tt::LogLevel::Info);
    
    // Log with categories
    TT_LOG_INFO_CAT(tt::LogCategory::Device, "Device message");
    TT_LOG_DEBUG_CAT(tt::LogCategory::Model, "Model debug message");
    
    // Set default category
    tt::Logger::setDefaultCategory(tt::LogCategory::Device);
    
    // Log using default category
    TT_LOG_INFO("Default category message");
    
    // Trace logging (includes file and line)
    TT_LOG_TRACE_CAT(tt::LogCategory::Device, "Trace message");
    
    return 0;
}
```

## Available Categories

- Always
- Device
- Model
- Runtime
- Loader
- IO
- Compile
- Build
- Verification
- Golden
- Operation
- HLK
- Graph
- Dispatch
- Fabric
- Metal
- SiliconDriver
- EmulationDriver
- Custom

## CMake Integration

Add to your project's CMakeLists.txt:

```cmake
# Add CPM
include(cmake/CPM.cmake)

# Add tt-logger
CPMAddPackage("gh:blozano-tt/tt-logger@0.1.0")

# Link to your target
target_link_libraries(your_target PRIVATE tt-logger::tt-logger)
```

## Project Structure

```
tt-logger/
├── include/
│   └── tt-logger/
│       └── tt-logger.hpp
├── src/
│   └── tt-logger/
│       └── tt-logger.cpp
├── tests/
│   ├── tt-logger-test.cpp
│   └── CMakeLists.txt
├── cmake/
│   ├── CPM.cmake
│   └── get_cpm.cmake
├── CMakeLists.txt
└── README.md
```

## License

[Your chosen license]
