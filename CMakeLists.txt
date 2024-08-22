cmake_minimum_required(VERSION 3.15)

project(Triangle)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# 寻找 Vulkan 和 GLFW 库
if(WIN32)
    set(Vulkan_INCLUDE_DIRS "$ENV{VULKAN_SDK}\\Include")
    set(Vulkan_LIBRARY "$ENV{VULKAN_SDK}\\Lib32")
    set(GLFW_INCLUDE_DIR "$ENV{GLFW_PATH}\\include")
else()
    find_package(glfw3 REQUIRED)
    find_package(Vulkan REQUIRED FATAL_ERROR)
endif()

# 根据选项定义 GPU
option(GPU "define your GPU" MOORETHREADS)
message("GPU is ${GPU}")
if(GPU STREQUAL "MooreThread")
    add_definitions(-DMOORETHREADS)
endif()

# 添加源文件
file(GLOB_RECURSE SOURCES "modules/*.cpp")

# 设置可执行文件
add_executable(Triangle main.cpp ${SOURCES})

# 定义宏和资源路径
set(SHADER_PATH "${CMAKE_SOURCE_DIR}/shaders/")
add_definitions(-DSHADER_PATH="${SHADER_PATH}")

set(ASSET_PATH "${CMAKE_SOURCE_DIR}/assets/")
add_definitions(-DASSET_PATH="${ASSET_PATH}")

# 包含自定义目录和项目内部目录
target_include_directories(Triangle PRIVATE
    ./glm/
    ./stb/
    ./tinyobjloader/
    ./include/
    ${Vulkan_INCLUDE_DIRS}  # Vulkan 包含目录
    ${GLFW_INCLUDE_DIR}    # GLFW 包含目录
)

# 特定于 Windows 的设置
if(WIN32)
    # 确保链接到 Vulkan 和 GLFW 的动态库
    target_link_libraries(Triangle
        PRIVATE
        ${Vulkan_LIBRARY}/vulkan-1.lib
        $ENV{GLFW_PATH}/lib-vc2019/glfw3dll.lib
    )
else()
    # 链接 Vulkan 和 GLFW 库
    target_link_libraries(Triangle
        PRIVATE
        Vulkan::Vulkan  # Vulkan 库
        glfw            # GLFW 库，CMake 3.11+ 支持这样的链接方式
    )
endif()
message(${Vulkan_LIBRARY})