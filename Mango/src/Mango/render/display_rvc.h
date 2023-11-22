#ifndef DISPLAY_RVC_H
#define DISPLAY_RVC_H

#include <stdint.h>

#include "../math/vec4.h"

#define BG_W 512
#define BG_H 288
#define BG_SIZE 147456

typedef struct Display Display;

void display_reset(Display *);
void display_update(Display *);
void display_set_pixel(Display *, int32_t x, int32_t y, Vec4 color);
Display *display_alloc(const char *title, int32_t, int32_t);
void display_stop(Display *);
#endif
