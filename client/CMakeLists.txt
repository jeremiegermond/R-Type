set(CLIENT client)
project(${CLIENT})
include_directories(includes)
add_executable(${CLIENT}
    src/main.cpp)
add_subdirectory(lib/renderer)
add_dependencies(${CLIENT} renderer)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
