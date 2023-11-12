#ifndef FRAME_DATA_H
#define FRAME_DATA_H

#include <SDL.h>
#include <float.h>

#include "../math/vec4.h"

typedef struct {
    int width, height;
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_PixelFormat *format;
    float *zBuffer;
} Frame;

float *init_zbuffer(int width, int height);

void reset_zbuffer(Frame *frame);

void setPixel(Frame *, int x, int y, Vec4 color);

Frame *init_frame(int width, int height);

void free_frame(Frame *frame);
#endif  // FRAME_DATA_H
