#include "drawing.h"

Plane clip_planes[NUM_CLIP_PLANES];
const Vec4 WIREFRAME_COLOR = (Vec4){{255, 165, 0, 255}}; // Orange

/*
* Wireframe
*/
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

/*
* Triangle clipping
*/
Vertex vertex_intersect(const Vertex a, const Vertex b, Plane plane) {
    Vec3 line_dir = vec3_sub(b.position, a.position);
    float dot_denominator = vec3_dot(plane.normal, line_dir);

    // Ensure we're not dividing by zero
    // No intersection or parallel
    if (fabs(dot_denominator) < EPSILON) {
        return a;
    }

    float t = -(vec3_dot(a.position, plane.normal) + plane.distance) / dot_denominator;

    // Compute the new vertex intersection point
    Vertex result = {0};
    result.position = vec3_add(a.position, vec3_scale(line_dir, t));
    result.normal = vec3_add(a.normal, vec3_scale(vec3_sub(b.normal, a.normal),t));
    result.uv = vec2_add(a.uv, vec2_scale(vec2_sub(b.uv, a.uv), t));
    return result;
}

void init_clip_planes() {
    // Clip planes: Dot of <normal, posiiton> precalculated
    // Near plane
    clip_planes[0] = (Plane){(Vec3){{0.0f, 0.0f, 1.0f}}, 1.0f};
    // Left plane
    clip_planes[1] = (Plane){(Vec3){{1.0f * sqrt(2.0f), 0.0f, 1.0f * sqrt(2.0f)}}, 0.0f};
    // Right Plane
    clip_planes[2] = (Plane){(Vec3){{-1.0f * sqrt(2.0f), 0.0f, 1.0f * sqrt(2.0f)}}, 0.0f};
    // Bottom Plane
    clip_planes[3] = (Plane){(Vec3){{0.0f, 1.0f * sqrt(2.0f), 1.0f * sqrt(2.0f)}}, 0.0f};
    // Top Plane
    clip_planes[4] = (Plane){(Vec3){{0.0f, -1.0f * sqrt(2.0f), 1.0f * sqrt(2.0f)}}, 0.0f};

#ifdef DEBUG_CLIP_PLANE
    // Near plane
    // clip_planes[0] = (Plane){(Vec3){{0.0f, 0.0f, 1.0f}}, 0.5f}; // Adjusted distance to 0.5f
    // Left plane
    clip_planes[1] = (Plane){(Vec3){{1.0f * q_rsqrt(2.0f), 0.0f, 1.0f * q_rsqrt(2.0f)}}, -0.5f}; // Adjusted distance to -0.5f
    // Right Plane
    clip_planes[2] = (Plane){(Vec3){{-1.0f * q_rsqrt(2.0f), 0.0f, 1.0f * q_rsqrt(2.0f)}}, -0.5f}; // Adjusted distance to -0.5f
    // Bottom Plane
    clip_planes[3] = (Plane){(Vec3){{0.0f, 1.0f * q_rsqrt(2.0f), 1.0f * q_rsqrt(2.0f)}}, -0.5f}; // Adjusted distance to -0.5f
    // Top Plane
    clip_planes[4] = (Plane){(Vec3){{0.0f, -1.0f * q_rsqrt(2.0f), 1.0f * q_rsqrt(2.0f)}}, -0.5f}; // Adjusted distance to -0.5f
#endif
}

float signed_distance(Plane plane, Vec3 point) {
    return vec3_dot(plane.normal, point) + plane.distance;
}

/*
* Render Pipeline
*/
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

    if (ubo->options & OPT_USE_RASTERIZE) {
        rasterize(frame, view_verts, screen_space, perspective_w, ubo);
    }

    if (ubo->options & OPT_USE_WIREFRAME) {
        wire_frame(frame, screen_space);
    }
}

void transform_triangle(Frame *frame, Vertex *verts, UBO *ubo) {
    // Maximum of 21 new vertices can be created from clipping
    Vertex ndc_verts[21];
    size_t num_verts = 3;
    size_t num_outside = 0;
    for (size_t i = 0; i < 3; ++i) {
        // Passed into shader
        Vec4 a_position = vec3_to_vec4(verts[i].position, 1.0f);
        ubo->v_data.in_normal = verts[i].normal;

        // Apply vertex shader
        vertex_shader(ubo, a_position);

        // Clip space coordinates (Normalized Device Coordinates)
        ndc_verts[i].position = vec4_homogenize(ubo->v_data.gl_position);
        ndc_verts[i].normal = ubo->v_data.out_normal;
        ndc_verts[i].uv = verts[i].uv;

        // Check if currect vertex is outside clip space
        if (ndc_verts[i].position.x < -1.0f || ndc_verts[i].position.x > 1.0f ||
            ndc_verts[i].position.y < -1.0f || ndc_verts[i].position.y > 1.0f ||
            ndc_verts[i].position.z < -1.0f || ndc_verts[i].position.z > 1.0f) {
                ++num_outside;
        }
    }

    // Check if triangle is enitrely outside clip space
    if (num_outside >= 3) {
        return;
    }

    for (size_t i = 0; i < NUM_CLIP_PLANES; ++i) {
        // Signed distance between polygon and plane
        float d0 = signed_distance(clip_planes[i], ndc_verts[0].position);
        float d1 = signed_distance(clip_planes[i], ndc_verts[1].position);
        float d2 = signed_distance(clip_planes[i], ndc_verts[2].position);

        // Check vertices against clip planes
        Vertex *A = &ndc_verts[0];
        Vertex *B = &ndc_verts[1];
        Vertex *C = &ndc_verts[2];
        if (d0 > 0 && d1 > 0 && d2 > 0) {
            // Triangle fully in clip space continue through plane checks
            continue;
        } else if (d0 < 0 && d1 < 0 && d2 < 0) {
            // Triangle fully outside clip space
            return;
        } else if (d0 > 0 && d1 < 0 && d2 < 0) {
            // Only vertex A inside clip volume
            Vertex B_prime = vertex_intersect(*A, *B, clip_planes[i]);
            Vertex C_prime = vertex_intersect(*A, *C, clip_planes[i]);
            ndc_verts[1] = B_prime;
            ndc_verts[2] = C_prime;
        } else if (d0 < 0 && d1 > 0 && d2 < 0) {
            // Only vertex B inside clip volume
            Vertex A_prime = vertex_intersect(*B, *A, clip_planes[i]);
            Vertex C_prime = vertex_intersect(*B, *C, clip_planes[i]);
            ndc_verts[0] = A_prime;
            ndc_verts[2] = C_prime;
        } else if (d0 < 0 && d1 < 0 && d2 > 0) {
            // Only vertex C inside clip volume
            Vertex A_prime = vertex_intersect(*C, *A, clip_planes[i]);
            Vertex B_prime = vertex_intersect(*C, *B, clip_planes[i]);
            ndc_verts[0] = A_prime;
            ndc_verts[1] = B_prime;
        } else if (d0 < 0 && d1 > 0 && d2 > 0) {
            // Vertex A not in clip volume
            Vertex B_prime = vertex_intersect(*B, *A, clip_planes[i]);
            Vertex C_prime = vertex_intersect(*C, *A, clip_planes[i]);
            ndc_verts[0] = B_prime;

            // Add new triangle
            ndc_verts[num_verts] = B_prime;
            ndc_verts[num_verts + 1] = *C;
            ndc_verts[num_verts + 2] = C_prime;
            num_verts += 3;
        } else if (d0 > 0 && d1 < 0 && d2 > 0) {
            // Vertex B not in clip volume
            Vertex A_prime = vertex_intersect(*A, *B, clip_planes[i]);
            Vertex C_prime = vertex_intersect(*C, *B, clip_planes[i]);
            ndc_verts[1] = C_prime;

            // Add new triangle
            ndc_verts[num_verts] = C_prime;
            ndc_verts[num_verts + 1] = *A;
            ndc_verts[num_verts + 2] = A_prime;
            num_verts += 3;
        } else if (d0 > 0 && d1 > 0 && d2 < 0) {
            // Vertex C not in clip volume
            Vertex A_prime = vertex_intersect(*A, *C, clip_planes[i]);
            Vertex B_prime = vertex_intersect(*B, *C, clip_planes[i]);
            ndc_verts[2] = A_prime;

            // Add new triangle
            ndc_verts[num_verts] = A_prime;
            ndc_verts[num_verts + 1] = *B;
            ndc_verts[num_verts + 2] = B_prime;
            num_verts += 3;
        }
    }

    // Draw one or two ndc_verts
    for (size_t i = 0; i < num_verts; i += 3) {
        Vertex triangle[3] = {
            ndc_verts[i],
            ndc_verts[i + 1],
            ndc_verts[i + 2]
        };
        draw_triangle(frame, triangle, ubo);
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
