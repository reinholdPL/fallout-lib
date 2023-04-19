find_package(SDL2 REQUIRED)

add_executable(frm_example frm.cpp)
target_link_libraries(frm_example ${SDL2_LIBRARIES} ${PROJECT_NAME})

include_directories(${CMAKE_SOURCE_DIR}/falloutLib/include)

add_executable(map_test map_main.cpp)
target_link_libraries(map_test ${SDL2_LIBRARIES} ${PROJECT_NAME})