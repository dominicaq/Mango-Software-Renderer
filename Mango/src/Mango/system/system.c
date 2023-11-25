#ifdef RISCV_CONSOLE
#include "system.h"

extern uint8_t _data[];
extern uint8_t _sdata[];
extern uint8_t _edata[];
extern uint8_t _data_source[];
extern uint8_t _bss[];
extern uint8_t _ebss[];
extern uint8_t _stack[];

uint32_t get_mtime_low();

void init(void) {
    // set bss to zero
    for (uint8_t *i = _bss; i < _ebss; ++i) {
        *i = 0;
    }
    // copy data rom to data ram
    for (uint32_t i = 0; i < (_edata - _data); ++i) {
        _data[i] = _data_source[i];
    }
}

Real clock() {
    static uint32_t *mtime_low;
    if (mtime_low == NULL) {
        mtime_low = (uint32_t *)get_mtime_low();
    }
    return real_from_i32(*mtime_low);
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
