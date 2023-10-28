#ifndef MAX4x4_H
#define MAX4x4_H

#include <stdio.h>
#include "vec3.h"

typedef struct {
    float elem[4][4];
} Mat4x4;

extern const Mat4x4 ZERO_MATRIX;

extern const Mat4x4 IDENTITY;

// Matrix operations
// -----------------------------------------------------------------------------
// Right to left multiplication
Mat4x4 mat_mul(const Mat4x4 a, const Mat4x4 b);

Mat4x4 mat_scale(Mat4x4 a, vec3 scale);

Mat4x4 translate(Mat4x4 a, vec3 v);

Mat4x4 transpose(Mat4x4 a);

// Helper functions
// -----------------------------------------------------------------------------
void print_matrix(const Mat4x4 m);

Mat4x4 mat_invert(Mat4x4 const *m);

#endif // MAX4x4_H
