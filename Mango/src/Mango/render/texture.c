#include "texture.h"

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

Texture load_texture(const char* path) {
    Texture new_texture = (Texture)malloc(sizeof(Texture));
    if (new_texture == NULL) {
        return NULL;
    }
    new_texture->width = 0;
    new_texture->height = 0;
    new_texture->channels = 0;
    new_texture->data = NULL;

    // new_texture->data = stbi_load(
    //     path,
    //     &new_texture->width,
    //     &new_texture->height,
    //     &new_texture->channels,
    //     0
    // );

    // if (new_texture->data == NULL) {
    //     free(new_texture);
    //     return NULL;
    // }
    return new_texture;
}

Vec3 sample_texture(Vec2 uv, Texture texture) {
    // Remap UV coordinates to texture space
    int x = uv.x * (texture->width - 1);
    int y = uv.y * (texture->height - 1);

    int index = (y * texture->width + x) * texture->channels;
    unsigned char r = texture->data[index];
    unsigned char g = texture->data[index + 1];
    unsigned char b = texture->data[index + 2];

    return (Vec3){{r / 255.0f, g / 255.0f, b / 255.0f}};
}

void free_texture(Texture texture) {
    // stbi_image_free(texture->data);
    free(texture);
}

