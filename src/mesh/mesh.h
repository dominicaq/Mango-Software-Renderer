#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <stdlib.h>

#include "../math/vec2.h"  // Texture UV
#include "../math/vec3.h"  // Model data

// TODO: Shrink these values as needed

typedef struct {
    int ind_count;
    int vert_count;
    int norm_count;
    int uv_count;

    Vec3 color;

    Vec3 *verts;
    Vec3 *norms;
    Vec2 *uvs;

    int *vert_inds;
    int *norm_inds;
    int *uv_inds;
} Mesh;

Mesh *init_mesh_data();

void free_mesh_data(Mesh *mesh);

#endif
