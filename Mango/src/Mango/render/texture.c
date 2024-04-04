#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture *load_texture(const char* path) {
    Texture* new_texture = malloc(sizeof(Texture));
    if (new_texture == NULL) {
        printf("Failed to allocate memory for texture\n");
        return NULL;
    }

    new_texture->data = stbi_load(
        path,
        &new_texture->width,
        &new_texture->height,
        &new_texture->channels,
        STBI_rgb
    );

    if (new_texture->data == NULL) {
        printf("Failed to load texture: %s\n", path);
        free(new_texture);
        return NULL;
    }

    return new_texture;
}

#include "texture.h"

Vec3 sample_texture(Vec2 uv, Texture *texture) {
    // Remap UV coordinates to texture space
    int x = uv.x * (texture->width - 1);
    int y = uv.y * (texture->height - 1);

    // Handle texture wrap modes
    // For simplicity, assuming clamp to edge
    x = clamp(x, 0, texture->width - 1);
    y = clamp(y, 0, texture->height - 1);

    // Calculate texture index
    int index = (y * texture->width + x) * texture->channels;

    // Sample color from texture
    unsigned char r = texture->data[index];
    unsigned char g = texture->data[index + 1];
    unsigned char b = texture->data[index + 2];

    // For simplicity, assuming 24-bit RGB texture
    return (Vec3){{r / 255.0f, g / 255.0f, b / 255.0f}};
}

void free_texture(Texture *texture) {
    stbi_image_free(texture->data);
    free(texture);
}

