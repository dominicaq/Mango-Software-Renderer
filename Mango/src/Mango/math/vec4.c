#include "vec4.h"

// Vec4 opertaions
// -----------------------------------------------------------------------------
Vec4 mat_mul_vec4(const Mat4 m, const Vec4 v) {
    Vec4 ret = (Vec4){{0.0f, 0.0f, 0.0f, 0.0f}};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ret.elem[i] =
                real_add(ret.elem[i], real_mul(m.elem[i][j], v.elem[j]));
        }
    }
    return ret;
}

Vec4 vec3_to_vec4(const Vec3 v, Real w) {
    Vec4 result;
    result.elem[0] = v.x;
    result.elem[1] = v.y;
    result.elem[2] = v.z;
    result.elem[3] = w;
    return result;
}

Vec3 vec4_homogenize(const Vec4 v) {
    Real epsilon = 1e-6;
    Real w = v.elem[3];
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

Real vec4_magnitude(Vec4 a) {
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

Vec4 vec4_normalize(Vec4 a) {
    Real len = vec4_magnitude(a);
    if (len == 0.0f) {
        return (Vec4){{0.0f, 0.0f, 0.0f}};
    }
    Real inv_len = 1.0f / len;

    a.x *= inv_len;
    a.y *= inv_len;
    a.z *= inv_len;
    return a;
}

// Quaternions
// -----------------------------------------------------------------------------
Vec4 quat_from_axis(Vec3 axis, Real angle) {
    // assumes axis is normalized

    Real halfAngle = angle / 2, s = sinf(halfAngle);

    Vec4 quat = {{
        axis.x * s,
        axis.y * s,
        axis.z * s,
        cosf(halfAngle),
    }};

    return quat;
}

Vec4 quat_from_units(Vec3 vFrom, Vec3 vTo) {
    // assumes direction vectors vFrom and vTo are normalized

    Real r = vec3_dot(vFrom, vTo) + 1;
    Vec4 quat;

    if (r < EPSILON) {
        // vFrom and vTo point in opposite directions

        if (fabsf(vFrom.x) > fabsf(vFrom.z)) {
            quat = (Vec4){{-vFrom.y, vFrom.x, 0, 0}};

        } else {
            quat = (Vec4){{0, -vFrom.z, vFrom.y, 0}};
        }

    } else {
        // crossVectors( vFrom, vTo ); // inlined to avoid cyclic dependency on
        // Vector3

        quat = (Vec4){{vFrom.y * vTo.z - vFrom.z * vTo.y,
                       vFrom.z * vTo.x - vFrom.x * vTo.z,
                       vFrom.x * vTo.y - vFrom.y * vTo.x, r

        }};
    }

    return vec4_normalize(quat);
}

Vec4 *quat_mul(Vec4 *a, const Vec4 *b) {
    Real qax = a->x, qay = a->y, qaz = a->z, qaw = a->w;
    Real qbx = b->x, qby = b->y, qbz = b->z, qbw = b->w;

    a->x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
    a->y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
    a->z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
    a->w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;

    return a;
}

// Helper functions
// -----------------------------------------------------------------------------
void vec4_print(const Vec4 v) {
    printf("x: %.10f, y: %.10f, z: %.10f, w: %.10f \n", v.elem[0], v.elem[1],
           v.elem[2], v.elem[3]);
}

Vec4 quat_from_euler(Vec3 euler) {
    Real x = euler.x, y = euler.y, z = euler.z;

    // http://www.mathworks.com/matlabcentral/fileexchange/
    // 	20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/
    //	content/SpinCalc.m

    Real c1 = real_cos(x / 2);
    Real c2 = real_cos(y / 2);
    Real c3 = real_cos(z / 2);

    Real s1 = real_sin(x / 2);
    Real s2 = real_sin(y / 2);
    Real s3 = real_sin(z / 2);

    // always XYZ order
    Vec4 quat;
    quat.x = real_add(real_mul(real_mul(s1, c2), c3),
                      real_mul(real_mul(c1, s2), s3));
    quat.y = c1 * s2 * c3 - s1 * c2 * s3;
    quat.z = c1 * c2 * s3 + s1 * s2 * c3;
    quat.w = c1 * c2 * c3 - s1 * s2 * s3;

    return quat;
}
