#include "shader.h"

const bool FLAT_SHADING = true;
const bool SMOOTH_SHADING = false;
const bool PHONG_SHADING = false;

void vertex_shader(UBO *ubo, vec4 a_position) {
    // If you you ever want non-uniform scaling, use this:
    // vec3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    Mat4x4 mv_no_trans = ubo->u_model_view;
    mv_no_trans.elem[0][3] = 0.0f;
    mv_no_trans.elem[1][3] = 0.0f;
    mv_no_trans.elem[2][3] = 0.0f;
    vec4 mv_normal = mat_mul_vec4(
        mv_no_trans,
        vec3_to_vec4(ubo->v_normal, 0.0f)
    );

    vec4 world_space = mat_mul_vec4(ubo->u_model, a_position);
    ubo->frag_pos = vec4_to_vec3(world_space);

    // Don't homogenize
    ubo->v_normal = normalize(vec4_to_vec3(mv_normal));
    ubo->gl_position = mat_mul_vec4(ubo->u_mvp, a_position);

}

void fragment_shader(UBO *ubo, vec3 frag_coord) {
    vec3 view_vec = vec3_sub(ubo->u_cam_pos, ubo->frag_pos);
    vec3 N = normalize(ubo->gl_normal);
    vec3 V = normalize(view_vec);

    // Multiple lights
    vec3 total_diffuse = ubo->u_color;
    vec3 total_specular = (vec3){0.0f, 0.0f, 0.0f};
    for (int i = 0; i < ubo->num_lights; i++){
        vec3 light_pos = ubo->lights[i].u_position;
        vec3 light_color = vec4_to_vec3(ubo->lights[i].u_color);
        vec3 light_vec = vec3_sub(light_pos, ubo->frag_pos);
        float light_radius = ubo->lights[i].u_radius;

        vec3 L = normalize(light_vec);
        float dist_squared = dot(light_vec, light_vec);
        float attenuation = light_radius / dist_squared;

        // Diffuse
        float angle = fmax(dot(N, L), 0.0f);
        vec3 intensity = vec3_add(light_color, ubo->u_color);
        intensity = scale(attenuation, intensity);

        // Specular
        vec3 half_angle = normalize(vec3_add(L, V));
        float blinn = clamp(dot(N, half_angle), 0.0f, 1.0f);
        blinn = pow(blinn, 512.0f);

        vec3 specular = scale(blinn, intensity);
        specular.x *= light_color.x;
        specular.y *= light_color.y;
        specular.z *= light_color.z;

        total_specular = vec3_add(total_specular, specular);
        total_diffuse = vec3_add(total_diffuse, scale(angle, intensity));
	}

    // Scale to RGB to TGA format
    vec3 lighting = vec3_add(total_diffuse, total_specular);
    lighting = scale(255.0f / ubo->num_lights, lighting); // this does not look right
    lighting.x = clamp(lighting.x, 0.0f, 255.0f);
    lighting.y = clamp(lighting.y, 0.0f, 255.0f);
    lighting.z = clamp(lighting.z, 0.0f, 255.0f);
    ubo->gl_frag_color = vec3_to_vec4(lighting, 255.0f); // TGA RGBA
}

float clamp(float value, float min, float max) {
    if (value > max) {
        return max;
    } else if (value < min) {
        return min;
    }
    return value;
}
