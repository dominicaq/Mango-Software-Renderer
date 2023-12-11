#ifndef SYSTEM_H
#define SYSTEM_H
#include <stddef.h>
#include <stdint.h>

#include "real.h"

#ifdef RISCV_CONSOLE

typedef enum {
    INPUT_BUTTON_4 = 128,
    INPUT_BUTTON_3 = 64,
    INPUT_BUTTON_2 = 32,
    INPUT_BUTTON_1 = 16,
    INPUT_DIRECTION_RIGHT = 8,
    INPUT_DIRECTION_DOWN = 4,
    INPUT_DIRECTION_UP = 2,
    INPUT_DIRECTION_LEFT = 1,
} Controls;

// library functions
Real clock();
int32_t abs(int32_t);
void *malloc(size_t size);
int32_t srand(int32_t seed);

#define BUILD_LG_SP(i, x, y) \
    SET_LG_CONT(i, ((128 + x) << 2) | ((64 + y) << 12) | (3 << 22))
#define BUILD_MD_SP(i, x, y) \
    SET_MD_CONT(i, ((128 + x) << 2) | ((32 + y) << 12) | (3 << 22))
#define BUILD_SM_SP(i, x, y) \
    SET_SM_CONT(i, ((128 + x) << 2) | ((16 + y) << 12) | (3 << 22))

// syscalls
uint32_t printf(const char *, ...);
uint32_t memcpy(void *src, void *dest, uint32_t size);
uint32_t set_mode(uint32_t mode);
uint32_t get_controller();
uint32_t get_pixel_bg_data(uint32_t index);
uint32_t set_pixel_bg_controls(uint32_t index, uint32_t controls);
uint32_t get_bg_palette(uint32_t index);

uint32_t get_lg_sp_palette(uint32_t index);
uint32_t get_md_sp_palette(uint32_t index);
uint32_t get_sm_sp_palette(uint32_t index);

uint32_t set_lg_sp_controls(uint32_t index, uint32_t controls);
uint32_t set_md_sp_controls(uint32_t index, uint32_t controls);
uint32_t set_sm_sp_controls(uint32_t index, uint32_t controls);

uint32_t get_lg_sp_data(uint32_t index);
uint32_t get_md_sp_data(uint32_t index);
uint32_t get_sm_sp_data(uint32_t index);
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#endif

#endif
