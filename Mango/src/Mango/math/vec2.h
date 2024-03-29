#ifndef VEC_2_H
#define VEC_2_H

#include <stdio.h>
#include <math.h>

#include "shader_math.h"
#include "vec3.h"

typedef union {
    struct {
        float x, y;
    };
    float elem[2];
} Vec2;

/**
 * Add two 2D vectors.
 *
 * This function adds two 2D vectors `a` and `b`.
 *
 * @param a The first 2D vector.
 * @param b The second 2D vector.
 * @return The resulting 2D vector.
 */
Vec2 vec2_add(Vec2 a, Vec2 b);

/**
 * Subtract one 2D vector from another.
 *
 * This function subtracts 2D vector `b` from 2D vector `a`.
 *
 * @param a The first 2D vector.
 * @param b The second 2D vector to be subtracted.
 * @return The resulting 2D vector.
 */
Vec2 vec2_sub(Vec2 a, Vec2 b);

/**
 * Scale a 2D vector by a scalar.
 *
 * This function scales the 2D vector `a` by a scalar value `s`.
 *
 * @param s The scalar value.
 * @param a The 2D vector to be scaled.
 * @return The resulting scaled 2D vector.
 */
Vec2 vec2_scale(Vec2 a, float s);

/**
 * Calculate the magnitude (length) of a 2D vector.
 *
 * This function calculates the magnitude (length) of a 2D vector `a`.
 *
 * @param a The 2D vector.
 * @return The magnitude of the vector.
 */
float vec2_magnitude(Vec2 a);

/**
 * Calculate the dot product of two 2D vectors.
 *
 * This function calculates the dot product of two 2D vectors `a` and `b`.
 *
 * @param a The first 2D vector.
 * @param b The second 2D vector.
 * @return The dot product of the two vectors.
 */
float vec2_dot(Vec2 a, Vec2 b);

/**
 * Calculate the cross product of two 2D vectors.
 *
 * This function calculates the cross product of two 2D vectors `a` and `b`.
 *
 * @param a The first 2D vector.
 * @param b The second 2D vector.
 * @return The cross product value.
 */
float vec2_cross(Vec2 a, Vec2 b);

Vec2 vec2_lerp(Vec2 a, Vec2 b, float t);

/**
 * Swap the contents of two 2D vectors.
 *
 * This function swaps the contents of two 2D vectors, `v1` and `v2`.
 *
 * @param v1 The first 2D vector.
 * @param v2 The second 2D vector.
 */
void vec2_swap(Vec2 *v1, Vec2 *v2);

Vec3 vec2_to_vec3(Vec2 a, float z);

/**
 * Print the components of a 2D vector to the standard output.
 *
 * This function prints the X and Y components of a 2D vector 'v'
 * to the standard output.
 *
 * @param v The 2D vector to print.
 */
void vec2_print(Vec2 v);

#endif  // VEC_2_H
