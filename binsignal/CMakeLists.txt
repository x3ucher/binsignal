cmake_minimum_required(VERSION 3.16)

project(binsignal)

set(CMAKE_CXX_STANDARD 20)

# Добавление файлов BinarySignal.h и BinarySignal.cpp
add_library(binsignal STATIC source/SignalState.cpp source/BinarySignal.cpp)

# Экспортируем include-директории этой библиотеки
# target_include_directories(binsignal PUBLIC include)
# Экспортируем include-директории этой библиотеки
target_include_directories(binsignal PUBLIC include ../utils)
