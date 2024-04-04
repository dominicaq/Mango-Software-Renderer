#include "scene.h"

#define MAX_DEPTH 16
void scene_update_matrices(Scene *scene) {
    int parent_inds[MAX_DEPTH] = {0};
    int depth = -1;
    bool update_world = false;
    for (int i = 0; i < scene->object_count; ++i) {
        GameObject *obj = scene->objects + i;
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
        scene->dirty_locals[i] = false;
    }
}
