#include <stdio.h>
#include <stdlib.h>

#include "tga.h"
#include "drawing.h"
#include "obj_parser.h"

// const int SCREEN_WIDTH = 512;
// const int SCREEN_HEIGHT = 288;
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

// 16:9 aspect ratio
int main() {
    // vec3 camera_pos = {0.0f, 0.0f, 0.0f};
    // Colors
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

    int frame_count = 1; // Number of frames to render
    for (int frame = 0; frame < frame_count; ++frame) {
        // Init image
        TGAImage frame_buffer = createTGAImage(SCREEN_WIDTH, SCREEN_HEIGHT);
        setTGAImageBackground(&frame_buffer, black);

        // Init frame buffer
        float *zbuffer = init_zbuffer(&frame_buffer);
        if (zbuffer == NULL) {
            printf("ERROR: Failed to malloc zbuffer\n");
            return -1;
        }

        draw_model(&frame_buffer, zbuffer, cube_model, white);

        // Set (0,0) origin to top left
        flipImageVertically(&frame_buffer);
        writeTGAImageToFile(&frame_buffer, "output.tga");

        // End of frame cleanup
        free(zbuffer);
        free(frame_buffer.data);
    }

    return 0;
}
