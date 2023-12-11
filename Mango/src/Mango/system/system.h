#ifndef SYSTEM_H
#define SYSTEM_H
#include <stddef.h>
#include <stdint.h>

#include "real.h"

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

#ifdef RVC
// library functions
Real clock();
int32_t abs(int32_t);
void *malloc(size_t size);
int32_t rand();
void *memset(void *dest, uint32_t n, uint32_t size);

#ifdef RVC11
// syscalls
uint32_t printf(const char *, ...);
uint32_t memcpy(void *src, void *dest, uint32_t size);
uint32_t set_mode(uint32_t mode);
uint32_t get_controller();
uint32_t get_pixel_bg_data(uint32_t index);
uint32_t set_pixel_bg_controls(uint32_t index, uint32_t controls);
uint32_t get_bg_palette(uint32_t index);
uint32_t set_video_callback(void (*callback)(void *), void *arg);
uint32_t set_timer_callback(void (*callback)());
#elif defined RVC7
inline uint32_t printf(const char *, ...) {}
void *memcpy(void *dest, void *src, uint32_t size);
typedef enum BACKGROUND_TYPE { PIXEL, TILE } BACKGROUND_TYPE;

typedef struct background_t {
    BACKGROUND_TYPE type;
    int16_t x, y, z;
    uint8_t palette, data_index;
} background_t;

// SYSCALL WRAPPER FUNCTIONS:

/** @brief load background palette
 *
 * @param palette The palette data buffer of 256 values to load into the
 * palette
 *
 * @param index the index to load the palette into
 *
 * @return 0 upon success, -1 upon failure.
 */
int8_t load_background_palette(uint32_t *palette, uint8_t index);

/** @brief Load background data pixel - loads the background data buffer in
 * pixel mode
 *
 * @param data the data buffer of pixel values to load in (assumes a full
 * buffer)
 *
 * @param index the index to load the data into
 *
 * @return 0 upon success, -1 upon failure.
 */
int8_t load_background_data_pixel(uint8_t *data, uint8_t index);

/** @note THIS IS AN UNIMPLEMENTED FUNCTION AND SHOULD NOT BE USED
 *
 * @brief load background data tile - loads the background data in tile mode
 *
 * @param data the data buffer of pixel values to load in (assumes a full
 * buffer)
 *
 * @param index the index to load the data into
 *
 * @return 0 upon success, -1 upon failure.
 */
int8_t load_background_data_tile(uint32_t *data, uint8_t index);

/** @brief load background - loads the background control structure from the
 * background_t type
 *
 * @param background the background data type to create the control
 * structure from
 *
 *
 * @return returns the background ID upon success, -1 upon failure.
 */
uint8_t load_background(background_t background);
/** @brief delete background - deletes the background associated with the
 * given ID
 *
 * @param background_id the background ID to delete.
 *
 * @return 0 upon success, -1 upon failure.
 */
uint8_t delete_background(uint8_t background_id);

// Button enum type
typedef enum Button { w = 0, a, x, d, u, i, j, k, NONE } Button;
/** @brief get controller - Gets the current value of the controller
 *
 * @return A Button type representing the button being pressed (or none)
 */
Button get_controller();

/** @brief get ticks - gets the number of timer interrupts (ticks)
 *
 * @return a uint32_t value representing the number of ticks
 */
uint32_t GetTicks(void);

/** @brief get cmd - gets whether the cmd button has been pressed
 *
 * @return 0 if the cmd button has not been pressed 1 if it has
 */
uint32_t GetCmd(void);

/** @brief clear cmd - clears the cmd variable to allow for another cmd
 * button press
 *
 * @return 0, is essentially a void
 */
uint32_t clear_cmd(void);
#endif
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#endif

#endif
