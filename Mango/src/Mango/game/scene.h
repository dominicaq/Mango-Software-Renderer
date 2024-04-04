#ifndef SCENE_H
#define SCENE_H

#include <math.h>

#include "../shaders/shader.h"
#include "camera.h"
#include "gameobject.h"

typedef struct {
    Camera *camera;
    bool *dirty_locals;
    GameObject *objects;
    GameObjectAttr *attributes;
    int object_count;
    Options options;
} Scene;

void scene_update_matrices(Scene *scene);

#endif
