project(falloutLib)
cmake_minimum_required(VERSION 3.22)
add_executable(falloutlib src/main.cpp src/utils.cpp src/falloutLib.cpp src/cPalette.cpp src/cFrm.cpp src/cError.cpp)

include_directories(include)