#ifndef VEC4_H
#define VEC4_H

#include <stdio.h>

#include "mat4.h"
#include "vec3.h"

typedef union {
    struct {
        float x, y, z, w;
    };
    float elem[4];
} Vec4;

// Vec4 opertaions
// -----------------------------------------------------------------------------
Vec4 mat_mul_vec4(Mat4 m, Vec4 v);

Vec4 vec3_to_vec4(Vec3 v, float w);

Vec3 vec4_homogenize(Vec4 v);

Vec3 vec4_to_vec3(Vec4 v);

Vec4 vec4_add(Vec4 a, Vec4 b);

float vec4_magnitude(Vec4 a);

Vec4 vec4_normalize(Vec4 a);

Vec4 quat_from_units(Vec3 from, Vec3 to);

Vec4 *quat_mul(Vec4 *, const Vec4 *);

Vec4 quat_from_axis(Vec3 axis, float angle);

// Helper functions
// -----------------------------------------------------------------------------
void vec4_print(Vec4 v);

#endif
