
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game/scene.h"
#include "math/vec4.h"
#include "mesh/obj_parser.h"
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
const bool USE_RASTERIZE = true;

void init_camera(Scene *scene, int frame_width, int frame_height) {
    // Camera properties
    scene->camera.game_object = game_object_default();
    scene->camera.game_object.position = (Vec3){{0.0f, 2.0f, 30.0f}};
    scene->camera.fov = 45.0f;
    scene->camera.aspect = (float)(frame_width) / frame_height;
    scene->camera.zNear = 0.1f;
    scene->camera.zFar = 1000.0f;
}

int alloc_objects(Scene *scene) {
    Vec3 white = (Vec3){{1.0f, 1.0f, 1.0f}};
    Vec3 red = (Vec3){{0.0f, 1.0f, 0.5f}};
    // Objects
    scene->object_count = 3 + spider_object_amt;
    scene->dirty_locals = malloc(scene->object_count * sizeof(bool));
    if (scene->dirty_locals == NULL) {
        fprintf(stderr, "malloc failed objects");
    }
    for (int i = 0; i < scene->object_count; ++i) {
        scene->dirty_locals[i] = true;
    }
    scene->camera_dirty_local = true;
    scene->attributes = malloc(scene->object_count * sizeof(GameObjectAttr));
    if (scene->attributes == NULL) {
        fprintf(stderr, "malloc failed attributes");
    }
    scene->objects = malloc(scene->object_count * sizeof(GameObject));
    if (scene->objects == NULL) {
        fprintf(stderr, "malloc failed objects");
    }

    scene->objects[0] = game_object_default();
    scene->objects[0].position = (Vec3){{6.0f, -3.0f, -8.0f}};
    scene->objects[0].scale = (Vec3){{5.0f, 5.0f, 5.0f}};
    scene->attributes[0].type = MESH;
    scene->attributes[0].mesh = load_obj_mesh("../models/head.obj");
    scene->attributes[0].mesh.color = white;

    scene->objects[1] = game_object_default();
    scene->objects[1].quaternion = quat_from_units(UNIT_X, UNIT_Z);
    scene->objects[1].position = (Vec3){{0.0f, 6.0f, -10.0f}};
    scene->attributes[1].type = MESH;
    scene->attributes[1].mesh = load_obj_mesh("../models/light_box.obj");
    scene->attributes[1].mesh.color = red;

    scene->objects[2] = game_object_default();
    scene->objects[2].position = (Vec3){{-5.0f, -3.0f, -10.0f}};
    scene->objects[2].scale = (Vec3){{6.0f, 6.0f, 6.0f}};
    scene->attributes[2].type = MESH;
    scene->attributes[2].mesh = load_obj_mesh("../models/head.obj");
    scene->attributes[2].mesh.color = white;

    memcpy(scene->objects + 3, spider_game_objects,
           spider_object_amt * sizeof(GameObject));
    memcpy(scene->attributes + 3, spider_attrs,
           spider_object_amt * sizeof(GameObjectAttr));
    scene->max_depth = MAX(scene->max_depth, spider_max_depth);

    scene->objects[3].scale = (Vec3){{0.1f, 0.1f, 0.1f}};

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
        lights[i].color = vec3_to_vec4(COLLOR_PALLETE[i], 1.0f);
        lights[i].radius = light_radius;
    }
    ubo.lights = lights;
    ubo.num_lights = num_lights;

    // Update loop
    // -------------------------------------------------------------------------
    Vec3 black = (Vec3){{0.0f, 0.0f, 0.0f}};
    Vec4 slight_right = quat_from_axis(UNIT_Y, 0.01f);

    int frame_count = 1000;
    float delta_time = 0.0f;
    clock_t frame_rate_start = clock();
    for (int i = 0; i < frame_count; ++i) {
        // Reset frame
        setTGAImageBackground(frame->framebuffer, black);
        reset_zbuffer(frame);

        scene_update(&scene, &ubo, delta_time);
        ubo.u_time = delta_time;

        // Update object(s)
        quat_mul(&scene.objects[3].quaternion, &slight_right);
        scene.dirty_locals[3] = true;
        // quat_mul(&scene.camera.game_object.quaternion, &slight_right);
        // scene.camera.game_object.needs_update = true;
        // End

        // Update camera transform
        scene_update_matrices(&scene);

        // Update MVP Matrix: projection * view * model (multiplication order)
        Mat4 projection_matrix = perspective(&scene.camera);
        Mat4 view_matrix = mat4_inverse(scene.camera.game_object.world_matrix);
        for (int i = 0; i < scene.object_count; ++i) {
            if (scene.attributes[i].type != MESH) {
                continue;
            }
            Mesh *target_mesh = &scene.attributes[i].mesh;
            GameObject *target_object = scene.objects + i;

            // Update matricies
            Mat4 model_matrix = target_object->world_matrix;
            Mat4 model_view_matrix = mat4_mul(view_matrix, model_matrix);
            Mat4 mvp = mat4_mul(projection_matrix, model_view_matrix);
            Mat4 vp = mat4_mul(projection_matrix, view_matrix);

            // Update UBO
            ubo.u_mvp = mvp;
            ubo.u_vp_inv = mat4_inverse(vp);
            ubo.u_model_view = model_view_matrix;
            ubo.u_color = target_mesh->color;

            // UBO debug options
            ubo.debug.wireframe = USE_WIREFRAME;
            ubo.debug.rasterize = USE_RASTERIZE;

            draw_mesh(frame, target_mesh, &ubo);
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
