#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <math.h>

#include "vec3.h"
#include "mat4x4.h"
#include "../tga.h"

const int DEPTH = 255;
// Coordinate functions
// -----------------------------------------------------------------------------
// Efficient barycentric coordinates
// Source: https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
vec3 barycentric_coords(vec3 p, vec3 a, vec3 b, vec3 c) {
    vec3 v0 = vec3_sub(b,a);
    vec3 v1 = vec3_sub(c,a);
    vec3 v2 = vec3_sub(p,a);

    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);
    float invDenom = 1.0f / (d00 * d11 - d01 * d01);
    float v = (d11 * d20 - d01 * d21) * invDenom;
    float w = (d00 * d21 - d01 * d20) * invDenom;
    float u = 1.0f - v - w;
    return (vec3){u,v,w};
}

// Normalized device coordinates to screen coordinates
vec3 ndc_to_screen(int screenWidth, int screenHeight, vec3 ndc_coords) {
    vec3 screen_coords;
    screen_coords.x = (ndc_coords.x + 1.0f) / 2.0f * screenWidth;
    screen_coords.y = (-ndc_coords.y + 1.0f) / 2.0f * screenHeight;
    screen_coords.z = ndc_coords.z; // z-coordinate can be used for depth testing
    return screen_coords;
}

vec3 world_to_screen(int width, int height, vec3 v) {
    // Preserve aspect ratio
    float scaleX = width * 0.5f;
    float scaleY = height;
    if (width == height) {
        scaleY = height * 0.5f;
    }

    vec3 screen_coord = {
        (v.x + 1.0) * scaleX,
        (1.0 - (v.y + 1.0)) * scaleY,
        v.z
    };
    return screen_coord;
}

// Misc
// -----------------------------------------------------------------------------
void swap_ints(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

#endif