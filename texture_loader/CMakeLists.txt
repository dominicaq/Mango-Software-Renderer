cmake_minimum_required(VERSION 3.0)

project(texture_loader)

set(CMAKE_C_STANDARD 11)
set(C_STANDARD_REQUIRED ON)

option(BUILD_TEXTURE_LOADER "Build Texture Loader" ON)

add_executable(texture_loader src/main.c)
