#ifndef UBO_H
#define UBO_H

#define MAX_LIGHTS 16

#include "../game/gameobject.h"
#include "../math/geometry.h"
#include "../math/mat4.h"
#include "../math/vec3.h"
#include "../math/vec4.h"

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

    GameObject *light_objects[MAX_LIGHTS];
    Light *lights[MAX_LIGHTS];
    int num_lights;

    // Pipeline data
    VertexData v_data;
    FragmentData f_data;
    DebugOptions debug;
} UBO;

#endif