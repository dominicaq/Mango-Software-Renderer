#include "obj_parser.h"

Mesh *load_obj_mesh(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("ERROR: failed to open file: %s\n", filename);
        return NULL;
    }

    // Init data onto heap
    Mesh *mesh = init_mesh_data();
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
