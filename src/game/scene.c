#include "scene.h"

void scene_update(Scene *scene, UBO *ubo, float delta_time) {
    float circle_radius = 4.0f;
    float angle_increment = 2.0f * M_PI / ubo->num_lights;
    for (int i = 0; i < ubo->num_lights; ++i) {
        float angle = angle_increment * i + delta_time;
        float x = circle_radius * cosf(angle);
        float z = circle_radius * sinf(angle);
        ubo->lights[i].position = (Vec3){{x, 0.0f, z}};
    }

    ubo->u_cam_pos = scene->camera.game_object.position;
}

#define MAX_DEPTH 16
void scene_update_matrices(Scene *scene) {
    int remaining_children[MAX_DEPTH] = {0};
    int parent_inds[MAX_DEPTH] = {0};
    int depth = -1;
    for (int i = 0; i < scene->num_game_objects; ++i) {
        GameObject *obj = scene->game_objects + i;
        while (depth != -1 && remaining_children[depth] == 0) {
            --depth;
        }
        if (depth != -1) {
            if (remaining_children[depth] > 0) {
                --remaining_children[depth];
            } else {
                obj->needs_update = true;
                ++remaining_children[depth];
            }
        }
        if (obj->needs_update) {
            game_object_update_model_matrix(obj);
            if (depth != -1) {
                obj->model_matrix = mat4_mul(
                    obj->model_matrix,
                    scene->game_objects[parent_inds[depth]].model_matrix);
            }
        }
        if (obj->num_children) {
            ++depth;
            remaining_children[depth] =
                obj->num_children * (obj->needs_update ? 1 : -1);
            parent_inds[depth] = i;
        }
        obj->needs_update = false;
    }
    if (scene->camera.game_object.needs_update) {
        game_object_update_model_matrix(&scene->camera.game_object);
        scene->camera.game_object.needs_update = false;
    }
}
