# tt-logger

A simple logging library built on top of spdlog.

## Features

- Singleton-based logger instance
- Thread-safe logging
- Colored console output

```bash
# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build .

# Install (optional)
cmake --install .
```

## Usage

```cpp
#include <tt-logger/tt-logger.hpp>

int main() {
    // Initialize the logger
    tt::Logger::getInstance().initialize("my-app");
    
    // Use the logging macros
    TT_LOG_INFO("Hello, world!");
    TT_LOG_ERROR("Something went wrong: {}", error_code);
    
    return 0;
}
```

## CMake Integration

Add to your project's CMakeLists.txt:

```cmake
# Add CPM
include(cmake/CPM.cmake)

# Add tt-logger
CPMAddPackage("gh:your-username/tt-logger@0.1.0")

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
├── cmake/
│   ├── CPM.cmake
│   └── get_cpm.cmake
├── CMakeLists.txt
└── README.md
```

## License

[Your chosen license]
