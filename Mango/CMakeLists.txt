# SDL
# https://github.com/libsdl-org/SDL
set(SDL_VIDEO_DRIVER_X11_SUPPORTS_GENERIC_EVENTS 1)
set( SDL_LIBC ON CACHE BOOL "" FORCE )
set( SDL_TEST OFF CACHE BOOL "" FORCE )
add_subdirectory( SDL )

# Recursivley get all files in
file(GLOB_RECURSE SOURCES
    src/*/*.c
    src/*/*.h
)

add_library(Mango ${SOURCES})
target_link_libraries(Mango
  PUBLIC m 
  PUBLIC SDL2::SDL2main SDL2::SDL2-static )
target_include_directories(Mango PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
