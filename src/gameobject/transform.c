#include "transform.h"

const float DEG2RAD = M_PI / 180.0f;

Mat4 rotate(Vec3 angles) {
    // Z = Roll
    // X = Pitch
    // Y = Yaw
    float x = angles.x * DEG2RAD;
    float y = angles.y * DEG2RAD;
    float z = angles.z * DEG2RAD;

    float a = cosf(x);
    float b = sinf(x);

    float c = cosf(y);
    float d = sinf(y);

    float e = cosf(z);
    float f = sinf(z);

    float ae = a * e, af = a * f, be = b * e, bf = b * f;

    float m1 = c * e;
    float m2 = -c * f;
    float m3 = d;

    float m4 = af + be * d;
    float m5 = ae - bf * d;
    float m6 = -b * c;

    float m7 = bf - ae * d;
    float m8 = be + af * d;
    float m9 = a * c;

    Mat4 result = {
        {{m1, m2, m3, 0.0f},
         {m4, m5, m6, 0.0f},
         {m7, m8, m9, 0.0f},
         {0.0f, 0.0f, 0.0f, 1.0f}}  // Last row is homogeneous
    };

    return result;
}

Mat4 transform_to_mat(Transform tsf) {
    float x = tsf.quaternion.x, y = tsf.quaternion.y, z = tsf.quaternion.z,
          w = tsf.quaternion.w;
    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx = x * x2, xy = x * y2, xz = x * z2;
    float yy = y * y2, yz = y * z2, zz = z * z2;
    float wx = w * x2, wy = w * y2, wz = w * z2;

    float sx = tsf.scale.x, sy = tsf.scale.y, sz = tsf.scale.z;

    Mat4 res;
    res.elem[0][0] = (1 - (yy + zz)) * sx;
    res.elem[1][0] = (xy + wz) * sx;
    res.elem[2][0] = (xz - wy) * sx;
    res.elem[3][0] = 0;

    res.elem[0][1] = (xy - wz) * sy;
    res.elem[1][1] = (1 - (xx + zz)) * sy;
    res.elem[2][1] = (yz + wx) * sy;
    res.elem[3][1] = 0;

    res.elem[0][2] = (xz + wy) * sz;
    res.elem[1][2] = (yz - wx) * sz;
    res.elem[2][2] = (1 - (xx + yy)) * sz;
    res.elem[3][2] = 0;

    res.elem[0][3] = tsf.position.x;
    res.elem[1][3] = tsf.position.y;
    res.elem[2][3] = tsf.position.z;
    res.elem[3][3] = 1;
    return res;
}

Transform transform_default() {
    Transform tsf = {.position = {{0.0f, 0.0f, 0.0f}},
                     .quaternion = {{0.0f, 0.0f, 0.0f, 1.0f}},
                     .scale = {{1.0f, 1.0f, 1.0f}}};
    return tsf;
}
