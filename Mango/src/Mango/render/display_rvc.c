#include "display_rvc.h"
#define BG_SIZE 147456
#define PALETTE_DELTA 0x33

struct Display {
    uint8_t *background_data0;
    uint8_t *background_data1;
    uint32_t *palette;
    bool data0;
    uint32_t palette_i;
    uint32_t bg_i0;
    uint32_t bg_i1;
};

uint8_t *BG_DATAS = (void *)0x50000000;
uint32_t BG_DATA_SIZE = 0x24000;

#define GET_PIXEL_BG_DATA(input) (BG_DATAS + (BG_DATA_SIZE * input))
#define SET_PIXEL_BG_CTRL(index, control) (BG_CONTROLS[index] = (control))

volatile uint32_t *BG_CONTROLS = (volatile void *)0x500F5A00;

void display_reset(Display *display) {
    uint8_t *data =
        display->data0 ? display->background_data0 : display->background_data1;
    for (int i = 0; i < BG_W * BG_H; ++i) {
        data[i] = 0;
    }
}
void display_update(Display *display) {
    (*((volatile uint32_t *)0x500F6780)) = (0b11111);
    SET_PIXEL_BG_CTRL(0, (0 << 29) | ((display->data0 * 2) << 22) |
                             (287 << 12) | (511 << 2) | display->palette_i);
    display->data0 = !display->data0;
}
void display_set_pixel(Display *display, int32_t x, int32_t y, Vec4 color) {
    int32_t r = real_to_i32(color.x) / PALETTE_DELTA;
    int32_t g = real_to_i32(color.y) / PALETTE_DELTA;
    int32_t b = real_to_i32(color.z) / PALETTE_DELTA;
    int32_t palette_i = 36 * r + 6 * g + b;
    (display->data0 ? display->background_data0
                    : display->background_data1)[y * BG_W + x] = palette_i;
}

Display *display_alloc(const char *title, int32_t _0, int32_t _1) {
    Display *display = (Display *)malloc(sizeof(Display));
    if (display == NULL) {
        printf("display_alloc display malloc failed\n");
        return NULL;
    }
    printf("allocated display\n");
    display->palette_i = 0;

    uint8_t *BG_PALETTES = (void *)0x500F0000;
    uint32_t BG_PALETTE_SIZE = 0x400;
    // display->palette = (uint32_t *)get_bg_palette(display->palette_i);
    display->palette =
        (uint32_t *)(BG_PALETTES + (BG_PALETTE_SIZE * display->palette_i));

    for (int32_t i = 0; i < 6; ++i) {
        for (int32_t j = 0; j < 6; ++j) {
            for (int32_t k = 0; k < 6; ++k) {
                display->palette[36 * i + 6 * j + k] =
                    0xff000000 | ((i * PALETTE_DELTA) << 16) |
                    ((j * PALETTE_DELTA) << 8) | (k * PALETTE_DELTA);
            }
        }
    }

    display->bg_i0 = 0;
    display->bg_i1 = 1;
    display->background_data0 = (uint8_t *)GET_PIXEL_BG_DATA(display->bg_i0);
    printf("data %d", display->background_data0);
    display->background_data1 = (uint8_t *)GET_PIXEL_BG_DATA(display->bg_i1);
    printf("here");
    display->data0 = false;
    SET_PIXEL_BG_CTRL(0, (display->bg_i0 << 29) | (0 << 22) | (287 << 12) |
                             (511 << 2) | display->palette_i);
    SET_PIXEL_BG_CTRL(1, (display->bg_i1 << 29) | (1 << 22) | (287 << 12) |
                             (511 << 2) | display->palette_i);
    return display;
}
void display_stop(Display *display) {}

void move_lg_sp(int32_t x, int32_t y) { BUILD_LG_SP(0, x, y); }
void move_md_sp(int32_t x, int32_t y) { BUILD_MD_SP(0, x, y); }
void move_sm_sp(int32_t x, int32_t y) { BUILD_SM_SP(0, x, y); }
