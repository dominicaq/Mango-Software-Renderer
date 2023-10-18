#include <stdio.h>
#include <stdlib.h>

#include "framedata.h"
#include "drawing.h"
#include "obj_parser.h"

#include "gameobject/transform.h"
#include "gameobject/camera.h"

const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 288;
// const int SCREEN_WIDTH = 1920;
// const int SCREEN_HEIGHT = 1080;

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

    // Allocate space for frame data
    Frame *frame = init_frame(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (frame == NULL) {
        return -1;
    }

    // Model(s) and gameobjects
    // -------------------------------------------------------------------------
    Model *cube_model = load_obj_mesh("models/diablo.obj");
    if (cube_model == NULL) {
        return -1;
    }
    cube_model->color = white;

    Transform cube_transform;
    vec3 model_pos = {0.0f, -6.0f, -5.0f};
    vec3 model_euler = {0.0f, 0.0f, 0.0f};
    vec3 model_scale = {6.0f, 6.0f, 6.0f};
    cube_transform.position = model_pos;
    cube_transform.eulerAngles = model_euler;
    cube_transform.scale = model_scale;

    // Camera properties
    // -------------------------------------------------------------------------
    vec3 camera_pos = {0.0f, 5.0f, -20.0f};
    vec3 cam_euler_angles = {0.0f, 0.0f, 0.0f};
    Camera camera;
    camera.transform.position = camera_pos;
    camera.transform.eulerAngles = cam_euler_angles;
    camera.fov = 45.0f;
    camera.aspect = (float)(frame->width) / (float)(frame->height);
    camera.zNear = 0.01f;
    camera.zFar = 100.0f;

    // Update loop
    int frame_count = 1;
    for (int i = 0; i < frame_count; ++i) {
        // Reset frame
        setTGAImageBackground(frame->framebuffer, black);
        reset_zbuffer(frame);

        // Update MVP Matrix: projection * view * model of a model
        Mat4x4 projection_matrix = perspective(&camera);
        Mat4x4 view_matrix = view(&camera);
        Mat4x4 model_matrix = get_model_matrix(cube_transform);
        Mat4x4 mvp = mat_mul(projection_matrix, mat_mul(view_matrix, model_matrix));

        // Draw the scene
        draw_model(frame, cube_model, mvp);

        // Set (0,0) origin to top left
        flipImageVertically(frame->framebuffer);
        writeTGAImageToFile(frame->framebuffer, "output/output.tga");
    }

    // Free mallocs
    free_frame(frame);
    return 0;
}
