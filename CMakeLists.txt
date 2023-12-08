option(RVC "compile for riscv console" OFF)
if (NOT RVC)
  cmake_minimum_required(VERSION 3.13)
  set(CMAKE_C_STANDARD 11)
  set(C_STANDARD_REQUIRED ON)
  project(c_software_renderer LANGUAGES C)
  set(CMAKE_C_FLAGS "-Wall -Werror")
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )
endif()


add_executable(renderer 
    game/src/main.c 
    game/src/crt0.s 
    game/src/interrupt.s 
    game/src/cube.c 
    game/src/cube.h)

target_link_options(renderer PUBLIC "-Wl,-T${CMAKE_CURRENT_SOURCE_DIR}/game/riscv32-console.ld")
add_subdirectory(Mango)
if (UNIX)
    target_compile_definitions(renderer PRIVATE MAIN=main)
else()
    target_compile_definitions(renderer PRIVATE MAIN=SDL_main)
endif()

target_link_libraries(renderer PRIVATE Mango)
