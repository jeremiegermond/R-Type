set(SERVER server)
project(${SERVER})
include_directories(includes)
add_executable(${SERVER}
    src/main.cpp
    src/UpdServer.cpp)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
