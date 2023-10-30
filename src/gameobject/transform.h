#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <math.h>

#include "../math/mat4.h"
#include "../math/vec3.h"
#include "../math/vec4.h"

extern const float DEG2RAD;

typedef struct {
    Vec3 position;
    Vec4 quaternion;
    Vec3 scale;
    Vec3 up, forward, right;
    Mat4 model_matirx;
} Transform;


/*
 * rotate - Calculate a rotation matrix based on Tait-Bryan (Euler) angles
 * @eulerAngles: The Euler angles (in degrees) for rotation order Z X Y
 *
 * Calculate a 4x4 rotation matrix based on Tait-Bryan (Euler) angles.
 *
 * Source: https://en.wikipedia.org/wiki/Euler_angles
 *
 * Return: The rotation matrix.
 */
Mat4 rotate(Vec3 angles);

/**
 * get the forward vector from a transformation matrix.
 *
 * @param transform_matrix The 4x4 transformation matrix.
 * @return The forward vector as a normalized Vec3.
 */
Vec3 get_forward_vector(const Mat4* transform_matrix);

/**
 * get the right vector from a transformation matrix.
 *
 * @param transform_matrix The 4x4 transformation matrix.
 * @return The right vector as a normalized Vec3.
 */
Vec3 get_right_vector(const Mat4* transform_matrix);

/**
 * get the up vector from a transformation matrix.
 *
 * @param transform_matrix The 4x4 transformation matrix.
 * @return The up vector as a normalized Vec3.
 */
Vec3 get_up_vector(const Mat4* transform_matrix);

/*
 * get_model_matrix - Calculate the model matrix based on a transform
 * @transform: The transform containing the position, scale, and rotation
 *
 * Calculate the model matrix based on a transform, which includes position,
 * scale, and rotation data.
 *
 *
 * Return: The model matrix.
 */
Mat4 transform_to_mat(Transform transform);

Transform update_transform(const Transform* transform);

Transform transform_default();

#endif
