cmake_minimum_required(VERSION 3.15)

project(Triangle)
set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED FATAL_ERROR)
find_package(glfw3 REQUIRED)

include_directories(./glm/)
include_directories(./include/)
file(GLOB SOURCES "src/*.cpp")

add_executable(triangle main.cpp ${SOURCES})
target_link_libraries(triangle glfw Vulkan::Vulkan)