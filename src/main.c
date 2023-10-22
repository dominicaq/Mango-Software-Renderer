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

#include "shaders/shader.h"

// EMU Resolution:
const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 288;

// Debug resolution (clear to see issues)
// const int SCREEN_WIDTH = 1920;
// const int SCREEN_HEIGHT = 1080;
const bool USE_WIREFRAME = false;

int main() {
    // Colors pallete
    vec4 black = (vec4){{0.0f, 0.0f, 0.0f,255.0f}};
    vec4 white = (vec4){{1.0f,1.0f,1.0f,255.0f}};

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
    cube_model->color = white;

    Transform cube_transform;
    cube_transform.position = (vec3){0.0f, -2.0f, -5.0f};
    cube_transform.euler_angles = (vec3){0.0f, 0.0f, 0.0f};
    cube_transform.scale = (vec3){0.4f, 0.4f, 0.4f};

    // Camera properties
    Camera camera;
    camera.transform.position = (vec3){0.0f, 0.0f, -3.0f};
    camera.transform.euler_angles = (vec3){35.0f, 0.0f, 0.0f};
    camera.fov = 90.0f;
    camera.aspect = (float)(frame->width) / frame->height;
    camera.zNear = 0.001f;
    camera.zFar = 1000.0f;

    clock_t frame_rate_start = clock();

    // Update loop
    // -------------------------------------------------------------------------
    int frame_count = 60;
    float delta_time = 0.0f;
    printf("Timing: %d frames with model: %s\n", frame_count, model_name);
    for (int i = 0; i < frame_count; ++i) {
        clock_t start_frame = clock();

        // Reset frame
        setTGAImageBackground(frame->framebuffer, black);
        reset_zbuffer(frame);

        cube_transform.euler_angles.y += sinf(delta_time * 50);

        // Update MVP Matrix: projection * view * model (multiplication order)
        Mat4x4 projection_matrix = perspective(&camera);
        Mat4x4 view_matrix = view(&camera);
        Mat4x4 model_matrix = get_model_matrix(cube_transform);
        Mat4x4 model_view = mat_mul(view_matrix, model_matrix);
        Mat4x4 mvp = mat_mul(projection_matrix, model_view);

        // Create the uniform buffer object for shaders
        UBO ubo;
        ubo.u_mvp = mvp;
        ubo.u_model_view = model_view;
        ubo.u_wireframe = USE_WIREFRAME;
        ubo.u_light_position = (vec3){1.0f, 1.0f, 1.0f};
        ubo.u_light_color = (vec4){{1.0f,1.0f,0.0f,1.0f}};
        ubo.u_color = cube_model->color;

        // Draw the scene
        draw_mesh(frame, cube_model, &ubo);

        // Game clock
        clock_t end_frame = clock();
        delta_time = (float)(end_frame - start_frame) / CLOCKS_PER_SEC;

        // Set (0,0) origin to top left
        flipImageVertically(frame->framebuffer);
        writeTGAImageToFile(frame->framebuffer, "../output/output.tga");
    }

    clock_t frame_rate_end = clock();
    printf("Elapsed: %f seconds\n",
        (float)(frame_rate_end - frame_rate_start) / CLOCKS_PER_SEC
    );

    // Free mallocs
    free_frame(frame);
    return 0;
}
