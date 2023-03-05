set(project_name r-type_client)
set(engine_path ${CMAKE_CURRENT_SOURCE_DIR}/../engine)

project(${project_name})

add_subdirectory(${engine_path} ${CMAKE_CURRENT_BINARY_DIR}/engine)

file(GLOB_RECURSE sources src/*.cpp)
file(GLOB_RECURSE headers include/*.h)

include_directories(include ${engine_path}/include)

add_executable(${project_name} ${sources} ${headers})

target_link_libraries(${project_name} PRIVATE Engine)

set_target_properties(${project_name} PROPERTIES
                        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin
                        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../bin
                        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../bin
)
