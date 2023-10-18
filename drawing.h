#ifndef DRAWING_H
#define DRAWING_H

#include <float.h>
#include <stdbool.h>

#include "math/vec3.h"
#include "math/vec4.h"
#include "math/geometry.h"

#include "framedata.h"
#include "obj_parser.h"

// Temp
#include "gameobject/transform.h"
#include "gameobject/camera.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
    vec3 vertices[3];
    vec2 uvs[3];
    vec3 normals[3];
} Triangle;

// Wireframe mode
// -----------------------------------------------------------------------------
// Bresenham’s Line Drawing Algorithm
// Source: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
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

void wire_frame(Frame *frame, vec3 verts[3], TGAColor color) {
    line(frame, verts[0], verts[1], color);
    line(frame, verts[1], verts[2], color);
    line(frame, verts[2], verts[0], color);
}

// Drawing
// -----------------------------------------------------------------------------
// Source: http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
void rasterize(Frame *frame, vec4 clip_space[3], TGAColor color, bool wireframe) {
    // NOTE: Fragment related code exist here
    vec3 v[3];
    for (int i = 0; i < 3; ++i) {
        // Convert from NDC to screen position
        v[i] = ndc_to_screen(
            frame->width,
            frame->height,
            homogenous_to_vec3(clip_space[i]) // NDC
        );
    }

    if (wireframe == true) {
        TGAColor orange = createTGAColor(255, 165, 0, 255);
        wire_frame(frame, v, orange);
    }

    // Bounding box around triangle
    int x_min = MAX(0, MIN(MIN(v[0].x, v[1].x), v[2].x));
    int y_min = MAX(0, MIN(MIN(v[0].x, v[1].y), v[2].y));
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
            if (P.z > frame->zBuffer[buffer_index]) {
                frame->zBuffer[buffer_index] = P.z;
                setPixel(frame->framebuffer, P.x, P.y, color);
            }
        }
    }
}

void draw(Frame *frame, Triangle *triangle, Mat4x4 mvp, TGAColor color) {
    // NOTE: Vertex related code exist here
    // Apply transformations to each vertex
    vec4 clip_space[3];
    for (int i = 0; i < 3; ++i) {
        vec4 a_pos = vec3_to_homogenous(triangle->vertices[i], 1.0f);
        clip_space[i] = mat_mul_vec4(mvp, a_pos);
    }

    // Flat shading
    vec3 light_pos = {0.0f, 0.0f, 1.0f};
    vec3 face_normal = triangle->normals[0];
    face_normal = vec3_add(face_normal, triangle->normals[1]);
    face_normal = vec3_add(face_normal, triangle->normals[2]);
    face_normal = normalize(face_normal);
    float intensity = dot(normalize(light_pos), face_normal);
    TGAColor face_lighting = createTGAColor(
        color.r * intensity,
        color.g * intensity,
        color.b * intensity,
        color.a
    );

    // Draw triangle
    if (intensity > 0) {
        rasterize(frame, clip_space, face_lighting, false);
    }
}

void draw_model(Frame *frame, Model *mesh, Mat4x4 mvp) {
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

        // Rasterize
        draw(frame, &triangle, mvp, mesh->color);
    }
}

#endif // DRAWING_H