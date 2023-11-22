#include "display_SDL.h"

#include <SDL.h>

struct Display {
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_PixelFormat *format;
};

void display_reset(Display *display) {
    uint32_t rgb_map = SDL_MapRGB(display->surface->format, 0, 0, 0);
    SDL_FillRect(display->surface, NULL, rgb_map);
    SDL_LockSurface(display->surface);
}

void display_update(Display *display) {
    SDL_UnlockSurface(display->surface);
    SDL_UpdateWindowSurface(display->window);
}

void display_set_pixel(Display *display, int x, int y, Vec4 color) {
    // Calculate the address of the target pixel
    int pitch = y * display->surface->pitch;
    int bytes_per_pixel = x * display->surface->format->BytesPerPixel;
    Uint8 *pixel_base = (Uint8 *)display->surface->pixels;
    Uint32 *target_pixel = (Uint32 *)(pixel_base + pitch + bytes_per_pixel);

    // Set the pixel color using SDL_MapRGBA
    *target_pixel = SDL_MapRGBA(display->format, color.elem[0], color.elem[1],
                                color.elem[2], color.elem[3]);
}

Display *display_init(const char *title, int width, int height) {
    Display *display = malloc(sizeof(Display));
    if (display == NULL) {
        printf("ERROR: Failed to malloc display\n");
        return NULL;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    // Create window
    display->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED, width, height,
                                       SDL_WINDOW_SHOWN);
    if (display->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    Uint32 format = SDL_GetWindowPixelFormat(display->window);
    display->format = SDL_AllocFormat(format);
    if (display->format == NULL) {
        printf("Format could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    // Get window surface
    display->surface = SDL_GetWindowSurface(display->window);
    if (display->surface == NULL) {
        printf("Surface could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    return display;
}

void display_stop(Display *display) {
    SDL_DestroyWindow(display->window);
    SDL_Quit();
}
