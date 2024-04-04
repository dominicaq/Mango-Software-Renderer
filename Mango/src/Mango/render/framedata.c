#include "framedata.h"

float *frame_init_zbuffer(int width, int height) {
    int pixel_count = width * height;
    float *z_buffer = (float *)malloc(sizeof(float) * pixel_count);
    if (z_buffer == NULL) {
        printf("frame_init_zbuffer z_buffer malloc failed\n");
        return NULL;
    }
    printf("allocated z_buffer\n");

    // Init z_buffer to be "far away"
    for (int i = 0; i < pixel_count; ++i) {
        z_buffer[i] = 1.0f;
    }
    return z_buffer;
}

void frame_reset(Frame *frame) {
    display_reset(frame->display);
    // Reset zbuffer to be "far away"
    int pixel_count = frame->width * frame->height;
    for (int i = 0; i < pixel_count; ++i) {
        frame->z_buffer[i] = 1.0f;
    }
}

void frame_update(Frame *frame) { display_update(frame->display); }

void frame_set_pixel(Frame *frame, int x, int y, Vec4 color) {
    // Convert color values that should be between 0-1 to 0-255
    color.x = color.x * 255.0f;
    color.y = color.y * 255.0f;
    color.z = color.z * 255.0f;
    color.w = color.w * 255.0f;
    display_set_pixel(frame->display, x, y, color);
}

Frame *frame_alloc(const char *title, int width, int height) {
    Frame *frame = (Frame *)malloc(sizeof(Frame));
    if (frame == NULL) {
        printf("frame_alloc frame malloc failed\n");
        return NULL;
    }
    printf("allocated frame\n");

    frame->display = display_alloc(title, width, height);
    if (frame->display == NULL) {
        printf("frame_alloc display malloc failed\n");
        return NULL;
    }
    printf("allocated display\n");

    frame->z_buffer = frame_init_zbuffer(width, height);
    if (frame->z_buffer == NULL) {
        printf("frame_alloc z_buffer malloc failed\n");
        return NULL;
    }

    frame->width = width;
    frame->height = height;
    return frame;
}

void frame_free(Frame *frame) {
    display_stop(frame->display);
    // free(frame->z_buffer);
    // free(frame);
}
