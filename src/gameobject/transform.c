#include "transform.h"

const float DEG2RAD = M_PI / 180.0f;

Mat4x4 rotate(vec3 angles) {
    // Z = Roll
    // X = Pitch
    // Y = Yaw
    float x = angles.x * DEG2RAD;
    float y  = angles.y * DEG2RAD;
    float z = angles.z * DEG2RAD;

    float a = cosf(x);
    float b = sinf(x);

    float c = cosf(y);
    float d = sinf(y);

    float e = cosf(z);
    float f = sinf(z);

    float ae = a * e, af = a * f, be = b * e, bf = b * f;

    float m1 = c * e;
    float m2 = - c * f;
    float m3 = d;

    float m4 = af + be * d;
    float m5 = ae - bf * d;
    float m6 = - b * c;

    float m7 = bf - ae * d;
    float m8 = be + af * d;
    float m9 = a * c;

    Mat4x4 result = {
        {{m1, m2, m3, 0.0f},
         {m4, m5, m6, 0.0f},
         {m7, m8, m9, 0.0f},
         {0.0f, 0.0f, 0.0f, 1.0f}} // Last row is homogeneous
    };

    return result;
}

Mat4x4 transform_to_mat(Transform transform) {
    Mat4x4 rot = rotate(transform.euler_angles);
    Mat4x4 trans = translate(IDENTITY, transform.position);

    return mat_mul(trans, mat_mul(rot, mat_scale(IDENTITY, transform.scale)));
}
