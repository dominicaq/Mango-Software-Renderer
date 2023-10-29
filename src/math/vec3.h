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
} Vec3;

// Vector operations
// -----------------------------------------------------------------------------
Vec3 vec3_add( Vec3 a,  Vec3 b);

Vec3 vec3_sub( Vec3 a,  Vec3 b);

Vec3 vec3_lerp( Vec3 a,  Vec3 b, float alpha);

Vec3 vec3_scale(float s, Vec3 a);

float vec3_magnitude( Vec3 a);

float vec3_dot( Vec3 a,  Vec3 b);

Vec3 vec3_cross( Vec3 a,  Vec3 b);

Vec3 vec3_normalize( Vec3 a);

Vec3 vec3_reflect( Vec3 position,  Vec3 normal);

// Helper Functions
// -----------------------------------------------------------------------------
void vec3_swap(Vec3 *v1, Vec3 *v2);
void vec3_print(Vec3 v);

#endif  // VEC3_H
