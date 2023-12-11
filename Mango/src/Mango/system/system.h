#ifndef SYSTEM_H
#define SYSTEM_H
#include <stddef.h>
#include <stdint.h>

#include "real.h"

#ifdef RVC

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
int32_t rand();
uint32_t printf(const char *, ...);
uint32_t memcpy(void *src, void *dest, uint32_t size);

#ifdef RVC11
// syscalls
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

uint32_t set_timer(void (*callback)(), uint32_t time);
uint32_t set_video(void (*callback)(void *), void *arg);
#elif defined RVC3
#include <stdatomic.h>
int drawPixelText(int x, int y, char c,
                  uint8_t color);  // for text mode. "color" is kinda buggy

int placeSmallSprite(int x_coord, int y_coord, int sprite_index,
                     int control_index);
int drawSmallSprite(int xdim, int ydim, int palette, int sprite);
int setPaletteSmall(int ind,
                    uint32_t p);     // changes the palette of SMALL_PALETTES
int deleteSmallSprite(int control);  // remove the small sprite from the screen
int moveSmallSprite(int control_ind, int newX, int newY);
int numSmallFreeSpaces();  // number of free spaces left in small sprite
                           // controls
int numSmallUsedSpaces();  // number of used spaces left in small sprite
                           // controls
int getSizeSmallSprite(
    int sprite_ind);  // returns 16 bits. vertical dimestion is bits 15-8 and
                      // horiz. domension is bits 7-0
int getSmallCorner1(int control);  // top left corner of the sprite
int getSmallCorner2(int control);  // bottom right corner of the sprite
uint32_t getSmallControl(int control_ind);
uint32_t getSmallPalette(int palette_ind);

int placeMediumSprite(int x_coord, int y_coord, int sprite_index,
                      int control_index);
int drawMediumSprite(int xdim, int ydim, int palette, int sprite);
int setPaletteMedium(int ind,
                     uint32_t p);     // changes the palette of SMALL_PALETTES
int deleteMediumSprite(int control);  // remove the small sprite from the screen
int moveMediumSprite(int control_ind, int newX, int newY);
int numMediumFreeSpaces();  // number of free spaces left in small sprite
                            // controls
int numMediumUsedSpaces();  // number of used spaces left in small sprite
                            // controls
int getSizeMediumSprite(
    int sprite_ind);  // returns 16 bits. vertical dimestion is bits 15-8 and
                      // horiz. domension is bits 7-0
int getMediumCorner1(int control);  // top left corner of the sprite
int getMediumCorner2(int control);  // bottom right corner of the sprite
uint32_t getMediumControl(int control_ind);
uint32_t getMediumPalette(int palette_ind);

int placeLargeSprite(int x, int y, int s_index, int c_index);
int drawLargeSprite(int xdim, int ydim, int palette_ind, int sprite_ind);
int setPaletteLarge(int ind,
                    uint32_t p);     // changes the palette of LARGE_PALETTES
int deleteLargeSprite(int control);  // removes the large sprite from the screen
int moveLargeSprite(int control_ind, int newX, int newY);
int numLargeFreeSpaces();  // number of free spaces left in large sprite
                           // controls
int numLargeUsedSpaces();  // number of used spaces left in large sprite
                           // controls
int getSizeLargeSprite(
    int sprite_ind);  // returns 16 bits. vertical dimestion is bits 15-8 and
                      // horiz. domension is bits 7-0
int getLargeCorner1(int control);  // top left corner of the sprite
int getLargeCorner2(int control);  // bottom right corner of the sprite
uint32_t getLargeControl(int control_ind);
uint32_t getLargePalette(int palette_ind);

// We don't totally know how to do background sprites
int drawBackgroundPixels(uint32_t start, uint32_t dims, int palette_ind,
                         int sprite_index);
int setBackgroundControlPixel(int control_ind, int x, int y, int PxInd);
int setBackgroundControlTile(int control_ind, int x, int y, int TiInd,
                             int SubInd);
int setBackgroundPalette(int palette_ind, uint32_t palette);
int deleteBackgroundSprite(int control_ind);
uint32_t getBackgroundControl(int control_ind);
uint32_t getBackgroundPalette(int palette_ind);

// For L or S: 0 means small, 1 means medium, >1 means large
int collided(int size1, int control_ind1, int size2, int control_ind2);

uint32_t mergeXY(int x, int y);  // bits 31 - 16 are y, bits 15 - 0 are x;

typedef void (*TThreadEntry)(void *);
typedef uint32_t *TThreadContext;

TThreadContext InitThread(uint32_t *stacktop, TThreadEntry entry, void *param);
void SwitchThread(TThreadContext *oldcontext, TThreadContext newcontext);

int spinlockInit(atomic_flag *flag);
int spinlockLock(atomic_flag *flag);
int spinlockUnlock(atomic_flag *flag);

// Example of placing a large sprite on the screen
// setPaletteLarge(3, 0xFFFFFF00); // we have yellow going into palette index 3
// drawLargeSprite(16, 16, 3, 3); // we make a 16x16 sprite with palette index 3
// (first 3) and sprite index 3 (second 3) placeLargeSprite(160, 160, 3, 100);
// // place sprite index 3 at coords 160, 160. It is also sprite control index
// 100
//  Sprite Index = the unique sprite stored in the data, like a sprite sheet
//  (idk how to word this :P) Control Index = instances of a sprite on the
//  screen. Can be multiple of a sprite from the same sprite index.

// Can change colors of a sprite in 2 ways:
// 1. Just so setPaletteLarge/Small on the palette with the sprite, which
// changes the color of all sprites with that palette index
// 2. Call drawLarge/SmallSprite with the same x dimension, y dimension, sprite
// index, but different palette index
//    Make sure that palette index is mapped to the color you want it to be, if
//    it is not, just so setPaletteLarge/Small

int clearScreen();  // the square you control may still show up
int clearBackground();

// can have at most 64 timers going at once, therefore timer_ind has to be
// anywhere from 0 to 63
int startTimer(int timer_ind);  // can be used to reset timer
int pauseTimer(int timer_ind);
int resumeTimer(int timer_ind);
int stopTimer(int timer_ind);  // stops the timer and returns the final time
int getTime(
    int timer_ind);  // gets the current time on this timer since startTimer()
                     // was called. Gets paused when timer is paused.
int isTimerOn(int timer_ind);  // 1 means on, 0 means off
int getPausedTime(
    int timer_ind);  // gets the amount of time the timer is currently paused at
int createTimer(int timer_ind);   // can be used to initalize a timer
int destroyTimer(int timer_ind);  // unused

uint32_t getTicks(void);
uint32_t getController(void);
uint32_t getMachineTime(void);  // gets microseconds
uint32_t getVideoTime(void);    // gets milliseconds
uint32_t toggleVideoInterrupt(int value);
uint32_t toggleCommandInterrupt(int value);

// can have 64 canvases, therefore canvas_id is from 0 to 63
// Note: cells are a constant of 4096 cells (having bad luck with dynamically
// allocated arrays) you can select the number of rows. The number os colums is
// 4096 / row_size. Recommended to have 4096 divisible by row_size or else your
// grid won't be rectangular. If you want to switch canvases, you can
// clearScreen, then for loop through a new canvas_id, then populate sprites
// with placeLarge/SmallSpriteOnGrid to make a new canvas, or to just switch to
// that canvas by taking grid[x][y] as the placeLarge/SmallSpriteOnGrid's
// sprite_index parameter, incrementing control_index in the process.
int initGrid(int canvas_id, int dimension_of_square_cells, int row_size);
int placeLargeSpriteOnGrid(int x, int y, int sprite_index, int control_index,
                           int canvas_id);
int placeMediumSpriteOnGrid(int x, int y, int sprite_index, int control_index,
                            int canvas_id);
int placeSmallSpriteOnGrid(int x, int y, int sprite_index, int control_index,
                           int canvas_id);
int updateCell(int x, int y, int new_val, int canvas_id);
int getCellFromCoords(int x, int y, int canvas_id);
int getCellType(int x, int y, int canvas_id);
int getCellTypeFromCoords(int x, int y, int canvas_id);
int getSquareDims(
    int canvas_id);  // cells are all squares of getSquareDims by getSquareDims
int getRowSize(int canvas_id);
int colorGridCell(int canvas_id, int x, int y, int palette_ind, int type);
int removeBGPixels(int x, int y, int xdim, int ydim, int spriteID);
int removeBGPixelsByGrid(int canvas_id, int x, int y, int spriteID);
// int colorGridCell(int canvas_id, uint32_t xy, int palette_ind, int
// sprite_ind, int cell_type);
int changeCanvas(
    int canvas_id);  // populates cells according to cell type. When populated,
                     // the cell type is used to read in the sprite index. Also
                     // it increments control index by 1 starting at 1. Sprite
                     // size depends on dimensions of the cells. 1 to 16 is
                     // SMALL, 17 to 32 is MEDIUM, larger than 32 is LARGE

int delay(int ms);  // in milliseconds

// Heap implementation partially implemented
int allocateMemory(int size);  // allocates into the heap
int fillMemory(int destination, int value,
               int size);  // fills the heap with values starting at
                           // destination, size times
// Note, value converts to char according to ASCII values
int copyMemory(int source, int destination, int size);
int deallocate(void *);

int drawChar(char c, int small_palette_ind,
             int small_sprite_ind);  // draws a character in graphics mode,
                                     // which uses the small sprites
int print(char *string,
          int start);  // prints a string starting at VIDEO_MEMORY[start]
int printNum(int number, int start_pos,
             int num_digits_printed);  // only works for non-negative integers
// number: the number being printed
// start_pos: the location of where to start printing.
// For example if start_pos = 4, then it starts printing at VIDEO_MEMORY[4]
// num_digits_printed is the number of digits printed starting from the least
// sig. digit. For example if num_digits_printed = 5 and number = 256, then it
// prints "00256"

int createUser(int ID, char *name);
int deleteUser(int ID);
int setHighScore(int ID, int hs);
int setName(int ID, char *name);
int getName(int ID);
int getHighScore(int ID);
int setCurrentScore(int ID, int score);
int getCurrentScore(int ID);

int switchMode(int mode);
int clearAllText();
int randomNumber(int lowerBound, int upperBound);
int setSeed(int seed);

#elif defined RVC7

typedef enum BACKGROUND_TYPE { PIXEL, TILE } BACKGROUND_TYPE;

typedef struct background_t {
    BACKGROUND_TYPE type;
    int16_t x, y, z;
    uint8_t palette, data_index;
} background_t;

uint32_t load_background_palette_sys(uint32_t data_ptr, uint32_t index);
uint32_t load_background_data_pixel_sys(uint32_t data_ptr, uint32_t index);
uint32_t load_background_sys(uint32_t background_ptr);
uint32_t delete_background_sys(uint32_t background_id);

// SYSCALL WRAPPER FUNCTIONS:

/** @brief load background palette
 *
 * @param palette The palette data buffer of 256 values to load into the palette
 *
 * @param index the index to load the palette into
 *
 * @return 0 upon success, -1 upon failure.
 */
static inline int8_t load_background_palette(uint32_t *palette, uint8_t index) {
    return load_background_palette_sys((uint32_t)palette, (uint32_t)index);
}

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
static inline int8_t load_background_data_pixel(uint8_t *data, uint8_t index) {
    return load_background_data_pixel_sys((uint32_t)data, (uint32_t)index);
}

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
 * @param background the background data type to create the control structure
 * from
 *
 *
 * @return returns the background ID upon success, -1 upon failure.
 */
static inline uint8_t load_background(background_t background) {
    return load_background_sys((uint32_t)&background);
}

/** @brief delete background - deletes the background associated with the given
 * ID
 *
 * @param background_id the background ID to delete.
 *
 * @return 0 upon success, -1 upon failure.
 */
static inline uint8_t delete_background(uint8_t background_id) {
    return delete_background_sys((uint32_t)background_id);
}

#endif
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#endif

#endif
