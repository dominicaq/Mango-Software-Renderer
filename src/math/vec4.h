#ifndef VEC4_H
#define VEC4_H

#include <stdio.h>

#include "mat4x4.h"
#include "vec3.h"

typedef struct {
    float elem[4];
} vec4;

// Vec4 opertaions
// -----------------------------------------------------------------------------
vec4 mat_mul_vec4(const Mat4x4 m, const vec4 v);

vec4 vec3_to_vec4(const vec3 v, float w);

vec3 homogenize_vec4(const vec4 v);

vec4 color_scale(const float scale, const vec4 v);

vec4 vec4_add(const vec4 a, const vec4 b);

// Helper functions
// -----------------------------------------------------------------------------
void print_vec4(const vec4 v);

#endif