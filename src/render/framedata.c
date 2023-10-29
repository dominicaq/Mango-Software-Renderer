#include "framedata.h"

#include <stdio.h>
#include <stdlib.h>

float *init_zbuffer(int width, int height) {
    int pixel_count = width * height;
    float *zbuffer = malloc(sizeof(float) * pixel_count);
    if (zbuffer == NULL) {
        return NULL;
    }

    // Init zbuffer to be "far away"
    for (int i = 0; i < pixel_count; ++i) {
        zbuffer[i] = 1.0f;
    }
    return zbuffer;
}

void reset_zbuffer(Frame *frame) {
    // Reset zbuffer to be "far away"
    int pixel_count = frame->width * frame->height;
    for (int i = 0; i < pixel_count; ++i) {
        frame->zBuffer[i] = 1.0f;
    }
}

Frame *init_frame(int width, int height) {
    Frame *frame = malloc(sizeof(Frame));
    if (frame == NULL) {
        printf("ERROR: Failed to malloc frame\n");
        return NULL;
    }

    frame->framebuffer = createTGAImage(width, height);
    if (frame->framebuffer == NULL) {
        printf("ERROR: Failed to malloc TGAImage\n");
        return NULL;
    }

    frame->zBuffer = init_zbuffer(width, height);
    if (frame->zBuffer == NULL) {
        printf("ERROR: Failed to malloc zBuffer\n");
        return NULL;
    }

    frame->width = width;
    frame->height = height;
    return frame;
}

void free_frame(Frame *frame) {
    free(frame->zBuffer);
    free(frame->framebuffer);
    free(frame);
}
