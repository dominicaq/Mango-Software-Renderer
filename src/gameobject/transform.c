#include "transform.h"

const float DEG2RAD = M_PI / 180.0f;

Mat4x4 rotate(vec3 angles) {
    // Z = Roll
    // X = Pitch
    // Y = Yaw
    float alpha = angles.z * DEG2RAD;
    float beta  = angles.x * DEG2RAD;
    float gamma = angles.y * DEG2RAD;

    float c1 = cosf(alpha);
    float c2 = cosf(beta);
    float c3 = cosf(gamma);

    float s1 = sinf(alpha);
    float s2 = sinf(beta);
    float s3 = sinf(gamma);

    float m1 = c1 * c3 - s1 * s2 * s3;
    float m2 = -c2 * s1;
    float m3 = c1 * s3 + c3 * s1 * s2;

    float m4 = c3 * s1 + c1 * s2 * s3;
    float m5 = c1 * c2;
    float m6 = s1 * s3 - c1 * c3 * s2;

    float m7 = -c2 * s3;
    float m8 = s2;
    float m9 = c2 * c3;

    Mat4x4 result = {
        {{m1, m2, m3, 0.0f},
         {m4, m5, m6, 0.0f},
         {m7, m8, m9, 0.0f},
         {0.0f, 0.0f, 0.0f, 1.0f}} // Last row is homogeneous
    };

    return result;
}

Mat4x4 get_model_matrix(Transform transform) {
    Mat4x4 rot_matrix = rotate(transform.euler_angles);
    Mat4x4 scale_matrix = mat_scale(IDENTITY, transform.scale);
    Mat4x4 translation = translate(IDENTITY, transform.position);

    return mat_mul(mat_mul(translation, rot_matrix), scale_matrix);
}