#include "mango.h"
#include <SDL.h>
#include <time.h>

#include "render/drawing.h"
#include "render/framedata.h"

void (*user_update)(MangoReal dt);
void mango_on_update(void (*callback)(MangoReal dt)) { user_update = callback; }

void mango_run(Scene *scene, Camera *camera) {
    Frame *frame = frame_alloc(camera->width, camera->height);
    if (frame == NULL) {
        return;
    }

    UBO ubo;
    ubo.debug = scene->debug;
    SDL_Event e;
    bool quit = false;
    clock_t start_time = clock();
    clock_t last_time = start_time;

    // populate lights
    ubo.num_lights = 0;
    for (int i = 0; i < scene->object_count; ++i) {
        if (scene->attributes[i].type == ATTR_LIGHT) {
            ubo.lights[ubo.num_lights] = &scene->attributes[i].light;
            ubo.light_objects[ubo.num_lights] = &scene->objects[i];
            ++ubo.num_lights;
        }
    }

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

        clock_t current_time = clock();
        double dt = current_time - last_time;
        user_update(dt);
        last_time = current_time;
        frame_reset(frame);

        ubo.u_cam_pos = camera->game_object.position;

        // Update scene transforms
        scene_update_matrices(scene);
        camera_update_matrix(camera);
        // idealy later we here we will organize scene into bvh and then add
        // and remove lights from a light stack ad we render objects

        // Update MVP Matrix: projection * view * model (multiplication
        // order)
        Mat4 projection_matrix = perspective(camera);
        Mat4 view_matrix = mat4_inverse(camera->game_object.world_matrix);
        for (int i = 0; i < scene->object_count; ++i) {
            if (scene->attributes[i].type != ATTR_MESH) {
                continue;
            }
            Mesh *target_mesh = &scene->attributes[i].mesh;
            GameObject *target_object = scene->objects + i;

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
            draw_mesh(frame, target_mesh, &ubo);
        }

        // TODO: Temp
        SDFSphere dummy_sphere;
        dummy_sphere.position = (Vec3){{frame->width / 2, frame->height / 2, 0}};
        dummy_sphere.radius = 50.0f;
        dummy_sphere.color = (Vec3){{255.0f, 255.0f, 255.0f}};
        draw_sdf(frame, &dummy_sphere);

        frame_update(frame);
    }
    frame_free(frame);
}
