#include "shader.h"

// TODO:
// https://learnopengl.com/PBR/Theory
const bool FLAT_SHADING = true;
const bool SMOOTH_SHADING = false;
const bool PHONG_SHADING = false;

void vertex_shader(UBO *ubo, Vec4 a_position) {
    // If you you ever want non-uniform scaling, use this:
    // vec3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    Mat4 mv_no_trans = ubo->u_model_view;
    mv_no_trans.elem[0][3] = 0.0f;
    mv_no_trans.elem[1][3] = 0.0f;
    mv_no_trans.elem[2][3] = 0.0f;
    Vec4 normal_homogeneous = vec3_to_vec4(ubo->v_data.in_normal, 0.0f);
    Vec4 mv_normal = mat_mul_vec4(mv_no_trans, normal_homogeneous);

    // Don't homogenize
    ubo->v_data.out_normal = vec3_normalize(vec4_to_vec3(mv_normal));
    ubo->v_data.gl_position = mat_mul_vec4(ubo->u_mvp, a_position);
}

void fragment_shader(UBO *ubo, Vec3 frag_coord) {
    Vec3 frag_pos = ubo->f_data.frag_pos;
    Vec3 view_vec = vec3_sub(ubo->u_cam_pos, frag_pos);
    Vec3 N = vec3_normalize(ubo->f_data.gl_normal);
    if (ubo->u_mat->normal_map != NULL) {
        N = vec4_homogenize(sample_texture(ubo->f_data.uv, ubo->u_mat->normal_map));
    }
    Vec3 V = vec3_normalize(view_vec);

    // DEBUG OPTIONS
    if (ubo->options & OPT_VIEW_NORMALS) {
        N = vec3_clamp(N, 0.0f, 1.0f);
        ubo->f_data.gl_frag_color = vec3_to_vec4(N, 1.0f);
        return;
    } else if (ubo->options & OPT_VIEW_UV_MAP) {
        Vec3 uv_color = vec2_to_vec3(ubo->f_data.uv, 1.0f);
        uv_color = vec3_clamp(uv_color, 0.0f, 1.0f);
        ubo->f_data.gl_frag_color = vec3_to_vec4(uv_color, 1.0f);
        return;
    }

    // Sample albedo color from texture
    Vec4 albedo_color = (Vec4){{1.0f,1.0f,1.0f,1.0f}};
    if (ubo->u_mat->albedo_map != NULL) {
        albedo_color = sample_texture(ubo->f_data.uv, ubo->u_mat->albedo_map);
    }

    if (ubo->options & OPT_NO_LIGHTING) {
        ubo->f_data.gl_frag_color = albedo_color;
        return;
    }

    // Specular texture map
    Vec3 normal_tangent = N;
    if (ubo->u_mat->tangent_map != NULL) {
        Vec4 tangent_map = sample_texture(ubo->f_data.uv, ubo->u_mat->tangent_map);
        if (ubo->options & OPT_VIEW_TANGENT_MAP) {
            ubo->f_data.gl_frag_color = tangent_map;
            return;
        }
        normal_tangent = vec4_to_vec3(tangent_map);
    }

    // No texture (set model color to white)
    if (ubo->options & OPT_NO_TEXTURE) {
        albedo_color = vec3_to_vec4((Vec3){{1.0f,1.01,1.0f}}, 1.0f);
    }

    // Initialize total diffuse and specular components
    Vec3 total_diffuse = VEC3_ZERO;
    Vec3 total_specular = VEC3_ZERO;
    for (int i = 0; i < ubo->num_lights; ++i) {
        if (ubo->lights[i]->type == LIGHT_POINT) {
            Vec3 light_pos = ubo->light_objects[i]->position;
            Vec3 light_color = ubo->lights[i]->color;
            Vec3 light_vec = vec3_sub(light_pos, frag_pos);
            float distance = vec3_magnitude(light_vec);
            float light_radius = ubo->lights[i]->radius;

            float distance2 = distance * distance;
            float light_radius2 = light_radius * light_radius;
            float attenuation = 1.0f / (1.0f + distance / light_radius + distance2 / light_radius2);
            Vec3 L = vec3_normalize(light_vec);

            // Diffuse
            float angle = fmax(vec3_dot(N, L), 0.0f) *  ubo->lights[i]->intensity;
            Vec3 diffuse = vec3_scale(light_color, angle * attenuation);
            total_diffuse = vec3_add(total_diffuse, diffuse);

            // Specular
            Vec3 half_angle = vec3_normalize(vec3_add(L, V));
            float specular_angle = pow(fmax(vec3_dot(normal_tangent, half_angle), 0.0), 4.0f);
            Vec3 specular = vec3_scale(light_color, ubo->lights[i]->intensity * specular_angle * attenuation);
            total_specular = vec3_add(total_specular, specular);
        }
    }

    // Apply kd and ks (sum of coefficients should be = 1)
    total_diffuse = vec3_scale(total_diffuse, 0.5f);
    total_specular = vec3_scale(total_specular, 0.5f);

    // Combine lighting components with albedo color
    Vec3 lighting_rgb = vec3_add(total_diffuse, total_specular);
    Vec4 lighting_rgba = vec3_to_vec4(lighting_rgb, 1.0f);

    Vec4 final_color = vec4_mul_vec4(albedo_color, lighting_rgba);

    // Gamma correction
    float gamma = 0.5f;
    final_color.x = powf(final_color.x, gamma);
    final_color.y = powf(final_color.y, gamma);
    final_color.z = powf(final_color.z, gamma);
    ubo->f_data.gl_frag_color = final_color;
}