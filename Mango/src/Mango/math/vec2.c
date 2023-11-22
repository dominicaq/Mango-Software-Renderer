#include "vec2.h"

#include "real.h"

Vec2 vec2_add(Vec2 a, Vec2 b) {
    a.x = real_add(a.x, b.x);
    a.y = real_add(a.y, b.y);
    return a;
}

Vec2 vec2_sub(Vec2 a, Vec2 b) {
    a.x = real_sub(a.x, b.x);
    a.y = real_sub(a.y, b.y);
    return a;
}

Vec2 vec2_scale(Vec2 a, Real s) {
    a.x = real_mul(s, a.x);
    a.y = real_mul(s, a.y);
    return a;
}

Real vec2_magnitude(Vec2 a) {
    return real_sqrt(real_add(real_mul(a.x, a.x), real_mul(a.y, a.y)));
}

Real vec2_dot(Vec2 a, Vec2 b) {
    return real_add(real_mul(a.x, b.x), real_mul(a.y, b.y));
}

Real vec2_cross(Vec2 a, Vec2 b) {
    return real_sub(real_mul(a.x, b.y), real_mul(a.y, b.x));
}

Vec3 vec2_to_vec3(Vec2 a, float z) { return (Vec3){{a.x, a.y, z}}; }

// Helper Function(s)
// -----------------------------------------------------------------------------
void vec2_swap(Vec2 *v1, Vec2 *v2) {
    Vec2 temp = *v2;
    *v2 = *v1;
    *v1 = temp;
}

void print_vec2(Vec2 v) { printf("x: %f, y: %f \n", v.x, v.y); }
