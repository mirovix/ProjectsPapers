cmake_minimum_required(VERSION 2.8)

project(lab4)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lab4 lab4.cpp hough_circle.cpp hough_line.cpp canny.cpp)

target_link_libraries(lab4 ${OpenCV_LIBS})

file(COPY data DESTINATION ${CMAKE_BINARY_DIR})

file(COPY include DESTINATION ${CMAKE_BINARY_DIR})