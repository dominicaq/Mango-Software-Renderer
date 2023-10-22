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
    vec4 mv_pos = mat_mul_vec4(
        mv_no_trans,
        a_position
    );
    vec4 mv_light_pos = mat_mul_vec4(
        mv_no_trans,
        vec3_to_vec4(ubo->u_light_position, 1.0f)
    );
    vec3 N = normalize(homogenize_vec4(mv_normal));
    vec3 L = normalize(vec3_sub(homogenize_vec4(mv_light_pos), homogenize_vec4(mv_pos)));

    float angle = dot(N, L);
    float intensity = fmin(angle, 0.1f);
    vec4 diffuse = color_scale(intensity, ubo->u_color);

    ubo->v_color = color_scale(255.0f, diffuse); // TGA RGB conversion
    ubo->gl_position = mat_mul_vec4(ubo->u_mvp, a_position);
}

void fragment_shader() {

}
