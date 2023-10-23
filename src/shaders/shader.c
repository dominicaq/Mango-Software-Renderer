#include "shader.h"

const bool FLAT_SHADING = false;
const bool SMOOTH_SHADING = true;
const bool PHONG_SHADING = false;

void vertex_shader(UBO *ubo, vec4 a_position) {
    // If you you ever want non-uniform scaling, use this:
    // vec3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    Mat4x4 mv_no_trans = ubo->u_model_view;
    mv_no_trans.elem[3][0] = 0.0f;
    mv_no_trans.elem[3][1] = 0.0f;
    mv_no_trans.elem[3][2] = 0.0f;
    mv_no_trans.elem[3][3] = 1.0f;
    vec4 mv_normal = mat_mul_vec4(
        mv_no_trans,
        vec3_to_vec4(ubo->v_normal, 0.0f)
    );

#ifdef FLAT_SHADING
    vec3 N = normalize(homogenize_vec4(mv_normal));
    print_vec3(N);
    vec3 light_vec = vec3_sub(ubo->lights.u_light_position, N);

    float distance = magnitude(light_vec) * 0.1f;
    vec3 L = normalize(light_vec);

    float angle = dot(N, L);
    float attenuation = 1.0f / (1.0 + (distance * distance));
    float intensity = fmax(angle, 0.0f) * attenuation;

    // Stylized lighting
    if (intensity <= 0.2f) {
        intensity = 0.0f;
    } else if (intensity <= 0.3f) {
        intensity = 0.2f;
    } else if (intensity <= 0.4f) {
        intensity = 0.5f;
    } else {
        intensity = 1.0f;
    }
#endif

    // Don't homogenize
    ubo->v_normal = normalize(vec4_to_vec3(mv_normal));
    ubo->gl_position = mat_mul_vec4(ubo->u_mvp, a_position);
}

void fragment_shader(UBO *ubo, vec3 frag_coord) {
    vec3 N = normalize(ubo->gl_normal);
    vec3 light_vec = vec3_sub(ubo->lights.u_light_position, frag_coord);
    vec3 L = normalize(light_vec);

    float diffuse_factor = fmax(dot(N, L), 0.0f);
    vec3 diffuse_color = scale(diffuse_factor, ubo->u_color);

    // Scale to RGB to TGA format
    diffuse_color = scale(255.0f, diffuse_color);
    ubo->gl_frag_color = vec3_to_vec4(diffuse_color, 255.0f); // TGA RGBA
}