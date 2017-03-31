include_directories(../inc)

set(SOURCE_FILES main.cpp ../inc/Body.h Body.cpp ../inc/Vec2f.h Vec2f.cpp ../inc/Node.h Node.cpp ../inc/Bound.h Bound.cpp)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "-O3 -g0")

add_library(nexuscore ${SOURCE_FILES} ${HEADER_FILES})
add_executable(nexus main.cpp)
target_link_libraries(nexus nexuscore ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY})