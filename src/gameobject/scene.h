#ifndef SCENE_H
#define SCENE_H

#include <math.h>

#include "transform.h"
#include "camera.h"
#include "../shaders/shader.h"
#include "../math/vec3.h"

#include "../mesh/mesh.h"
#include "../mesh/obj_parser.h"

#define MAX_SCENE_OBJECTS 10

const extern vec3 COLLOR_PALLETE[7];

typedef struct {
    Transform transform;
    Mesh *mesh;
} GameObject;

typedef struct {
    GameObject game_objects[MAX_SCENE_OBJECTS];
    int num_objects;

    // Lights and camera dont count as objects
    Camera camera;
    Light lights[MAX_LIGHTS];
    vec3 light_colors[MAX_LIGHTS];
} Scene;

int init_scene(Scene *scene, UBO *ubo, int frame_width, int frame_height);

void scene_update(Scene *scene, UBO *ubo, float delta_time);

void add_object_to_scene(Scene *scene, GameObject object);

#endif