set(project_name "Engine")
project(${project_name})

file(GLOB_RECURSE sources "src/*.cpp")
file(GLOB_RECURSE headers "include/*.h")

include_directories(include)

add_library(${project_name} STATIC ${sources} ${headers})

target_link_libraries(${project_name} PRIVATE ${CONAN_LIBS})
