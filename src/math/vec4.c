#include "vec4.h"

Vec4 mat_mul_vec4(const Mat4 m, const Vec4 v) {
    Vec4 ret;
    for (int i = 0; i < 4; ++i) {
        ret.elem[i] = 0.0f;
        for (int j = 0; j < 4; ++j) {
            ret.elem[i] += m.elem[i][j] * v.elem[j];
        }
    }
    return ret;
}

Vec4 vec3_to_vec4(const Vec3 v, float w) {
    Vec4 result;
    result.elem[0] = v.x;
    result.elem[1] = v.y;
    result.elem[2] = v.z;
    result.elem[3] = w;
    return result;
}

Vec3 vec4_homogenize(const Vec4 v) {
    float epsilon = 1e-6;
    float w = v.elem[3];
    if (fabs(w) < epsilon) {
        return (Vec3){{0.0f, 0.0f, 0.0f}};
    }

    Vec3 result = {{v.elem[0] / w, v.elem[1] / w, v.elem[2] / w}};
    return result;
}

Vec3 vec4_to_vec3(const Vec4 v) {
    return (Vec3){{v.elem[0], v.elem[1], v.elem[2]}};
}

Vec4 vec4_add(const Vec4 a, const Vec4 b) {
    Vec4 result;
    result.elem[0] = a.elem[0] + b.elem[0];
    result.elem[1] = a.elem[1] + b.elem[1];
    result.elem[2] = a.elem[2] + b.elem[2];
    result.elem[3] = a.elem[3] + b.elem[3];
    return result;
}

void print_vec4(const Vec4 v) {
    printf("x: %.10f, y: %.10f, z: %.10f, w: %.10f \n", v.elem[0], v.elem[1],
           v.elem[2], v.elem[3]);
}

Vec4 *mul(Vec4 *a, const Vec4 *b) {
    float qax = a->x, qay = a->y, qaz = a->z, qaw = a->w;
    float qbx = b->x, qby = b->y, qbz = b->z, qbw = b->w;

    a->x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
    a->y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
    a->z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
    a->w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;

    return a;
}
