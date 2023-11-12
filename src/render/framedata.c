#include "framedata.h"

#include <stdio.h>
#include <stdlib.h>

float *init_zbuffer(int width, int height) {
    int pixel_count = width * height;
    float *zbuffer = malloc(sizeof(float) * pixel_count);
    if (zbuffer == NULL) {
        return NULL;
    }

    // Init zbuffer to be "far away"
    for (int i = 0; i < pixel_count; ++i) {
        zbuffer[i] = 1.0f;
    }
    return zbuffer;
}

void reset_zbuffer(Frame *frame) {
    // Reset zbuffer to be "far away"
    int pixel_count = frame->width * frame->height;
    for (int i = 0; i < pixel_count; ++i) {
        frame->zBuffer[i] = 1.0f;
    }
}

void setPixel(Frame *frame, int x, int y, Vec4 color) {
    Uint32 *const target_pixel =
        (Uint32 *)((Uint8 *)frame->surface->pixels + y * frame->surface->pitch +
                   x * frame->surface->format->BytesPerPixel);
    *target_pixel = SDL_MapRGBA(frame->format, color.elem[0], color.elem[1],
                                color.elem[2], color.elem[3]);
}

Frame *init_frame(int width, int height) {
    Frame *frame = malloc(sizeof(Frame));

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }
    // Create window
    frame->window = SDL_CreateWindow(
        "c_software_renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height, SDL_WINDOW_SHOWN);
    if (frame->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    Uint32 format = SDL_GetWindowPixelFormat(frame->window);
    frame->format = SDL_AllocFormat(format);

    // Get window surface
    frame->surface = SDL_GetWindowSurface(frame->window);

    if (frame == NULL) {
        printf("ERROR: Failed to malloc frame\n");
        return NULL;
    }

    frame->zBuffer = init_zbuffer(width, height);
    if (frame->zBuffer == NULL) {
        printf("ERROR: Failed to malloc zBuffer\n");
        return NULL;
    }

    frame->width = width;
    frame->height = height;
    return frame;
}

void free_frame(Frame *frame) {
    free(frame->zBuffer);
    SDL_DestroyWindow(frame->window);
    SDL_Quit();
    free(frame);
}
