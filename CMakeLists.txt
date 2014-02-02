cmake_minimum_required(VERSION 2.8)
project(kianagy++)

SET (CMAKE_CXX_COMPILER           "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS              "-Wall -Wno-deprecated-register -std=c++11 -g")

find_package(GLEW REQUIRED)

find_package(BISON)
find_package(FLEX)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(kianagy++ main.cpp Camera.cpp Cube.cpp Entity.cpp Axis.cpp)

target_link_libraries(kianagy++ ${SDL2_LIBRARIES} GLEW)

FILE(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})


BISON_TARGET(map-parser map_parser.yy ${CMAKE_CURRENT_BINARY_DIR}/map_parser.cpp)
FLEX_TARGET(map-scanner map_scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/map_scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(map-scanner map-parser)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

FILE(COPY ${CMAKE_SOURCE_DIR}/Map.hpp DESTINATION ${CMAKE_BINARY_DIR})
FILE(COPY ${CMAKE_SOURCE_DIR}/Map_Driver.hpp DESTINATION ${CMAKE_BINARY_DIR})

add_executable(map_parser ${BISON_map-parser_OUTPUTS} ${FLEX_map-scanner_OUTPUTS} Map_Driver.cpp map_main.cpp ${CMAKE_CURRENT_BINARY_DIR}/map_parser.cpp)

FILE(COPY ${CMAKE_SOURCE_DIR}/maps DESTINATION ${CMAKE_BINARY_DIR})

