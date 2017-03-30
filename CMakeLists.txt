cmake_minimum_required(VERSION 3.7)

project(nexus)

set(CMAKE_CXX_STANDARD 14)

include_directories(inc)

add_subdirectory(inc)
add_subdirectory(src)
add_subdirectory(test)