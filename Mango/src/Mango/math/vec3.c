#include "vec3.h"

Vec3 UNIT_X = {{1, 0, 0}};
Vec3 UNIT_Y = {{0, 1, 0}};
Vec3 UNIT_Z = {{0, 0, 1}};
Vec3 VEC3_ZERO = {{0, 0, 0}};

// Vector operations
// -----------------------------------------------------------------------------
Vec3 vec3_add(Vec3 a, Vec3 b) {
    a.x = a.x + b.x;
    a.y = a.y + b.y;
    a.z = a.z + b.z;
    return a;
}

Vec3 vec3_sub(Vec3 a, Vec3 b) {
    a.x = a.x - b.x;
    a.y = a.y - b.y;
    a.z = a.z - b.z;
    return a;
}

Vec3 vec3_lerp(Vec3 a, Vec3 b, float alpha) {
    a.x += alpha * (b.x - a.x);
    a.y += alpha * (b.y - a.y);
    a.z += alpha * (b.z - a.z);
    return a;
}

Vec3 vec3_scale(Vec3 a, float s) {
    a.x *= s;
    a.y *= s;
    a.z *= s;
    return a;
}

float vec3_magnitude(Vec3 a) { return sqrt(a.x * a.x + a.y * a.y + a.z * a.z); }

float vec3_dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

Vec3 vec3_cross(Vec3 a, Vec3 b) {
    a.x = a.y * b.z - a.z * b.y;
    a.y = a.z * b.x - a.x * b.z;
    a.z = a.x * b.y - a.y * b.x;
    return a;
}

Vec3 vec3_normalize(Vec3 a) {
    float len_sq = vec3_dot(a, a);
    // float len = vec3_magnitude(a);
    if (len_sq == 0.0f) {
        return (Vec3){{0.0f, 0.0f, 0.0f}};
    }
    float inv_len = q_rsqrt(len_sq);
    // float inv_len = 1.0f / len;

    a.x *= inv_len;
    a.y *= inv_len;
    a.z *= inv_len;
    return a;
}

Vec3 vec3_reflect(Vec3 position, Vec3 normal) {
    float angle = vec3_dot(position, normal);
    Vec3 scaled_normal = vec3_scale(normal, 2.0f * angle);
    return vec3_sub(position, scaled_normal);
}

Vec3 vec3_negate(const Vec3 v) {
    Vec3 result;
    result.x = -v.x;
    result.y = -v.y;
    result.z = -v.z;
    return result;
}

Vec3 vec3_abs(Vec3 v) {
    v.x = fabs(v.x);
    v.y = fabs(v.y);
    v.z = fabs(v.z);
    return v;
}

Vec3 vec3_max(Vec3 a, Vec3 b) {
    float len_a = vec3_magnitude(a);
    float len_b = vec3_magnitude(b);
    if (len_a > len_b) {
        return a;
    }
    return b;
}

Vec3 vec3_min(Vec3 a, Vec3 b) {
    float len_a = vec3_magnitude(a);
    float len_b = vec3_magnitude(b);
    if (len_a < len_b) {
        return a;
    }
    return b;
}

bool vec3_equal(Vec3 a, Vec3 b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

Vec3 vec3_clamp(Vec3 a, float min, float max) {
    a.x = clamp(a.x, min, max);
    a.y = clamp(a.y, min, max);
    a.z = clamp(a.z, min, max);
    return a;
}

// Helper Function(s)
// -----------------------------------------------------------------------------
void vec3_swap(Vec3 *v1, Vec3 *v2) {
    Vec3 temp = *v2;
    *v2 = *v1;
    *v1 = temp;
}

void vec3_print(Vec3 v) {
    printf("x: %.10f, y: %.10f, z: %.10f \n", v.x, v.y, v.z);
}
