cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_STANDARD 11)
set(C_STANDARD_REQUIRED ON)

project(c_software_renderer)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -fPIC")
set( BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )


add_subdirectory(Mango)
add_executable(c_software_renderer native/main.c native/models/spider.c native/models/spider.h)
target_link_libraries(c_software_renderer PRIVATE Mango)

# Link Math
