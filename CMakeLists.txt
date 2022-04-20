cmake_minimum_required(VERSION 3.10)

project(space-invaders)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# git submodule add https://gitlab.com/aminosbh/sdl2-cmake-modules.git cmake/sdl2
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

file(GLOB SOURCES "src/*.c")
add_executable(space-invaders ${SOURCES})

target_link_libraries(space-invaders ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

# EOF
