#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <math.h>
#include <stdbool.h>

#include "../math/mat4.h"
#include "../math/vec2.h"
#include "../math/vec3.h"
#include "../math/vec4.h"
#include "material.h"

#define MAX_VERT_WEIGHTS 8

const extern Vec3 COLLOR_PALLETE[7];
extern const Real DEG2RAD;

typedef struct {
    int boneIndex;
    Real weight;
} BoneWeight;

typedef struct {
    int ind_count;
    int vert_count;
    int norm_count;
    int uv_count;

    Vec3 color; // I dont want to edit the spider code. color wont be used.
    Material *material;

    BoneWeight (*weights)[MAX_VERT_WEIGHTS];
    Vec3 *verts;
    Vec3 *norms;
    Vec2 *uvs;

    int *vert_inds;
    int *norm_inds;
    int *uv_inds;
} Mesh;

Mesh mesh_empty();

typedef struct {
    Real radius;
    Real length;
} Bone;

typedef struct {
    enum {
        LIGHT_POINT,
        LIGHT_AMBIENT,
    } type;
    Vec3 color;
    Real intensity;
} Light;

typedef struct {
    enum {
        ATTR_NONE,
        ATTR_LIGHT,
        ATTR_SDF,
        ATTR_MESH,
        ATTR_BONE,
    } type;
    union {
        Light light;
        Mesh mesh;
        Bone bone;
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

/*
 * get_model_matrix - Calculate the model matrix based on a transform
 * @transform: The transform containing the position, scale, and rotation
 *
 * Calculate the model matrix based on a transform, which includes position,
 * scale, and rotation data.
 *
 *
 * Return: The model matrix.
 */
void game_object_update_local_matrix(GameObject *);
GameObject game_object_default();

#endif
