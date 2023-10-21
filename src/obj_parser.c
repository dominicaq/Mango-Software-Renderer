#include <stdio.h>
#include <stdlib.h>

#include "obj_parser.h"  // Model data

Model *init_mesh_data() {
    Model *mesh = (Model *)malloc(sizeof(Model));

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

void free_mesh_data(Model *mesh) {
    free(mesh);
}

Model *load_obj_mesh(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("ERROR: failed to open file: %s\n", filename);
        return NULL;
    }

    // Init data onto heap
    Model *mesh = init_mesh_data();
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
            mesh->uvs[mesh->uv_count] = uv;
            ++(mesh->uv_count);

        } else if (line[0] == 'f' && line[1] == ' ') {
            int f, t, n;
            int offset;
            char *ptr = line;
            ptr += 2; // Skip initial 'f' and ' ' characters

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
