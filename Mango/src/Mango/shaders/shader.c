#include "shader.h"

const bool FLAT_SHADING = true;
const bool SMOOTH_SHADING = false;
const bool PHONG_SHADING = false;

Vec3 sample_tex(Vec2 uv, Texture *texture) {
    if (texture == NULL) {
        return VEC3_ZERO;
    }

    // Calculate the index in the texture data array
    int index = (uv.y * texture->width + uv.x * texture->height);
    Vec3 rgb;
    rgb.x = texture->data[index];
    rgb.y = texture->data[index + 1];
    rgb.z = texture->data[index + 2];
    return rgb;
}

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
    Vec3 V = vec3_normalize(view_vec);

    // DEBUG OPTIONS
    if (ubo->options & OPT_VIEW_NORMALS) {
        N = vec3_scale(N, 255.0f);
        N = vec3_clamp(N, 0.0f, 255.0f);
        ubo->f_data.gl_frag_color = vec3_to_vec4(N, 255.0f);
        return;
    } else if (ubo->options & OPT_VIEW_UV_MAP) {
        Vec3 uv_color = vec2_to_vec3(ubo->f_data.uv, 1.0f);
        uv_color = vec3_scale(uv_color, 255.0f);
        uv_color = vec3_clamp(uv_color, 0.0f, 255.0f);
        ubo->f_data.gl_frag_color = vec3_to_vec4(uv_color, 255.0f);
        return;
    }

    // Multiple lights
    // Vec3 total_diffuse = sample_tex(ubo->f_data.uv, ubo->u_mat->albedo_map);
    Vec3 total_diffuse = VEC3_ZERO;
    Vec3 total_specular = VEC3_ZERO;
    for (int i = 0; i < ubo->num_lights; i++) {
        if (ubo->lights[i]->type == LIGHT_POINT) {
            Vec3 light_pos = ubo->light_objects[i]->position;
            Vec3 light_color = ubo->lights[i]->color;
            Vec3 light_vec = vec3_sub(light_pos, frag_pos);
            float light_radius = ubo->lights[i]->intensity;

            Vec3 L = vec3_normalize(light_vec);
            float dist_squared = vec3_dot(light_vec, light_vec);
            float attenuation = light_radius / dist_squared;

            // Diffuse
            float angle = fmax(vec3_dot(N, L), 0.0f);
            Vec3 intensity = vec3_add(light_color, ubo->u_color);
            intensity = vec3_scale(intensity, attenuation);

            // Specular
            Vec3 half_angle = vec3_normalize(vec3_add(L, V));
            float blinn = clamp(vec3_dot(N, half_angle), 0.0f, 1.0f);
            blinn = pow(blinn, 2.0f);

            Vec3 specular = intensity;
            specular = vec3_scale(specular, blinn);
            specular.x *= light_color.x;
            specular.y *= light_color.y;
            specular.z *= light_color.z;

            total_specular = vec3_add(total_specular, specular);
            intensity = vec3_scale(intensity, angle);

            // total_diffuse = vec3_add(total_diffuse, intensity);
            total_diffuse.x *= intensity.x;
            total_diffuse.y *= intensity.y;
            total_diffuse.z *= intensity.z;
        }
    }

    // Scale to RGB to TGA format
    Vec3 lighting = vec3_add(total_diffuse, total_specular);
    lighting = vec3_scale(lighting, 255.0f / ubo->num_lights);
    lighting = vec3_clamp(lighting, 0.0f, 255.0f);
    ubo->f_data.gl_frag_color = vec3_to_vec4(lighting, 255.0f);

    // DEBUG
    // ubo->f_data.tex_albedo = sample_tex(ubo->f_data.uv,
    // ubo->u_mat->albedo_map); ubo->f_data.gl_frag_color =
    // vec3_to_vec4(ubo->f_data.tex_albedo, 255.0f);
}
