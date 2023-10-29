#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>

#include "../math/geometry.h"
#include "../math/mat4.h"
#include "../math/vec3.h"
#include "../math/vec4.h"

const extern bool FLAT_SHADING;
const extern bool SMOOTH_SHADING;
const extern bool PHONG_SHADING;

typedef struct {
    float u_radius;
    Vec3 u_position;
    Vec4 u_color;
} Light;

// Uniform buffer object
typedef struct {
    // Uniform variables
    Mat4 u_mvp;
    Mat4 u_vp_inv;
    Mat4 u_model_view;
    Vec3 u_cam_pos;
    Vec3 u_color;
    Vec3 u_ambient;
    bool u_wireframe;
    float u_time;

    // Lighting
    Light *lights;
    int num_lights;

    // Fragment data
    Vec3 frag_pos;

    // Vertex data
    Vec3 v_position;
    Vec3 v_normal;

    // Data from pipeline
    Vec3 gl_normal;

    // Data used by pipeline
    Vec4 gl_position;
    Vec4 gl_frag_color;
} UBO;

void vertex_shader(UBO *ubo, Vec4 a_position);

void fragment_shader(UBO *ubo, Vec3 frag_coord);

// Common shader helpers
float clamp(float value, float min, float max);

#endif  // SHADER_H
