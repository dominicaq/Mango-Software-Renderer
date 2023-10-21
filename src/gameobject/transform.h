#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <math.h>

#include "../math/vec3.h"
#include "../math/vec4.h"
#include "../math/mat4x4.h"

typedef struct {
    vec3 position;
    vec3 eulerAngles;
    vec3 scale;
} Transform;

extern const float DEG2RAD;

/*
 * rotate - Calculate a rotation matrix based on Tait-Bryan (Euler) angles
 * @eulerAngles: The Euler angles (in radians) for rotation order Z X Y
 *
 * Calculate a 4x4 rotation matrix based on Tait-Bryan (Euler) angles.
 *
 * Source: https://en.wikipedia.org/wiki/Euler_angles
 *
 * Return: The rotation matrix.
 */
Mat4x4 rotate(vec3 eulerAngles);

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
Mat4x4 get_model_matrix(Transform transform);

#endif