#include "sdf.h"

// SDF Render
// -----------------------------------------------------------------------------

float scene_sdf(Vec3 sample_point) {
    return sdf_sphere(sample_point, 5.0f);
}

Vec3 sdf_ray_dir(Vec2 uv, Vec2 size, float fov) {
    Vec2 xy;
    xy.x = uv.x - 0.5f * size.x;
    xy.y = uv.y - 0.5f * size.y;

    float z = size.y / tan((fov * DEG2RAD) / 2.0f);
    return vec3_normalize(vec2_to_vec3(xy, -z));
}

// SDF Note:
// If the result is negative, the point is inside the sphere.
// If the result is zero, the point is on the sphere's surface.
// If the result is positive, the point is outside the sphere.
void sdf_draw(Frame *frame, const Camera *camera) {
    Vec3 eye = camera->game_object.position;
    Vec2 inv_size = (Vec2){{1.0f / frame->width, 1.0f / frame->height}};

    // Loop over entire frame
    for (int x = 0; x < frame->width; ++x) {
        for (int y = 0; y < frame->height; ++y) {
            Vec2 frag_coord = (Vec2){{x, y}};
            Vec2 uv = (Vec2){{x * inv_size.x, y * inv_size.y}};
            // Vec2 uv = (Vec2){{2.0f * x - frame->width, 2.0f * y - frame->height}};

            // Ray marching
            Vec3 ray_dir = vec3_normalize(vec2_to_vec3(frag_coord, -1.0f));
            float dist = sdf_ray_march(eye, ray_dir);

            // Alpha background
            if (dist > SDF_END - EPSILON) {
                continue;
            }

            Vec3 n = sdf_estimate_normal(vec2_to_vec3(uv, -1.0f));
            // Vec3 pixel = vec2_to_vec3(n, 1.0f);
            Vec3 pixel = vec3_scale(n, 255.0f);

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
    float depth = 0.0f;
    for (int i = 0; i < SDF_MAX_MARCH_STEPS; ++i) {
        Vec3 ray = vec3_add(origin, vec3_scale(direction, depth));
        float distance_to_closest = scene_sdf(ray);

        if (distance_to_closest < SDF_EPSILON) {
            // Inside SDF surface
            return distance_to_closest;
        }

        depth += distance_to_closest;

        if (depth > SDF_END) {
            break;
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