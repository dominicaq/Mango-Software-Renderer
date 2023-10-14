#ifndef VEC_2_H
#define VEC_2_H

#include <math.h>

typedef struct {
    int x;
    int y;
} vec2i;

typedef struct {
    float x;
    float y;
} vec2;

vec2 v2add(vec2 a, vec2 b) {
    a.x = a.x + b.x;
    a.y = a.y + b.y;
    return a;
}

vec2 v2sub(vec2 a, vec2 b) {
    a.x = a.x - b.x;
    a.y = a.y - b.y;
    return a;
}

vec2 v2scale(float s, vec2 a) {
    a.x = s * a.x;
    a.y = s * a.y;
    return a;
}

float v2magnitude(vec2 a) {
    return sqrt(a.x + a.y);
}

float v2dot(vec2 a, vec2 b) {
    return a.x * b.x + a.y * b.y;
}

int v2i_cross(vec2i a, vec2i b) {
    return (a.x * b.y) - (a.y * b.x);
}

float v2_cross(vec2 a, vec2 b) {
    return (a.x * b.y) - (a.y * b.x);
}

// Helper Function(s)
// -----------------------------------------------------------------------------
void vec2_swap(vec2* v1, vec2* v2) {
    vec2 temp = *v2;
    *v2 = *v1;
    *v1 = temp;
}

#endif // VEC_2_H