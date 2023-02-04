# Find raylib installed on the system or download and build it
# Check if the library is already installed
find_package(raylib ${raylib_VERSION} QUIET)
if (NOT raylib_FOUND)
    # Download the library if it's not installed
    message(STATUS "raylib ${raylib_version} not found: downloading and building it...")
    include(FetchContent)
    FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/${raylib_version}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()
