#include "material.h"

Material *default_material() {
    Material *new_mat = malloc(sizeof(Material));
    if (new_mat == NULL) {
        return NULL;
    }
    new_mat->albedo_map = NULL;
    new_mat->normal_map = NULL;
    new_mat->tangent_map = NULL;
    return new_mat;
}