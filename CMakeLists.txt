cmake_minimum_required(VERSION 3.12)
project(KISS LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(kiss src/kiss.c)
target_include_directories(kiss PUBLIC include/)

add_subdirectory(example)
enable_testing()
add_subdirectory(tests)

