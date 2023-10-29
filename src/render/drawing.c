#include "drawing.h"

const vec4 WIREFRAME_COLOR = (vec4){{255, 165, 0, 255}};

// Rasterizer
// -----------------------------------------------------------------------------
void rasterize(Frame *frame, vec3 ss[3], vec3 model_space[3], vec3 normals[3], UBO *ubo) {
    // Bounding box around triangle (in screen space) (ss = screen space)
    int x_min = MAX(0, MIN(MIN(ss[0].x, ss[1].x), ss[2].x));
    int y_min = MAX(0, MIN(MIN(ss[0].y, ss[1].y), ss[2].y));
    int x_max = MIN(frame->width - 1, MAX(MAX(ss[0].x, ss[1].x), ss[2].x));
    int y_max = MIN(frame->height - 1, MAX(MAX(ss[0].y, ss[1].y), ss[2].y));

    // Loop through the bounding box
    for (int y = y_min; y <= y_max; ++y) {
        for (int x = x_min; x <= x_max; ++x) {
            vec3 P = {{x, y, 0.0f}};
            vec3 bc_coords = barycentric_coords(P, ss[0], ss[1], ss[2]);
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
                frame->zBuffer[buffer_index] = P.z;
                ubo->gl_normal = lerp_barycentric_coords(bc_coords, normals);
                ubo->frag_pos = lerp_barycentric_coords(bc_coords, model_space);
                fragment_shader(ubo, P);
                setPixel(frame->framebuffer, P.x, P.y, ubo->gl_frag_color);
            }
        }
    }
}
void rasterize_triangle(Frame* frame, vec3 clip_space[3], vec3 normals[3], UBO *ubo) {
    // Backface culling
    if (is_backface(clip_space) == true) {
        return;
    }

    vec3 screen_space[3];
    vec3 model_space [3];

    for (int i = 0; i < 3; ++i) {
        screen_space[i] = ndc_to_screen(
            frame->width,
            frame->height,
            clip_space[i]
        );
        model_space[i] = homogenize_vec4(
            mat_mul_vec4(ubo->u_vp_inv, vec3_to_vec4(clip_space[i], 1)));
    }


    if (ubo->u_wireframe == true) {
        wire_frame(frame, screen_space);
    }
    rasterize(frame, screen_space, model_space, normals, ubo);
}

void clip_one_vert(Frame * frame, vec3 cs1[3], vec3 norms1[3], UBO * ubo) {
    vec3 cs2[3] = {
        cs1[0],
        cs1[1],
        cs1[2],
    };
    vec3 norms2[3] = {
        norms1[0],
        norms1[1],
        norms1[2],
    };

    float alpha1 = -cs1[0].z / (cs1[1].z - cs1[0].z);
    float alpha2 = -cs2[0].z / (cs2[2].z - cs2[0].z);
    vec3_lerp(cs1, cs1 + 1, alpha1);
    vec3_lerp(norms1, norms1 + 1, alpha1);
    vec3_lerp(cs2, cs2 + 2, alpha2);
    vec3_lerp(norms2, norms2 + 2, alpha2);

    rasterize_triangle(frame, cs1, norms1, ubo);
    rasterize_triangle(frame, cs2, norms2, ubo);
}

void clip_two_verts(Frame * frame, vec3 cs[3], vec3 norms[3], UBO * ubo) {
    float alpha0 = -cs[0].z / (cs[2].z - cs[0].z);
    float alpha1 = -cs[1].z / (cs[2].z - cs[1].z);
    vec3_lerp(cs, cs + 2, alpha0);
    vec3_lerp(cs + 1, cs + 2, alpha1);
    vec3_lerp(norms, norms + 2, alpha0);
    vec3_lerp(norms + 1, norms + 2, alpha1);

    rasterize_triangle(frame, cs, norms, ubo);
}

void draw_triangle(Frame *frame, Triangle *triangle, UBO *ubo) {
    // Apply vertex shader
    vec3 clip_space[3];
    vec3 world_space[3];
    vec3 normals[3];
    for (int i = 0; i < 3; ++i) {
        // Passed into shader
        vec4 a_position = vec3_to_vec4(triangle->vertices[i], 1.0f);
        ubo->v_normal = triangle->normals[i];
        vertex_shader(ubo, a_position);

        // Shader output
        // ndc[i] = clip_space
        clip_space[i] = homogenize_vec4(ubo->gl_position);
        world_space[i] = ubo->frag_pos;
        normals[i] = ubo->v_normal;

    }

    if (clip_space[0].z > 1.0f) {
        if (clip_space[1].z > 1.0f) {
            if (clip_space[2].z > 1.0f) {
                return;
            }
            clip_two_verts(frame, clip_space, normals, ubo);
        }else if (clip_space[2].z > 1.0f) {
            vec3 cs[3] = { clip_space[2], clip_space[0], clip_space[1] };
            vec3 norms[3] = { normals[2], normals[0], normals[1] };
            clip_two_verts(frame, cs, norms, ubo);
        } else {
            clip_one_vert(frame, clip_space, normals, ubo);
        }
    } else if (clip_space[1].z > 1.0f) {
        if (clip_space[2].z > 1.0f) {
            vec3 cs[3] = { clip_space[1], clip_space[2], clip_space[0] };
            vec3 norms[3] = { normals[1], normals[2], normals[0] };
            clip_two_verts(frame, cs, norms, ubo);
        } else {
            vec3 cs[3] = { clip_space[1], clip_space[2], clip_space[0] };
            vec3 norms[3] = { normals[1], normals[2], normals[0] };
            clip_one_vert(frame, cs, norms, ubo);
        }
    } else if (clip_space[2].z > 1.0f) {
        vec3 cs[3] = { clip_space[2], clip_space[0], clip_space[1] };
        vec3 norms[3] = { normals[2], normals[0], normals[1] };
        clip_one_vert(frame, cs, norms, ubo);
    } else {
        rasterize_triangle(frame, clip_space,  normals, ubo);
    }
}


void draw_mesh(Frame *frame, Mesh *mesh, UBO *ubo) {
    for (int i = 0; i < mesh->ind_count; i += 3) {
        if (i + 2 > mesh->ind_count) {
            break;
        }

        // Input assembly
        Triangle triangle;
        for (int j = 0; j < 3; ++j) {
            int index = i + j;
            triangle.vertices[j] = mesh->verts[mesh->vert_inds[index]];
            triangle.normals[j]  = mesh->norms[mesh->norm_inds[index]];
            triangle.uvs[j]      = mesh->uvs[mesh->uv_inds[index]];
        }
        draw_triangle(frame, &triangle, ubo);
    }
}

// Wireframe mode
// -----------------------------------------------------------------------------
void swap_ints(int *a, int *b) {
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

void line(Frame *frame, vec3 v0, vec3 v1) {
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

void wire_frame(Frame *frame, vec3 screen_space[3]) {
    line(frame, screen_space[0], screen_space[1]);
    line(frame, screen_space[1], screen_space[2]);
    line(frame, screen_space[2], screen_space[0]);
}
