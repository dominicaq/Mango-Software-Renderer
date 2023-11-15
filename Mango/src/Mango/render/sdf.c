#include "sdf.h"

float sdf_sphere(Vec3 p, float s) { return vec3_magnitude(p) - s; }

float sdf_box(Vec3 p, Vec3 b) {
    Vec3 q = vec3_sub(vec3_abs(p), b);
    float q_len = vec3_magnitude(vec3_max(q, VEC3_ZERO));
    float q_x = fmaxf(q.x, fmaxf(q.y, q.z));
    return q_len + fminf(q_x, 0.0f);
}

float sdf_torus(Vec3 p, Vec2 t) {
    Vec2 swizzle_xz = {{p.x, p.z}};
    float swizzle_xz_len = vec2_magnitude(swizzle_xz);
    Vec2 q = {{swizzle_xz_len - t.x, p.y}};
    return vec2_magnitude(q) - t.y;
}

// Operations
// -----------------------------------------------------------------------------

float sdf_op_union(float d1, float d2) {
    return fminf(d1, d2);
}

float sdf_op_sub(float d1, float d2) {
    return fmaxf(-d1, d2);
}

float sdf_op_intersect(float d1, float d2) {
    return fmaxf(d1, d2);
}

float sdf_op_smooth_union(float d1, float d2, float k) {
    float h = clamp(0.5f + 0.5f * (d1 - d2) / k, 0.0f, 1.0f);
    return lerp(d2, d1, h) - k * h * (1.0f - h);
}

float sdf_op_smooth_sub( float d1, float d2, float k) {
    float h = clamp(0.5f - 0.5f * (d2 + d1) / k, 0.0f, 1.0f);
    return lerp(d2, -d1, h ) + k * h * (1.0f - h);
}

float sdf_op_smooth_intersect( float d1, float d2, float k) {
    float h = clamp(0.5f - 0.5f * (d2 - d1) / k, 0.0f, 1.0f);
    return lerp(d2, d1, h) + k * h * (1.0f - h);
}