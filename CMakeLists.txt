cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(opengl_thing)

find_package(OpenGL REQUIRED)

add_subdirectory(deps/glfw)

set(GLAD_SRCS deps/glad/src/gl.c)

add_executable(opengl_thing main.cpp ${GLAD_SRCS})

message("test: OPENGL_INCLUDE_DIRS=${OPENGL_INCLUDE_DIRS}")
message("test: OPENGL_LIBRARIES=${OPENGL_LIBRARIES}")

target_include_directories(opengl_thing PRIVATE deps/glfw/include)
target_include_directories(opengl_thing PRIVATE deps/glad/include)
target_include_directories(opengl_thing PRIVATE ${OPENGL_INCLUDE_DIRS})

target_link_libraries(opengl_thing ${OPENGL_LIBRARIES} glfw)
