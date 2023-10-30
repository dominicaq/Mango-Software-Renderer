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
    float radius;
    Vec3 position;
    Vec4 color;
} Light;

typedef struct {
    bool wireframe;
    bool rasterize;
} DebugOptions;

typedef struct {
    Vec3 in_normal;
    Vec3 out_normal;
    Vec4 gl_position;
} VertexData;

typedef struct {
    Vec3 frag_pos;
    Vec3 gl_normal;
    Vec4 gl_frag_color;
} FragmentData;

// Uniform buffer object
typedef struct {
    // Uniform variables
    Mat4 u_mvp;
    Mat4 u_vp_inv;
    Mat4 u_model_view;
    Vec3 u_cam_pos;
    Vec3 u_color;
    Vec3 u_ambient;
    float u_time;

    // Lighting
    Light *lights;
    int num_lights;

    // Pipeline data
    VertexData v_data;
    FragmentData f_data;
    DebugOptions debug;
} UBO;

void vertex_shader(UBO *ubo, Vec4 a_position);

void fragment_shader(UBO *ubo, Vec3 frag_coord);

// Common shader helpers
float clamp(float value, float min, float max);

#endif  // SHADER_H
