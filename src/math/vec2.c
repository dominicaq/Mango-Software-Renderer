#include <vec2.h>

vec2 vec2_add(vec2 a, vec2 b) {
    a.x = a.x + b.x;
    a.y = a.y + b.y;
    return a;
}

vec2 vec2_sub(vec2 a, vec2 b) {
    a.x = a.x - b.x;
    a.y = a.y - b.y;
    return a;
}

vec2 vec2_scale(float s, vec2 a) {
    a.x = s * a.x;
    a.y = s * a.y;
    return a;
}

float vec2_magnitude(vec2 a) {
    return sqrt(a.x * a.x + a.y * a.y);
}

float vec2_dot(vec2 a, vec2 b) {
    return a.x * b.x + a.y * b.y;
}

float vec2_cross(vec2 a, vec2 b) {
    return (a.x * b.y) - (a.y * b.x);
}

// Helper Function(s)
// -----------------------------------------------------------------------------
void vec2_swap(vec2* v1, vec2* v2) {
    vec2 temp = *v2;
    *v2 = *v1;
    *v1 = temp;
}

void print_vec2(vec2 v) {
    printf("x: %f, y: %f \n", v.x, v.y;
}