cmake_minimum_required(VERSION 3.20)

project(c_memory_pool_study LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(c_memory_pool STATIC
    src/memory_pool.c
)

target_include_directories(c_memory_pool
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(c_memory_pool_tests
    tests/memory_pool_tests.c
)

target_link_libraries(c_memory_pool_tests PRIVATE c_memory_pool)

if(MSVC)
    target_compile_options(c_memory_pool PRIVATE /W4 /WX /utf-8)
    target_compile_options(c_memory_pool_tests PRIVATE /W4 /WX /utf-8)
else()
    target_compile_options(c_memory_pool PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(c_memory_pool_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

enable_testing()
add_test(NAME c_memory_pool_tests COMMAND c_memory_pool_tests)
