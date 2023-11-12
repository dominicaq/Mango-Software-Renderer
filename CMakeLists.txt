cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_STANDARD 99)
set(C_STANDARD_REQUIRED ON)

project(c_software_renderer)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -O3")
set( BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )

# SDL
# https://github.com/libsdl-org/SDL
set( SDL_LIBC ON CACHE BOOL "" FORCE )
set( SDL_TEST OFF CACHE BOOL "" FORCE )
add_subdirectory( SDL )

# Recursivley get all files in src
file(GLOB_RECURSE SOURCES
    src/*/*.c
    src/*/*.h
)

add_executable(c_software_renderer src/main.c ${SOURCES})

# Link Math
target_link_libraries(${PROJECT_NAME} m)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2-static )
