#ifndef MATERIAL_H
#define MATERIAL_H

#include "../math/vec3.h"

typedef struct {
    char *data;
    int data_size;
    int bpp; // Bytes per pixel
    int width;
    int height;
} Texture;

// PBR
typedef struct {
    Vec3 color;
    Texture *albedo_map;
    Texture *normal_map;
} Material;

#endif