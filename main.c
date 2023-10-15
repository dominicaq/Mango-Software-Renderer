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

// Camera position
vec3 camera_pos = {0.0f, 0.0f, 0.0f};

int main() {
    // Colors
    float brightness = 1.0f;
    TGAColor white = createTGAColor(
        255 * brightness, // R
        255 * brightness, // G
        255 * brightness, // B
        255               // A
    );
    TGAColor black = createTGAColor(0, 0, 0, 255);

    // Init image
    TGAImage frame_buffer = createTGAImage(SCREEN_WIDTH, SCREEN_HEIGHT);
    setTGAImageBackground(&frame_buffer, black);

    // Load OBJ file(s)
    Model *cube_model = load_obj_mesh("models/rotated_cube.obj");
    if (cube_model == NULL) {
        return -1;
    }

    int *zbuffer = malloc(sizeof(int) * SCREEN_WIDTH * SCREEN_HEIGHT);
    if (zbuffer == NULL) {
        printf("ERROR: Failed to malloc zbuffer\n");
        return -1;
    }
    draw_model(&frame_buffer, zbuffer, cube_model, white);

    // Create image file
    flipImageVertically(&frame_buffer);
    writeTGAImageToFile(&frame_buffer, "output.tga");

    // Cleanup
    free(frame_buffer.data);
    free(zbuffer);
    return 0;
}
