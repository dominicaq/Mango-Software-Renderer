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
    int alloc_amt = 1024;
    Vec3 *verts = (Vec3 *)malloc(alloc_amt * sizeof(Vec3));
    Vec3 *norms = (Vec3 *)malloc(alloc_amt * sizeof(Vec3));
    Vec2 *uvs = (Vec2 *)malloc(alloc_amt * sizeof(Vec2));
    int *vert_inds = (int *)malloc(alloc_amt * sizeof(int));
    int *norm_inds = (int *)malloc(alloc_amt * sizeof(int));
    int *uv_inds = (int *)malloc(alloc_amt * sizeof(int));
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            Vec3 vertex;
            if (mesh->vert_count && mesh->vert_count % alloc_amt == 0) {
                verts = realloc(verts,
                                (mesh->vert_count + alloc_amt) * sizeof(Vec3));
            }
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            verts[mesh->vert_count] = vertex;
            ++(mesh->vert_count);

        } else if (line[0] == 'v' && line[1] == 'n') {
            Vec3 normal;
            if (mesh->vert_count && mesh->vert_count % alloc_amt == 0) {
                verts = realloc(verts,
                                (mesh->vert_count + alloc_amt) * sizeof(Vec3));
            }
            sscanf(line, "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
            norms[mesh->norm_count] = normal;
            ++(mesh->norm_count);

        } else if (line[0] == 'v' && line[1] == 't') {
            Vec2 uv;
            if (mesh->uv_count && mesh->uv_count % alloc_amt == 0) {
                uvs = realloc(uvs, (mesh->uv_count + alloc_amt) * sizeof(Vec2));
            }
            sscanf(line, "vt %f %f\n", &uv.x, &uv.y);
            uvs[mesh->uv_count] = uv;
            ++(mesh->uv_count);

        } else if (line[0] == 'f' && line[1] == ' ') {
            int f, t, n;
            int offset;
            char *ptr = line;
            ptr += 2;  // Skip initial 'f' and ' ' characters

            // Assumes triangles, cannot do quads
            while (sscanf(ptr, "%d/%d/%d%n", &f, &t, &n, &offset) == 3) {
                if (mesh->uv_count && mesh->ind_count % alloc_amt == 0) {
                    vert_inds = realloc(
                        vert_inds, (mesh->ind_count + alloc_amt) * sizeof(int));
                    uv_inds = realloc(
                        uv_inds, (mesh->ind_count + alloc_amt) * sizeof(int));
                    norm_inds = realloc(
                        norm_inds, (mesh->ind_count + alloc_amt) * sizeof(int));
                }
                vert_inds[mesh->ind_count] = f - 1;
                uv_inds[mesh->ind_count] = t - 1;
                norm_inds[mesh->ind_count] = n - 1;
                ++(mesh->ind_count);

                // Move the pointer along the line
                ptr += offset;
            }
        }
        mesh->verts = verts;
        mesh->norms = norms;
        mesh->uvs = uvs;
        mesh->vert_inds = vert_inds;
        mesh->norm_inds = norm_inds;
        mesh->uv_inds = uv_inds;
    }

    fclose(file);
    return mesh;
}
