#ifndef DISPLAY_SDL_H
#define DISPLAY_SDL_H

#include <SDL.h>

#include "../math/vec4.h"
typedef struct {
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_PixelFormat *format;
} Display;

Display *display_init(const char *title, int width, int height);
void display_reset(Display *);
void display_update(Display *);
void display_set_pixel(Display *, int x, int y, Vec4 color);
void display_stop(Display *);
#endif
