project(Neural-Image-Compression)
cmake_minimum_required(VERSION 2.4)
aux_source_directory(. SRC_LIST)
aux_source_directory(kernel SRC_LIST)
#aux_source_directory(lpng1619 SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

add_definitions(-DNOT_QT_AVAILABLE)
#SET(GCC_COVERAGE_COMPILE_FLAGS "-lpng -lz -lm")
target_link_libraries(Neural-Image-Compression png)