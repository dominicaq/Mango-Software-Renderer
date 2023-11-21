#ifndef FRAME_DATA_H
#define FRAME_DATA_H

#include <float.h>

#include "../math/vec4.h"

#ifdef RISCV_CONSOLE
#include "display_rvc.h"
#else
#include "display_SDL.h"
#endif

typedef struct {
    int width, height;
    Real *z_buffer;
    Display display;
} Frame;

float *frame_init_zbuffer(int width, int height);

void frame_reset(Frame *frame);

void frame_update(Frame *frame);

void frame_set_pixel(Frame *, int x, int y, Vec4 color);

Frame *frame_alloc(const char *title, int width, int height);

void frame_free(Frame *frame);

#endif  // FRAME_DATA_H
