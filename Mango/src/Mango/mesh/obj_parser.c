#include "obj_parser.h"

#include <stdlib.h>
#include <string.h>

Mesh load_obj(const char *fpath, const char *fname) {
    // File loading
    char absolute_path[256];
    snprintf(absolute_path, sizeof(absolute_path), "%s/%s", fpath, fname);

    FILE *file = fopen(absolute_path, "r");
    if (file == NULL) {
        printf("ERROR: failed to open file: %s\n", absolute_path);
        exit(1);
    }

    // Parse .mtl
    // TOOD: dont hardcore file name
    int num_materials = 0;
    Material **materials = parse_mtl(fpath, "Atlsas.mtl", &num_materials);
    if (materials == NULL) {
        materials = (Material**)malloc(sizeof(Material*));
        if (materials != NULL) {
            num_materials = 1;
            materials[0] = default_material();
        }
    }

    // Parsing
    char line[128];
    int alloc_amt = 1024;
    Mesh mesh = mesh_empty();

    // Triangle data
    Vec3 *verts = (Vec3*)malloc(alloc_amt * sizeof(Vec3));
    Vec3 *norms = (Vec3*)malloc(alloc_amt * sizeof(Vec3));
    Vec2 *uvs   = (Vec2*)malloc(alloc_amt * sizeof(Vec2));
    if (verts == NULL || norms == NULL || uvs == NULL) {
        return mesh;
    }

    // Index data
    int *vert_indices = (int*)malloc(alloc_amt * sizeof(int));
    int *norm_indices = (int*)malloc(alloc_amt * sizeof(int));
    int *uv_indices   = (int*)malloc(alloc_amt * sizeof(int));
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
            char *ptr_head = line + 2;

            // Parse the triangle
            while (sscanf(ptr_head, "%d/%d/%d%n", &f, &t, &n, &offset) == 3) {
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
                ptr_head += offset;
            }
        }

        // Save mesh triangle data
        mesh.verts = verts;
        mesh.norms = norms;
        mesh.uvs = uvs;

        mesh.vert_indices = vert_indices;
        mesh.norm_indices = norm_indices;
        mesh.uv_indices = uv_indices;

        // Save material data
        mesh.materials = materials;
        mesh.material_count = num_materials;
    }

    fclose(file);
    return mesh;
}

Material **parse_mtl(const char* fpath, const char *fname, int *num_materials) {
    // File management
    char absolute_path[256];
    snprintf(absolute_path, sizeof(absolute_path), "%s/%s", fpath, fname);

    FILE *file = fopen(absolute_path, "r");
    if (file == NULL) {
        printf("ERROR: failed to open .mtl file: %s\n", absolute_path);
        return NULL;
    }

    // Data
    int max_materials = 3;
    Material **materials = (Material**)malloc(max_materials * sizeof(Material*));
    if (materials == NULL) {
        fclose(file);
        return NULL;
    }
    *num_materials = 0;

    // Parsing
    char line[128];
    Material *current_mat = NULL;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "newmtl", 6) == 0) {
            // Create a new Material
            current_mat = default_material();
            sscanf(line, "newmtl %[^\n]", current_mat->name);

            // Resize array if needed
            if (*num_materials >= max_materials) {
                max_materials *= 2;
                materials = realloc(materials, max_materials * sizeof(Material*));
                if (materials == NULL) {
                    fclose(file);
                    return NULL; // Memory reallocation failed
                }
            }

            // Add the new material to the array
            materials[*num_materials] = current_mat;
            ++(*num_materials);
        } else if (strncmp(line, "Ka", 2) == 0) {
            // Ambient
            sscanf(line, "Ka %f %f %f",
                &current_mat->ambient.x,
                &current_mat->ambient.y,
                &current_mat->ambient.z
            );
        } else if (strncmp(line, "Kd", 2) == 0) {
            // Diffuse
            sscanf(line, "Kd %f %f %f",
                &current_mat->diffuse.x,
                &current_mat->diffuse.y,
                &current_mat->diffuse.z
            );
        } else if (strncmp(line, "Ks", 2) == 0) {
            // Specular
            sscanf(line, "Ks %f %f %f",
                &current_mat->specular.x,
                &current_mat->specular.y,
                &current_mat->specular.z
            );
        } else if (strncmp(line, "Ns", 2) == 0) {
            // Shininess
            sscanf(line, "Ns %f", &current_mat->shininess);
        } else if (strncmp(line, "map_Kd", 6) == 0) {
            // Diffuse texture
            char mat_name[64];
            sscanf(line, "map_Kd %s", mat_name);

            char kd_path[256];
            snprintf(kd_path, sizeof(kd_path), "%s/%s", fpath, mat_name);
            Texture *diffuse_texture = load_texture(kd_path);
            current_mat->albedo_map = diffuse_texture;
        }
    }

    fclose(file);
    return materials;
}