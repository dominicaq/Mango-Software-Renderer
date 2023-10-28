#ifndef VEC4_H
#define VEC4_H

#include <stdio.h>

#include "mat4x4.h"
#include "vec3.h"



typedef union {
  struct {
    float x, y, z, w;
  };
  float elem[4];
} vec4;

// Vec4 opertaions
// -----------------------------------------------------------------------------
vec4 mat_mul_vec4(const Mat4x4 m, const vec4 v);

vec4 vec3_to_vec4(const vec3 v, float w);

vec3 homogenize_vec4(const vec4 v);

vec3 vec4_to_vec3(const vec4 v);

vec4 vec4_add(const vec4 a, const vec4 b);

vec4 quat_from_units(const vec3 *from, const vec3 *to);

vec4 quat_mul(const vec4 *, const vec4*);

// Helper functions
// -----------------------------------------------------------------------------
void print_vec4(const vec4 v);

#endif
