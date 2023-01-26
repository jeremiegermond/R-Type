set(CLIENT client)
project(${CLIENT})

# Dependencies
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)
set(RAYLIB_VERSION 4.2.0)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

# Default client
include_directories(includes)
add_executable(${CLIENT}
    src/main.cpp
    src/box.cpp
    src/slider.cpp
    src/game-object.cpp
    src/game.cpp
    src/engine.cpp
    src/bullet.cpp src/particle2d.cpp)
# Link library
target_link_libraries(${CLIENT} PRIVATE raylib nlohmann_json::nlohmann_json)
#add_dependencies(${CLIENT} renderer)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
