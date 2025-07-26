cmake_minimum_required(VERSION 3.10)
project(OG)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(assimp REQUIRED)
add_subdirectory(external/glfw)
add_library(glad external/glad/src/glad.c)

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)
# ImGUI
set(IMGUI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
    external/imgui
    external/imgui/backends
)

include_directories(
    external/glad/include
    external/glfw/include
    external/glm
    external/imgui
    external/imgui/backends
    external
    engine
)

file(GLOB_RECURSE SRC_FILES engine/*.cpp)

add_executable(game ${SRC_FILES})
# copy shaders
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders"
        "${CMAKE_BINARY_DIR}/shaders")
target_link_libraries(game glfw glad assimp::assimp imgui ${CMAKE_DL_LIBS})

target_link_options(game PRIVATE "-Wl,-export-dynamic")
