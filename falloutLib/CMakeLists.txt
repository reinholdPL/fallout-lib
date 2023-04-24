project(falloutLib)
SET(CMAKE_CXX_FLAGS "-O0")
SET(CMAKE_C_FLAGS "-O0")

find_package(ZLIB REQUIRED)


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
    cDAT.cpp
    )

    

    target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})
