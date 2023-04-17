project(falloutLib)
SET(CMAKE_CXX_FLAGS "-O0")
SET(CMAKE_C_FLAGS "-O0")
cmake_minimum_required(VERSION 3.22)
add_executable(falloutlib src/main.cpp src/utils.cpp src/falloutLib.cpp src/cPalette.cpp src/cFrm.cpp src/cError.cpp src/cFrmFrame.cpp)

include_directories(include)