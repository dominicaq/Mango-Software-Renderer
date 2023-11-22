cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)

set(CXX_STANDARD_REQUIRED ON)


project(loader)

option(BUILD_MODEL_LOADER "Build Model loader" OFF)

add_executable(model_loader main.cc ufbx/ufbx.c ufbx/ufbx.h)

