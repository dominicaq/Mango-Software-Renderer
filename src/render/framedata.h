#ifndef FRAME_DATA_H
#define FRAME_DATA_H

#include "tga.h"
#include <float.h>

typedef struct {
    int width, height;
    TGAImage *framebuffer;
    float *zBuffer;
} Frame;

float *init_zbuffer(int width, int height);

void reset_zbuffer(Frame *frame);

Frame *init_frame(int width, int height);

void free_frame(Frame *frame);
#endif // FRAME_DATA_H
