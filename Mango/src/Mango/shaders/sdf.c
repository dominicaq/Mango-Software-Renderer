#include "sdf.h"

// SDF Render
// -----------------------------------------------------------------------------

Mat4 sdf_model_matrix(Vec3 position) {
    Mat4 IDENTITY = {{
        {1.0f, 0.0f, 0.0f, position.x},
        {0.0f, 1.0f, 0.0f, position.y},
        {0.0f, 0.0f, 1.0f, position.z},
        {0.0f, 0.0f, 0.0f, 1.0f}}};
    return IDENTITY;
}

float scene_sdf(Vec3 sample_point) {
    return sdf_sphere(vec3_sub(sample_point, (Vec3){{0,0,0}}), 1.0f);
}

Vec3 sdf_ray(float fov, Vec2 uv) {
    Vec2 xy = uv;
    xy.x -= 0.5f;
    xy.y -= 0.5f;
    float z = 1.0 / tan((fov * DEG2RAD) / 2.0f);
    return vec3_normalize(vec2_to_vec3(xy, -z));
}

// SDF Note:
// If the result is negative, the point is inside the sphere.
// If the result is zero, the point is on the sphere's surface.
// If the result is positive, the point is outside the sphere.
void sdf_draw(Frame *frame, const Camera *camera, Vec3 sdf_ndc) {
    Vec2 inv_size = (Vec2){{1.0f / frame->width, 1.0f / frame->height}};

    Vec3 eye = camera->game_object.position;
    // Vec3 sdf_ss = ndc_to_screen(frame->width, frame->height, sdf_ndc);

    // Loop over entire frame
    for (int x = 0; x < frame->width; ++x) {
        for (int y = 0; y < frame->height; ++y) {
            float u = x * camera->aspect * inv_size.x;
            float v = y * inv_size.y;
            Vec2 uv = (Vec2){{u, v}};

            Vec3 rd = sdf_ray(camera->fov, uv);
            float dist = sdf_ray_march(eye, rd);
            if (dist >= SDF_MAX) {
                continue;
            }

            Vec3 p = vec3_add(eye, vec3_scale(rd, dist));
            Vec3 n = sdf_estimate_normal(p);
            Vec3 pixel = vec3_scale(n, 255.0f);
            pixel.x = clamp(pixel.x, 0.0, 255.0f);
            pixel.y = clamp(pixel.y, 0.0, 255.0f);
            pixel.z = clamp(pixel.z, 0.0, 255.0f);
            Vec4 pixel_color = vec3_to_vec4(pixel, 1.0f);
            frame_set_pixel(frame, x, y, pixel_color);
        }
    }
}


// SDF Helpers
// -----------------------------------------------------------------------------

Vec3 sdf_estimate_normal(Vec3 p) {
    float x_plus_epsilon = scene_sdf((Vec3){{p.x + SDF_EPSILON, p.y, p.z}});
    float x_minus_epsilon = scene_sdf((Vec3){{p.x - SDF_EPSILON, p.y, p.z}});

    float y_plus_epsilon = scene_sdf((Vec3){{p.x, p.y + SDF_EPSILON, p.z}});
    float y_minus_epsilon = scene_sdf((Vec3){{p.x, p.y - SDF_EPSILON, p.z}});

    float z_plus_epsilon = scene_sdf((Vec3){{p.x, p.y, p.z + SDF_EPSILON}});
    float z_minus_epsilon = scene_sdf((Vec3){{p.x, p.y, p.z - SDF_EPSILON}});

    float dx = x_plus_epsilon - x_minus_epsilon;
    float dy = y_plus_epsilon - y_minus_epsilon;
    float dz = z_plus_epsilon - z_minus_epsilon;

    Vec3 gradient = (Vec3){{dx, dy, dz}};

    return vec3_normalize(gradient);
}

float sdf_ray_march(Vec3 origin, Vec3 direction) {
    float dist = 0.0f;
    for (int i = 0; i < SDF_MAX_MARCH_STEPS; ++i) {
        Vec3 ray = vec3_add(origin, vec3_scale(direction, dist));
        float sdf_dist = scene_sdf(ray);

        // Inside SDF
        if (sdf_dist < SDF_EPSILON) {
            break;
        }

        dist += sdf_dist;

        if (sdf_dist > SDF_MAX) {
            break;
        }
    }
    return dist;
}

// SDF Functions
// -----------------------------------------------------------------------------

float sdf_sphere(Vec3 p, float s) { return vec3_magnitude(p) - s; }

float sdf_box(Vec3 p, Vec3 b) {
    Vec3 q = vec3_sub(vec3_abs(p), b);
    float q_len = vec3_magnitude(vec3_max(q, VEC3_ZERO));
    float q_x = fmaxf(q.x, fmaxf(q.y, q.z));
    return q_len + fminf(q_x, 0.0f);
}

float sdf_torus(Vec3 p, Vec2 t) {
    Vec2 swizzle_xz = {{p.x, p.z}};
    float swizzle_xz_len = vec2_magnitude(swizzle_xz);
    Vec2 q = {{swizzle_xz_len - t.x, p.y}};
    return vec2_magnitude(q) - t.y;
}

// Operations
// -----------------------------------------------------------------------------

float sdf_op_union(float d1, float d2) {
    return fminf(d1, d2);
}

float sdf_op_sub(float d1, float d2) {
    return fmaxf(-d1, d2);
}

float sdf_op_intersect(float d1, float d2) {
    return fmaxf(d1, d2);
}

float sdf_op_smooth_union(float d1, float d2, float k) {
    float h = clamp(0.5f + 0.5f * (d1 - d2) / k, 0.0f, 1.0f);
    return lerp(d2, d1, h) - k * h * (1.0f - h);
}

float sdf_op_smooth_sub( float d1, float d2, float k) {
    float h = clamp(0.5f - 0.5f * (d2 + d1) / k, 0.0f, 1.0f);
    return lerp(d2, -d1, h ) + k * h * (1.0f - h);
}

float sdf_op_smooth_intersect( float d1, float d2, float k) {
    float h = clamp(0.5f - 0.5f * (d2 - d1) / k, 0.0f, 1.0f);
    return lerp(d2, d1, h) + k * h * (1.0f - h);
}