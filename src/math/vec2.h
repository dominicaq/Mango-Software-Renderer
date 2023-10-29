#ifndef VEC_2_H
#define VEC_2_H

#include <math.h>
#include <stdio.h>

typedef union {
    struct {
        float x;
        float y;
    };
    float elem[2];
} Vec2;

// Vector operations
// -----------------------------------------------------------------------------
Vec2 vec2_add(Vec2 a, Vec2 b);

Vec2 vec2_sub(Vec2 a, Vec2 b);

Vec2 vec2_scale(float s, Vec2 a);

float vec2_magnitude(Vec2 a);

float vec2_dot(Vec2 a, Vec2 b);

float vec2_cross(Vec2 a, Vec2 b);

// Helper Functions
// -----------------------------------------------------------------------------
void vec2_swap(Vec2 *v1, Vec2 *v2);

#endif  // VEC_2_H
