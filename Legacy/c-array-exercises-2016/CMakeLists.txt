cmake_minimum_required(VERSION 3.25)

project(c_array_exercises LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(c_array_exercises STATIC
    src/array_exercises.c
)
target_include_directories(c_array_exercises PUBLIC src)

add_executable(c_array_exercises_tests
    tests/array_exercises_tests.c
)
target_link_libraries(c_array_exercises_tests PRIVATE c_array_exercises)

if(MSVC)
    target_compile_options(c_array_exercises PRIVATE /W4 /WX)
    target_compile_options(c_array_exercises_tests PRIVATE /W4 /WX)
else()
    target_compile_options(c_array_exercises PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(c_array_exercises_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

enable_testing()
add_test(NAME c_array_exercises_tests COMMAND c_array_exercises_tests)
