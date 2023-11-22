#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Texture loader expects an argument.\n");
        return -1;
    }

    // Load image(s)
    for (int i = 1; i < argc; ++i) {
        printf("Loading texture: %s\n", argv[i]);
        int width, height, channels;
        unsigned char* image_buffer = stbi_load(argv[i], &width, &height, &channels, 0);
        if (image_buffer == NULL) {
            printf("Failed to load image: %s\n", argv[i]);
            continue;
        }

        // Create a file to save the image data
        char out_f_name[256];
        snprintf(out_f_name, sizeof(out_f_name), "output_%d.png", i);
        FILE* out_file = fopen(out_f_name, "wb");
        if (out_file == NULL) {
            printf("Failed to open the output file for writing.\n");
            continue;
        }

        // Write the image data to the file
        fwrite(image_buffer, sizeof(unsigned char), width * height * channels, out_file);
        printf("Saved image data to %s\n", out_f_name);
        fclose(out_file);
        stbi_image_free(image_buffer); // Free the loaded image data
    }

    return 0;
}
