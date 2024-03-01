cmake_minimum_required(VERSION 3.16)
project(dialogue)

set(CMAKE_CXX_STANDARD 20)

add_executable (lab2 main.cpp)

target_link_libraries(lab2 binsignal libutils)
