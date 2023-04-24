project(falloutLib)
SET(CMAKE_CXX_FLAGS "-O0")
SET(CMAKE_C_FLAGS "-O0")
cmake_minimum_required(VERSION 3.12)

include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(falloutLib)
add_subdirectory(examples)