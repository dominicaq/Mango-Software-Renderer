#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include <math.h>

#include "vec3.h"
#include "mat4x4.h"

#include "../tga.h"

const int DEPTH = 255;
// Coordinate functions
// -----------------------------------------------------------------------------
// Efficient barycentric coordinates
// Source: https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
static inline vec3 barycentric_coords(vec3 p, vec3 a, vec3 b, vec3 c) {
    vec3 v0 = vec3_sub(b,a), v1 = vec3_sub(c,a), v2 = vec3_sub(p,a);

    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);
    float invDenom = 1.0f / (d00 * d11 - d01 * d01);
    vec3 result;
    result.x = (d11 * d20 - d01 * d21) * invDenom; // V
    result.y = (d00 * d21 - d01 * d20) * invDenom; // W
    result.z = 1.0f - result.x - result.y;         // U
    return result;
}

static inline vec3 world_to_screen(int width, int height, vec3 v) {
    // Preserve aspect ratio
    float scaleX = width * 0.5f;
    float scaleY = height * 0.5f;

    vec3 screen_coord = {
        (v.x + 1.0) * scaleX,
        (1.0 - (v.y + 1.0)) * scaleY,
        v.z
    };
    return screen_coord;
}

// Function to create a perspective projection mat4x4 matrix
static inline mat4x4 perspective(float fov, float aspect, float zNear, float zFar) {
    mat4x4 result = mat4x4_identity();
    float tanHalfFov = tanf(fov / 2.0f);

    result.data[0][0] = 1.0f / (aspect * tanHalfFov);
    result.data[1][1] = 1.0f / tanHalfFov;
    result.data[2][2] = -(zFar + zNear) / (zFar - zNear);
    result.data[2][3] = -1.0f;
    result.data[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
    result.data[3][3] = 0.0f;

    return result;
}

// Viewport matrix
static inline mat4x4 viewport(int x, int y, int w, int h) {
    mat4x4 m = mat4x4_identity();
    m.data[0][3] = x + w / 2.0f;
    m.data[1][3] = y + h / 2.0f;
    m.data[2][3] = DEPTH / 2.0f;

    m.data[0][0] = w / 2.0f;
    m.data[1][1] = h / 2.0f;
    m.data[2][2] = DEPTH / 2.0f;
    return m;
}

// Vector to matrix
static inline mat4x4 vec3_to_mat4x4(vec3 v) {
    mat4x4 m;
    m.data[0][0] = v.x;
    m.data[1][0] = v.y;
    m.data[2][0] = v.z;
    m.data[3][0] = 1.0f;
    return m;
}

// Matrix to vector
static inline vec3 mat4x4_to_vec3(const mat4x4 m) {
    vec3 result;
    result.x = m.data[0][0] / m.data[3][0];
    result.y = m.data[1][0] / m.data[3][0];
    result.z = m.data[2][0] / m.data[3][0];
    return result;
}

// Misc
// -----------------------------------------------------------------------------
void swap_ints(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

#endif