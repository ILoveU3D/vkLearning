cmake_minimum_required(VERSION 3.15)

project(Triangle)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
find_package(Vulkan REQUIRED FATAL_ERROR)
find_package(glfw3 REQUIRED)

include_directories(./glm/)
include_directories(./stb/)
include_directories(./tinyobjloader/)

include_directories(./include/)
file(GLOB_RECURSE SOURCES "modules/*.cpp")

set(SHADER_PATH "${CMAKE_SOURCE_DIR}/shaders/")
add_definitions(-DSHADER_PATH="${SHADER_PATH}")

set(ASSET_PATH "${CMAKE_SOURCE_DIR}/assets/")
add_definitions(-DASSET_PATH="${ASSET_PATH}")

add_executable(triangle main.cpp ${SOURCES})
target_link_libraries(triangle glfw Vulkan::Vulkan)