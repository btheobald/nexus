cmake_minimum_required(VERSION 3.7)
project(nexus)

set(HEADER_DIR inc)
set(SOURCE_DIR src)
set(BINARY_DIR bin)

set(CMAKE_CXX_STANDARD 14)

include_directories(${HEADER_DIR})

set(SOURCE_FILES src/main.cpp src/Body.cpp src/Vec2f.cpp inc/Body.h inc/Vec2f.h)

add_library(nexuscore ${SOURCE_FILES})
add_executable(nexus src/main.cpp)
target_link_libraries(nexus nexuscore)