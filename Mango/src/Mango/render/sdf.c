#include "sdf.h"

// SDF Render
// -----------------------------------------------------------------------------

float scene_sdf(Vec3 sample_point) {
    return sdf_sphere(sample_point, 5.0f);
}

// SDF Note:
// If the result is negative, the point is inside the sphere.
// If the result is zero, the point is on the sphere's surface.
// If the result is positive, the point is outside the sphere.
void sdf_draw(Frame *frame, const Camera *camera) {
    Vec2 resolution = (Vec2){{frame->width / 2, frame->height / 2}};
    Vec3 eye = camera->game_object.position;

    // Loop over entire frame
    for (int x = 0; x < frame->width; ++x) {
        for (int y = 0; y < frame->height; ++y) {
            // Pixel data
            // int buffer_index = x + y * frame->width;
            // float pixel_depth = frame->z_buffer[buffer_index];

            Vec2 frag_coord = (Vec2){{x, y}};
            Vec3 dir = sdf_ray_dir(camera->fov, resolution, frag_coord);
            float dist = sdf_ray_march(frag_coord, eye, dir);

            if (dist > SDF_END - EPSILON) {
                continue;
            }

            Vec3 p = vec3_add(eye, vec3_scale(dir, dist));
            Vec3 normal = sdf_estimate_normal(p);
            Vec3 color_scale = vec3_scale(normal, 100.0f);

            // frame->z_buffer[buffer_index] = sdf;
            Vec4 pixel_color = vec3_to_vec4(color_scale, 1.0f);
            frame_set_pixel(frame, x, y, pixel_color);
        }
    }
}

// SDF Helpers
// -----------------------------------------------------------------------------

Vec3 sdf_estimate_normal(Vec3 p) {
    float x_plus_epsilon = scene_sdf((Vec3){{p.x + EPSILON, p.y, p.z}});
    float x_minus_epsilon = scene_sdf((Vec3){{p.x - EPSILON, p.y, p.z}});
    float y_plus_epsilon = scene_sdf((Vec3){{p.x, p.y + EPSILON, p.z}});
    float y_minus_epsilon = scene_sdf((Vec3){{p.x, p.y - EPSILON, p.z}});
    float z_plus_epsilon = scene_sdf((Vec3){{p.x, p.y, p.z + EPSILON}});
    float z_minus_epsilon = scene_sdf((Vec3){{p.x, p.y, p.z - EPSILON}});


    float dx = x_plus_epsilon - x_minus_epsilon;
    float dy = y_plus_epsilon - y_minus_epsilon;
    float dz = z_plus_epsilon - z_minus_epsilon;

    Vec3 gradient = (Vec3){{dx, dy, dz}};
    return vec3_normalize(gradient);
}

Vec3 sdf_ray_dir(float fov, Vec2 size, Vec2 frag_coord) {
    Vec2 xy = vec2_sub(frag_coord, size);
    float z = size.y / tan((fov * DEG2RAD) / 2.0f);
    return vec3_normalize(vec2_to_vec3(xy, -z));
}

float sdf_ray_march(Vec2 pixel, Vec3 eye, Vec3 view_dir) {
    float depth = SDF_START;
    for (int i = 0; i < SDF_MAX_MARCH_STEPS; i++) {
        Vec3 march = vec3_add(eye, vec3_scale(view_dir, depth));
        float dist = scene_sdf(march);

        if (dist < EPSILON) {
            // Inside SDF surface
            return depth;
        }

        depth += dist;

        if (depth >= SDF_END) {
            return SDF_END;
        }
    }
    return SDF_END;
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