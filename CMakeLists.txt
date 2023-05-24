cmake_minimum_required(VERSION 3.23)
project(SDLSDL C)

set(CMAKE_C_STANDARD 99)

set(SDL2_PATH "C:\\Users\\sidi ahmed\\Downloads\\SDL2-2.26.1")
set(SDL2_TTF_PATH "C:\\Users\\sidi ahmed\\Downloads\\SDL2_ttf-devel-2.20.1-VC\\SDL2_ttf-2.20.1")
set(SDL2_IMAGE_PATH "C:\\Users\\sidi ahmed\\Downloads\\SDL2_image-devel-2.6.2-VC\\SDL2_image-2.6.2")

find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)
find_package(SDL2_TTF REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIR})
add_executable(SDLSDL main.c)

target_link_libraries(SDLSDL ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} )
