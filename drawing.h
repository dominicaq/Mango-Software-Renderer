#ifndef DRAWING_H
#define DRAWING_H

#include <float.h>

#include "tga.h"
#include "math/vec3.h"
#include "math/vec2.h"
#include "obj_parser.h"

// Rasterization source:
// https://github.com/ssloy/tinyrenderer/blob/68a5ae382135d679891423fb5285fdd582ca389d/main.cpp

typedef struct {
    vec3 vertices[3];
    vec2 uvs[3];
    vec3 face_normal;
} Triangle;

// Helper functions
// -----------------------------------------------------------------------------
vec3 barycentric(vec3 p, vec3 a, vec3 b, vec3 c) {
    vec3 v0 = vec3_sub(b, a);
    vec3 v1 = vec3_sub(c, a);
    vec3 v2 = vec3_sub(p, a);
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

vec3 world_to_screen(TGAImage *framebuffer, vec3 v) {
    // Preserve aspect ratio
    float scaleX = framebuffer->width * 0.5f;
    float scaleY = framebuffer->height * 0.5f;

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

int min(int a, int b) {
    if (a > b) {
        return b;
    }
    return a;
}

int max(int a, int b) {
    if (a < b) {
        return b;
    }
    return a;
}

// Wireframe mode
// -----------------------------------------------------------------------------
// Bresenham’s Line Drawing Algorithm
// Source: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void line(TGAImage *framebuffer, vec3 v0, vec3 v1, TGAColor color) {
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
            setPixel(framebuffer, y, x, color);
        } else {
            setPixel(framebuffer, x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void wire_frame(TGAImage *framebuffer, vec3 verts[3], TGAColor color) {
    line(framebuffer, verts[0], verts[1], color);
    line(framebuffer, verts[1], verts[2], color);
    line(framebuffer, verts[2], verts[0], color);
}

// Triangle rasterization
// -----------------------------------------------------------------------------
// Triangle rasterizer
// Source: http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
void rasterize(TGAImage *framebuffer, int *zbuffer, vec3 verts[3], TGAColor color) {
    vec3 v0 = verts[0];
    vec3 v1 = verts[1];
    vec3 v2 = verts[2];

    // Bounding box
    int x_min = min(min(v0.x, v1.x), v2.x);
    int y_min = min(min(v0.y, v1.y), v2.y);
    int x_max = max(max(v0.x, v1.x), v2.x);
    int y_max = max(max(v0.y, v1.y), v2.y);

    for (int y = y_min; y <= y_max; ++y) {
        for (int x = x_min; x <= x_max; ++x) {
            vec3 P = {x, y, 0.0f};
            vec3 bc_screen = barycentric(P, v0, v1, v2);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) {
                continue;
            }

            for (int i = 0; i < 3; i++) {
                P.z += verts[i].z * bc_screen.z;
            }

            // Apply z-buffer
            int z_index = P.x + P.y * framebuffer->width;
            if (zbuffer[z_index] < P.z) {
                zbuffer[z_index] = P.z;
                setPixel(framebuffer, P.x, P.y, color);
            }
        }
    }
}

// Drawing
// -----------------------------------------------------------------------------
void draw_triangle(TGAImage *framebuffer, int *zbuffer, Triangle *triangle, TGAColor color) {
    vec3 screen_coords[3];

    for (int i = 0; i < 3; ++i) {
        // TODO: Perspective matrix here
        screen_coords[i] = world_to_screen(framebuffer, triangle->vertices[i]);
        // TEMP: Shift the coords into camera for now
        screen_coords[i].y += 250;
    }

    // Vertex shading
    vec3 light_pos = {0.0f, 3.0f, 5.0f};
    float intensity = dot(normalize(light_pos), triangle->face_normal);
    TGAColor face_lighting = createTGAColor(
        color.r * intensity,
        color.g * intensity,
        color.b * intensity,
        color.a
    );

    // Draw triangle
    if (intensity > 0) {
        rasterize(framebuffer, zbuffer, screen_coords, face_lighting);
    }

    // Wireframe
    // TGAColor orange = createTGAColor(255, 165, 0, 255);
    // wire_frame(framebuffer, screen_coords, orange);
}

void draw_model(TGAImage *framebuffer, int *zbuffer, Model *mesh, TGAColor color) {
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
        vec3 face_normal = {0.0f, 0.0f, 0.0f};
        face_normal = vec3_add(face_normal, mesh->normals[mesh->normal_index[i]]);
        face_normal = vec3_add(face_normal, mesh->normals[mesh->normal_index[i+1]]);
        face_normal = vec3_add(face_normal, mesh->normals[mesh->normal_index[i+2]]);
        face_normal = normalize(face_normal);
        current_triangle.face_normal = face_normal;

        // UV
        current_triangle.uvs[0] = mesh->uvs[mesh->uv_index[i]];
        current_triangle.uvs[1] = mesh->uvs[mesh->uv_index[i+1]];
        current_triangle.uvs[2] = mesh->uvs[mesh->uv_index[i+2]];

        // Rasterizer
        draw_triangle(framebuffer, zbuffer, &current_triangle, color);
    }
}

int *init_zbuffer(TGAImage *framebuffer) {
    int size = framebuffer->width * framebuffer->height;
    int *zbuffer = malloc(sizeof(int) * size);
    if (zbuffer == NULL) {
        return NULL;
    }

    // Init zbuffer to have int_min
    for (int i = 0; i < size; ++i) {
        zbuffer[i] = -INT_MAX;
    }
    return zbuffer;
}

#endif // DRAWING_H