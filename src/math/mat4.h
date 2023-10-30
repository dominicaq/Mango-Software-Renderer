#ifndef MAX4_H
#define MAX4_H

#include <stdio.h>
#include "vec3.h"

typedef struct {
    float elem[4][4];
} Mat4;

extern const Mat4 ZERO_MATRIX;
extern const Mat4 IDENTITY;

/**
 * Multiply two 4x4 matrices.
 *
 * This function performs matrix multiplication of two 4x4 matrices 'a' and 'b'.
 *
 * @param a The first 4x4 matrix.
 * @param b The second 4x4 matrix.
 * @return The resulting 4x4 matrix.
 */
Mat4 mat4_mul(const Mat4 a, const Mat4 b);

/**
 * Create a scaling matrix based on a 3D vector.
 *
 * This function creates a 4x4 scaling matrix based on a 3D vector 'scale'.
 *
 * @param scale The 3D vector representing scaling factors in x, y, and
 * z directions.
 * @return The resulting scaling matrix.
 */
Mat4 mat4_scale(const Vec3 scale);

/**
 * Create a translation matrix based on a 3D vector.
 *
 * This function creates a 4x4 translation matrix based on a 3D vector 'v'.
 *
 * @param v The 3D vector representing the translation in x, y, and
 * z directions.
 * @return The resulting translation matrix.
 */
Mat4 mat4_translation(const Vec3 v);

/**
 * Transpose a 4x4 matrix.
 *
 * This function transposes a 4x4 matrix 'a'.
 *
 * @param a The input 4x4 matrix.
 * @return The transposed 4x4 matrix.
 */
Mat4 mat4_transpose(const Mat4 a);

/**
 * Calculate the inverse of a 4x4 matrix.
 *
 * This function calculates the inverse of a 4x4 matrix 'm'.
 *
 * @param m The input 4x4 matrix.
 * @return The inverse of the input matrix.
 */
Mat4 mat4_inverse(const Mat4 m);


/**
 * Print the components of a 4x4 matrix to the console.
 *
 * This function prints the components of a 4x4 matrix 'm' to the console.
 *
 * @param m The 4x4 matrix to print.
 */
void mat4_print(const Mat4 m);

#endif  // MAX4_H
