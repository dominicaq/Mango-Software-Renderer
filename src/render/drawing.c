#include "drawing.h"

const vec4 WIREFRAME_COLOR = (vec4){{255, 165, 0, 255}};

// Rasterizer
// -----------------------------------------------------------------------------
void rasterize(Frame *frame, vec4 clip_space[3], UBO *ubo) {
    vec3 v[3];
    vec3 ndc[3];
    for (int i = 0; i < 3; ++i) {
        ndc[i] = homogenize_vec4(clip_space[i]);
        // Convert from clipspace -> NDC -> screen space
        v[i] = ndc_to_screen(
            frame->width,
            frame->height,
            ndc[i]
        );
    }

    // Backface culling
    if (is_backface(ndc) == true) {
        return;
    }

    // Bounding box around triangle
    int x_min = MAX(0, MIN(MIN(v[0].x, v[1].x), v[2].x));
    int y_min = MAX(0, MIN(MIN(v[0].y, v[1].y), v[2].y));
    int x_max = MIN(frame->width - 1, MAX(MAX(v[0].x, v[1].x), v[2].x));
    int y_max = MIN(frame->height - 1, MAX(MAX(v[0].x, v[1].y), v[2].y));

    // Loop through the bounding box
    for (int y = y_min; y <= y_max; ++y) {
        for (int x = x_min; x <= x_max; ++x) {
            vec3 P = {x, y, 0.0f};
            vec3 bc_coords = barycentric_coords(P, v[0], v[1], v[2]);
            // Not within triangle
            if (bc_coords.x < 0 || bc_coords.y < 0 || bc_coords.z < 0) {
                continue;
            }

            // Calculate depth of triangle
            P.z += bc_coords.x * v[0].z;
            P.z += bc_coords.y * v[1].z;
            P.z += bc_coords.z * v[2].z;

            // Determine if triangle is on top
            int buffer_index = x + y * frame->width;
            if (P.z < frame->zBuffer[buffer_index]) {
                frame->zBuffer[buffer_index] = P.z;
                fragment_shader();
                setPixel(frame->framebuffer, P.x, P.y, ubo->v_color);
            }
        }
    }
}

void draw_triangle(Frame *frame, Triangle *triangle, UBO *ubo) {
    // Average normal
   vec3 face_normal = (vec3){0.0f, 0.0f, 0.0f};
    for (int i = 0; i < 3; ++i) {
        face_normal = vec3_add(face_normal, triangle->normals[i]);
    }
    ubo->normal = normalize(face_normal);

    // Apply vertex shader
    vec4 clip_space[3];
    vec4 vertex_colors[3];
    for (int i = 0; i < 3; ++i) {
        vec4 a_position = vec3_to_vec4(triangle->vertices[i], 1.0f);
        vertex_shader(ubo, a_position);
        clip_space[i] = ubo->gl_position;
        vertex_colors[i] = ubo->v_color;
    }

    // Average colors
   vec4 new_color = (vec4){{0.0f, 0.0f, 0.0f, 0.0f}};
    for (int i = 0; i < 3; ++i) {
        new_color = vec4_add(new_color, vertex_colors[i]);
    }
    new_color.elem[0] = new_color.elem[0] / 3;
    new_color.elem[1] = new_color.elem[1] / 3;
    new_color.elem[2] = new_color.elem[2] / 3;
    new_color.elem[3] = new_color.elem[3] / 3;
    ubo->v_color = new_color;

    if (ubo->u_wireframe == true) {
        wire_frame(frame, clip_space);
    }
    rasterize(frame, clip_space, ubo);
}

void draw_mesh(Frame *frame, Mesh *mesh, UBO *ubo) {
    for (int i = 0; i < mesh->index_count; i += 3) {
        if (i + 2 > mesh->index_count) {
            break;
        }

        // Input assembly
        Triangle triangle;
        for (int j = 0; j < 3; ++j) {
            int index = i + j;
            triangle.vertices[j] = mesh->vertices[mesh->vertex_index[index]];
            triangle.normals[j]  = mesh->normals[mesh->normal_index[index]];
            triangle.uvs[j]      = mesh->uvs[mesh->uv_index[index]];
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

void wire_frame(Frame *frame, vec4 clip_space[3]) {
    vec3 v[3];
    vec3 ndc[3];
    for (int i = 0; i < 3; ++i) {
        ndc[i] = homogenize_vec4(clip_space[i]);
        // Convert from clipspace -> NDC -> screen space
        v[i] = ndc_to_screen(
            frame->width,
            frame->height,
            ndc[i]
        );
    }

    if (is_backface(ndc) == true) {
        return;
    }

    line(frame, v[0], v[1]);
    line(frame, v[1], v[2]);
    line(frame, v[2], v[0]);
}
