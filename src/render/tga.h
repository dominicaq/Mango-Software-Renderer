#ifndef TGA_H
#define TGA_H

// NOTE: This file is temporary, allows for writing to pixels at X,Y

typedef struct {
    unsigned char r, b, g, a;
} TGAColor;

typedef struct {
    int width, height;
    unsigned char *data;
} TGAImage;

TGAColor createTGAColor(int r, int g, int b, int a);

TGAImage *createTGAImage(int width, int height);

void setPixel(TGAImage *image, int x, int y, TGAColor color);

void flipImageVertically(TGAImage *image);

void writeTGAImageToFile(TGAImage *image, const char *filename);

void setTGAImageBackground(TGAImage *image, TGAColor color);
#endif // TGA_H
