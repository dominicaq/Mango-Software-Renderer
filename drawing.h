#ifndef DRAWING_H
#define DRAWING_H

#include <float.h>

#include "tga.h"
#include "math/vec3.h"
#include "math/vec2.h"
#include "obj_parser.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
    vec3 vertices[3];
    vec2 uvs[3];
    vec3 face_normal;
} Triangle;

// Helper functions
// -----------------------------------------------------------------------------
// Efficient barycentric coordinates
// Source: https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
vec3 barycentric_coords(vec3 p, vec3 a, vec3 b, vec3 c) {
    vec3 v0 = vec3_sub(b,a), v1 = vec3_sub(c,a), v2 = vec3_sub(p,a);

    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);
    float invDenom = 1.0f / (d00 * d11 - d01 * d01);
    vec3 result;
    result.x = (d11 * d20 - d01 * d21) * invDenom; // V
    result.y = (d00 * d21 - d01 * d20) * invDenom; // W
    result.z = 1.0f - result.x - result.y;         // U
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

float *init_zbuffer(TGAImage *framebuffer) {
    int buffer_size = framebuffer->width * framebuffer->height;
    float *zbuffer = malloc(sizeof(float) * buffer_size);
    if (zbuffer == NULL) {
        return NULL;
    }

    // Init zbuffer to be "far away"
    for (int i = 0; i < buffer_size; ++i) {
        zbuffer[i] = -1.0f;
    }
    return zbuffer;
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

// Drawing
// -----------------------------------------------------------------------------
// Source: http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
void rasterize(TGAImage *framebuffer, float *zbuffer, vec3 verts[3], TGAColor color) {
    vec3 v0 = verts[0];
    vec3 v1 = verts[1];
    vec3 v2 = verts[2];

    // Bounding box
    int x_min = MAX(0, MIN(MIN(v0.x, v1.x), v2.x));
    int y_min = MAX(0, MIN(MIN(v0.y, v1.y), v2.y));
    int x_max = MIN(framebuffer->width - 1, MAX(MAX(v0.x, v1.x), v2.x));
    int y_max = MIN(framebuffer->height - 1, MAX(MAX(v0.y, v1.y), v2.y));

    // Loop through the bounding box
    for (int y = y_min; y <= y_max; ++y) {
        for (int x = x_min; x <= x_max; ++x) {
            vec3 P = {x, y, 0.0f};
            vec3 bc_screen = barycentric_coords(P, v0, v1, v2);
            // Not within triangle
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) {
                continue;
            }

            // Calculate depth of polygon
            P.z += verts[0].z * bc_screen.x;
            P.z += verts[1].z * bc_screen.y;
            P.z += verts[2].z * bc_screen.z;

            // Apply z-buffer
            int z_index = P.x + P.y * framebuffer->width;
            if (zbuffer[z_index] < P.z) {
                zbuffer[z_index] = P.z;
                setPixel(framebuffer, P.x, P.y, color);
            }
        }
    }
}

void draw_triangle(TGAImage *framebuffer, float *zbuffer, Triangle *triangle, TGAColor color) {
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

void draw_model(TGAImage *framebuffer, float *zbuffer, Model *mesh, TGAColor color) {
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

#endif // DRAWING_H