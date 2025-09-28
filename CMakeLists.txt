cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(opengl_thing)

# TODO: install jinja2 in venv

add_subdirectory(deps/glfw)
add_subdirectory(deps/glad/cmake)

# TODO: probably change this somehow
# glad_add_library(glad_gl REPRODUCIBLE MX API gl:core=3.3)
glad_add_library(glad_gl STATIC API gl:core=3.3)

add_executable(opengl_thing main.cpp)

target_include_directories(opengl_thing PRIVATE deps/glfw/include)
# target_include_directories(opengl_thing PRIVATE deps/glfw/deps)

target_link_libraries(opengl_thing glfw glad_gl)

target_compile_options(opengl_thing PRIVATE -Wall -Wformat -Werror=format -Werror=mismatched-new-delete)
