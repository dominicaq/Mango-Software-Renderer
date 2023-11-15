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
            if (P.z < frame->zBuffer[buffer_index]) {
                // Update depth
                frame->zBuffer[buffer_index] = P.z;

                // Lerp data
                ubo->f_data.gl_normal = lerp_bc_coords(bc_coords, normals);
                ubo->f_data.frag_pos = lerp_bc_coords(bc_coords, model_space);

                fragment_shader(ubo, P);

                frame_set_pixel(frame, P.x, P.y, ubo->f_data.gl_frag_color);
            }
        }
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
            frame_set_pixel(frame, y, x, WIREFRAME_COLOR);
        } else {
            frame_set_pixel(frame, x, y, WIREFRAME_COLOR);
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

void draw_triangle(Frame *frame, Vec3 ndc[3], Vec3 normals[3], UBO *ubo) {
    // Backface culling
    if (is_backface(ndc) == true) {
        return;
    }

    // Get coordinate spaces
    Vec3 model_space[3];
    Vec3 screen_space[3];
    for (int i = 0; i < 3; ++i) {
        screen_space[i] = ndc_to_screen(frame->width, frame->height, ndc[i]);

        Vec4 ndc_vec4 = vec3_to_vec4(ndc[i], 1.0f);
        Vec4 view_space = mat_mul_vec4(ubo->u_vp_inv, ndc_vec4);
        model_space[i] = vec4_homogenize(view_space);
    }

    if (ubo->debug.wireframe == true) {
        wire_frame(frame, screen_space);
    }

    if (ubo->debug.rasterize == true) {
        rasterize(frame, screen_space, model_space, normals, ubo);
    }
}

void clip_one_vert(Frame *frame, Vec3 ndc1[3], Vec3 norms1[3], UBO *ubo) {
    Vec3 ndc2[3] = {
        ndc1[0],
        ndc1[1],
        ndc1[2],
    };
    Vec3 norms2[3] = {
        norms1[0],
        norms1[1],
        norms1[2],
    };

    float alpha1 = -ndc1[0].z / (ndc1[1].z - ndc1[0].z);
    ndc1[0] = vec3_lerp(ndc1[0], ndc1[1], alpha1);
    // norms1[0] = vec3_lerp(norms1[0], norms1[1], alpha1);

    ndc2[1] = ndc1[0];
    norms1[1] = norms1[0];

    float alpha2 = -ndc2[0].z / (ndc2[2].z - ndc2[0].z);
    ndc2[0] = vec3_lerp(ndc2[0], ndc2[2], alpha2);
    // norms2[0] = vec3_lerp(norms2[0], norms2[2], alpha2);

    draw_triangle(frame, ndc1, norms1, ubo);
    draw_triangle(frame, ndc2, norms2, ubo);
}

void clip_two_verts(Frame *frame, Vec3 ndc[3], Vec3 norms[3], UBO *ubo) {
    float alpha0 = -ndc[0].z / (ndc[2].z - ndc[0].z);
    float alpha1 = -ndc[1].z / (ndc[2].z - ndc[1].z);
    ndc[0] = vec3_lerp(ndc[0], ndc[2], alpha0);
    ndc[1] = vec3_lerp(ndc[1], ndc[2], alpha1);
    draw_triangle(frame, ndc, norms, ubo);
}

void transform_triangle(Frame *frame, Vertex *verts, UBO *ubo) {
    // Transform triangle data
    Vec3 normals[3];
    // Vec4 clip_space[3];
    Vec3 ndc[3];
    for (int i = 0; i < 3; ++i) {
        // Passed into shader
        Vertex current_vertex = verts[i];
        Vec4 a_position = vec3_to_vec4(current_vertex.position, 1.0f);
        ubo->v_data.in_normal = current_vertex.normal;

        // Apply vertex shader
        vertex_shader(ubo, a_position);

        // clip_space[i] = ubo->v_data.gl_position;
        ndc[i] = vec4_homogenize(ubo->v_data.gl_position);
        normals[i] = ubo->v_data.out_normal;
    }

    // TODO: Note in the future, apparently clipping using the clip_space is
    // easier, but cant find a paper / dont have the time to translate one
    // Vertex clipping (nasty)
    if (ndc[0].z > 1.0f) {
        if (ndc[1].z > 1.0f) {
            if (ndc[2].z > 1.0f) {
                return;
            }
            clip_two_verts(frame, ndc, normals, ubo);
        } else if (ndc[2].z > 1.0f) {
            Vec3 ndc_reorder[3] = {ndc[2], ndc[0], ndc[1]};
            Vec3 norms[3] = {normals[2], normals[0], normals[1]};
            clip_two_verts(frame, ndc_reorder, norms, ubo);
        } else {
            clip_one_vert(frame, ndc, normals, ubo);
        }
    } else if (ndc[1].z > 1.0f) {
        if (ndc[2].z > 1.0f) {
            Vec3 ndc_reorder[3] = {ndc[1], ndc[2], ndc[0]};
            Vec3 norms[3] = {normals[1], normals[2], normals[0]};
            clip_two_verts(frame, ndc_reorder, norms, ubo);
        } else {
            Vec3 ndc_reorder[3] = {ndc[1], ndc[2], ndc[0]};
            Vec3 norms[3] = {normals[1], normals[2], normals[0]};
            clip_one_vert(frame, ndc_reorder, norms, ubo);
        }
    } else if (ndc[2].z > 1.0f) {
        Vec3 ndc_reorder[3] = {ndc[2], ndc[0], ndc[1]};
        Vec3 norms[3] = {normals[2], normals[0], normals[1]};
        clip_one_vert(frame, ndc_reorder, norms, ubo);
    } else {
        draw_triangle(frame, ndc, normals, ubo);
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

void draw_sdf(Frame *frame, SDFSphere *sphere) {
    // If the result is negative, the point is inside the sphere.
    // If the result is zero, the point is on the sphere's surface.
    // If the result is positive, the point is outside the sphere.
    for (int x = 0; x < frame->width; ++x) {
        for (int y = 0; y < frame->height; ++y) {
            // Depth
            int buffer_index = x + y * frame->width;
            float current_depth = frame->zBuffer[buffer_index];

            // SDF
            Vec3 frame_pos = (Vec3){{x, y, current_depth}};
            Vec3 sdf_pos = vec3_sub(frame_pos, sphere->position);
            float sdf = sdf_sphere(sdf_pos, sphere->radius);

            // SDF is behind or out of range, dont draw
            if (current_depth < sdf || sdf > 0.0f) {
                continue;
            }

            Vec4 pixel_color = vec3_to_vec4(sphere->color, 1.0f);

            // Edge
            if (sdf == 0.0f) {

            }

            frame->zBuffer[buffer_index] = sdf;
            frame_set_pixel(frame, x, y, pixel_color);
        }
    }
}