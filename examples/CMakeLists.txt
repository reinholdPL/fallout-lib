find_package(SDL2 REQUIRED)
find_package(PNG REQUIRED)
find_package( OpenCV REQUIRED )

include_directories(${CMAKE_SOURCE_DIR}/falloutLib/include)
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(frm_example frm.cpp)
target_link_libraries(frm_example ${SDL2_LIBRARIES} ${PROJECT_NAME})

add_executable(map_test map_main.cpp)
target_link_libraries(map_test ${SDL2_LIBRARIES} ${PNG_LIBRARIES} ${OpenCV_LIBRARIES} ${PROJECT_NAME})

add_executable(frm2png frm2png.cpp)
target_link_libraries(frm2png ${PNG_LIBRARIES} ${PROJECT_NAME})

add_executable(dat_example dat_example.cpp)
target_link_libraries(dat_example ${PROJECT_NAME})