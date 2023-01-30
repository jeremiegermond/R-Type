# Find nlohmann_json installed on the system or download and build it
# Check if the library is already installed
find_package(nlohmann_json ${nlohmann_json_VERSION} QUIET)
if (NOT nlohmann_json_FOUND)
    # Download the library if it's not installed
    include(FetchContent)
    FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v${nlohmann_json_VERSION}/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP true
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()
