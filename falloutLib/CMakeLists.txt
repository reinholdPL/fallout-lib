project(falloutLib)
SET(CMAKE_CXX_FLAGS "-O0")
SET(CMAKE_C_FLAGS "-O0")

include_directories(include)

add_library(falloutLib 
    utils.cpp 
    falloutLib.cpp 
    cPalette.cpp 
    cFrm.cpp 
    cError.cpp 
    cFrmFrame.cpp
    cMap.cpp
    cList.cpp
    )
