set(SERVER server)
project(${SERVER})

include_directories(includes)
add_executable(${SERVER}
    src/main.cpp
    src/UpdServer.cpp
    src/utils.cpp
    src/player.cpp)

target_link_libraries(${SERVER} PRIVATE ${CONAN_LIBS})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
set_target_properties(${SERVER} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../bin
)