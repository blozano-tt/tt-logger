cmake_minimum_required(VERSION 3.14)
project(tt-logger VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_TESTING "Build tests" ON)

include(cmake/CPM.cmake)
CPMAddPackage("gh:gabime/spdlog@1.15.2")

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        src/${PROJECT_NAME}/${PROJECT_NAME}.cpp
)

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
            include/${PROJECT_NAME}/${PROJECT_NAME}.hpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        spdlog::spdlog
)

# Add -rdynamic for debug builds to enable proper backtrace
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_options(${PROJECT_NAME} PUBLIC -rdynamic)
endif()

# Convert project name to use underscores instead of hyphens for library name
string(REPLACE "-" "_" LIBRARY_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${LIBRARY_NAME})

# Install spdlog if not installed already
if(TARGET spdlog)
    get_target_property(spdlog_is_imported spdlog::spdlog IMPORTED)
    if(NOT spdlog_is_imported)
        install(TARGETS spdlog EXPORT spdlog-targets)
        install(EXPORT spdlog-targets
            FILE spdlog-targets.cmake
            NAMESPACE spdlog::
            DESTINATION lib/cmake/spdlog)
    endif()
endif()

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    FILE_SET HEADERS DESTINATION include/${PROJECT_NAME}
)

install(EXPORT ${PROJECT_NAME}-targets
    FILE ${PROJECT_NAME}-targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_NAME}-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)

install(FILES
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    DESTINATION lib/cmake/${PROJECT_NAME}
)

# Add tests if testing is enabled
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

