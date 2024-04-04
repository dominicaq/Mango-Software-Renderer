cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_STANDARD 11)
set(C_STANDARD_REQUIRED ON)

project(mango_renderer)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC")
set( BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )

add_subdirectory(Mango)
add_executable(mango_renderer example/main.c example/models/spider.c example/models/spider.h)
if (UNIX)
    target_compile_definitions(mango_renderer PRIVATE MAIN=main)
else()
    target_compile_definitions(mango_renderer PRIVATE MAIN=SDL_main)
endif()

target_link_libraries(mango_renderer PRIVATE Mango)
