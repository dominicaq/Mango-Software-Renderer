#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdio.h>
#define EPSILON 1.0e-6

typedef union {
    struct {
        float x, y, z;
    };
    float elem[4];
} vec3;

// Vector operations
// -----------------------------------------------------------------------------
vec3 vec3_add(vec3 a, vec3 b);

vec3 vec3_sub(vec3 a, vec3 b);

vec3 *vec3_lerp(vec3 *a, const vec3 *b, float alpha);

vec3 scale(float s, vec3 a);

float magnitude(vec3 a);

float dot(vec3 a, vec3 b);

vec3 cross(vec3 a, vec3 b);

vec3 normalize(vec3 a);

vec3 reflect(vec3 position, vec3 normal);

// Helper Functions
// -----------------------------------------------------------------------------
void vec3_swap(vec3 *v1, vec3 *v2);
void print_vec3(vec3 v);

#endif  // VEC3_H
