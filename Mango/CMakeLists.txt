# Recursivley get all files in
file(GLOB_RECURSE SOURCES
    src/*/*.c
    src/*/*.h
)

add_library(Mango ${SOURCES})
target_include_directories(Mango PUBLIC src)
target_compile_options(Mango PRIVATE -O3)
target_link_libraries(Mango PUBLIC m)
if (${RISCV_CONSOLE})
    add_definitions(-DRISCV_CONSOLE)
else()
    # SDL
    # https://github.com/libsdl-org/SDL
    set(SDL_VIDEO_DRIVER_X11_SUPPORTS_GENERIC_EVENTS 1)
    add_compile_definitions(SDL_MAIN_HANDLED)
    set( SDL_LIBC ON CACHE BOOL "" FORCE )
    set( SDL_TEST OFF CACHE BOOL "" FORCE )
    add_subdirectory(SDL)
    target_link_libraries(Mango PRIVATE SDL2::SDL2main SDL2::SDL2-static)
endif()

