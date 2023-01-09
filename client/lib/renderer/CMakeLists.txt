if (WIN32)
    # Make sure the output library has a .dll extension on Windows
    set_target_properties(renderer PROPERTIES PREFIX "engine_" SUFFIX ".dll")
endif()
set(RENDERER renderer)
set(BUILD_SHARED_LIBS ON)
add_compile_options(-fno-gnu-unique)
add_library(${RENDERER} SHARED src/sfml.cpp)
target_include_directories(${RENDERER} PRIVATE
        ../../includes
        ../Display/sfml/includes)
target_link_libraries(${RENDERER} PRIVATE sfml-graphics sfml-audio sfml-system sfml-window)
add_compile_options(-Wall -Wextra)
set_target_properties(${RENDERER} PROPERTIES PREFIX "engine_")
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ../engine_${RENDERER}.*)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin/lib)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin/lib)
