#include "shader.h"

const bool FLAT_SHADING = true;
const bool SMOOTH_SHADING = false;
const bool PHONG_SHADING = false;

// Source: https://www.learnopengles.com/tag/per-vertex-lighting/
void vertex_shader(UBO *ubo, vec4 a_position) {
    // Transform the vertex into eye space.
    // vec3 model_view_vertex = homogenize_vec4(
    //     mat_mul_vec4(ubo->u_model_view, a_position)
    // );

    // // Transform the normal's orientation into eye space.
    // vec4 transformed_normal = mat_mul_vec4(
    //     ubo->u_model_view,
    //     vec3_to_vec4(ubo->normal, 0.0f)
    // );
    // vec3 model_view_normal = normalize(homogenize_vec4(transformed_normal));

    // // Calculate the light direction, distance, and angle.
    // vec3 light_dir = vec3_sub(ubo->u_light_position, model_view_vertex);
    // float distance = magnitude(light_dir);
    // light_dir = normalize(light_dir);
    // float angle = dot(model_view_normal, light_dir);

    // // Attenuate the light based on distance.
    // float diffuse = fmax(angle, 0.1f) * (1.0 / (1.0 + (0.25 * distance * distance);
    ubo->v_color = color_scale(1.0f, ubo->v_color);
    ubo->gl_position = mat_mul_vec4(ubo->u_mvp, a_position);
}

void fragment_shader() {

}