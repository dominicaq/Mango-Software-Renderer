#include "vec2.h"

Vec2 vec2_add(Vec2 a, Vec2 b) {
    a.x = a.x + b.x;
    a.y = a.y + b.y;
    return a;
}

Vec2 vec2_sub(Vec2 a, Vec2 b) {
    a.x = a.x - b.x;
    a.y = a.y - b.y;
    return a;
}

Vec2 vec2_scale(Vec2 a, float s) {
    a.x = s * a.x;
    a.y = s * a.y;
    return a;
}

float vec2_magnitude(Vec2 a) {
    return q_rsqrt(a.x * a.x + a.y * a.y);
}

float vec2_dot(Vec2 a, Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

float vec2_cross(Vec2 a, Vec2 b) {
    return a.x * b.y - a.y * b.x;
}

Vec2 vec2_lerp(Vec2 a, Vec2 b, float t) {
    return vec2_add(a, vec2_scale(vec2_sub(b, a), t));
}

Vec3 vec2_to_vec3(Vec2 a, float z) { return (Vec3){{a.x, a.y, z}}; }

// Helper Function(s)
// -----------------------------------------------------------------------------
void vec2_swap(Vec2 *v1, Vec2 *v2) {
    Vec2 temp = *v2;
    *v2 = *v1;
    *v1 = temp;
}

void vec2_print(Vec2 v) {
    printf("x: %f, y: %f \n", v.x, v.y);
}
