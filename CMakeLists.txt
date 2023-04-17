project(falloutLib)
SET(CMAKE_CXX_FLAGS "-O0")
SET(CMAKE_C_FLAGS "-O0")
cmake_minimum_required(VERSION 3.22)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_library(${PROJECT_NAME} src/utils.cpp src/falloutLib.cpp src/cPalette.cpp src/cFrm.cpp src/cError.cpp src/cFrmFrame.cpp)

include_directories(include)

add_executable(frm_example examples/frm.cpp)
target_link_libraries(frm_example ${SDL2_LIBRARIES} ${PROJECT_NAME})