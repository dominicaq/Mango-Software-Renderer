#include "drawing.h"

const Vec4 WIREFRAME_COLOR = (Vec4){{255, 165, 0, 255}};

// Rasterizer
// -----------------------------------------------------------------------------
void rasterize(Frame *frame, Vec3 ss[3], Vec3 model_space[3], Vec3 normals[3],
               UBO *ubo) {
    // Bounding box around triangle (in screen space) (ss = screen space)
    int x_min = MAX(0, MIN(MIN(ss[0].x, ss[1].x), ss[2].x));
    int y_min = MAX(0, MIN(MIN(ss[0].y, ss[1].y), ss[2].y));
    int x_max = MIN(frame->width - 1, MAX(MAX(ss[0].x, ss[1].x), ss[2].x));
    int y_max = MIN(frame->height - 1, MAX(MAX(ss[0].y, ss[1].y), ss[2].y));

    // Loop through the bounding box
    for (int y = y_min; y <= y_max; ++y) {
        for (int x = x_min; x <= x_max; ++x) {
            Vec3 P = {{x, y, 0.0f}};
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
            if (P.z < frame->zBuffer[buffer_index]) {
                // Update depth
                frame->zBuffer[buffer_index] = P.z;

                // Lerp data
                ubo->f_data.gl_normal = lerp_bc_coords(bc_coords, normals);
                ubo->f_data.frag_pos = lerp_bc_coords(bc_coords, model_space);

                fragment_shader(ubo, P);

                setPixel(frame->framebuffer, P.x, P.y, ubo->f_data.gl_frag_color);
            }
        }
    }
}

void draw_triangle(Frame *frame, Vec3 clip_space[3], Vec3 normals[3], UBO *ubo) {
    // Backface culling
    if (is_backface(clip_space) == true) {
        return;
    }

    // Get coordinate spaces
    Vec3 model_space[3];
    Vec3 screen_space[3];
    for (int i = 0; i < 3; ++i) {
        screen_space[i] = ndc_to_screen(
            frame->width,
            frame->height,
            clip_space[i]
        );

        Vec4 clip_space_vec4 = vec3_to_vec4(clip_space[i], 1.0f);
        Vec4 view_space = mat_mul_vec4(ubo->u_vp_inv, clip_space_vec4);
        model_space[i] = vec4_homogenize(view_space);
    }

    if (ubo->debug.wireframe == true) {
        wire_frame(frame, screen_space);
    }

    if (ubo->debug.rasterize == true) {
        rasterize(frame, screen_space, model_space, normals, ubo);
    }
}

void clip_one_vert(Frame *frame, Vec3 cs1[3], Vec3 norms1[3], UBO *ubo) {
    Vec3 cs2[3] = {
        cs1[0],
        cs1[1],
        cs1[2],
    };
    Vec3 norms2[3] = {
        norms1[0],
        norms1[1],
        norms1[2],
    };

    float alpha1 = -cs1[0].z / (cs1[1].z - cs1[0].z);
    float alpha2 = -cs2[0].z / (cs2[2].z - cs2[0].z);
    cs1[0] = vec3_lerp(cs1[0], cs1[1], alpha1);
    norms1[0] = vec3_lerp(norms1[0], norms1[1], alpha1);
    cs2[0] = vec3_lerp(cs2[0], cs2[2], alpha2);
    norms2[0] = vec3_lerp(norms2[0], norms2[2], alpha2);

    draw_triangle(frame, cs1, norms1, ubo);
    draw_triangle(frame, cs2, norms2, ubo);
}

void clip_two_verts(Frame *frame, Vec3 cs[3], Vec3 norms[3], UBO *ubo) {
    float alpha0 = -cs[0].z / (cs[2].z - cs[0].z);
    float alpha1 = -cs[1].z / (cs[2].z - cs[1].z);
    cs[0] = vec3_lerp(cs[0], cs[2], alpha0);
    cs[1] = vec3_lerp(cs[1], cs[2], alpha1);
    norms[0] = vec3_lerp(norms[0], norms[2], alpha0);
    norms[1] = vec3_lerp(norms[1], norms[2], alpha1);

    draw_triangle(frame, cs, norms, ubo);
}

void transform_triangle(Frame *frame, Vertex *verts, UBO *ubo) {
    // Transform triangle data
    Vec3 normals[3];
    Vec3 clip_space[3];
    for (int i = 0; i < 3; ++i) {
        // Passed into shader
        Vertex current_vertex = verts[i];
        Vec4 a_position = vec3_to_vec4(current_vertex.position, 1.0f);
        ubo->v_data.in_normal = current_vertex.normal;

        // Apply vertex shader
        vertex_shader(ubo, a_position);

        // gl_position means NDC
        // Perspective divide
        clip_space[i] = vec4_homogenize(ubo->v_data.gl_position);
        normals[i] = ubo->v_data.out_normal;
    }

    // Vertex clipping (nasty)
    if (clip_space[0].z > 1.0f) {
        if (clip_space[1].z > 1.0f) {
            if (clip_space[2].z > 1.0f) {
                return;
            }
            clip_two_verts(frame, clip_space, normals, ubo);
        } else if (clip_space[2].z > 1.0f) {
            Vec3 cs[3] = {clip_space[2], clip_space[0], clip_space[1]};
            Vec3 norms[3] = {normals[2], normals[0], normals[1]};
            clip_two_verts(frame, cs, norms, ubo);
        } else {
            clip_one_vert(frame, clip_space, normals, ubo);
        }
    } else if (clip_space[1].z > 1.0f) {
        if (clip_space[2].z > 1.0f) {
            Vec3 cs[3] = {clip_space[1], clip_space[2], clip_space[0]};
            Vec3 norms[3] = {normals[1], normals[2], normals[0]};
            clip_two_verts(frame, cs, norms, ubo);
        } else {
            Vec3 cs[3] = {clip_space[1], clip_space[2], clip_space[0]};
            Vec3 norms[3] = {normals[1], normals[2], normals[0]};
            clip_one_vert(frame, cs, norms, ubo);
        }
    } else if (clip_space[2].z > 1.0f) {
        Vec3 cs[3] = {clip_space[2], clip_space[0], clip_space[1]};
        Vec3 norms[3] = {normals[2], normals[0], normals[1]};
        clip_one_vert(frame, cs, norms, ubo);
    } else {
        draw_triangle(frame, clip_space, normals, ubo);
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
            verts[j].normal   = mesh->norms[mesh->norm_inds[index]];
            verts[j].uv       = mesh->uvs[mesh->uv_inds[index]];
        }
        transform_triangle(frame, verts, ubo);
    }
}

// Wireframe mode
// -----------------------------------------------------------------------------
void swap_ints(int *a, int *b) {
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

void line(Frame *frame, Vec3 v0, Vec3 v1) {
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
            setPixel(frame->framebuffer, y, x, WIREFRAME_COLOR);
        } else {
            setPixel(frame->framebuffer, x, y, WIREFRAME_COLOR);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void wire_frame(Frame *frame, Vec3 screen_space[3]) {
    line(frame, screen_space[0], screen_space[1]);
    line(frame, screen_space[1], screen_space[2]);
    line(frame, screen_space[2], screen_space[0]);
}
