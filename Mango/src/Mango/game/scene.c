#include "scene.h"

#define MAX_DEPTH 16
void scene_update_matrices(Scene *scene) {
    int remaining_children[MAX_DEPTH] = {0};
    int parent_inds[MAX_DEPTH] = {0};
    int depth = -1;
    bool update_world = false;
    for (int i = 0; i < scene->object_count; ++i) {
        GameObject *obj = scene->objects + i;
        while (depth != -1 && remaining_children[depth] == 0) {
            --depth;
        }
        if (depth != -1) {
            if (remaining_children[depth] > 0) {
                --remaining_children[depth];
            } else {
                update_world = true;
                ++remaining_children[depth];
            }
        }
        if (scene->dirty_locals[i]) {
            game_object_update_local_matrix(obj);
        }
        if (scene->dirty_locals[i] || update_world) {
            if (depth == -1) {
                obj->world_matrix = obj->local_matrix;
            } else {
                obj->world_matrix =
                    mat4_mul(obj->local_matrix,
                             scene->objects[parent_inds[depth]].local_matrix);
            }
        }
        if (obj->num_children) {
            ++depth;
            remaining_children[depth] =
                obj->num_children * (scene->dirty_locals[i] ? 1 : -1);
            parent_inds[depth] = i;
        }
        scene->dirty_locals[i] = false;
    }
}
