#include "vec3.h"

// Vector operations
// -----------------------------------------------------------------------------
vec3 vec3_add(vec3 a, vec3 b) {
    a.x = a.x + b.x;
    a.y = a.y + b.y;
    a.z = a.z + b.z;
    return a;
}

vec3 vec3_sub(vec3 a, vec3 b) {
    a.x = a.x - b.x;
    a.y = a.y - b.y;
    a.z = a.z - b.z;
    return a;
}

vec3 scale(float s, vec3 a) {
    a.x = s * a.x;
    a.y = s * a.y;
    a.z = s * a.z;
    return a;
}

float magnitude(vec3 a) {
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

float dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 cross(vec3 a, vec3 b) {
    a.x = a.y * b.z - a.z * b.y;
    a.y = a.z * b.x - a.x * b.z;
    a.z = a.x * b.y - a.y * b.x;
    return a;
}

vec3 normalize(vec3 a) {
    // float len = q_rsqrt(dot(a, a));
    float len = magnitude(a);
    if (len == 0.0f) {
        return (vec3){0.0f,0.0f,0.0f};
    }
    float inv_len = 1.0f / len;

    a.x *= inv_len;
    a.y *= inv_len;
    a.z *= inv_len;
    return a;
}

vec3 reflect(vec3 position, vec3 normal) {
    return vec3_sub(position, scale(2.0f * dot(position, normal), normal));
}

// Helper Function(s)
// -----------------------------------------------------------------------------
void vec3_swap(vec3 *v1, vec3 *v2) {
    vec3 temp = *v2;
    *v2 = *v1;
    *v1 = temp;
}

void print_vec3(vec3 v) {
    printf("x: %.10f, y: %.10f, z: %.10f \n", v.x, v.y, v.z);
}