#ifdef RISCV_CONSOLE
#include "system.h"

uint32_t get_mtime_low(){return (*((volatile uint32_t *)0x40000008));}

extern uint8_t _data[];
extern uint8_t _sdata[];
extern uint8_t _edata[];
extern uint8_t _data_source[];
extern uint8_t _bss[];
extern uint8_t _ebss[];
extern uint8_t _stack[];

void init(void) {
    // set bss to zero
    // for (uint8_t *i = _bss; i < _ebss; ++i) {
    //*i = 0;
    //}
    // copy data rom to data ram
    printf("%d, %d, %d", _data, _data_source);
    memcpy(_data, _data_source, _edata - _data);
}

Real clock() { return real_from_i32((*((volatile uint32_t *)0x40000008))); }

int32_t srand(int32_t x) {
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

    printf("ptr: %d", heap_ptr);
    printf("mem: %d, %d", size, heap_ptr + size);
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
