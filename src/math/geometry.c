#include "geometry.h"

// Coordinate functions
// -----------------------------------------------------------------------------
Vec3 barycentric_coords(Vec3 p, Vec3 a, Vec3 b, Vec3 c) {
    Vec3 v0 = vec3_sub(b, a);
    Vec3 v1 = vec3_sub(c, a);
    Vec3 v2 = vec3_sub(p, a);

    float d00 = vec3_dot(v0, v0);
    float d01 = vec3_dot(v0, v1);
    float d11 = vec3_dot(v1, v1);
    float d20 = vec3_dot(v2, v0);
    float d21 = vec3_dot(v2, v1);
    float invDenom = 1.0f / (d00 * d11 - d01 * d01);
    float v = (d11 * d20 - d01 * d21) * invDenom;
    float w = (d00 * d21 - d01 * d20) * invDenom;
    float u = 1.0f - v - w;
    return (Vec3){{u, v, w}};
}

Vec3 lerp_bc_coords(Vec3 bc_coords, Vec3 data[3]) {
    // Interpolate the color using barycentric coordinates
    Vec3 scale_x = vec3_scale(bc_coords.x, data[0]);
    Vec3 scale_y = vec3_scale(bc_coords.y, data[1]);
    Vec3 scale_z = vec3_scale(bc_coords.z, data[2]);

    Vec3 color = vec3_add(scale_x, scale_y);
    color = vec3_add(color, scale_z);
    return color;
}

// Normalized device coordinates to screen coordinates
Vec3 ndc_to_screen(int screenWidth, int screenHeight, Vec3 ndc_coords) {
    Vec3 screen_coords;
    screen_coords.x = (ndc_coords.x + 1.0f) * 0.5f * screenWidth;
    screen_coords.y = (1.0f - ndc_coords.y) * 0.5f * screenHeight;
    // Preserve z-coordinate for depth testing
    screen_coords.z = ndc_coords.z;
    return screen_coords;
}

Vec3 world_to_screen(int width, int height, Vec3 v) {
    // Preserve aspect ratio
    float scaleX = width * 0.5f;
    float scaleY = height;
    if (width == height) {
        scaleY = height * 0.5f;
    }

    Vec3 screen_coord = {
        {(v.x + 1.0) * scaleX, (1.0 - (v.y + 1.0)) * scaleY, v.z}};
    return screen_coord;
}

// Geometry functions
// -----------------------------------------------------------------------------
bool is_backface(Vec3 ndc[3]) {
    Vec3 ab = vec3_sub(ndc[0], ndc[1]);
    Vec3 ac = vec3_sub(ndc[0], ndc[2]);
    float sign = ab.x * ac.y - ac.x * ab.y;
    return sign < 0.0f;
}
