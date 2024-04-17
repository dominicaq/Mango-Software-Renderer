#ifndef MATERIAL_H
#define MATERIAL_H

#include "../math/vec3.h"
#include "texture.h"

#include <stdlib.h>

struct Material {
    char name[64];
    Vec3 color;

    // Properties
    Vec3 ambient;
    Vec3 diffuse;
    Vec3 specular;
    float shininess;

    // Texture maps
    Texture *albedo_map;
    Texture *normal_map;
    Texture *tangent_map;
} typedef Material;

Material *default_material();

#endif