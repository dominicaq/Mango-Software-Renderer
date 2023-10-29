#ifndef MAX4_H
#define MAX4_H

#include <stdio.h>

#include "vec3.h"

typedef struct {
    float elem[4][4];
} Mat4;

extern const Mat4 ZERO_MATRIX;

extern const Mat4 IDENTITY;

// Matrix operations
// -----------------------------------------------------------------------------
// Right to left multiplication
Mat4 mat4_mul(const Mat4 a, const Mat4 b);

Mat4 mat4_scale(const vec3 scale);

Mat4 mat4_translation(const vec3 v);

Mat4 mat4_transpose(const Mat4 a);

// Helper functions
// -----------------------------------------------------------------------------
void mat4_print(const Mat4 m);

Mat4 mat4_invert(const Mat4 m);

#endif  // MAX4_H
