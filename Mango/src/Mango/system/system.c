#ifdef RVC
#include "system.h"

uint32_t get_mtime_low();

extern uint8_t _data[];
extern uint8_t _sdata[];
extern uint8_t _edata[];
extern uint8_t _data_source[];
extern uint8_t _bss[];
extern uint8_t _ebss[];
extern uint8_t _stack[];

void *memset(void *dest, uint32_t n, uint32_t size) {
    for (int32_t i = size - 1; i > -1; --i) {
        ((uint8_t *)dest)[i] = n;
    }
    return dest;
}

#ifdef RVC7
void *memcpy(void *dest, void *src, uint32_t size) {
    // DMA1_SRC = (uint32_t)src;
    // DMA1_DEST = (uint32_t)dest;
    // DMA1_SIZE = size;
    // DMA1_STATUS = DTA;
    // while (DMA1_STATUS & DTA) {
    // }
    for (int32_t i = size - 1; i > -1; --i) {
        ((uint8_t *)dest)[i] = ((uint8_t *)src)[i];
    }
    return dest;
}

uint32_t load_background_palette_sys(uint32_t data_ptr, uint32_t index);
uint32_t load_background_data_pixel_sys(uint32_t data_ptr, uint32_t index);
uint32_t load_background_sys(uint32_t background_ptr);
uint32_t delete_background_sys(uint32_t background_id);
uint32_t get_controller_sys();
int8_t load_background_palette(uint32_t *palette, uint8_t index) {
    return load_background_palette_sys((uint32_t)palette, (uint32_t)index);
}

int8_t load_background_data_pixel(uint8_t *data, uint8_t index) {
    return load_background_data_pixel_sys((uint32_t)data, (uint32_t)index);
}

int8_t load_background_data_tile(uint32_t *data, uint8_t index);

uint8_t load_background(background_t background) {
    return load_background_sys((uint32_t)&background);
}

uint8_t delete_background(uint8_t background_id) {
    return delete_background_sys((uint32_t)background_id);
}

Button get_controller() { return (Button)get_controller_sys(); }

#endif

void init(void) {
    memset(_bss, 0, _ebss - _bss);
    memcpy(_data, _data_source, _edata - _data);
}

Real clock() {
#ifdef RVC11
    return real_from_i32(get_mtime_low());
#elif defined RVC7
    return real_from_i32(GetTicks());
#endif
}

int32_t rand() {
    static int32_t x = 1;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

int32_t abs(int32_t value) {
    uint32_t temp = value >> 31;  // make a mask of the sign bit
    value ^= temp;                // toggle the bits if value is negative
    value += temp & 1;
    return value;
}

void *malloc(size_t size) {
    static uint8_t *heap_ptr = NULL;
    uint8_t *base;

    if (heap_ptr == NULL) {
        heap_ptr = _ebss;
        printf("base %d, stack %d", _ebss, _stack);
    }

    printf("malloc: %d, %d, %d", heap_ptr, size, _stack);
    if ((heap_ptr + size) <= _stack) {
        base = heap_ptr;
        heap_ptr += size;
        return base;
    } else {
        // errno = ENOMEM;
        return NULL;
    }
}
#endif
