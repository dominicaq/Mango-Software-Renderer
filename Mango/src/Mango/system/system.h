#ifndef SYSTEM_H
#define SYSTEM_H
#include <stddef.h>
#include <stdint.h>

#include "real.h"

#ifdef RISCV_CONSOLE
Real clock();
uint32_t printf(const char *, ...);
uint32_t memcpy(void *src, void *dest, uint32_t size);
uint32_t set_mode(uint32_t mode);
uint32_t get_controller();
uint32_t get_pixel_bg_data(uint32_t index);
uint32_t set_pixel_bg_controls(uint32_t index, uint32_t controls);
uint32_t get_bg_palette(uint32_t index);
void *malloc(size_t size);
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#endif

#endif
