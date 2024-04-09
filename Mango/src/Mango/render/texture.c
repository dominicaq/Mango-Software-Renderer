#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture* load_texture(const char* path) {
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
        0
    );

    if (new_texture->data == NULL) {
        printf("Failed to load texture: %s\n", path);
        free(new_texture);
        return NULL;
    }

    // Flip the image vertically (required)
    int width_in_bytes = new_texture->width * new_texture->channels;
    unsigned char* temp_row = (unsigned char*)malloc(width_in_bytes);
    if (temp_row == NULL) {
        printf("Failed to allocate memory for image flip\n");
        stbi_image_free(new_texture->data);
        free(new_texture);
        return NULL;
    }

    int half_height = new_texture->height / 2;
    for (int y = 0; y < half_height; ++y) {
        unsigned char* top_row = new_texture->data + y * width_in_bytes;
        unsigned char* bottom_row = new_texture->data + (new_texture->height - y - 1) * width_in_bytes;
        // Swap rows
        memcpy(temp_row, top_row, width_in_bytes);
        memcpy(top_row, bottom_row, width_in_bytes);
        memcpy(bottom_row, temp_row, width_in_bytes);
    }
    free(temp_row);

    return new_texture;
}

Vec4 sample_texture(Vec2 uv, Texture *texture) {
    // Wrap UV coordinates if necessary
    uv.x = fmod(uv.x, 1.0f);
    uv.y = fmod(uv.y, 1.0f);
    if (uv.x < 0)
        uv.x += 1.0f;
    if (uv.y < 0)
        uv.y += 1.0f;

    // Calculate texture index
    int u_index = (int)(uv.x * (texture->width - 1));
    int v_index = (int)(uv.y * (texture->height - 1));

    int index = (u_index + v_index * texture->width) * texture->channels;

    // Sample color from texture
    unsigned char r = texture->data[index];
    unsigned char g = texture->data[index + 1];
    unsigned char b = texture->data[index + 2];
    unsigned char a = (texture->channels == 4) ? texture->data[index + 3] : 255;

    return (Vec4){{r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f}};
}


void free_texture(Texture *texture) {
    stbi_image_free(texture->data);
    free(texture);
}

