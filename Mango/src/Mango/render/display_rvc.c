#include "display_rvc.h"
#define BG_SIZE 147456
#define PALETTE_DELTA 0x33

struct Display {
    uint8_t *bgd0;
    uint8_t *bgd1;
    uint32_t *palette;
    bool data0;
    uint32_t palette_i;
    uint32_t bgi0;
    uint32_t bgi1;
#ifdef RVC7
    background_t bg0;
    background_t bg1;
#endif
};

void display_reset(Display *display) {
    uint8_t *data = display->data0 ? display->bgd0 : display->bgd1;
    for (int i = 0; i < BG_W * BG_H; ++i) {
        data[i] = 0;
    }
}
void display_update(Display *display) {
#ifdef RVC11
    set_mode(0b11111);
    set_pixel_bg_controls(0, (0 << 29) | ((display->data0 * 2) << 22) |
                                 (287 << 12) | (511 << 2) | display->palette_i);
#elif defined RVC7
    display->bg0.z = display->data0 * 2;
    load_background(display->bg0);
#endif
    display->data0 = !display->data0;
}
void display_set_pixel(Display *display, int32_t x, int32_t y, Vec4 color) {
    int32_t r = real_to_i32(color.x) / PALETTE_DELTA;
    int32_t g = real_to_i32(color.y) / PALETTE_DELTA;
    int32_t b = real_to_i32(color.z) / PALETTE_DELTA;
    int32_t palette_i = 36 * r + 6 * g + b;
    (display->data0 ? display->bgd0 : display->bgd1)[y * BG_W + x] = palette_i;
}

Display *display_alloc(const char *title, int32_t _0, int32_t _1) {
    Display *display = (Display *)malloc(sizeof(Display));
    if (display == NULL) {
        printf("display_alloc display malloc failed\n");
        return NULL;
    }
    printf("allocated display\n");
    display->palette_i = 0;
#ifdef RVC11
    display->palette = (uint32_t *)get_bg_palette(display->palette_i);
#elif defined RVC7
    display->palette = (uint32_t *)malloc(sizeof(uint32_t) * 256);
#endif

    for (int32_t i = 0; i < 6; ++i) {
        for (int32_t j = 0; j < 6; ++j) {
            for (int32_t k = 0; k < 6; ++k) {
                display->palette[36 * i + 6 * j + k] =
                    0xff000000 | ((i * PALETTE_DELTA) << 16) |
                    ((j * PALETTE_DELTA) << 8) | (k * PALETTE_DELTA);
            }
        }
    }
#ifdef RVC7
    load_background_palette(display->palette, display->palette_i);
#endif

    display->bgi0 = 0;
    display->bgi1 = 1;
    display->data0 = false;
#ifdef RVC11
    display->background_data0 = (uint8_t *)get_pixel_bg_data(display->bgi0);
    display->background_data1 = (uint8_t *)get_pixel_bg_data(display->bgi1);
    set_pixel_bg_controls(0, (display->bg_i0 << 29) | (0 << 22) | (287 << 12) |
                                 (511 << 2) | display->palette_i);
    set_pixel_bg_controls(1, (display->bg_i1 << 29) | (1 << 22) | (287 << 12) |
                                 (511 << 2) | display->palette_i);
#elif defined RVC7
    display->bgd0 = (uint8_t *)malloc(sizeof(uint8_t) * 4 * BG_H * BG_W);
    display->bgd1 = (uint8_t *)malloc(sizeof(uint8_t) * 4 * BG_H * BG_W);
    display->bg0 =
        (background_t){PIXEL, 287, 511, 0, display->palette_i, display->bgi0};
    display->bg1 =
        (background_t){PIXEL, 287, 511, 1, display->palette_i, display->bgi1};
#endif
    return display;
}
void display_stop(Display *display) {}
