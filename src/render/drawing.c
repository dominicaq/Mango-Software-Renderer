#include <stdlib.h>
#include "drawing.h"


// Rasterizer
// -----------------------------------------------------------------------------
void rasterize(Frame *frame, vec4 clip_space[3], TGAColor color) {
    // NOTE: Fragment related code exist here
    // NOTE: NDC will be used for backface culling
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
                setPixel(frame->framebuffer, P.x, P.y, color);
            }
        }
    }
}

void draw(Frame *frame, Triangle *triangle, Mat4x4 mvp, TGAColor color, bool wireframe) {
    // NOTE: Vertex related code exist here
    // Apply transformations to each vertex
    vec4 clip_space[3];
    for (int i = 0; i < 3; ++i) {
        vec4 a_pos = vec3_to_vec4(triangle->vertices[i], 1.0f);
        clip_space[i] = mat_mul_vec4(mvp, a_pos);

        vec4 a_norm = mat_mul_vec4(mvp, vec3_to_vec4(triangle->normals[i], 0.0f));
        triangle->normals[i] = homogenize_vec4(a_norm);
    }

    // Flat shading (vertex lighting)
    vec3 light_pos = {1.0f, 0.0f, 0.0f};
    vec3 norm_avg = triangle->normals[0];
    norm_avg = vec3_add(norm_avg, triangle->normals[1]);
    norm_avg = vec3_add(norm_avg, triangle->normals[2]);
    norm_avg = normalize(norm_avg);
    float intensity = dot(normalize(light_pos), norm_avg);
    TGAColor face_lighting = createTGAColor(
        color.r * intensity,
        color.g * intensity,
        color.b * intensity,
        color.a
    );

    if (intensity < 0.0f) {
        face_lighting.r = 10;
        face_lighting.b = 10;
        face_lighting.g = 10;
    }
    rasterize(frame, clip_space, face_lighting);

    if (wireframe == true) {
        TGAColor orange = createTGAColor(255, 165, 0, 255);
        wire_frame(frame, clip_space, orange);
    }
}

void draw_model(Frame *frame, Model *mesh, Mat4x4 mvp, bool wireframe) {
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

        // Draw a single triangle
        draw(frame, &triangle, mvp, mesh->color, wireframe);
    }
}

void swap_ints(int *a, int *b) {
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

// Wireframe mode
// -----------------------------------------------------------------------------
void line(Frame *frame, vec3 v0, vec3 v1, TGAColor color) {
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
            setPixel(frame->framebuffer, y, x, color);
        } else {
            setPixel(frame->framebuffer, x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void wire_frame(Frame *frame, vec4 clip_space[3], TGAColor color) {
    // NOTE: NDC will be used for backface culling
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

    line(frame, v[0], v[1], color);
    line(frame, v[1], v[2], color);
    line(frame, v[2], v[0], color);
}
