cmake_minimum_required(VERSION 3.17)

project(carbon)

set(CMAKE_CXX_STANDARD 20)

if(NOT TARGET glm::glm)
    find_package(glm CONFIG REQUIRED)
endif()

add_subdirectory(lib)

file(GLOB_RECURSE SOURCES src/*.*)
add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm yogacore renderer)