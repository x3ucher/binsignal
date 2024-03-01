# 
cmake_minimum_required(VERSION 3.16)

#
project(ooplab2)

#
set(CMAKE_CXX_STANDARD 20)

#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra")

add_compile_options(--coverage)
add_link_options(--coverage)

#add_compile_options(-fprofile-arcs -ftest-coverage)
#link_libraries(gcov)

# добавление подпроекта с библиотекой
add_subdirectory(binsignal)
add_subdirectory(utils)
#add_subdirectory(dialogue)
add_subdirectory(tests)
