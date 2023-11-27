#ifndef SYSTEM_H
#define SYSTEM_H
#include <stddef.h>
#include <stdint.h>

#include "real.h"

#ifdef RISCV_CONSOLE

/*
    Project Proposals notes

    Group 1: The design of the security_check API is commendable. It
    demonstrates a clear understanding of security essentials in operating
    systems, especially in handling external data sources like cartridges. This
    function's ability to check for malicious data and return specific details
    about potential security issues in files is a vital feature for any
    operating system.

    Group 2: For the graphics mode operations, it seems the project heavily
    relies on manual control through the controller inputs. While this is great
    for interactive purposes, it might be beneficial to include automated
    testing features or scripts that can simulate these inputs. This would not
    only aid in debugging but also in demonstrating the functionality of your
    application in environments where a physical controller might not be
    available. Additionally, expanding the variety of graphics that can be
    manipulated beyond a pink square to include other shapes and colors would
    allow for more creative and diverse application development.

    Group 3: The detailed Graphics API is impressive, particularly the functions
    like drawPixel, createCanvas, and setCellType. These functions allow for a
    high degree of customization and flexibility in game design, essential for
    creating a visually engaging 2D maze game. The ability to manipulate
    individual pixels and canvas elements will enable developers to create
    intricate and dynamic game environments.

    Group 4: The introduction of a new Graphics API, including functions like
    set_background and init_sprites_palettes, is a great addition. This could be
    further enhanced by including 3D graphics support or more advanced rendering
    techniques, which would allow developers to create more visually engaging
    and dynamic applications.

    Group 5: The APIs for controller management, such as addController and
    deleteController, are well thought out. Expanding these to include support
    for newer input devices or more complex input schemes, like touch gestures
    or VR controllers, could further enhance the system's interactivity.

    Group 6: The focus on providing a single-threaded system initially, with
    plans to extend to a multithreaded system, demonstrates a pragmatic approach
    to system development. This allows for a solid foundation to be established
    before introducing the complexities of concurrency. Additionally, the
    emphasis on abstractions, such as memory manager and device manager, is
    excellent for ensuring system robustness and security.

    Group 7: For the Video Controller API, including advanced features such as
    support for 3D graphics, hardware acceleration, or shader programming could
    enhance the system's capabilities for more graphically demanding games.

    Group 8: The detailed approach to threading, including mechanisms for
    creation, termination, and process identification, shows a robust
    understanding of concurrent programming essentials. This is crucial for a
    gaming-focused operating system that may need to handle multiple processes
    simultaneously. Additionally, the focus on memory management through malloc,
    realloc, and free functions demonstrates a solid foundation for efficient
    resource utilization.

    Group 9: The focus on graphics, with APIs for loading and rendering sprites
    and backgrounds, is well-targeted towards game development. Enhancing these
    APIs to support more complex graphical operations, such as animations, 3D
    rendering, or shader programming, could significantly broaden the range of
    games that can be developed on this platform.

    Group 10: The introduction of time management APIs like timeStart and
    timeEnd is a positive step towards enabling developers to measure and manage
    execution times of processes or tasks. Enhancing these APIs to support more
    granular time measurements or integrating them with profiling tools could
    offer more insights for performance optimization.

    Our OS only writes to the background data as a pixel buffer to render the
    scene, and we don't rely on threads or anything, so the advanced features
    and APIs implemented by some of the operating systems aren't very useful for
    us. We'll just pick the operating system that seems the most stable and best
    documented when we're ready to port. Group 3's looks promising as of now,
    and will only require changing the calls to get_controller,
    get_pixel_bg_data, set_pixel_bg_controls, get_bg_palette.



*/

/*
    Summary

    Our OS is designed for single-player 3D games, so it doesn't provide
    functions for sprites. Our 3D game interacts with the display by writing
    directly to background data pixel buffer. Our game's renderer is supported
    different modes of rendering that can be toggled, like wireframe,
    rasterized, and lit. 3D rendering on the limited hardware of the simulator
    is really slow, especially in the rasterized and lit modes, so we needed as
    much CPU time allocated to the renderer as possible. With this goal, our OS
    doesn't have upcalls for input or interrupts for scheduling CPU threads.

    The U key enables rasterized rendering, but this is significantly slower
    than wireframe mode and the game might appear frozen or broken. It's not,
    it's just slow.

    The whole API was revised to minimize overhead of the OS, so every function
    is new.


    Shape Orienter - The 3D Game

    Game Description / Control Scheme:
    Our game is called "Shape Orienter". To play, orient the red shape to the
    green shapes orientation. Once you successfuly oririent the shape to the
    target shape a new shape orientation will appear. This will go on
    indefinitely. Scoring is not stored or displayed.

    Control Scheme:
    W - Forward rotation
    A - Left rotation
    D - Right rotation
    X - Backwards rotation

    ** WARNING, SUPER SLOW. WILL APPEAR TO BE FROZEN BUT ITS NOT FROZEN! **
    ** FILLING TRIANGLES CAN RANGE BETWEEN 1 MINUTE TO A VERY LONG TIME **
    U - Rasterize triangles (Fill in triangles) and show surface normals.
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
// index. Valid indexes: 1-4 The pixel buffer contains 512x288 8-bit indicies
// into the corresponding color palette.
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
