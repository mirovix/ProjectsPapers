cmake_minimum_required(VERSION 2.8)
project(lab5)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lab5 lab5.cpp panormaic_utils.h)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
