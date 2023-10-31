#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <math.h>
#include <stdbool.h>

#include "../game/animation.h"
#include "../math/mat4.h"
#include "../math/vec2.h"
#include "../math/vec3.h"
#include "../math/vec4.h"

const extern Vec3 COLLOR_PALLETE[7];
extern const float DEG2RAD;

typedef struct {
    int ind_count;
    int vert_count;
    int norm_count;
    int uv_count;

    Vec3 color;

    BoneWeight *bone_weights;
    Vec3 *verts;
    Vec3 *norms;
    Vec2 *uvs;

    int *vert_inds;
    int *norm_inds;
    int *uv_inds;
} Mesh;

Mesh mesh_empty();

typedef struct {
    float radius;
    float length;
} Bone;

typedef struct {
    enum {
        NONE,
        MESH,
        BONE,
    } type;
    union {
        Mesh mesh;
        Bone bone;
    };
} GameObjectAttr;

typedef struct GameObject {
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
