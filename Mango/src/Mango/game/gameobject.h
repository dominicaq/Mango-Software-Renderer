#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <math.h>
#include <stdbool.h>

#include "../math/mat4.h"
#include "../math/vec2.h"
#include "../math/vec3.h"
#include "../math/vec4.h"
#include "../render/material.h"

const extern Vec3 COLLOR_PALLETE[7];
extern const float DEG2RAD;

/*
* Mesh
*/
typedef struct {
    Vec3 position;
    Vec3 normal;
    Vec2 uv;
} Vertex;

typedef struct {
    // Triangle data
    int vert_count;
    int norm_count;
    int uv_count;

    Vec3 *verts;
    Vec3 *norms;
    Vec2 *uvs;

    int ind_count;
    int *vert_indices;
    int *norm_indices;
    int *uv_indices;

    // Material data
    int material_count;
    int *material_indices;
    Material **materials;
} Mesh;

Mesh mesh_empty();

/*
* Game object
*/
typedef struct {
    enum {
        LIGHT_POINT,
        LIGHT_AMBIENT,
    } type;
    Vec3 color;
    float intensity;
    float radius;
} Light;

typedef struct {
    enum {
        ATTR_NONE,
        ATTR_LIGHT,
        ATTR_SDF,
        ATTR_MESH,
    } type;
    union {
        Light light;
        Mesh mesh;
    };
} GameObjectAttr;

typedef struct GameObject {
    Vec3 forward;
    Vec3 right;
    Vec3 up;

    Vec3 position;
    Vec4 quaternion;
    Vec3 scale;

    int num_children;
    Mat4 local_matrix;
    Mat4 world_matrix;
} GameObject;


void game_object_update_local_matrix(GameObject *);
GameObject game_object_default();

#endif
