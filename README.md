# tt-logger

A Tenstorrent logging library built on top of spdlog and fmt.

## Features

- Built on top of spdlog for robust logging infrastructure

## Dependencies

- spdlog (automatically managed via CPM)
- fmt (automatically managed via CPM)
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
    // Log with different levels and categories
    tt::log_info(tt::LogDevice, "Device message");
    tt::log_debug(tt::LogModel, "Model debug message");
    tt::log_warning(tt::LogLLRuntime, "Runtime warning");
    tt::log_error(tt::LogDevice, "Device error");
    tt::log_critical(tt::LogModel, "Model critical error");

    // Log with format strings
    tt::log_info(tt::LogDevice, "Device {} message", 123);
    tt::log_info(tt::LogModel, "Model {} with {} parameters", "test", 42);

    // Log with default category (LogAlways)
    tt::log_info("Default category message");

    return 0;
}
```

## Available Categories
To be refined

- Always
- Test
- Timer
- Device
- Model
- LLRuntime
- Loader
- IO
- CompileTrisc
- BuildKernels
- Verif
- Golden
- Op
- HLK
- HLKC
- Reportify
- GraphCompiler
- Dispatch
- Fabric
- Metal
- MetalTrace
- SiliconDriver
- EmulationDriver

## CMake Integration

Add to your project's CMakeLists.txt:

```cmake
# Add CPM
include(cmake/CPM.cmake)

# Add tt-logger
CPMAddPackage("gh:blozano-tt/tt-logger@0.2.0")

# Link to your target
target_link_libraries(your_target PRIVATE tt-logger::tt-logger)
```

## Project Structure

```
tt-logger/
├── include/
│   └── tt-logger/
│       └── tt-logger.hpp
├── tests/
│   ├── tt-logger-test.cpp
│   └── CMakeLists.txt
├── cmake/
│   └── CPM.cmake
├── CMakeLists.txt
└── README.md
```

## License

Copyright 2024 Tenstorrent Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
