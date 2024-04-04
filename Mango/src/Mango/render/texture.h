#ifndef TEXTURE_H
#define TEXTURE_H

#include "../math/vec4.h"
#include "../math/vec3.h"
#include "../math/vec2.h"

struct Texture {
    int width;
    int height;
    int channels;
    unsigned char *data;
} typedef Texture;

Texture *load_texture(const char* path);
void free_texture(Texture *texture);

Vec3 sample_texture(Vec2 uv, Texture *texture);

#endif