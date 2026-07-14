cmake_minimum_required(VERSION 3.24)

project(png_image_tool VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include(CTest)

option(PIT_BUILD_CLI "Build the libpng-backed command-line application" ON)

add_library(pit_warnings INTERFACE)
if(MSVC)
    target_compile_options(pit_warnings INTERFACE /W4 /WX /utf-8)
    target_compile_definitions(pit_warnings INTERFACE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(pit_warnings INTERFACE -Wall -Wextra -Wpedantic -Werror)
endif()

add_library(pit_core
    src/cli.c
    src/image.c
    src/operations.c
)
target_include_directories(pit_core PUBLIC include)
target_link_libraries(pit_core PRIVATE pit_warnings)

if(BUILD_TESTING)
    add_executable(pit_core_tests tests/core_tests.c)
    target_link_libraries(pit_core_tests PRIVATE pit_core pit_warnings)
    add_test(NAME pit_core_tests COMMAND pit_core_tests)
endif()

if(PIT_BUILD_CLI)
    find_package(PNG 1.6 REQUIRED)

    add_library(pit_png src/png_io.c)
    target_include_directories(pit_png PUBLIC include)
    target_link_libraries(pit_png PUBLIC pit_core PNG::PNG PRIVATE pit_warnings)

    add_executable(png-image-tool src/main.c)
    target_link_libraries(png-image-tool PRIVATE pit_core pit_png pit_warnings)

    if(BUILD_TESTING)
        add_executable(pit_png_io_tests tests/png_io_tests.c)
        target_link_libraries(pit_png_io_tests PRIVATE pit_png pit_warnings)
        add_test(NAME pit_png_io_tests COMMAND pit_png_io_tests)
        add_test(NAME pit_cli_help COMMAND png-image-tool --help)
    endif()
endif()
