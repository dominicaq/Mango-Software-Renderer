#ifndef MATERIAL_H
#define MATERIAL_H

#include "../math/vec3.h"
#include "texture.h"

struct Material {
    Vec3 color;
    Texture albedo_map;
    Texture normal_map;
} typedef *Material;

#endif