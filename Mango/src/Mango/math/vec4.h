#ifndef VEC4_H
#define VEC4_H

#include "../system/system.h"
#include "mat4.h"
#include "vec3.h"

typedef union {
    struct {
        Real x, y, z, w;
    };
    Real elem[4];
} Vec4;

/**
 * Multiply a 4x4 matrix by a 4D vector.
 *
 * This function multiplies a 4x4 matrix `m` by a 4D vector `v` to produce
 * a new 4D vector.
 *
 * @param m The 4x4 matrix.
 * @param v The 4D vector.
 * @return The result of the matrix-vector multiplication.
 */
Vec4 mat_mul_vec4(Mat4 m, Vec4 v);

/**
 * Convert a 3D vector to a 4D vector with a specified 'w' component.
 *
 * This function converts a 3D vector `v` into a 4D vector by specifying
 * the 'w' component.
 *
 * @param v The 3D vector.
 * @param w The 'w' component of the resulting 4D vector.
 * @return The 4D vector.
 */
Vec4 vec3_to_vec4(Vec3 v, Real w);

/**
 * Homogenize a 4D vector to a 3D vector by dividing by its 'w' component.
 *
 * This function converts a 4D vector into a 3D vector by homogenizing it.
 *
 * @param v The 4D vector to homogenize.
 * @return The resulting 3D vector.
 */
Vec3 vec4_homogenize(Vec4 v);

/**
 * Convert a 4D vector to a 3D vector by discarding the 'w' component.
 *
 * This function converts a 4D vector into a 3D vector by removing the 'w'
 * component.
 *
 * @param v The 4D vector to convert.
 * @return The resulting 3D vector.
 */
Vec3 vec4_to_vec3(Vec4 v);

/**
 * Add two 4D vectors element-wise.
 *
 * This function adds two 4D vectors `a` and `b` element-wise, producing a new
 * 4D vector.
 *
 * @param a The first 4D vector.
 * @param b The second 4D vector.
 * @return The result of the addition.
 */
Vec4 vec4_add(Vec4 a, Vec4 b);
Vec4 vec4_sub(Vec4 a, Vec4 b);

/**
 * Calculate the magnitude (length) of a 4D vector.
 *
 * This function calculates the magnitude (length) of a 4D vector `a`.
 *
 * @param a The 4D vector.
 * @return The magnitude of the vector.
 */
Real vec4_magnitude(Vec4 a);

/**
 * Normalize a 4D vector, making it a unit vector.
 *
 * This function normalizes a 4D vector, making it a unit vector (having a
 * magnitude of 1).
 *
 * @param a The 4D vector to normalize.
 * @return The normalized (unit) vector.
 */
Vec4 quat_normalize(Vec4 a);

/**
 * Create a quaternion from two 3D vectors representing rotation.
 *
 * This function creates a quaternion that represents the rotation from the
 * 'from' vector to the 'to' vector.
 *
 * @param from The starting 3D vector.
 * @param to The target 3D vector.
 * @return The quaternion representing the rotation.
 */
Vec4 quat_from_units(Vec3 from, Vec3 to);

/**
 * Multiply two quaternions.
 *
 * This function multiplies two quaternions, producing a new quaternion.
 *
 * @param q1 The first quaternion.
 * @param q2 The second quaternion.
 * @return The result of quaternion multiplication.
 * @source
 * http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/code/index.htm
 */
Vec4 quat_mul(const Vec4 q1, const Vec4 q2);

/**
 * Create a quaternion from an axis and an angle.
 *
 * This function creates a quaternion that represents a rotation around the
 * specified axis by the given angle.
 *
 * @param axis The rotation axis.
 * @param angle The rotation angle (in radians).
 * @return The quaternion representing the rotation.
 * @source
 * http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm
 */
Vec4 quat_from_axis(Vec3 axis, Real angle);

/**
 * Create a quaternion from an euler.
 *
 * This function creates a quaternion from euler angle
 *
 * @param euler Euler angle in XYZ order.
 * @return The quaternion representing the rotation.
 * @source
 * http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm
 * http://www.mathworks.com/matlabcentral/fileexchange/20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/content/SpinCalc.m
 *
 */
Vec4 quat_from_euler(Vec3 euler);

Vec4 quat_inv(Vec4);
// Helper functions
// -----------------------------------------------------------------------------

/**
 * Print the components of a 4D vector to the standard output.
 *
 * This function prints the components of a 4D vector to the standard output.
 *
 * @param v The 4D vector to print.
 */
void vec4_print(Vec4 v);

#endif
