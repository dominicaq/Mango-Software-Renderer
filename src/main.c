#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "render/framedata.h"
#include "render/drawing.h"
#include "render/tga.h"

#include "gameobject/transform.h"
#include "gameobject/camera.h"

#include "mesh/mesh.h"
#include "mesh/obj_parser.h"

// EMU Resolution:
const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 288;

// Debug resolution (clear to see issues)
// const int SCREEN_WIDTH = 1920;
// const int SCREEN_HEIGHT = 1080;
const bool USE_WIREFRAME = false;

int main() {
    // Colors pallete
    vec4 black = (vec4){{0, 0, 0, 255}};

    // Allocate space for frame data
    Frame *frame = init_frame(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (frame == NULL) {
        return -1;
    }

    // Scene setup
    // -------------------------------------------------------------------------
    // Model(s) and gameobjects
    char *model_name = "../models/Atlas.obj";
    Mesh *cube_model = load_obj_mesh(model_name);
    if (cube_model == NULL) {
        return -1;
    }
    cube_model->color = (vec4){{255,255,255,255}};

    Transform cube_transform;
    cube_transform.position = (vec3){0.0f, -2.0f, -5.0f};
    cube_transform.eulerAngles = (vec3){0.0f, 0.0f, 0.0f};
    cube_transform.scale = (vec3){0.4f, 0.4f, 0.4f};

    // Camera properties
    Camera camera;
    camera.transform.position = (vec3){0.0f, 0.0f, -3.0f};
    camera.transform.eulerAngles = (vec3){35.0f, 0.0f, 0.0f};
    camera.fov = 90.0f;
    camera.aspect = (float)(frame->width) / frame->height;
    camera.zNear = 0.001f;
    camera.zFar = 1000.0f;

    clock_t tic = clock();

    // Update loop
    // -------------------------------------------------------------------------
    int frame_count = 60;
    printf("Timing: %d frames with model: %s\n", frame_count, model_name);
    for (int i = 0; i < frame_count; ++i) {
        // Reset frame
        cube_transform.eulerAngles.y += sinf(frame_count * 5);
        setTGAImageBackground(frame->framebuffer, black);
        reset_zbuffer(frame);

        // Update MVP Matrix: projection * view * model (multiplication order)
        Mat4x4 projection_matrix = perspective(&camera);
        Mat4x4 view_matrix = view(&camera);
        Mat4x4 model_matrix = get_model_matrix(cube_transform);
        Mat4x4 mvp = mat_mul(projection_matrix, mat_mul(view_matrix, model_matrix));

        // Draw the scene
        draw_mesh(frame, cube_model, mvp, USE_WIREFRAME);

        // Set (0,0) origin to top left
        flipImageVertically(frame->framebuffer);
        writeTGAImageToFile(frame->framebuffer, "../output/output.tga");
    }

    clock_t toc = clock();
    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);

    // Free mallocs
    free_frame(frame);
    return 0;
}
