cmake_minimum_required(VERSION 3.17)
project(Pacman)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/usr/local/lib -lSDL2")

include_directories(include/)

add_executable(Pacman src/gui/main.cpp include/moveable.h)

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)

include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(Pacman ${SDL2_LIBRARIES})
