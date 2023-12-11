
#ifndef RVC
#include <SDL.h>
#include <time.h>
#endif

#include <stdarg.h>

#include "mango.h"
#include "render/drawing.h"
#include "render/framedata.h"
#include "shaders/sdf.h"

#ifdef RVC

#define MODE_CONTROL (*((volatile uint32_t *)0x500F6780))
#define LINE_LEN 64
char *TEXT_DATA = (char *)0x500F4800;

uint32_t copyMemory(void *src, void *dest, uint32_t size);

uint32_t memcpy(void *dest, void *src, uint32_t size) {
    // DMA1_SRC = (uint32_t)src;
    // DMA1_DEST = (uint32_t)dest;
    // DMA1_SIZE = size;
    // DMA1_STATUS = DTA;
    // while (DMA1_STATUS & DTA) {
    // }
    for (int32_t i = size - 1; i > -1; --i) {
        ((uint8_t *)dest)[i] = ((uint8_t *)src)[i];
    }
    return (uint32_t)dest;
}

uint32_t printf(const char *fmt, ...) {
    if (MODE_CONTROL & 0b1) {
        return 2;
    }
    va_list args;
    va_start(args, fmt);
    char line[LINE_LEN] = {0};
    int line_i = 0;
    for (int i = 0; fmt[i] != '\0'; ++i) {
        if (fmt[i] != '%') {
            line[line_i++] = fmt[i];
            continue;
        }
        ++i;
        if (fmt[i] == '\0') {
            break;
        }

        switch (fmt[i]) {
        case 'd':
            {
                int num = va_arg(args, int);
                int start = line_i;
                // Extract digits in reverse order
                do {
                    line[line_i++] = num % 10 + '0';
                    num /= 10;
                } while (num != 0);

                // Reverse the string
                int end = line_i - 1;
                while (start < end) {
                    char temp = line[start];
                    line[start] = line[end];
                    line[end] = temp;
                    start++;
                    end--;
                }
            }
        case 'f':
            break;
        }
    }

    memcpy(TEXT_DATA + LINE_LEN, TEXT_DATA, LINE_LEN * 31);
    memcpy(TEXT_DATA, line, LINE_LEN);
    va_end(args);
    return 22;
}

#endif

void mango_play_anim(Mango *mango, int object_index, AnimStack *stack) {
    for (int i = 0; i < mango->running_anims.len; ++i) {
        Anim *anim = mango->running_anims.arr + i;
        if (anim->time_progress >= anim->stack.time_end) {
            *anim = (Anim){0, object_index, *stack};
        }
    }
}

void mango_update_anim(Mango *mango, Anim *anim, Real dt) {
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

Real mango_update(Mango *mango, Real last_time) {
    Real current_time = clock();
    Real dt = current_time - last_time;
    mango->user_update(dt);
    frame_reset(mango->frame);

    Scene *current_scene = mango->scene;
    Camera *current_camera = current_scene->camera;
    mango->ubo.u_cam_pos = current_camera->game_object.position;

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

        // Update UBO
        mango->ubo.u_mvp = mvp;
        mango->ubo.u_vp_inv = mat4_inverse(vp);
        mango->ubo.u_model_view = model_view_matrix;
        mango->ubo.u_color = target_mesh->color;
        mango->ubo.u_mat = target_mesh->material;

        draw_mesh(mango->frame, target_mesh, &mango->ubo);
    }

    // Draw SDF scene
    if (mango->ubo.options & OPT_SDF_ENABLE) {
        Vec3 sphere_position = (Vec3){{0, 5.0f, -5.0f}};
        Mat4 sdf_model = sdf_model_matrix(sphere_position);
        Mat4 sdf_mv = mat4_mul(view_matrix, sdf_model);
        Mat4 sdf_mvp = mat4_mul(projection_matrix, sdf_mv);

        // Apply transformation
        Vec4 sdf_pos = vec3_to_vec4(sphere_position, 1.0f);
        Vec4 sdf_clip_space = mat_mul_vec4(sdf_mvp, sdf_pos);
        Vec3 sdf_ndc = vec4_homogenize(sdf_clip_space);
        sdf_draw(mango->frame, current_camera, sdf_ndc);
    }

    frame_update(mango->frame);
    return current_time;
}

Mango *mango_alloc(Scene *scene, const char *title, int width, int height) {
    Mango *mango = (Mango *)malloc(sizeof(Mango));
    if (mango == NULL) {
        printf("mango_alloc mango malloc failed\n");
        return NULL;
    }
    printf("allocated mango\n");
    mango->scene = scene;
    mango->frame = frame_alloc(title, width, height);
    if (mango->frame == NULL) {
        printf("mango_alloc frame malloc failed\n");
        return NULL;
    }
    printf("allocated frame\n");

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
#ifdef RVC
    uint32_t last_time = clock();
    printf("running mango %d", last_time);
    while (1) {
        last_time = mango_update(mango, last_time);
    }
#else
    Real last_time = clock();
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
#endif
}

void mango_free(Mango *mango) { frame_free(mango->frame); }
