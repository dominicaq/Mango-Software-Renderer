cmake_minimum_required(VERSION 3.0)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

set(C_STANDARD_REQUIRED ON)
set(CXX_STANDARD_REQUIRED ON)


project(c_software_renderer)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -O3")

# Recursivley get all files in src
file(GLOB_RECURSE SOURCES
    src/*/*.c
    src/*/*.h
)
option(BUILD_APP1 "Build app1" ON)

add_executable(c_software_renderer src/main.c ${SOURCES})
add_executable(model_loader src/loader.cc ${SOURCES})

# Link Math
target_link_libraries(${PROJECT_NAME} m)
