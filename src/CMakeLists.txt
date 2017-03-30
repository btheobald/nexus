include_directories(../inc)

set(SOURCE_FILES main.cpp ../inc/Body.h Body.cpp ../inc/Vec2f.h Vec2f.cpp ../inc/Node.h Node.cpp ../inc/Bound.h Bound.cpp)

add_library(nexuscore ${SOURCE_FILES} ${HEADER_FILES})
add_executable(nexus main.cpp)
target_link_libraries(nexus nexuscore)