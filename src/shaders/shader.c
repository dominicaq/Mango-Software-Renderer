#include "shader.h"

const bool FLAT_SHADING = true;
const bool SMOOTH_SHADING = false;
const bool PHONG_SHADING = false;

void vertex_shader(UBO *ubo, vec4 a_position) {
    Mat4x4 mv_no_trans = ubo->u_model_view;
    mv_no_trans.elem[3][0] = 0;
    mv_no_trans.elem[3][1] = 0;
    mv_no_trans.elem[3][2] = 0;
    vec4 mv_normal = mat_mul_vec4(
        mv_no_trans,
        vec3_to_vec4(ubo->normal, 1.0f)
    );
    // Specular
    // vec4 mv_pos = mat_mul_vec4(
    //     mv_no_trans,
    //     a_position
    // );
    // vec3 V = homogenize_vec4(mv_pos);
    vec3 N = normalize(homogenize_vec4(mv_normal));

    vec3 light_vec = vec3_sub(ubo->u_light_position, N);

    float distance = magnitude(light_vec) * 0.1f;
    vec3 L = normalize(light_vec);

    float angle = dot(N, L);
    float attenuation = 1.0f / (1.0 + (0.25 * distance * distance));
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

    vec4 diffuse = color_scale(intensity, ubo->u_color);

    ubo->v_color = color_scale(255.0f, diffuse); // TGA RGB conversion
    ubo->gl_position = mat_mul_vec4(ubo->u_mvp, a_position);
}

void fragment_shader() {

}
