#ifndef SHAPES_H
#define SHAPES_H

#include <float.h>

#include "tga.h"
#include "math/vec3.h"
#include "math/vec2.h"
#include "obj_parser.h"

// Rasterization source:
// https://github.com/ssloy/tinyrenderer/blob/68a5ae382135d679891423fb5285fdd582ca389d/main.cpp

// Helper functions
// -----------------------------------------------------------------------------
vec3 barycentric(vec3 p, vec3 a, vec3 b, vec3 c) {
    vec3 v0 = sub(b, a);
    vec3 v1 = sub(c, a);
    vec3 v2 = sub(p, a);
    float den = v0.x * v1.y - v1.x * v0.y;

    // Check for a degenerate triangle
    vec3 result = {-1.0f, -1.0f, - 1.0f};
    if (fabsf(den) < 1e-6) {
        return result;
    }

    result.x = (v2.x * v1.y - v1.x * v2.y) / den; // U
    result.y = (v0.x * v2.y - v2.x * v0.y) / den; // V
    result.z = 1.0f - result.x - result.y;        // W
    return result;
}

vec3 world_to_screen(vec3 v, TGAImage *frame_buffer) {
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
void line(vec3 v0, vec3 v1, TGAImage *frame_buffer, TGAColor color) {
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

void wire_frame(vec3 verts[3], TGAImage *frame_buffer, TGAColor color) {
    line(verts[0], verts[1], frame_buffer, color);
    line(verts[1], verts[2], frame_buffer, color);
    line(verts[2], verts[0], frame_buffer, color);
}

// Triangle rasterization
// -----------------------------------------------------------------------------
// Triangle rasterizer
// Source: http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
void rasterize(vec3 verts[3], int *zbuffer, TGAImage *frame_buffer, TGAColor color) {
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

    for (int x = bboxmin.x; x <= bboxmax.x; x++) {
        for (int y = bboxmin.y; y <= bboxmax.y; y++) {
            vec3 P = {x, y, 0.0f};
            vec3 bc_screen = barycentric(P, verts[0], verts[1], verts[2]);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) {
                continue;
            }

            for (int i = 0; i < 3; i++) {
                P.z += verts[i].z * bc_screen.x;
            }

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
void draw_triangle(TGAImage *frame_buffer, int *zbuffer, vec3 verts[3], TGAColor color) {
    vec3 screen_coords[3];

    for (int i = 0; i < 3; ++i) {
        // TODO: Perspective matrix here
        screen_coords[i] = world_to_screen(verts[i], frame_buffer);
        // TEMP: Shift the coords into camera for now
        screen_coords[i].y += 150;
    }

    // TODO: Rasterizer is a bit off
    TGAColor orange = createTGAColor(255, 165, 0, 255);
    // rasterize(screen_coords, zbuffer, frame_buffer, color);
    wire_frame(screen_coords, frame_buffer, orange);
}

void draw_model(TGAImage *frame_buffer, int *zbuffer, Model *mesh, TGAColor color) {
    for (int i = 0; i < mesh->index_count; i += 3) {
        if (i + 2 > mesh->index_count) {
            break;
        }

        // Construct a triangle with appropriate indices
        vec3 triangle[3];
        triangle[0] = mesh->vertices[mesh->vertex_index[i]];
        triangle[1] = mesh->vertices[mesh->vertex_index[i+1]];
        triangle[2] = mesh->vertices[mesh->vertex_index[i+2]];

        // Rasterizer
        draw_triangle(frame_buffer, zbuffer, triangle, color);
    }
}
#endif // SHAPES_HH