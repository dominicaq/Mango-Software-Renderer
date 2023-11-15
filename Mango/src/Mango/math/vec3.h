#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdio.h>

#include "real.h"

#define EPSILON 1.0e-6

typedef union {
    struct {
        MangoReal x, y, z;
    };
    MangoReal elem[3];
} Vec3;

// Constants
// -----------------------------------------------------------------------------
extern Vec3 UNIT_X;
extern Vec3 UNIT_Y;
extern Vec3 UNIT_Z;
extern Vec3 VEC3_ZERO;

/**
 * Add two 3D vectors element-wise.
 *
 * This function adds two 3D vectors 'a' and 'b' element-wise, producing a
 * new 3D vector.
 *
 * @param a The first 3D vector.
 * @param b The second 3D vector.
 * @return The result of the addition.
 */
Vec3 vec3_add(Vec3 a, Vec3 b);

/**
 * Subtract one 3D vector from another element-wise.
 *
 * This function subtracts the 3D vector 'b' from the 3D vector 'a'
 * element-wise, producing a new 3D vector.
 *
 * @param a The first 3D vector.
 * @param b The 3D vector to subtract.
 * @return The result of the subtraction.
 */
Vec3 vec3_sub(Vec3 a, Vec3 b);

/**
 * Linearly interpolate between two 3D vectors.
 *
 * This function performs linear interpolation between two 3D vectors 'a'
 * and 'b' using the parameter 'alpha'.
 *
 * @param a The start 3D vector.
 * @param b The end 3D vector.
 * @param alpha The interpolation parameter (0.0 to 1.0).
 * @return The interpolated 3D vector.
 */
Vec3 vec3_lerp(Vec3 a, Vec3 b, MangoReal alpha);

/**
 * Scale a 3D vector by a scalar value.
 *
 * This function scales a 3D vector 'a' by a scalar value 's',
 * producing a new 3D vector.
 *
 * @param a The 3D vector to scale.
 * @param s The scalar value.
 * @return The scaled 3D vector.
 */
Vec3 vec3_scale(Vec3 a, MangoReal s);

/**
 * Calculate the magnitude (length) of a 3D vector.
 *
 * This function calculates the magnitude (length) of a 3D vector 'a'.
 *
 * @param a The 3D vector.
 * @return The magnitude of the vector.
 */
MangoReal vec3_magnitude(Vec3 a);

/**
 * Calculate the dot product of two 3D vectors.
 *
 * This function computes the dot product of 3D vectors 'a' and 'b'.
 *
 * @param a The first 3D vector.
 * @param b The second 3D vector.
 * @return The dot product.
 */
MangoReal vec3_dot(Vec3 a, Vec3 b);

/**
 * Calculate the cross product of two 3D vectors.
 *
 * This function computes the cross product of 3D vectors 'a' and 'b'.
 *
 * @param a The first 3D vector.
 * @param b The second 3D vector.
 * @return The cross product.
 */
Vec3 vec3_cross(Vec3 a, Vec3 b);

/**
 * Normalize a 3D vector, making it a unit vector.
 *
 * This function normalizes a 3D vector, making it a unit vector
 * (having a magnitude of 1).
 *
 * @param a The 3D vector to normalize.
 * @return The normalized (unit) vector.
 */
Vec3 vec3_normalize(Vec3 a);

/**
 * Reflect a 3D vector based on a given normal vector.
 *
 * This function reflects a 3D vector 'position' based on a provided
 * normal vector.
 *
 * @param position The input 3D vector to be reflected.
 * @param normal The normal vector.
 * @return The reflected 3D vector.
 */
Vec3 vec3_reflect(Vec3 position, Vec3 normal);

Vec3 vec3_negate(const Vec3 v);

/**
 * Swap the values of two 3D vectors.
 *
 * This function swaps the values of two 3D vectors 'v1' and 'v2'.
 *
 * @param v1 The first 3D vector to be swapped.
 * @param v2 The second 3D vector to be swapped.
 */
void vec3_swap(Vec3 *v1, Vec3 *v2);

/**
 * Print the components of a 3D vector to the standard output.
 *
 * This function prints the X, Y, and Z components of a 3D vector 'v' to
 * the standard output.
 *
 * @param v The 3D vector to print.
 */
void vec3_print(Vec3 v);

Vec3 vec3_abs(Vec3 v);

// Returns the vector with the largest length
Vec3 vec3_max(Vec3 a, Vec3 b);

// Returns the vector with the smallest length
Vec3 vec3_min(Vec3 a, Vec3 b);

#endif  // VEC3_H
