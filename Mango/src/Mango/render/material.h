#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdlib.h>

#include "../math/vec3.h"
#include "texture.h"

struct Material {
    Texture albedo_map;
    Texture normal_map;
} typedef *Material;

Material alloc_material();

#endif