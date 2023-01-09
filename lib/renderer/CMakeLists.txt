if (WIN32)
    # Make sure the output library has a .dll extension on Windows
    set_target_properties(renderer PROPERTIES PREFIX "engine_" SUFFIX ".dll")
    # Make sure the output library is placed in the correct location on Windows
    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_SOURCE_DIR}/lib/engine_renderer.dll)
    add_custom_command(TARGET renderer POST_BUILD COMMAND cp engine_renderer.dll ${PROJECT_SOURCE_DIR}/lib)
endif()

cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)
set(BUILD_SHARED_LIBS ON)
add_compile_options(-fno-gnu-unique)
add_library(renderer SHARED src/sfml.cpp)
target_include_directories(renderer PRIVATE
        ${PROJECT_SOURCE_DIR}/includes
        ${PROJECT_SOURCE_DIR}/lib/Display/sfml/includes)
target_link_libraries(renderer PRIVATE sfml-graphics sfml-audio sfml-system sfml-window)
add_compile_options(-Wall -Wextra)
set_target_properties(renderer PROPERTIES PREFIX "engine_")
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_SOURCE_DIR}/lib/engine_renderer.so)
add_custom_command(TARGET renderer POST_BUILD COMMAND cp engine_renderer.so ${PROJECT_SOURCE_DIR}/lib)