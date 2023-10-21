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
    float len = magnitude(a);
    a.x = a.x / len;
    a.y = a.y / len;
    a.z = a.z / len;
    return a;
}

// Helper Function(s)
// -----------------------------------------------------------------------------
void vec3_swap(vec3 *v1, vec3 *v2) {
    vec3 temp = *v2;
    *v2 = *v1;
    *v1 = temp;
}

void print_vec3(vec3 v) {
    printf("x: %f, y: %f, z: %f \n", v.x, v.y, v.z);
}
