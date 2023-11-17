#include <stdbool.h>
#include <stdint.h>

uint32_t GetTicks(void);
uint32_t GetController(void);
uint32_t SetTimerCallback(void *, uint32_t);
uint32_t SetVideoCallback(void *);

volatile int global = 42;
volatile uint32_t controller_status = 0;

volatile uint8_t *MEDIUM_DATA = (volatile uint8_t *)(0x500D0000);
volatile uint32_t *MEDIUM_PALETTE = (volatile uint32_t *)(0x500F2000);
volatile uint32_t *MEDIUM_CONTROL = (volatile uint32_t *)(0x500F5F00);

volatile uint8_t *LARGE_DATA = (volatile uint8_t *)(0x50090000);
#define LARGE_PALETTE ((volatile uint32_t *)(0x500F1000))
#define LARGE_CONTROL ((volatile uint32_t *)(0x500F5B00))

volatile uint32_t *MODE_REGISTER = (volatile uint32_t *)(0x500F6780);

uint32_t MediumControl(uint8_t palette, int16_t x, int16_t y, uint8_t z,
                       uint8_t index);

void onVideo() {
    static uint16_t x_pos = 0;
    static uint16_t y_pos = 0;

    // x position of square automatically moves right per frame.
    if (x_pos > 512 - 64) {
        x_pos = 0;
    }
    x_pos++;

    // y position of square controlled by user.
    uint32_t controller_status = (*((volatile uint32_t *)0x40000018));

    *(uint32_t *)0x500F5B00 = MediumControl(0, x_pos, 0, 0, 0);
}

void onTimer() {
    static bool a = false;
    if (a)
        LARGE_PALETTE[1] = 0xFF00FF00;
    else
        LARGE_PALETTE[1] = 0xFF00FFFF;
    a = !a;
}

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xF4800);
int main() {
    int last_global = 42;

    // Fill out sprite data
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            *((uint8_t *)0x50090000 + (y * 64 + x)) = 1;
        }
    }

    LARGE_PALETTE[1] = 0xFFFF0000;  // A R G B
    *(uint32_t *)0x500F5B00 = MediumControl(0, 0, 0, 0, 0);
    *MODE_REGISTER = 1;
    SetVideoCallback(onVideo);
    SetTimerCallback(onTimer, 10000);

    while (1) {
        uint32_t global = GetTicks();
        if (global != last_global) {
            last_global = global;
        }
    }

    return 0;
}

uint32_t MediumControl(uint8_t palette, int16_t x, int16_t y, uint8_t z,
                       uint8_t index) {
    return (((uint32_t)index) << 24) | (((uint32_t)z) << 21) |
           (((uint32_t)y + 64) << 12) | (((uint32_t)x + 64) << 2) |
           (palette & 0x3);
}
