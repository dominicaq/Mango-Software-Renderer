#include "obj_parser.h"

#include <stdlib.h>
#include <string.h>

Mesh load_obj(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("ERROR: failed to open file: %s\n", filename);
        exit(1);
    }

    // Parsing data
    char line[128];
    int alloc_amt = 1024;
    Mesh mesh = mesh_empty();

    // Triangle data
    Vec3 *verts = (Vec3 *)malloc(alloc_amt * sizeof(Vec3));
    Vec3 *norms = (Vec3 *)malloc(alloc_amt * sizeof(Vec3));
    Vec2 *uvs   = (Vec2 *)malloc(alloc_amt * sizeof(Vec2));
    if (verts == NULL || norms == NULL || uvs == NULL) {
        return mesh;
    }

    // Index data
    int *vert_indices = (int *)malloc(alloc_amt * sizeof(int));
    int *norm_indices = (int *)malloc(alloc_amt * sizeof(int));
    int *uv_indices   = (int *)malloc(alloc_amt * sizeof(int));
    if (vert_indices == NULL || norm_indices == NULL || uv_indices == NULL) {
        return mesh;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            if (mesh.vert_count && mesh.vert_count % alloc_amt == 0) {
                size_t new_size = (mesh.vert_count + alloc_amt) * sizeof(Vec3);
                verts = realloc(verts, new_size);
            }

            Vec3 vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            verts[mesh.vert_count] = vertex;
            ++(mesh.vert_count);

        } else if (strncmp(line, "vn ", 3) == 0) {
            if (mesh.norm_count && mesh.norm_count % alloc_amt == 0) {
                size_t new_size = (mesh.norm_count + alloc_amt) * sizeof(Vec3);
                norms = realloc(norms, new_size);
            }

            Vec3 normal;
            sscanf(line, "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
            norms[mesh.norm_count] = normal;
            ++(mesh.norm_count);

        } else if (strncmp(line, "vt ", 3) == 0) {
            if (mesh.uv_count && mesh.uv_count % alloc_amt == 0) {
                size_t new_size = (mesh.uv_count + alloc_amt) * sizeof(Vec2);
                uvs = realloc(uvs, new_size);
            }

            Vec2 uv;
            sscanf(line, "vt %f %f\n", &uv.x, &uv.y);
            uvs[mesh.uv_count] = uv;
            ++(mesh.uv_count);

        } else if (strncmp(line, "f ", 2) == 0) {
            int f, t, n;
            int offset;
            // Skip initial 'f' and ' ' characters
            char *head_ptr = line + 2;

            // Parse the triangle
            while (sscanf(head_ptr, "%d/%d/%d%n", &f, &t, &n, &offset) == 3) {
                if (mesh.ind_count && mesh.ind_count % alloc_amt == 0) {
                    size_t new_size = (mesh.ind_count + alloc_amt) * sizeof(int);
                    vert_indices = realloc(vert_indices, new_size);
                    uv_indices   = realloc(uv_indices,   new_size);
                    norm_indices = realloc(norm_indices, new_size);
                }

                vert_indices[mesh.ind_count] = f - 1;
                uv_indices[mesh.ind_count]   = t - 1;
                norm_indices[mesh.ind_count] = n - 1;
                ++(mesh.ind_count);

                // Move the pointer along the line
                head_ptr += offset;
            }
        }

        // Save mesh triangle data
        mesh.verts = verts;
        mesh.norms = norms;
        mesh.uvs = uvs;

        mesh.vert_indices = vert_indices;
        mesh.norm_indices = norm_indices;
        mesh.uv_indices = uv_indices;
    }

    fclose(file);
    return mesh;
}

Material *parse_mtl(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("ERROR: failed to open file: %s\n", filename);
        return NULL;
    }

    // Create a default material
    Material *new_mat = default_material();

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Ka", 2) == 0) {
            sscanf(line, "Ka %f %f %f",
                &new_mat->ambient.x,
                &new_mat->ambient.y,
                &new_mat->ambient.z
            );
        } else if (strncmp(line, "Kd", 2) == 0) {
            sscanf(line, "Kd %f %f %f",
                &new_mat->diffuse.x,
                &new_mat->diffuse.y,
                &new_mat->diffuse.z
            );
        } else if (strncmp(line, "Ks", 2) == 0) {
            sscanf(line, "Ks %f %f %f",
                &new_mat->specular.x,
                &new_mat->specular.y,
                &new_mat->specular.z)
            ;
        } else if (strncmp(line, "Ns", 2) == 0) {
            sscanf(line, "Ns %f", &new_mat->shininess);
        }
    }

    fclose(file);
    return new_mat;
}