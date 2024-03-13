cmake_minimum_required(VERSION 3.17)

project(carbon_test)

set(CMAKE_CXX_STANDARD 23)

if (NOT TARGET renderer)
    add_subdirectory(../ carbon)
endif()

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

file(GLOB_RECURSE SOURCES src/*.*)
add_executable(${PROJECT_NAME} ${SOURCES})

include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE carbon renderer dwmapi nlohmann_json::nlohmann_json)