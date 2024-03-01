cmake_minimum_required(VERSION 3.16)
project(libutils)

set(CMAKE_CXX_STANDARD 20)

add_library(libutils INTERFACE)

target_include_directories(libutils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
