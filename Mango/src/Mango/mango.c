
#include <SDL.h>
#include <time.h>

#include "mango.h"
#include "render/drawing.h"
#include "render/framedata.h"
#include "shaders/sdf.h"

/*
* Animations
*/
void mango_play_anim(Mango *mango, int object_index, AnimStack *stack) {
    for (int i = 0; i < mango->running_anims.len; ++i) {
        Anim *anim = mango->running_anims.arr + i;
        if (anim->time_progress >= anim->stack.time_end) {
            *anim = (Anim){0, object_index, *stack};
        }
    }
}

void mango_update_anim(Mango *mango, Anim *anim, float dt) {
    // if (anim->time_progress >= anim->stack.time_end) {
    //     return;
    // }
    // anim->time_progress += dt;
    // if (anim->time_progress < anim->stack.time_begin) {
    //     return;
    // }
    // for (int i = 0; i < anim->stack.layers.len; ++i) {
    //     AnimLayer *layer = &anim->stack.layers.arr[i];
    //     for (int j = 0; j < layer->anim_props.len; ++j) {
    //         AnimProp *prop = &layer->anim_props.arr[i];
    //         GameObject *obj = &mango->scene->objects[prop->node_index];
    //         prop_update(prop, obj, anim->time_progress);
    //     }
    // }
}

/*
* Core
*/
float mango_update(Mango *mango, float last_time) {
    float current_time = clock();
    float dt = current_time - last_time;
    mango->user_update(dt);
    frame_reset(mango->frame);

    Scene *current_scene = mango->scene;
    Camera *current_camera = current_scene->camera;
    mango->ubo.u_cam_pos = current_camera->game_object.position;

    // TODO: TEMP, create ambient light
    Light ambient_light;
    ambient_light.color = (Vec3){{1.0f, 1.0f, 1.0f}};
    ambient_light.intensity = 0.1f;
    mango->ubo.ambient_light = ambient_light;

    for (int i = 0; i < mango->running_anims.len; ++i) {
        mango_update_anim(mango, &mango->running_anims.arr[i], dt);
    }

    // Update scene transforms
    scene_update_matrices(current_scene);
    camera_update_matrix(current_camera);
    // idealy later we here we will organize scene into bvh and then add
    // and remove lights from a light stack ad we render objects

    // Update MVP Matrix: projection * view * model (multiplication order)
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

        // Update UBO per object
        mango->ubo.u_mvp = mvp;
        mango->ubo.u_vp_inv = mat4_inverse(vp);
        mango->ubo.u_model_view = model_view_matrix;
        mango->ubo.u_color = target_mesh->color;
        mango->ubo.u_mat = *target_mesh->material;

        draw_mesh(mango->frame, target_mesh, &mango->ubo);
    }

    // Draw SDF scene
    // if (mango->ubo.options & OPT_SDF_ENABLE) {
    //     Vec3 sphere_position = (Vec3){{0, 5.0f, -5.0f}};
    //     Mat4 sdf_model = sdf_model_matrix(sphere_position);
    //     Mat4 sdf_mv = mat4_mul(view_matrix, sdf_model);
    //     Mat4 sdf_mvp = mat4_mul(projection_matrix, sdf_mv);

    //     // Apply transformation
    //     Vec4 sdf_pos = vec3_to_vec4(sphere_position, 1.0f);
    //     Vec4 sdf_clip_space = mat_mul_vec4(sdf_mvp, sdf_pos);
    //     Vec3 sdf_ndc = vec4_homogenize(sdf_clip_space);
    //     sdf_draw(mango->frame, current_camera, sdf_ndc);
    // }

    frame_update(mango->frame);
    return current_time;
}

Mango *mango_alloc(Scene *scene, const char *title, int width, int height) {
    Mango *mango = (Mango *)malloc(sizeof(Mango));
    if (mango == NULL) {
        printf("mango_alloc mango malloc failed\n");
        return NULL;
    }

    mango->scene = scene;
    mango->frame = frame_alloc(title, width, height);
    if (mango->frame == NULL) {
        printf("mango_alloc frame malloc failed\n");
        return NULL;
    }

    init_clip_planes();

    mango->ubo.options = scene->options;
    mango->running_anims.len = 64;
    mango->running_anims.arr =
        (Anim *)malloc(mango->running_anims.len * sizeof(Anim));
    if (mango->running_anims.arr == NULL) {
        printf("mango_alloc anims malloc failed\n");
        return NULL;
    }
    printf("allocated animations\n");
    for (int i = 0; i < mango->running_anims.len; ++i) {
        mango->running_anims.arr[i].time_progress = 1;
        mango->running_anims.arr[i].stack.time_end = 0;
    }

    int num_lights = 0;
    for (int i = 0; i < scene->object_count; ++i) {
        if (scene->attributes[i].type == ATTR_LIGHT) {
            mango->ubo.lights[num_lights] = &scene->attributes[i].light;
            mango->ubo.light_objects[num_lights] = &scene->objects[i];
            ++num_lights;
        }
    }
    mango->ubo.num_lights = num_lights;
    return mango;
}

void mango_run(Mango *mango) {
    printf("running mango\n");
    float last_time = clock();
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
        last_time = mango_update(mango, last_time);
    }
}

void mango_free(Mango *mango) { frame_free(mango->frame); }