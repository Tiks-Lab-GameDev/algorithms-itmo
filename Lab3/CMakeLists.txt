cmake_minimum_required(VERSION 3.23)
project(Riders/Lab2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab3 Sort.h Sort.cpp)

add_subdirectory(module)
add_subdirectory(tests)