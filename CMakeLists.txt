project(rtype)
cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)
include_directories(includes)
add_executable(rtype
    src/main.cpp)
add_subdirectory(lib/renderer)
add_dependencies(rtype renderer)