set(CLIENT client)
project(${CLIENT})

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
target_link_libraries(${CLIENT} PRIVATE ${CONAN_LIBS})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
set_target_properties(${CLIENT} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../bin
)
