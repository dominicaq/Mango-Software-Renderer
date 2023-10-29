
#include <stdlib.h>
#include <time.h>

#include "gameobject/scene.h"
#include "models/spider.h"
#include "render/drawing.h"
#include "render/framedata.h"
#include "render/tga.h"
#include "shaders/shader.h"

// EMU Resolution:
// const int SCREEN_WIDTH = 512;
// const int SCREEN_HEIGHT = 288;

// Debug resolution (clear to see issues)
const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 440;
const bool USE_WIREFRAME = false;

void init_camera(Scene *scene, int frame_width, int frame_height) {
    // Camera properties
    scene->camera.transform.position = (Vec3){{0.0f, 2.0f, 10.0f}};
    scene->camera.transform.euler_angles = (Vec3){{0.0f, 0.0f, 0.0f}};
    scene->camera.transform.scale = (Vec3){{1.0f, 1.0f, 1.0f}};
    scene->camera.fov = 45.0f;
    scene->camera.aspect = (float)(frame_width) / frame_height;
    scene->camera.zNear = 0.1f;
    scene->camera.zFar = 1000.0f;
}

int alloc_objects(Scene *scene) {
    Vec3 white = (Vec3){{1.0f, 1.0f, 1.0f}};
    Vec3 red = (Vec3){{0.0f, 1.0f, 0.5f}};
    // Objects
    scene->num_objects = 4;
    GameObject *objects =
        malloc(scene->num_objects * scene->num_objects * sizeof(GameObject));
    if (objects == NULL) {
        return -1;
    }
    scene->objects = objects;

    objects[0].transform.position = (Vec3){{6.0f, -3.0f, -8.0f}};
    objects[0].transform.euler_angles = (Vec3){{0.0f, -20.0f, 0.0f}};
    objects[0].transform.scale = (Vec3){{5.0f, 5.0f, 5.0f}};
    Mesh *head_mesh = load_obj_mesh("../models/head.obj");
    if (head_mesh == NULL) {
        printf("ERROR: Failed to load head mesh\n");
        free(objects);
        return -1;
    }
    head_mesh->color = white;
    objects[0].mesh = head_mesh;

    objects[1].transform.position = (Vec3){{0.0f, 6.0f, -10.0f}};
    objects[1].transform.euler_angles = (Vec3){{90.0f, 90.0f, 90.0f}};
    objects[1].transform.scale = (Vec3){{1.0f, 1.0f, 1.0f}};
    Mesh *box_model = load_obj_mesh("../models/light_box.obj");
    if (box_model == NULL) {
        printf("ERROR: Failed to loead plane mesh\n");
        free(objects);
        return -1;
    }
    box_model->color = red;
    objects[1].mesh = box_model;

    objects[2].transform.position = (Vec3){{-5.0f, -3.0f, -10.0f}};
    objects[2].transform.euler_angles = (Vec3){{0.0f, 70.0f, 0.0f}};
    objects[2].transform.scale = (Vec3){{6.0f, 6.0f, 6.0f}};
    Mesh *diablo_model = load_obj_mesh("../models/head.obj");
    if (diablo_model == NULL) {
        printf("ERROR: Failed to load diablo mesh\n");
        free(objects);
        return -1;
    }
    diablo_model->color = white;
    objects[2].mesh = diablo_model;

    objects[3].transform.position = (Vec3){{0.0f, -3.0f, 0.0f}};
    objects[3].transform.euler_angles = (Vec3){{0.0f, 0.0f, 0.0f}};
    objects[3].transform.scale = (Vec3){{2.0f, 2.0f, 2.0f}};
    spider001_mesh.color = white;
    // objects[3].mesh = &spider001_mesh;

    return 0;
}

int main() {
    // Allocate space for frame data
    Frame *frame = init_frame(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (frame == NULL) {
        return -1;
    }

    // Scene data
    UBO ubo;
    Scene scene;
    init_camera(&scene, frame->width, frame->height);
    Vec3 ambience = (Vec3){{0.4f, 0.4f, 0.4f}};
    ubo.u_ambient = ambience;

    if (alloc_objects(&scene) != 0) {
        return 1;
    }

    // Init light data
    float light_radius = 20.0f;
    int num_lights = 3;
    Light lights[3];
    for (int i = 0; i < num_lights; ++i) {
        lights[i].u_color = vec3_to_vec4(COLLOR_PALLETE[i], 1.0f);
        lights[i].u_radius = light_radius;
    }
    ubo.lights = lights;
    ubo.num_lights = num_lights;

    // Update loop
    // -------------------------------------------------------------------------
    Vec3 black = (Vec3){{0.0f, 0.0f, 0.0f}};

    int frame_count = 1000;
    float delta_time = 0.0f;
    clock_t frame_rate_start = clock();
    for (int i = 0; i < frame_count; ++i) {
        // Reset frame
        setTGAImageBackground(frame->framebuffer, black);
        reset_zbuffer(frame);

        scene_update(&scene, &ubo, delta_time);
        ubo.u_time = delta_time;

        // Update MVP Matrix: projection * view * model (multiplication order)
        scene.camera.transform.euler_angles.y += 1;
        Mat4 projection_matrix = perspective(&scene.camera);
        Mat4 cam_matrix = transform_to_mat(scene.camera.transform);
        Mat4 view_matrix = mat4_invert(cam_matrix);
        // scene.objects[3].transform.euler_angles.y += 1;
        for (int i = 0; i < scene.num_objects; ++i) {
            GameObject render_target = scene.objects[i];
            if (render_target.mesh == NULL) {
                continue;
            }
            Mat4 model_matrix = transform_to_mat(render_target.transform);
            Mat4 model_view_matrix = mat4_mul(view_matrix, model_matrix);
            Mat4 mvp = mat4_mul(projection_matrix, model_view_matrix);
            Mat4 vp = mat4_mul(projection_matrix, view_matrix);

            ubo.u_mvp = mvp;
            ubo.u_vp_inv = mat4_invert(vp);
            ubo.u_model_view = model_view_matrix;
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
           (float)(frame_rate_end - frame_rate_start) / CLOCKS_PER_SEC);

    // Free mallocs
    free_frame(frame);
    return 0;
}
