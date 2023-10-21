#ifndef VEC_2_H
#define VEC_2_H

#include <stdio.h>
#include <math.h>

typedef struct {
    float x;
    float y;
} vec2;

// Vector operations
// -----------------------------------------------------------------------------
vec2 vec2_add(vec2 a, vec2 b);

vec2 vec2_sub(vec2 a, vec2 b);

vec2 vec2_scale(float s, vec2 a);

float vec2_magnitude(vec2 a);

float vec2_dot(vec2 a, vec2 b);

float vec2_cross(vec2 a, vec2 b);

// Helper Functions
// -----------------------------------------------------------------------------
void vec2_swap(vec2* v1, vec2* v2);

#endif // VEC_2_H
