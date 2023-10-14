#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <stdio.h>
#include <stdlib.h>

#include "math/vec3.h"  // Model data
#include "math/vec3i.h" // Indicies
#include "math/vec2.h"  // Texture UV

// NOTE: Canno't handle quads,
// - if output looks weird convert model(s) to triangles
// TODO: Shrink these values as needed
#define MAX_VERTS 100
#define MAX_NORMS 100
#define MAX_UVS 100

#define MAX_INDICES 50

typedef struct {
    vec3 vertices[MAX_VERTS];
    vec3 normals[MAX_NORMS];
    vec2 uvs[MAX_UVS]; // AKA tex coord

    int vertex_index[MAX_INDICES];
    int normal_index[MAX_INDICES];
    int uv_index[MAX_INDICES];

    int index_count;
    int vert_count;
    int norm_count;
    int uv_count;
} MeshData;

MeshData* init_mesh_data() {
    MeshData* mesh = malloc(sizeof(MeshData));

    // ERROR: Malloc failed for mesh data
    if (mesh == NULL) {
        printf("Malloc failed: MeshData\n");
        return NULL;
    }

    mesh->vert_count  = 0;
    mesh->norm_count  = 0;
    mesh->uv_count    = 0;
    mesh->index_count = 0;
    return mesh;
}

void free_mesh_data(MeshData* mesh) {
    free(mesh);
}

MeshData* load_obj_mesh(const char *filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return NULL;
    }

    // Init data onto heap
    MeshData* mesh = init_mesh_data();
    if (mesh == NULL) {
        return NULL;
    }

    // Parse OBJ file line by line
    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            vec3 vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            mesh->vertices[mesh->vert_count] = vertex;
            ++(mesh->vert_count);

        } else if (line[0] == 'v' && line[1] == 'n') {
            vec3 normal;
            sscanf(line, "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
            mesh->normals[mesh->norm_count] = normal;
            ++(mesh->norm_count);

        } else if (line[0] == 'v' && line[1] == 't') {
            vec2 uv;
            sscanf(line, "vt %f %f\n", &uv.x, &uv.y);
            mesh->uv_count++;
            mesh->uvs[mesh->uv_count] = uv;
            ++(mesh->uv_count);

        } else if (line[0] == 'f' && line[1] == ' ') {
            int f, t, n;
            int offset;
            char* ptr = line;
            ptr += 2; // Skip 'f' and ' ' character

            // Assumes triangles, cannot do quads
            while (sscanf(ptr, "%d/%d/%d%n", &f, &t, &n, &offset) == 3) {
                mesh->vertex_index[mesh->index_count] = f - 1;
                mesh->uv_index[mesh->index_count] = t - 1;
                mesh->normal_index[mesh->index_count] = n - 1;
                ++(mesh->index_count);

                // Move the pointer along the line
                ptr += offset;
            }
        }
    }

    fclose(file);
    return mesh;
}

#endif // OBJ_PARSER_H