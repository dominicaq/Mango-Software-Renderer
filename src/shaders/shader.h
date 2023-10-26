#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>

#include "../math/vec4.h"
#include "../math/vec3.h"
#include "../math/mat4x4.h"
#include "../math/geometry.h"

#define MAX_LIGHTS 3

const extern bool FLAT_SHADING;
const extern bool SMOOTH_SHADING;
const extern bool PHONG_SHADING;

typedef struct {
    float u_radius;
    vec3 u_position;
    vec4 u_color;
} Light;

// Uniform buffer object
typedef struct {
    // Uniform variables
    Mat4x4 u_mvp;
    Mat4x4 u_model;
    Mat4x4 u_model_view;
    vec3 u_cam_pos;
    vec3 u_color;
    vec3 u_ambient;
    bool u_wireframe;
    float u_time;

    // Lighting
    Light lights[MAX_LIGHTS];

    // Fragment data
    vec3 frag_pos;

    // Vertex data
    vec3 v_position;
    vec3 v_normal;

    // Data from pipeline
    vec3 gl_normal;

    // Data used by pipeline
    vec4 gl_position;
    vec4 gl_frag_color;
} UBO;

void vertex_shader(UBO *ubo, vec4 a_position);

void fragment_shader(UBO *ubo, vec3 frag_coord);

// Common shader helpers
float clamp(float value, float min, float max);

#endif // SHADER_H