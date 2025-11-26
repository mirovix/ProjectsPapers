cmake_minimum_required(VERSION 2.8)
project(lab2)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lab2 lab2.cpp lab2.h)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

file(COPY checkerboard_images DESTINATION ${CMAKE_BINARY_DIR})

file(COPY test_image.png DESTINATION ${CMAKE_BINARY_DIR})
