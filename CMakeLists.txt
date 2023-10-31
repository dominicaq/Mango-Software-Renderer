cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_STANDARD 99)
set(C_STANDARD_REQUIRED ON)

project(c_software_renderer)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -O3")

# Recursivley get all files in src
file(GLOB_RECURSE SOURCES
    src/*/*.c
    src/*/*.h
)

add_executable(c_software_renderer src/main.c ${SOURCES})

# Link Math
target_link_libraries(${PROJECT_NAME} m)
