#include "shader.h"

const bool FLAT_SHADING = true;
const bool SMOOTH_SHADING = false;
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
    vec3 total_diffuse = (vec3){0.0f, 0.0f, 0.0f};
    vec3 total_specular = (vec3){0.0f, 0.0f, 0.0f};
	for (int i = 0; i < MAX_LIGHTS; i++){
        vec3 light_pos = ubo->lights[i].u_light_position;
        vec3 light_color = vec4_to_vec3(ubo->lights[i].u_light_color);
        vec3 light_vec = vec3_sub(light_pos, ubo->frag_pos);

        vec3 L = normalize(light_vec);
		float attenuation = 1.0f / dot(light_vec, light_vec);

		// Diffuse
		float angle = fmax(dot(N, L), 0.0f);
		vec3 intensity = vec3_add(light_color, ubo->u_color);
        intensity = scale(attenuation, intensity);

		// Specular
		vec3 half_angle = normalize(vec3_add(L, V));
		float blinn = dot(N, half_angle);

        // Clamp
        if (blinn < 0.0f) {
            blinn = 0.0f;
        } else if (blinn > 1.0f) {
            blinn = 1.0f;
        }
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
    lighting = scale(200.0f, lighting);
    if (lighting.x > 255.0f) {
        lighting.x = 255.0f;
    }
    if (lighting.y > 255.0f) {
        lighting.y = 255.0f;
    }
    if (lighting.z > 255.0f) {
        lighting.z = 255.0f;
    }
    ubo->gl_frag_color = vec3_to_vec4(lighting, 255.0f); // TGA RGBA
}