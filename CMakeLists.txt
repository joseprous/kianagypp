cmake_minimum_required(VERSION 2.8)
project(kianagy++)

SET (CMAKE_CXX_COMPILER           "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS              "-std=c++11 -g")

find_package(GLEW REQUIRED)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(kianagy++ main.cpp Camera.cpp Cube.cpp Entity.cpp)

target_link_libraries(kianagy++ ${SDL2_LIBRARIES} GLEW)

FILE(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
