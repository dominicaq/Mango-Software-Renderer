option(RISCV_CONSOLE "compile for riscv console" OFF)

# Recursivley get all files in
file(GLOB_RECURSE ALL_SOURCES
    src/*/*.c
    src/*/*.h
)

if (RISCV_CONSOLE)
    set(EXCLUDE "display_SDL.")
else () 
    set(EXCLUDE "display_rvc.")
endif()

list(FILTER ALL_SOURCES EXCLUDE REGEX "${EXCLUDE}(c|h)$")

add_library(Mango ${ALL_SOURCES})
target_include_directories(Mango PUBLIC src)
target_compile_options(Mango PRIVATE -O3)
target_link_libraries(Mango PUBLIC m)
if (RISCV_CONSOLE)
    target_compile_definitions(Mango PUBLIC RISCV_CONSOLE)
else()
    # SDL
    # https://github.com/libsdl-org/SDL
    set(SDL_VIDEO_DRIVER_X11_SUPPORTS_GENERIC_EVENTS 1)
    target_compile_definitions(Mango PRIVATE SDL_MAIN_HANDLED)
    set( SDL_LIBC ON CACHE BOOL "" FORCE )
    set( SDL_TEST OFF CACHE BOOL "" FORCE )
    add_subdirectory(SDL)
    target_link_libraries(Mango PRIVATE SDL2::SDL2main SDL2::SDL2-static)
endif()

