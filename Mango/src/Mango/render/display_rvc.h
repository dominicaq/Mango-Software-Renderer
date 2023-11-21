#ifndef DISPLAY_RVC_H
#define DISPLAY_RVC_H

#include <stdint.h>

#include "../math/Vec4.h"

#define BG_W 512
#define BG_H 288
#define BG_SIZE 147456

typedef struct {
    uint8_t *background_data0;
    uint8_t *background_data1;
    uint32_t *palette;
    bool data0;
    uint32_t palette_i;
    uint32_t bg_i0;
    uint32_t bg_i1;
} Display;

void display_reset(Display *);
void display_update(Display *);
void display_set_pixel(Display *, int32_t x, int32_t y, Vec4 color);
void display_init(Display *, const char *title, int32_t, int32_t);
void display_stop(Display *);
#endif
