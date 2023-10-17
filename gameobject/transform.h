#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../math/vec3.h"
#include "../math/mat4x4.h"

const float DEG2RAD = M_PI / 180.0f;

typedef struct {
    vec3 position;
    vec3 eulerAngles;
    vec3 scale;
} Transform;

// Source: https://stackoverflow.com/questions/59908345/rotation-calculation-in-glm
Mat4x4 rotate(float angle, vec3 axis) {
    float s = sinf(angle);
    float c = cosf(angle);
    float oc = 1 - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    Mat4x4 result = IDENTITY;
    result.elem[0][0] = x * x * oc + c;
    result.elem[0][1] = x * y * oc - z * s;
    result.elem[0][2] = x * z * oc + y * s;
    result.elem[1][0] = y * x * oc + z * s;
    result.elem[1][1] = y * y * oc + c;
    result.elem[1][2] = y * z * oc - x * s;
    result.elem[2][0] = z * x * oc - y * s;
    result.elem[2][1] = z * y * oc + x * s;
    result.elem[2][2] = z * z * oc + c;

    return result;
}

Mat4x4 create_model_matrix(Transform transform) {
    // Rotation axis
    vec3 x_axis = {1.0f, 0.0f, 0.0f};
    vec3 y_axis = {0.0f, 1.0f, 0.0f};
    vec3 z_axis = {0.0f, 0.0f, 1.0f};

    Mat4x4 rot_matrix = IDENTITY;
    rot_matrix = rotate(transform.eulerAngles.x * DEG2RAD, x_axis);
    rot_matrix = rotate(transform.eulerAngles.y * DEG2RAD, y_axis);
    rot_matrix = rotate(transform.eulerAngles.z * DEG2RAD, z_axis);

    Mat4x4 scale_matrix = mat_scale(IDENTITY, transform.scale);
    Mat4x4 translation = translate(IDENTITY, transform.position);

    return mat_mul(mat_mul(translation, rot_matrix), scale_matrix);
}

#endif