#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tga.h"

TGAImage *createTGAImage(int width, int height) {
    TGAImage *image = malloc(sizeof(TGAImage));

    image->width = width;
    image->height = height;
    image->data = malloc(width * height * 4);
    if (image->data == NULL) {
        printf("ERROR: Failed to malloc TGAimage\n");
        return NULL;
    }
    return image;
}

void setPixel(TGAImage *image, int x, int y, vec4 color) {
    if (image == NULL || x < 0 || x >= image->width || y < 0 || y >= image->height) {
        return;
    }

    int index = (x + y * image->width) * 4;
    if (index + 3 < image->width * image->height * 4) {
        image->data[index + 0] = color.elem[1];
        image->data[index + 1] = color.elem[2];
        image->data[index + 2] = color.elem[0];
        image->data[index + 3] = color.elem[3];
    }
}

void flipImageVertically(TGAImage *image) {
    int bytes_per_line = image->width * 4;
    unsigned char *temp = malloc(bytes_per_line);
    for (int i = 0; i < image->height / 2; i++) {
        int line1 = i * bytes_per_line;
        int line2 = (image->height - 1 - i) * bytes_per_line;
        memcpy(temp, image->data + line1, bytes_per_line);
        memcpy(image->data + line1, image->data + line2, bytes_per_line);
        memcpy(image->data + line2, temp, bytes_per_line);
    }
    free(temp);
}

void writeTGAImageToFile(TGAImage *image, const char *filename) {
    FILE *file = fopen(filename, "wb+");
    if (!file) {
        perror("ERROR: Failed to write to TGA file");
        exit(1);
    }

    // Write the TGA header
    unsigned char header[18] = {0};
    header[2] = 2; // Uncompressed, true-color image
    header[12] = image->width & 0xFF;
    header[13] = (image->width >> 8) & 0xFF;
    header[14] = image->height & 0xFF;
    header[15] = (image->height >> 8) & 0xFF;
    header[16] = 32; // 32 bits per pixel
    fwrite(header, 1, sizeof(header), file);

    // Write the image data
    fwrite(image->data, 1, image->width * image->height * 4, file);
    fclose(file);
}

void setTGAImageBackground(TGAImage *image, vec4 color) {
    for (int i = 0; i < image->width; ++i) {
        for (int j = 0; j < image->height; ++j) {
            setPixel(image, i, j, color);
        }
    }
}
