#include "geometry.h"

// Coordinate functions
// -----------------------------------------------------------------------------
vec3 barycentric_coords(vec3 p, vec3 a, vec3 b, vec3 c) {
    vec3 v0 = vec3_sub(b, a);
    vec3 v1 = vec3_sub(c, a);
    vec3 v2 = vec3_sub(p, a);

    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);
    float invDenom = 1.0f / (d00 * d11 - d01 * d01);
    float v = (d11 * d20 - d01 * d21) * invDenom;
    float w = (d00 * d21 - d01 * d20) * invDenom;
    float u = 1.0f - v - w;
    return (vec3){{u, v, w}};
}

vec3 lerp_barycentric_coords(vec3 bc_coords, vec3 normals[3]) {
    // Interpolate the color using barycentric coordinates
    vec3 scale_x = scale(bc_coords.x, normals[0]);
    vec3 scale_y = scale(bc_coords.y, normals[1]);
    vec3 scale_z = scale(bc_coords.z, normals[2]);

    vec3 color = vec3_add(scale_x, scale_y);
    color = vec3_add(color, scale_z);
    return color;
}

// Normalized device coordinates to screen coordinates
vec3 ndc_to_screen(int screenWidth, int screenHeight, vec3 ndc_coords) {
    vec3 screen_coords;
    screen_coords.x = (ndc_coords.x + 1.0f) * 0.5f * screenWidth;
    screen_coords.y = (1.0f - ndc_coords.y) * 0.5f * screenHeight;
    // Preserve z-coordinate for depth testing
    screen_coords.z = ndc_coords.z;
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
        {(v.x + 1.0) * scaleX, (1.0 - (v.y + 1.0)) * scaleY, v.z}};
    return screen_coord;
}

// Geometry functions
// -----------------------------------------------------------------------------
bool is_backface(vec3 ndc[3]) {
    vec3 ab = vec3_sub(ndc[0], ndc[1]);
    vec3 ac = vec3_sub(ndc[0], ndc[2]);
    float sign = ab.x * ac.y - ac.x * ab.y;
    return sign < 0.0f;
}
