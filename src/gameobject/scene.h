#ifndef SCENE_H
#define SCENE_H

#include <math.h>

#include "../math/vec3.h"
#include "../mesh/mesh.h"
#include "../mesh/obj_parser.h"
#include "../shaders/shader.h"
#include "camera.h"
#include "transform.h"

#define MAX_SCENE_OBJECTS 10

const extern vec3 COLLOR_PALLETE[7];

typedef struct GameObject {
    Transform transform;
    Mesh *mesh;
    int num_children;
    struct GameObject *children;
} GameObject;

typedef struct {
    GameObject *objects;
    int num_objects;

    // Lights and camera dont count as objects
    Camera camera;
} Scene;

void scene_update(Scene *scene, UBO *ubo, float delta_time);

#endif
