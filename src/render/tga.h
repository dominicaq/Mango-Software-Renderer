#ifndef TGA_H
#define TGA_H

#include "../math/vec4.h"

// NOTE: This file is temporary, allows for writing to pixels at X,Y

typedef struct {
    int width, height;
    unsigned char *data;
} TGAImage;

TGAImage *createTGAImage(int width, int height);

void setPixel(TGAImage *image, int x, int y, vec4 color);

void flipImageVertically(TGAImage *image);

void writeTGAImageToFile(TGAImage *image, const char *filename);

void setTGAImageBackground(TGAImage *image, vec4 color);
#endif // TGA_H
