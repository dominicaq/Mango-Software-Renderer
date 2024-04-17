#ifndef UBO_H
#define UBO_H

#define MAX_LIGHTS 16

#include "../render/material.h"
#include "../game/gameobject.h"
#include "../math/geometry.h"
#include "../math/mat4.h"
#include "../math/vec3.h"
#include "../math/vec4.h"

typedef enum {
    OPT_USE_WIREFRAME = 1,
    OPT_USE_RASTERIZE = 2,
    OPT_VIEW_NORMALS = 4,
    OPT_VIEW_UV_MAP = 8,
    OPT_VIEW_DEPTH = 16,
    OPT_SDF_ENABLE = 32,
    OPT_FPS_COUNTER = 64,
    OPT_NO_LIGHTING = 128,
    OPT_NO_TEXTURE = 256,
    OPT_DEBUG_CLIP_PLANE = 512,
    OPT_VIEW_TANGENT_MAP = 1024
} Options;

typedef struct {
    Vec3 in_normal;
    Vec3 out_normal;
    Vec4 gl_position;
} VertexData;

typedef struct {
    // PBR
    Vec3 tex_albedo;
    Vec3 tex_normal;

    Vec2 uv;
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
    Material *u_mat;

    // Light data
    GameObject *light_objects[MAX_LIGHTS];
    Light *lights[MAX_LIGHTS];
    int num_lights;
    Light ambient_light;

    // Pipeline data
    VertexData v_data;
    FragmentData f_data;
    Options options;
} UBO;

#endif
