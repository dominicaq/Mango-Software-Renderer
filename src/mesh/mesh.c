#include "mesh.h"

Mesh *init_mesh_data() {
    Mesh *mesh = (Mesh*)malloc(sizeof(Mesh));

    // ERROR: Malloc failed for mesh data
    if (mesh == NULL) {
        printf("ERROR: init_mesh_data() malloc failed\n");
        return NULL;
    }

    mesh->vert_count  = 0;
    mesh->norm_count  = 0;
    mesh->uv_count    = 0;
    mesh->index_count = 0;
    return mesh;
}

void free_mesh_data(Mesh *mesh) {
    free(mesh);
}