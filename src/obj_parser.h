#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include "math/vec3.h"  // Model data
#include "math/vec2.h"  // Texture UV
#include "render/tga.h"

// NOTE: Canno't handle quads,
// - if output looks weird convert model(s) to triangles
// TODO: Shrink these values as needed
#define MAX_VERTS 1000000
#define MAX_NORMS 1000000
#define MAX_UVS   1000000

#define MAX_INDICES 1000000

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

    TGAColor color;
} Model;

Model *init_mesh_data();

Model *load_obj_mesh(const char *filename);
#endif
