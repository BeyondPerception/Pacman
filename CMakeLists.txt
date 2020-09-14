cmake_minimum_required(VERSION 3.17)
project(Pacman)

set(CMAKE_CXX_STANDARD 20)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(Pacman main.cpp)

target_link_libraries(Pacman ${SDL2_LIBRARIES})
