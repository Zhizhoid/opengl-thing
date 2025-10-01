cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(logs)

set(SRCS
    "log.cpp"
)

add_library(logs ${SRCS})

target_include_directories(logs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
