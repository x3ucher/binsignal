cmake_minimum_required(VERSION 3.16)
project(tests)

set(CMAKE_CXX_STANDARD 20)

find_package(Catch2 2 REQUIRED)

add_executable(tests testing.cpp)

target_link_libraries(tests binsignal Catch2::Catch2)
