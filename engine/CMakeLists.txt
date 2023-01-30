set(project_name "Engine")
project(${project_name})

#add_subdirectory(${ENGINE_PATH} ${CMAKE_CURRENT_BINARY_DIR}/engine)
set(raylib_VERSION "4.2.0")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/raylib ${CMAKE_CURRENT_BINARY_DIR}/raylib)
set(nlohmann_json_VERSION "3.11.2")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/json ${CMAKE_CURRENT_BINARY_DIR}/json)

file(GLOB_RECURSE sources "src/*.cpp")
file(GLOB_RECURSE headers "include/*.h")

include_directories(include)

add_library(${project_name} STATIC ${sources} ${headers})

target_link_libraries(${project_name} PUBLIC raylib nlohmann_json::nlohmann_json)
