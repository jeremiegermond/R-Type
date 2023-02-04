set(CLIENT client)
project(${CLIENT})

# Dependencies
set(raylib_VERSION "4.2.0")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/raylib ${CMAKE_CURRENT_BINARY_DIR}/raylib)
set(nlohmann_json_VERSION "3.11.2")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/json ${CMAKE_CURRENT_BINARY_DIR}/json)

# Default client
include_directories(includes)
include_directories(network)
add_executable(${CLIENT}
    src/main.cpp
    src/box.cpp
    src/slider.cpp
    src/game-object.cpp
    src/game.cpp
    src/engine.cpp
    src/bullet.cpp
    src/particle2d.cpp
    src/button.cpp
    src/Network/UdpClient.cpp
    src/utils.cpp)

# Link library
target_link_libraries(${CLIENT} PRIVATE raylib nlohmann_json::nlohmann_json)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
