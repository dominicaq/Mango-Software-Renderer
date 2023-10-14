#include <stdio.h>
#include <stdlib.h>

#include "tga.h"
#include "shapes.h"
#include "obj_parser.h"

const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 288;
// 16:9 aspect ratio

// Camera position
vec3 camera_pos = {0.0f, 0.0f, 0.0f};

int main() {
    // Colors
    float brightness = 0.7f;
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
    MeshData *cube_mesh = load_obj_mesh("models/rotated_cube.obj");
    if (cube_mesh == NULL) {
        return -1;
    }

    draw_model(&frame_buffer, cube_mesh, white);

    // Create image file
    flipImageVertically(&frame_buffer);
    writeTGAImageToFile(&frame_buffer, "output.tga");

    // Cleanup
    free(frame_buffer.data);
    return 0;
}
