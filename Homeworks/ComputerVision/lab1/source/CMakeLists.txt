cmake_minimum_required(VERSION 2.8)
project(lab1)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lab1 lab1.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

file(COPY robot.jpg DESTINATION ${CMAKE_BINARY_DIR})
