#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <stdlib.h>

#include "../math/vec2.h"  // Texture UV
#include "../math/vec3.h"  // Model data

// TODO: Shrink these values as needed
#define MAX_VERTS 99999
#define MAX_NORMS 99999
#define MAX_UVS   99999

#define MAX_INDICES 99999

typedef struct {
    vec3 vertices [MAX_VERTS];
    vec3 normals  [MAX_NORMS];
    vec2 uvs      [MAX_UVS]; // AKA tex coords

    int vertex_index [MAX_INDICES];
    int normal_index [MAX_INDICES];
    int uv_index     [MAX_INDICES];

    int index_count;
    int vert_count;
    int norm_count;
    int uv_count;

    vec3 color;
} Mesh;

Mesh *init_mesh_data();

void free_mesh_data(Mesh *mesh);

#endif