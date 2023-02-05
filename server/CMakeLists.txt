set(SERVER server)
project(${SERVER})

#dependencies
set(raylib_VERSION "4.2.0")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/raylib ${CMAKE_CURRENT_BINARY_DIR}/raylib)

include_directories(includes)
add_executable(${SERVER}
    src/main.cpp
    src/UpdServer.cpp
    src/utils.cpp)
target_link_libraries(${SERVER} PRIVATE raylib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
