cmake_minimum_required(VERSION 3.20)
project(c_file_finder_core LANGUAGES C)

add_library(byte_frequency STATIC
    src/byte_frequency.c
    src/byte_frequency.h)
target_include_directories(byte_frequency PUBLIC src)
target_compile_features(byte_frequency PUBLIC c_std_17)

add_executable(byte_frequency_tests tests/byte_frequency_tests.c)
target_link_libraries(byte_frequency_tests PRIVATE byte_frequency)

if(MSVC)
    target_compile_options(byte_frequency PRIVATE /W4 /WX)
    target_compile_options(byte_frequency_tests PRIVATE /W4 /WX)
else()
    target_compile_options(byte_frequency PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(byte_frequency_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

enable_testing()
add_test(NAME byte_frequency_tests COMMAND byte_frequency_tests)
