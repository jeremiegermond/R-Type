set(project_name client)
set(engine_path ${CMAKE_CURRENT_SOURCE_DIR}/../engine)

project(${project_name})

add_subdirectory(${engine_path} ${CMAKE_CURRENT_BINARY_DIR}/engine)

file(GLOB_RECURSE sources src/*.cpp)
file(GLOB_RECURSE headers include/*.h)

include_directories(include ${engine_path}/include)

add_executable(${project_name} ${sources} ${headers})

target_link_libraries(${project_name} PRIVATE Engine)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
