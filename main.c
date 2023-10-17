#include <stdio.h>
#include <stdlib.h>

#include "framedata.h"
#include "drawing.h"
#include "obj_parser.h"

#include "gameobject/transform.h"
#include "gameobject/camera.h"

// const int SCREEN_WIDTH = 512;
// const int SCREEN_HEIGHT = 288;
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

// 16:9 aspect ratio
int main() {
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
    //Transform cube_transform;
    Model *cube_model = load_obj_mesh("models/diablo.obj");
    if (cube_model == NULL) {
        return -1;
    }

    // Allocate space for frame data
    Frame *frame = init_frame(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (frame == NULL) {
        return -1;
    }

    // Update loop
    int frame_count = 1;
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
