cmake_minimum_required(VERSION 3.20)
project(rtype)
set(CMAKE_CXX_STANDARD 20)
add_subdirectory(server)
add_subdirectory(client)
file(COPY assets DESTINATION ${PROJECT_SOURCE_DIR}/bin)
# Make so that the assets are copied to the build directory every time we build the project
add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets
        ${PROJECT_SOURCE_DIR}/bin/assets
        COMMENT "Copying assets to build directory")
