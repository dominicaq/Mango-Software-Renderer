option(RVC "compile for riscv console" OFF)
option(RVC11 "compile for riscv group 11" OFF)
option(RVC7 "compile for riscv group 7" OFF)
if (NOT RVC)
  cmake_minimum_required(VERSION 3.13)
  set(CMAKE_C_STANDARD 11)
  set(C_STANDARD_REQUIRED ON)
  project(c_software_renderer LANGUAGES C)
  set(CMAKE_C_FLAGS "-Wall -Werror")
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )
endif()
file(GLOB SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/game/src/models/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/game/src/models/*.h"
    # Add more patterns if needed
)
if (RVC11)
  file(GLOB GROUP_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/game/src/group11/*.s"
  )
  list(APPEND SOURCES ${GROUP_SOURCES})
elseif(RVC7)
  file(GLOB GROUP_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/game/src/group7/*.s"
  )
  list(APPEND SOURCES ${GROUP_SOURCES})
endif()


add_executable(renderer 
    game/src/main.c 
    game/src/crt0.s
    ${SOURCES}
)

if (RVC)
  target_link_options(renderer PUBLIC "-Wl,-T${CMAKE_CURRENT_LIST_DIR}/game/riscv32-console.ld")
endif()

add_subdirectory(Mango)
if (UNIX)
  target_compile_definitions(renderer PUBLIC MAIN=main)
else()
  target_compile_definitions(renderer PUBLIC MAIN=SDL_main)
endif()


target_link_libraries(renderer PRIVATE Mango)
