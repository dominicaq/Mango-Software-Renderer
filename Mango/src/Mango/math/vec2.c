#include "vec2.h"

#include "real.h"

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

Vec2 vec2_scale(MangoReal s, Vec2 a) {
    a.x = s * a.x;
    a.y = s * a.y;
    return a;
}

MangoReal vec2_magnitude(Vec2 a) { return sqrt(a.x * a.x + a.y * a.y); }

MangoReal vec2_dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }

MangoReal vec2_cross(Vec2 a, Vec2 b) { return (a.x * b.y) - (a.y * b.x); }

// Helper Function(s)
// -----------------------------------------------------------------------------
void vec2_swap(Vec2 *v1, Vec2 *v2) {
    Vec2 temp = *v2;
    *v2 = *v1;
    *v1 = temp;
}

void print_vec2(Vec2 v) { printf("x: %f, y: %f \n", v.x, v.y); }
