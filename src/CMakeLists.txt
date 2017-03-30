set(SOURCE_FILES main.cpp Body.cpp Vec2f.cpp)

add_library(nexuscore ${SOURCE_FILES} ${HEADER_FILES})
add_executable(nexus main.cpp)
target_link_libraries(nexus nexuscore)