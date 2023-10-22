#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>

#include "../math/vec4.h"
#include "../math/vec3.h"
#include "../math/mat4x4.h"

const extern bool FLAT_SHADING;
const extern bool SMOOTH_SHADING;
const extern bool PHONG_SHADING;

// Uniform buffer object
typedef struct {
    // Uniform variables
    Mat4x4 u_mvp;
    Mat4x4 u_model_view;
    vec4 u_color;
    bool u_wireframe;

    // Lighting
    vec3 u_light_position;
    vec4 u_light_color;

    // Vertex
    vec3 normal;
    vec4 gl_position;

    // Fragment
    vec3 v_position;
    vec3 v_normal;
    vec4 v_color;
} UBO;

void vertex_shader(UBO *ubo, vec4 a_position);

void fragment_shader();

#endif // SHADER_H