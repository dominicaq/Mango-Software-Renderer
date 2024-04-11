#include "geometry.h"

// Coordinate functions
// -----------------------------------------------------------------------------
Vec3 barycentric_coords(Vec3 p, Vec3 a, Vec3 b, Vec3 c) {
    Vec3 v0 = vec3_sub(b,a);
    Vec3 v1 = vec3_sub(c,a);
    Vec3 v2 = vec3_sub(p,a);

    float d00 = vec3_dot(v0, v0);
    float d01 = vec3_dot(v0, v1);
    float d11 = vec3_dot(v1, v1);
    float d20 = vec3_dot(v2, v0);
    float d21 = vec3_dot(v2, v1);
    float inverse_denom = 1.0f / (d00 * d11 - d01 * d01);
    float v = (d11 * d20 - d01 * d21) * inverse_denom;
    float w = (d00 * d21 - d01 * d20) * inverse_denom;
    float u = 1.0f - v - w;
    return (Vec3){{u,v,w}};
}

Vec3 lerp_bc_coords(Vec3 bc_coords, Vec3 points[3]) {
    // Interpolate the position using barycentric coordinates
    Vec3 interpolated_pos;
    interpolated_pos.x = bc_coords.x * points[0].x
         + bc_coords.y * points[1].x
         + bc_coords.z * points[2].x;
    interpolated_pos.y = bc_coords.x * points[0].y
        + bc_coords.y * points[1].y
        + bc_coords.z * points[2].y;
    interpolated_pos.z = bc_coords.x * points[0].z
        + bc_coords.y * points[1].z
        + bc_coords.z * points[2].z;

    return interpolated_pos;
}

Vec2 lerp_uv_coords(Vec3 bc_coords, float inverse_w, Vec2 uv[3]) {
    // Extract barycentric coordinates
    float alpha = bc_coords.x;
    float beta = bc_coords.y;
    float gamma = bc_coords.z;

    // Interpolate UV coordinates
    Vec2 interpolated_uv;
    interpolated_uv.x = alpha * uv[0].x + beta * uv[1].x + gamma * uv[2].x;
    interpolated_uv.y = alpha * uv[0].y + beta * uv[1].y + gamma * uv[2].y;

    return interpolated_uv;
}

// Normalized device coordinates to screen coordinates
Vec3 ndc_to_screen(int screen_width, int screen_height, Vec3 ndc_coords) {
    Vec3 screen_coords;
    screen_coords.x = (ndc_coords.x + 1.0f) * 0.5f * screen_width;
    screen_coords.y = (1.0f - ndc_coords.y) * 0.5f * screen_height;
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
