cmake_minimum_required(VERSION 3.14)
project(tt-logger VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/CPM.cmake)
CPMAddPackage("gh:fmtlib/fmt#11.1.4")
CPMAddPackage(
    GITHUB_REPOSITORY gabime/spdlog
    VERSION 1.15.2
    OPTIONS
        "SPDLOG_FMT_EXTERNAL ON"
        "SPDLOG_INSTALL ON"
        "BUILD_SHARED_LIBS OFF"
        "SPDLOG_BUILD_SHARED OFF"
)

# Header-only library
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    ${PROJECT_NAME}
    INTERFACE
        spdlog::spdlog
        fmt::fmt
)

# Install headers
install(DIRECTORY include/${PROJECT_NAME}/ DESTINATION include/${PROJECT_NAME})

# Install target
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-targets)

# Install export file
install(
    EXPORT ${PROJECT_NAME}-targets
    FILE ${PROJECT_NAME}-targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}-config.cmake"
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}-config-version.cmake"
    DESTINATION lib/cmake/${PROJECT_NAME}
)

# Optional: build tests
option(TT_LOGGER_BUILD_TESTING "Build tests" OFF)
if(TT_LOGGER_BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
