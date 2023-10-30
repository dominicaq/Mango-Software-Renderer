#ifndef SCENE_H
#define SCENE_H

#include <math.h>

#include "../shaders/shader.h"
#include "camera.h"
#include "gameobject.h"

typedef struct {
    GameObject *game_objects;
    Mesh *meshes;
    int num_game_objects;
    int max_depth;

    // Lights and camera dont count as objects
    Camera camera;
} Scene;

void scene_update(Scene *scene, UBO *ubo, float delta_time);
void scene_update_matrices(Scene *scene);

#endif
