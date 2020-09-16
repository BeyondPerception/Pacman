cmake_minimum_required(VERSION 3.17)
project(Pacman)

set(CMAKE_CXX_STANDARD 20)

include_directories(include/)

add_executable(Pacman src/gui/main.cpp include/object/movable.h include/gui/colors.h src/gui/game_board.cpp include/gui/game_board.h src/object/movable.cpp include/object/paintable.h assets/pacman.png src/object/paintable.cpp)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(Pacman ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})