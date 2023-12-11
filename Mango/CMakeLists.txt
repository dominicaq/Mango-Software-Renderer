# Recursivley get all files in
file(GLOB_RECURSE ALL_SOURCES
    src/*/*.c
    src/*/*.h
)

if (RVC)
  set(EXCLUDE "(display_SDL.|obj_parser.)")
else () 
  set(EXCLUDE "display_rvc.")
endif()

list(FILTER ALL_SOURCES EXCLUDE REGEX "${EXCLUDE}(c|h)$")

add_library(Mango ${ALL_SOURCES})
target_include_directories(Mango PUBLIC src)
target_link_libraries(Mango PUBLIC m)
if (RVC)
  target_compile_definitions(Mango PUBLIC RVC)
  if (RVC11) 
    target_compile_definitions(Mango PUBLIC RVC11)
  elseif(RVC7)
    target_compile_definitions(Mango PUBLIC RVC7)
  elseif(RVC3)
    target_compile_definitions(Mango PUBLIC RVC3)
  endif()
else()
    # SDL
    # https://github.com/libsdl-org/SDL
    set(SDL_VIDEO_DRIVER_X11_SUPPORTS_GENERIC_EVENTS 1)
    target_compile_definitions(Mango PRIVATE SDL_MAIN_HANDLED)
    target_compile_options(Mango PRIVATE -fPIC)
    set( SDL_LIBC ON CACHE BOOL "" FORCE )
    set( SDL_TEST OFF CACHE BOOL "" FORCE )
    add_subdirectory(SDL)
    target_link_libraries(Mango PRIVATE SDL2::SDL2main SDL2::SDL2-static)
endif()

