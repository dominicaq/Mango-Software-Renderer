#include "material.h"

Material alloc_material() {
    Material new_mat = (Material)malloc(sizeof(Material));
    if (new_mat == NULL) {
        return NULL;
    }

    new_mat->albedo_map = NULL;
    new_mat->normal_map = NULL;
    return new_mat;
}