cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(shaders)

set(SRCS
    "VertexFragmentShader.cpp"
)

add_library(shaders ${SRCS})

target_include_directories(shaders PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(shaders PRIVATE logs glad_gl)
