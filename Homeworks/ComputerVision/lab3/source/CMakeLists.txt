cmake_minimum_required(VERSION 2.8)
project(lab3)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lab3 lab3.cpp filter.h filter.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

file(COPY data DESTINATION ${CMAKE_BINARY_DIR})

