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

typedef struct {
    Transform transform;
    Mesh *mesh;
} GameObject;

#define MAX_SCENE_OBJECTS 2
GameObject SCENE[MAX_SCENE_OBJECTS];

int main() {
    // Colors pallete
    vec3 black = (vec3){0.0f, 0.0f, 0.0f};
    vec3 white = (vec3){1.0f,1.0f,1.0f};

    // Allocate space for frame data
    Frame *frame = init_frame(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (frame == NULL) {
        return -1;
    }

    // Scene setup
    // -------------------------------------------------------------------------
    // Model(s) and gameobjects
    GameObject head;
    head.transform.position = (vec3){6.0f, -3.0f, -8.0f};
    head.transform.euler_angles = (vec3){0.0f, -20.0f, 0.0f};
    head.transform.scale = (vec3){6.0f, 6.0f, 6.0f};
    Mesh *head_mesh = load_obj_mesh("../models/head.obj");
    if (head_mesh == NULL) {
        return -1;
    }
    head_mesh->color = white;
    head.mesh = head_mesh;
    SCENE[0] = head;

    GameObject plane;
    plane.transform.position = (vec3){-3.0f, -1.0f, -3.0f};
    plane.transform.euler_angles = (vec3){0.0f, 40.0f, 0.0f};
    plane.transform.scale = (vec3){0.3f, 0.3f, 0.3f};
    Mesh *plane_model = load_obj_mesh("../models/Atlas.obj");
    if (plane_model == NULL) {
        return -1;
    }
    plane_model->color = white;
    plane.mesh = plane_model;
    SCENE[1] = plane;

    // Camera properties
    Camera camera;
    camera.transform.position = (vec3){0.0f, -1.0f, -3.0f};
    camera.transform.euler_angles = (vec3){35.0f, 0.0f, 0.0f};
    camera.fov = 90.0f;
    camera.aspect = (float)(frame->width) / frame->height;
    camera.zNear = 0.001f;
    camera.zFar = 1000.0f;

    clock_t frame_rate_start = clock();

    vec3 light_colors[7] = {
        (vec3){1.0f, 0.0f, 0.0f},  // Red
        (vec3){0.0f, 0.0f, 0.0f},  // White
        (vec3){1.0f, 1.0f, 0.0f},  // Yellow
        (vec3){0.0f, 1.0f, 0.0f},  // Green
        (vec3){0.0f, 0.0f, 1.0f},  // Blue
        (vec3){0.3f, 0.0f, 0.5f},  // Indigo
        (vec3){0.5f, 0.0f, 0.5f},  // Violet
    };

    // Light data
    UBO ubo;
    float light_radius = 3.5f;
    float angle_increment = 2.0f * M_PI / MAX_LIGHTS;
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        float angle = angle_increment * i;
        float x = light_radius * cosf(angle);
        float y = light_radius * sinf(angle);

        ubo.lights[i].u_light_position = (vec3){x, y, 0.0f};
        ubo.lights[i].u_light_color = vec3_to_vec4(light_colors[i], 1.0f);
    }

    // Update loop
    // -------------------------------------------------------------------------
    int frame_count = 1000;
    float delta_time = 0.0f;
    printf("Rendering %d frames...\n", frame_count);
    for (int i = 0; i < frame_count; ++i) {
        // Reset frame
        setTGAImageBackground(frame->framebuffer, black);
        reset_zbuffer(frame);
        // cube_transform.euler_angles.y += delta_time;

        // Light data (ring of lights)
        for (int i = 0; i < MAX_LIGHTS; ++i) {
            float angle = angle_increment * i + delta_time;
            float x = light_radius * cosf(angle);
            float y = light_radius * sinf(angle);
            ubo.lights[i].u_light_position = (vec3){x, y, -3};
        }

        // Update MVP Matrix: projection * view * model (multiplication order)
        Mat4x4 projection_matrix = perspective(&camera);
        Mat4x4 view_matrix = view(&camera);
        ubo.u_cam_pos = camera.transform.position;
        for (int i = 0; i < MAX_SCENE_OBJECTS; ++i) {
            Mat4x4 model_matrix = get_model_matrix(SCENE[i].transform);
            Mat4x4 model_view = mat_mul(view_matrix, model_matrix);
            Mat4x4 mvp = mat_mul(projection_matrix, model_view);

            ubo.u_mvp = mvp;
            ubo.u_model = model_matrix;
            ubo.u_model_view = model_view;
            ubo.u_wireframe = USE_WIREFRAME;
            ubo.u_color = SCENE[i].mesh->color;
            draw_mesh(frame, SCENE[i].mesh, &ubo);
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
