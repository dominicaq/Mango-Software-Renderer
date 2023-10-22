#include "shader.h"

const bool FLAT_SHADING = true;
const bool SMOOTH_SHADING = false;
const bool PHONG_SHADING = false;

void vertex_shader(UBO *ubo, vec4 a_position) {
    vec4 transformed_normal = mat_mul_vec4(
        ubo->u_model_view,
        vec3_to_vec4(ubo->normal, 0.0f)
    );
    vec3 N = normalize(homogenize_vec4(transformed_normal));
    vec3 L = normalize(vec3_sub(ubo->u_light_position, N));

    float angle = dot(N, L);
    float intensity = fmax(angle, 0.1f);
    vec4 diffuse = color_scale(intensity, ubo->u_color);

    ubo->v_color = color_scale(255.0f, diffuse); // TGA RGB conversion
    ubo->gl_position = mat_mul_vec4(ubo->u_mvp, a_position);
}

void fragment_shader() {

}