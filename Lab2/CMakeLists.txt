cmake_minimum_required(VERSION 3.23)
project(Riders/Lab2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab2 Array.h Array.cpp ../Lab3/Sort.h ../Lab3/module/main.cpp ../Lab3/tests/tests.cpp)

add_subdirectory(module)
add_subdirectory(tests)