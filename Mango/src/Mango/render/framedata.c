#include "framedata.h"

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

struct Display {
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_PixelFormat *format;
};

float *frame_init_zbuffer(int width, int height) {
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

void frame_reset(Frame *frame) {
    uint32_t rgb_map = SDL_MapRGB(frame->display->surface->format, 0, 0, 0);
    SDL_FillRect(frame->display->surface, NULL, rgb_map);
    SDL_LockSurface(frame->display->surface);
    // Reset zbuffer to be "far away"
    int pixel_count = frame->width * frame->height;
    for (int i = 0; i < pixel_count; ++i) {
        frame->zBuffer[i] = 1.0f;
    }
}

void frame_update(Frame *frame) {
    SDL_UnlockSurface(frame->display->surface);
    SDL_UpdateWindowSurface(frame->display->window);
}

void frame_set_pixel(Frame *frame, int x, int y, Vec4 color) {
    // Calculate the address of the target pixel
    int pitch = y * frame->display->surface->pitch;
    int bytes_per_pixel = x * frame->display->surface->format->BytesPerPixel;
    Uint8 *pixel_base = (Uint8 *)frame->display->surface->pixels;
    Uint32 *target_pixel = (Uint32 *)(pixel_base + pitch + bytes_per_pixel);

    // Set the pixel color using SDL_MapRGBA
    *target_pixel = SDL_MapRGBA(frame->display->format,
                                color.elem[0], color.elem[1],
                                color.elem[2], color.elem[3]);
}


Frame *frame_alloc(int width, int height) {
    Frame *frame = malloc(sizeof(Frame));
    if (frame == NULL) {
        printf("ERROR: Failed to malloc frame\n");
        return NULL;
    }

    frame->display = malloc(sizeof(Display));
    if (frame->display == NULL) {
        printf("ERROR: Failed to malloc frame\n");
        return NULL;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }
    // Create window
    frame->display->window = SDL_CreateWindow(
        "c_software_renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height, SDL_WINDOW_SHOWN);
    if (frame->display->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    Uint32 format = SDL_GetWindowPixelFormat(frame->display->window);
    frame->display->format = SDL_AllocFormat(format);
    if (frame->display->format == NULL) {
        printf("Format could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    // Get window surface
    frame->display->surface = SDL_GetWindowSurface(frame->display->window);

    if (frame == NULL) {
        printf("ERROR: Failed to malloc frame\n");
        return NULL;
    }

    frame->zBuffer = frame_init_zbuffer(width, height);
    if (frame->zBuffer == NULL) {
        printf("ERROR: Failed to malloc zBuffer\n");
        return NULL;
    }

    frame->width = width;
    frame->height = height;
    return frame;
}

void frame_free(Frame *frame) {
    free(frame->zBuffer);
    SDL_DestroyWindow(frame->display->window);
    SDL_Quit();
    free(frame);
}
