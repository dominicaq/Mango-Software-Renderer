#include <time.h>

#ifndef RISCV_CONSOLE
#include <SDL.h>
#endif

#include "mango.h"
#include "render/drawing.h"
#include "render/framedata.h"
#include "shaders/sdf.h"

#ifdef RISCV_CONSOLE
uint32_t GetTicks(void);
uint32_t GetController(void);
uint32_t SetVideoCallback(void (*on_video)());

uint32_t mango_get_controller() { return GetController(); }
#endif
void (*user_update)(Real dt);
void mango_on_update(void (*callback)(Real dt)) { user_update = callback; }

typedef struct {
    Frame *frame;
    Scene *scene;
    clock_t last_time;
    UBO ubo;
} Mango;

void mango_update(Mango *mango) {
    clock_t current_time = clock();
    double dt = current_time - mango->last_time;
    user_update(dt);
    mango->last_time = current_time;
    frame_reset(mango->frame);

    Scene *current_scene = mango->scene;
    Camera *current_camera = current_scene->camera;
    mango->ubo.u_cam_pos = current_camera->game_object.position;

    // Update scene transforms
    scene_update_matrices(current_scene);
    camera_update_matrix(current_camera);
    // idealy later we here we will organize scene into bvh and then add
    // and remove lights from a light stack ad we render objects

    // Update MVP Matrix: projection * view * model (multiplication
    // order)
    Mat4 projection_matrix = perspective(current_camera);
    Mat4 view_matrix = mat4_inverse(current_camera->game_object.world_matrix);
    for (int i = 0; i < current_scene->object_count; ++i) {
        if (current_scene->attributes[i].type != ATTR_MESH) {
            continue;
        }
        Mesh *target_mesh = &current_scene->attributes[i].mesh;
        GameObject *target_object = current_scene->objects + i;

        // Update matricies
        Mat4 model_matrix = target_object->world_matrix;
        Mat4 model_view_matrix = mat4_mul(view_matrix, model_matrix);
        Mat4 mvp = mat4_mul(projection_matrix, model_view_matrix);
        Mat4 vp = mat4_mul(projection_matrix, view_matrix);

        // Update UBO
        mango->ubo.u_mvp = mvp;
        mango->ubo.u_vp_inv = mat4_inverse(vp);
        mango->ubo.u_model_view = model_view_matrix;
        mango->ubo.u_color = target_mesh->color;

        // UBO debug options
        draw_mesh(mango->frame, target_mesh, &mango->ubo);
    }

    // TODO: Temp
    sdf_draw(mango->frame, mango->scene->camera);

    frame_update(mango->frame);
}

void mango_run(Scene *scene, const char* title, int width, int height) {
    Mango mango;
    mango.scene = scene;
    mango.frame = frame_alloc(title, width, height);
    if (mango.frame == NULL) {
        return;
    }

    mango.ubo.debug = scene->debug;
    clock_t start_time = clock();
    mango.last_time = start_time;

    int num_lights = 0;
    for (int i = 0; i < scene->object_count; ++i) {
        if (scene->attributes[i].type == ATTR_LIGHT) {
            mango.ubo.lights[num_lights] = &scene->attributes[i].light;
            mango.ubo.light_objects[num_lights] = &scene->objects[i];
            ++num_lights;
        }
    }
    mango.ubo.num_lights = num_lights;

#ifdef RISCV_CONSOLE
    SetVideoCallback(mango_update);
    SetTimerCallback(onTimer, 10000);

    while (1) {
    }
#else
    // populate lights
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                quit = true;
                break;
            default:
                break;
            }
        }
        mango_update(&mango);
    }
#endif

    frame_free(mango.frame);
}
