#ifndef SCENE_H
#define SCENE_H

#include <math.h>

#include "../shaders/shader.h"
#include "gameobject.h"
#include "camera.h"

typedef struct {
    Camera *camera;
    bool *dirty_locals;
    GameObject *objects;
    GameObjectAttr *attributes;
    int object_count;
    int max_depth;
    DebugOptions debug;
} Scene;

void scene_update(Scene *scene, UBO *ubo, float delta_time);
void scene_update_matrices(Scene *scene);

#endif
