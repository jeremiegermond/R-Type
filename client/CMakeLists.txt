project(client)
cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
include(raylib/Findraylib.cmake)
add_compile_options(-Wall -fpermissive)
include_directories(includes)
include_directories(src/ecs/core)

if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/build>)
endif()

add_executable(client
        src/main.cpp)

target_link_libraries(client raylib)