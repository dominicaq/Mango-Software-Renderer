#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "render/framedata.h"
#include "render/drawing.h"
#include "render/tga.h"

#include "gameobject/scene.h"

#include "shaders/shader.h"

// EMU Resolution:
const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 288;

// Debug resolution (clear to see issues)
// const int SCREEN_WIDTH = 1920;
// const int SCREEN_HEIGHT = 1080;
const bool USE_WIREFRAME = false;

int main() {
    // Allocate space for frame data
    Frame *frame = init_frame(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (frame == NULL) {
        return -1;
    }

    // Scene data
    UBO ubo;
    Scene scene;
    scene.num_objects = 0;
    int scene_success = init_scene(&scene, &ubo, frame->width, frame->height);
    if (scene_success < 0) {
        return -1;
    }

    // Update loop
    // -------------------------------------------------------------------------
    vec3 black = (vec3){0.0f, 0.0f, 0.0f};

    int frame_count = 9999;
    float delta_time = 0.0f;
    clock_t frame_rate_start = clock();
    printf("Rendering %d frames...\n", frame_count);
    for (int i = 0; i < frame_count; ++i) {
        // Reset frame
        setTGAImageBackground(frame->framebuffer, black);
        reset_zbuffer(frame);

        scene_update(&scene, &ubo, delta_time);
        ubo.u_time = delta_time;

        // Update MVP Matrix: projection * view * model (multiplication order)
        Mat4x4 projection_matrix = perspective(&scene.camera);
        Mat4x4 view_matrix = view(&scene.camera);
        for (int i = 0; i < scene.num_objects; ++i) {
            GameObject render_target = scene.game_objects[i];
            Mat4x4 model_matrix = get_model_matrix(render_target.transform);
            Mat4x4 model_view = mat_mul(view_matrix, model_matrix);
            Mat4x4 mvp = mat_mul(projection_matrix, model_view);

            ubo.u_mvp = mvp;
            ubo.u_model = model_matrix;
            ubo.u_model_view = model_view;
            ubo.u_wireframe = USE_WIREFRAME;
            ubo.u_color = render_target.mesh->color;
            draw_mesh(frame, render_target.mesh, &ubo);
        }

        delta_time += 0.05f;

        // Set (0,0) origin to top left
        flipImageVertically(frame->framebuffer);
        writeTGAImageToFile(frame->framebuffer, "../output.tga");
    }

    clock_t frame_rate_end = clock();
    printf("Elapsed: %f seconds\n",
        (float)(frame_rate_end - frame_rate_start) / CLOCKS_PER_SEC
    );

    // Free mallocs
    free_frame(frame);
    return 0;
}
