#ifndef DRAWING_H
#define DRAWING_H

#include <float.h>

#include "tga.h"
#include "math/vec3.h"
#include "math/vec2.h"
#include "obj_parser.h"

// Rasterization source:
// https://github.com/ssloy/tinyrenderer/blob/68a5ae382135d679891423fb5285fdd582ca389d/main.cpp

// Helper functions
// -----------------------------------------------------------------------------
typedef struct {
    vec3 vertices[3];
    vec3 face_normal;
} Triangle;

vec3 barycentric(vec3 p, vec3 a, vec3 b, vec3 c) {
    vec3 v0 = sub(b, a);
    vec3 v1 = sub(c, a);
    vec3 v2 = sub(p, a);
    float den = v0.x * v1.y - v1.x * v0.y;

    // Check for a degenerate triangle
    vec3 result = {-1.0f, -1.0f, -1.0f};
    if (fabsf(den) < 1) {
        return result;
    }

    result.x = (v2.x * v1.y - v1.x * v2.y) / den; // U
    result.y = (v0.x * v2.y - v2.x * v0.y) / den; // V
    result.z = 1.0f - result.x - result.y;        // W
    return result;
}

vec3 world_to_screen(TGAImage *frame_buffer, vec3 v) {
    // Preserve aspect ratio
    float scaleX = frame_buffer->width * 0.5f;
    float scaleY = frame_buffer->height * 0.5f;

    vec3 screen_coord = {
        (v.x + 1.0) * scaleX,
        (1.0 - (v.y + 1.0)) * scaleY,
        v.z
    };
    return screen_coord;
}

void swap_ints(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Wireframe mode
// -----------------------------------------------------------------------------
// Bresenham’s Line Drawing Algorithm
// Source: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void line(TGAImage *frame_buffer, vec3 v0, vec3 v1, TGAColor color) {
    int x0 = v0.x;
    int x1 = v1.x;
    int y0 = v0.y;
    int y1 = v1.y;

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
            setPixel(frame_buffer, y, x, color);
        } else {
            setPixel(frame_buffer, x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void wire_frame(TGAImage *frame_buffer, vec3 verts[3], TGAColor color) {
    line(frame_buffer, verts[0], verts[1], color);
    line(frame_buffer, verts[1], verts[2], color);
    line(frame_buffer, verts[2], verts[0], color);
}

// Triangle rasterization
// -----------------------------------------------------------------------------
// Triangle rasterizer
// Source: http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
void rasterize(TGAImage *frame_buffer, int *zbuffer, vec3 verts[3], TGAColor color) {
    vec2 bboxmin = {FLT_MAX, FLT_MAX};
    vec2 bboxmax = {-FLT_MAX, -FLT_MAX};
    vec2 clamp = {frame_buffer->width - 1, frame_buffer->height - 1};

    for (int i = 0; i < 3; i++) {
        // Update for x coordinate
        bboxmin.x = fmaxf(0.0f, fminf(bboxmin.x, verts[i].x));
        bboxmax.x = fminf(clamp.x, fmaxf(bboxmax.x, verts[i].x));

        // Update for y coordinate
        bboxmin.y = fmaxf(0.0f, fminf(bboxmin.y, verts[i].y));
        bboxmax.y = fminf(clamp.y, fmaxf(bboxmax.y, verts[i].y));
    }

    for (int x = bboxmin.x; x <= bboxmax.x; ++x) {
        for (int y = bboxmin.y; y <= bboxmax.y; ++y) {
            vec3 P = {x, y, 0.0f};
            vec3 bc_screen = barycentric(P, verts[0], verts[1], verts[2]);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) {
                continue;
            }

            for (int i = 0; i < 3; i++) {
                P.z += verts[i].z * (bc_screen.z * bc_screen.y * bc_screen.x);
            }

            // Apply z-buffer
            int z_index = P.x + P.y * frame_buffer->width;
            if (zbuffer[z_index] < P.z) {
                zbuffer[z_index] = P.z;
                setPixel(frame_buffer, P.x, P.y, color);
            }
        }
    }
}

// Drawing
// -----------------------------------------------------------------------------
void draw_triangle(TGAImage *frame_buffer, int *zbuffer, Triangle *triangle, TGAColor color) {
    vec3 screen_coords[3];

    for (int i = 0; i < 3; ++i) {
        // TODO: Perspective matrix here
        screen_coords[i] = world_to_screen(frame_buffer, triangle->vertices[i]);
        // TEMP: Shift the coords into camera for now
        screen_coords[i].y += 250;
    }

    // Fragment lighting
    vec3 light_pos = {0.0f, 0.0f, 2.0f};
    float intensity = dot(normalize(light_pos), triangle->face_normal);
    TGAColor diffuse_lighting = createTGAColor(
        color.r * intensity,
        color.g * intensity,
        color.b * intensity,
        color.a
    );

    // Draw face
    rasterize(frame_buffer, zbuffer, screen_coords, diffuse_lighting);

    // Wireframe
    TGAColor orange = createTGAColor(255, 165, 0, 255);
    wire_frame(frame_buffer, screen_coords, orange);
}

void draw_model(TGAImage *frame_buffer, int *zbuffer, Model *mesh, TGAColor color) {
    for (int i = 0; i < mesh->index_count; i += 3) {
        if (i + 2 > mesh->index_count) {
            break;
        }

        // Construct a single triangle
        Triangle current_triangle;

        // Vertex
        current_triangle.vertices[0] = mesh->vertices[mesh->vertex_index[i]];
        current_triangle.vertices[1] = mesh->vertices[mesh->vertex_index[i+1]];
        current_triangle.vertices[2] = mesh->vertices[mesh->vertex_index[i+2]];

        // Normals
        vec3 face_normal;
        face_normal = add(face_normal, mesh->normals[mesh->normal_index[i]]);
        face_normal = add(face_normal, mesh->normals[mesh->normal_index[i+1]]);
        face_normal = add(face_normal, mesh->normals[mesh->normal_index[i+2]]);
        face_normal = normalize(face_normal);
        current_triangle.face_normal = face_normal;

        // Rasterizer
        draw_triangle(frame_buffer, zbuffer, &current_triangle, color);
    }
}
#endif // DRAWING_H