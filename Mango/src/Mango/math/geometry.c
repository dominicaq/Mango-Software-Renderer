#include "geometry.h"

// Coordinate functions
// -----------------------------------------------------------------------------
Vec3 barycentric_coords(Vec3 p, Vec3 a, Vec3 b, Vec3 c) {
    Vec3 v0 = vec3_sub(b, a);
    Vec3 v1 = vec3_sub(c, a);
    Vec3 v2 = vec3_sub(p, a);

    MangoReal d00 = vec3_dot(v0, v0);
    MangoReal d01 = vec3_dot(v0, v1);
    MangoReal d11 = vec3_dot(v1, v1);
    MangoReal d20 = vec3_dot(v2, v0);
    MangoReal d21 = vec3_dot(v2, v1);
    MangoReal invDenom = mango_real_div(
        mango_real_from_float(1.0f),
        mango_real_sub(mango_real_mul(d00, d11), mango_real_mul(d01, d01)));
    MangoReal v =
        mango_real_sub(mango_real_mul(d11, d20), mango_real_mul(d01, d21)) *
        invDenom;
    MangoReal w = mango_real_mul(
        mango_real_sub(mango_real_mul(d00, d21), mango_real_mul(d01, d20)),
        invDenom);
    MangoReal u = mango_real_sub(mango_real_sub(1.0f, v), w);
    return (Vec3){{u, v, w}};
}

Vec3 lerp_bc_coords(Vec3 bc_coords, Vec3 data[3]) {
    // Interpolate the color using barycentric coordinates
    return (Vec3){{
        data[0].x * bc_coords.x + data[1].x * bc_coords.y +
            data[2].x * bc_coords.z,
        data[0].y * bc_coords.x + data[1].y * bc_coords.y +
            data[2].y * bc_coords.z,
        data[0].z * bc_coords.x + data[1].z * bc_coords.y +
            data[2].z * bc_coords.z,

    }};
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

bool is_point_in_frustum(const Vec4 *clip_space_point) {
    float x = clip_space_point->x;
    float y = clip_space_point->y;
    float z = clip_space_point->z;
    float w = clip_space_point->w;

    return (x >= -w && x <= w) && (y >= -w && y <= w) && (z >= -w && z <= w);
}
