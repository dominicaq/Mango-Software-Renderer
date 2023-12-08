set(SOURCES
    src/main.c
)
if (RVC) {
  file(GLOB_RECURSE 
      src/*.c
      src/*.h
      src/*.s
  )


add_executable(game 
    src/main.c 
    src/crt0.s 
    src/interrupt.s 
    src/cube.c 
    src/cube.h)
