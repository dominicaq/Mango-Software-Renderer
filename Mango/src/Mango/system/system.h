#ifndef SYSTEM_H
#define SYSTEM_H
#include <stddef.h>
#include <stdint.h>

#include "real.h"

#ifdef RISCV_CONSOLE

/*
    Our OS is designed for single-player 3D games, so it doesn't provide
    functions for sprites. Our 3D game interacts with the display by writing
    directly to background data pixel buffer. Our game's renderer is supported
    different modes of rendering that can be toggled, like wireframe,
    rasterized, and lit. 3D rendering on the limited hardware of the simulator
    is really slow, especially in the rasterized and lit modes, so we needed as
    much CPU time allocated to the renderer as possible. With this goal, our OS
    doesn't have upcalls for input or interrupts for scheduling CPU threads.
*/

// "Real" is a type representing decimal numbers, either as floats or as fixed
// point decimals.
Real clock();

//Writes to the screen in text mode.
uint32_t printf(const char *, ...);

// Efficiently copies of size from src to dest
uint32_t memcpy(void *src, void *dest, uint32_t size);

// Sets the graphics/text mode of the display. Mode 0 is text Mode 1 is graphics
uint32_t set_mode(uint32_t mode);

// Returns the current status of the controller. Each button is encoded in the
// bits of the return value.
// https://github.com/UCDClassNitta/riscv-console/blob/main/docs/chipset.md#multi-button-controller-status-register
uint32_t get_controller();

// Returns pointer to pixel buffer of the background image with the associated
// index. Valid indexes: 1-4 The grame index contains 512x288 8-bit indexes into
// the corresponding color palette
uint32_t get_pixel_bg_data(uint32_t index);

// Sets the control register of the background image with the associated index.
// The control register is mapped to bits in the control input parameter.
// https://github.com/UCDClassNitta/riscv-console/blob/main/docs/video.md#background-control-structure
uint32_t set_pixel_bg_controls(uint32_t index, uint32_t controls);

// Returns pointer to color palette of the background image with the associated
// index. There are 4 palettes, 1 for each background image Each palette
// contains 256 32-bit values
uint32_t get_bg_palette(uint32_t index);

// The malloc() function allocates size bytes and returns a pointer to the
// allocated memory. The memory is not initialized. 
void *malloc(size_t size);

#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#endif

#endif
