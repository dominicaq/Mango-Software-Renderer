#ifndef FRAME_DATA_H
#define FRAME_DATA_H

#include <float.h>

#include "../math/vec4.h"

typedef struct Display Display;

typedef struct {
    int width, height;
    float *zBuffer;
    Display *display;
} Frame;

float *frame_init_zbuffer(int width, int height);

void frame_reset(Frame *frame);

void frame_update(Frame *frame);

void frame_set_pixel(Frame *, int x, int y, Vec4 color);

Frame *frame_alloc(int width, int height);

void frame_free(Frame *frame);

#endif  // FRAME_DATA_H
