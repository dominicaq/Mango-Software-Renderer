#include <stdio.h>
#include <stdlib.h>

#include "framedata.h"
#include "drawing.h"
#include "obj_parser.h"

// const int SCREEN_WIDTH = 512;
// const int SCREEN_HEIGHT = 288;
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

// 16:9 aspect ratio
int main() {
    // vec3 camera_pos = {0.0f, 0.0f, 0.0f};
    // Colors pallete
    float brightness = 0.9f;
    TGAColor white = createTGAColor(
        255 * brightness, // R
        255 * brightness, // G
        255 * brightness, // B
        255               // A
    );
    TGAColor black = createTGAColor(0, 0, 0, 255);

    // Load OBJ file(s)
    Model *cube_model = load_obj_mesh("models/cube.obj");
    if (cube_model == NULL) {
        return -1;
    }

    // Allocate space for frame data (don't malloc every frame)
    Frame *frame = init_frame(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (frame == NULL) {
        return -1;
    }

    int frame_count = 1; // Number of frames to render (temp code)
    for (int i = 0; i < frame_count; ++i) {
        // Reset frame
        setTGAImageBackground(frame->framebuffer, black);
        reset_zbuffer(frame);

        // Draw the scene
        draw_model(frame, cube_model, white);

        // Set (0,0) origin to top left
        flipImageVertically(frame->framebuffer);
        writeTGAImageToFile(frame->framebuffer, "output/output.tga");
    }

    // Free mallocs
    free_frame(frame);
    return 0;
}
