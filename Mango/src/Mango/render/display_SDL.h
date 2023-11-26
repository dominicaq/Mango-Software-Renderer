#ifndef DISPLAY_SDL_H
#define DISPLAY_SDL_H

#include <stdlib.h>

#include "../math/vec4.h"
typedef struct Display Display;

Display *display_alloc(const char *title, int width, int height);
void display_reset(Display *);
void display_update(Display *);
void display_set_pixel(Display *, int x, int y, Vec4 color);
void display_stop(Display *);
#endif
