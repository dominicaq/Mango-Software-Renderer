#include "drawing.h"

// Wireframe mode
// -----------------------------------------------------------------------------
void swap_ints(int *a, int *b) {
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

void line(Frame *frame, Vec3 v0, Vec3 v1, Vec4 color) {
    int x0 = (int)v0.x;
    int x1 = (int)v1.x;
    int y0 = (int)v0.y;
    int y1 = (int)v1.y;

    int steep = 0;
    if (abs(x0 - x1) < abs(y0 - y1)) {
        swap_ints(&x0, &y0);
        swap_ints(&x1, &y1);
        steep = 1;
    }
    if (x0 > x1) {
        swap_ints(&x0, &x1);
        swap_ints(&y0, &y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            frame_set_pixel(frame, y, x, color);
        } else {
            frame_set_pixel(frame, x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void wire_frame(Frame *frame, Vec3 screen_space[3], Vec4 color) {
    line(frame, screen_space[0], screen_space[1], color);
    line(frame, screen_space[1], screen_space[2], color);
    line(frame, screen_space[2], screen_space[0], color);
}

// Rasterizer
// -----------------------------------------------------------------------------
void rasterize(Frame *frame, Vertex verts[3], Vec3 ss[3],
               float perspective_w[3], UBO *ubo) {
    // Bounding box around triangle (in screen space) (ss = screen space)
    int x_min = MAX(0, MIN(MIN(ss[0].x, ss[1].x), ss[2].x));
    int y_min = MAX(0, MIN(MIN(ss[0].y, ss[1].y), ss[2].y));
    int x_max = MIN(frame->width - 1, MAX(MAX(ss[0].x, ss[1].x), ss[2].x));
    int y_max = MIN(frame->height - 1, MAX(MAX(ss[0].y, ss[1].y), ss[2].y));

    // Extract vertex data
    Vec2 uvs[3];
    Vec3 normals[3];
    Vec3 view_space[3];
    for (int i = 0; i < 3; ++i) {
        uvs[i] = verts[i].uv;
        normals[i] = verts[i].normal;
        view_space[i] = verts[i].position;
    }

    // Loop through the bounding box
    for (int y = y_min; y <= y_max; ++y) {
        for (int x = x_min; x <= x_max; ++x) {
            Vec3 P = (Vec3){{x, y, 0.0f}};
            Vec3 bc_coords = barycentric_coords(P, ss[0], ss[1], ss[2]);
            // Not within triangle
            if (bc_coords.x < 0 || bc_coords.y < 0 || bc_coords.z < 0) {
                continue;
            }

            // Calculate depth of triangle
            P.z += bc_coords.x * ss[0].z;
            P.z += bc_coords.y * ss[1].z;
            P.z += bc_coords.z * ss[2].z;

            // Determine if triangle is on top
            int buffer_index = x + y * frame->width;
            if (P.z < frame->z_buffer[buffer_index]) {
                // Update depth
                frame->z_buffer[buffer_index] = P.z;

                // Interpolate vertex data
                ubo->f_data.gl_normal = lerp_bc_coords(bc_coords, normals);
                ubo->f_data.frag_pos = lerp_bc_coords(bc_coords, view_space);
                ubo->f_data.uv = lerp_uv_coords(bc_coords, perspective_w, uvs);

                fragment_shader(ubo, P);

                frame_set_pixel(frame, P.x, P.y, ubo->f_data.gl_frag_color);
            }
        }
    }
}

void draw_triangle(Frame *frame, Vertex verts[3], UBO *ubo) {
    // Cull triangle if backface
    Vec3 ndc[3] = {verts[0].position, verts[1].position, verts[2].position};
    if (is_backface(ndc) == true) {
        return;
    }

    // Get coordinate spaces
    Vec3 screen_space[3];
    float perspective_w[3];
    Vertex *view_verts = verts;
    for (int i = 0; i < 3; ++i) {
        Vec3 ndc = verts[i].position;

        screen_space[i] = ndc_to_screen(frame->width, frame->height, ndc);
        Vec4 view_space = mat_mul_vec4(ubo->u_vp_inv, vec3_to_vec4(ndc, 1.0f));
        view_verts[i].position = vec4_homogenize(view_space);
        perspective_w[i] = view_space.z;
    }

    if (ubo->options & OPT_USE_WIREFRAME) {
        wire_frame(frame, screen_space, vec3_to_vec4(ubo->u_color, 1));
    }

    if (ubo->options & OPT_USE_RASTERIZE) {
        rasterize(frame, view_verts, screen_space, perspective_w, ubo);
    }
}

void clip_one_vert(Frame *frame, Vertex verts[3], UBO *ubo) {
    Vec3 ndc1[3] = {verts[0].position, verts[1].position, verts[2].position};
    // Vec3 norms1[3] = {verts[0].normal, verts[1].normal, verts[2].normal};

    Vec3 ndc2[3] = {verts[0].position, verts[1].position, verts[2].position};
    // Vec3 norms2[3] = {verts[0].normal, verts[1].normal, verts[2].normal};

    float alpha1 = -ndc1[0].z / (ndc1[1].z - ndc1[0].z);
    ndc1[0] = vec3_lerp(ndc1[0], ndc1[1], alpha1);
    // norms1[0] = vec3_lerp(norms1[0], norms1[1], alpha1);

    ndc2[1] = ndc1[0];
    // norms1[1] = norms1[0];

    float alpha2 = -ndc2[0].z / (ndc2[2].z - ndc2[0].z);
    ndc2[0] = vec3_lerp(ndc2[0], ndc2[2], alpha2);
    // norms2[0] = vec3_lerp(norms2[0], norms2[2], alpha2);

    // Reassign and draw
    Vertex triangle1[3];
    Vertex triangle2[3];
    for (int i = 0; i < 3; ++i) {
        triangle1[i].position = ndc1[i];
        triangle1[i].normal = verts[i].normal;
        triangle1[i].uv = verts[i].uv;

        triangle2[i].position = ndc2[i];
        triangle2[i].normal = verts[i].normal;
        triangle2[i].uv = verts[i].uv;
    }
    draw_triangle(frame, triangle1, ubo);
    draw_triangle(frame, triangle2, ubo);
}

void clip_two_verts(Frame *frame, Vertex verts[3], UBO *ubo) {
    Vec3 ndc[3] = {verts[0].position, verts[1].position, verts[2].position};

    // Lerp data
    float alpha0 = -ndc[0].z / (ndc[2].z - ndc[0].z);
    float alpha1 = -ndc[1].z / (ndc[2].z - ndc[1].z);
    ndc[0] = vec3_lerp(ndc[0], ndc[2], alpha0);
    ndc[1] = vec3_lerp(ndc[1], ndc[2], alpha1);

    // Reassign and draw
    for (int i = 0; i < 3; ++i) {
        verts[i].position = ndc[i];
    }
    draw_triangle(frame, verts, ubo);
}

void transform_triangle(Frame *frame, Vertex *verts, UBO *ubo) {
    // Transform triangle data
    Vertex ndc_verts[3];
    for (int i = 0; i < 3; ++i) {
        // Passed into shader
        Vertex current_vertex = verts[i];
        Vec4 a_position = vec3_to_vec4(current_vertex.position, 1.0f);
        ubo->v_data.in_normal = current_vertex.normal;

        // Apply vertex shader
        vertex_shader(ubo, a_position);

        ndc_verts[i].position = vec4_homogenize(ubo->v_data.gl_position);
        ndc_verts[i].normal = ubo->v_data.out_normal;
        ndc_verts[i].uv = verts[i].uv;
    }

    // TODO: Note in the future, apparently clipping using the clip_space is
    // easier, but cant find a paper / dont have the time to translate one
    // Vertex clipping (nasty)
    float ndc0 = ndc_verts[0].position.z;
    float ndc1 = ndc_verts[1].position.z;
    float ndc2 = ndc_verts[2].position.z;
    if (ndc0 > 1.0f) {
        if (ndc1 > 1.0f) {
            if (ndc2 > 1.0f) {
                return;
            }
            clip_two_verts(frame, ndc_verts, ubo);
        } else if (ndc2 > 1.0f) {
            Vertex vert_reorder[3] = {ndc_verts[2], ndc_verts[0], ndc_verts[1]};
            clip_two_verts(frame, vert_reorder, ubo);
        } else {
            clip_one_vert(frame, ndc_verts, ubo);
        }
    } else if (ndc1 > 1.0f) {
        if (ndc2 > 1.0f) {
            Vertex vert_reorder[3] = {ndc_verts[1], ndc_verts[2], ndc_verts[0]};
            clip_two_verts(frame, vert_reorder, ubo);
        } else {
            Vertex vert_reorder[3] = {ndc_verts[1], ndc_verts[2], ndc_verts[0]};
            clip_one_vert(frame, vert_reorder, ubo);
        }
    } else if (ndc2 > 1.0f) {
        Vertex vert_reorder[3] = {ndc_verts[2], ndc_verts[0], ndc_verts[1]};
        clip_one_vert(frame, vert_reorder, ubo);
    } else {
        draw_triangle(frame, ndc_verts, ubo);
    }
}

void draw_mesh(Frame *frame, Mesh *mesh, UBO *ubo) {
    for (int i = 0; i < mesh->ind_count; i += 3) {
        if (i + 2 > mesh->ind_count) {
            break;
        }

        // Input assembly
        Vertex verts[3];
        for (int j = 0; j < 3; ++j) {
            int index = i + j;
            verts[j].position = mesh->verts[mesh->vert_inds[index]];
            verts[j].normal = mesh->norms[mesh->norm_inds[index]];
            verts[j].uv = mesh->uvs[mesh->uv_inds[index]];
        }
        transform_triangle(frame, verts, ubo);
    }
}
