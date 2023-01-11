set(SERVER server)
project(${SERVER})
include_directories(includes)
add_executable(${SERVER}
    src/main.cpp)
add_executable(minimal_udp_server
    src/test/mnimal_server.cpp)
add_executable(minimal_udp_client
    src/test/minimal_client.cpp)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
