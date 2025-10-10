cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(opengl_thing)

# TODO: install jinja2 in venv

add_subdirectory(deps/glfw)
add_subdirectory(deps/glad/cmake)

glad_add_library(glad_gl STATIC API gl:core=3.3)

# both for this and subdirectories in this repo
add_compile_options(-Wall -Wformat -Werror=format -Werror=mismatched-new-delete)

# core
add_subdirectory(core/logs)

# app
add_subdirectory(app/shaders)

add_executable(opengl_thing main.cpp)

target_include_directories(opengl_thing PRIVATE deps/glfw/include)
target_link_libraries(opengl_thing PRIVATE glfw glad_gl logs shaders)

