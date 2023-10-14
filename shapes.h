#ifndef SHAPES_H
#define SHAPES_H

#include "tga.h"
#include "math/vec3.h"
#include "math/vec2.h"
#include "obj_parser.h"

// Helper function(s)
// -----------------------------------------------------------------------------
void swap_int(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

vec2 world_to_screen_coords(vec3 world_coord, TGAImage* frame_buffer) {
    // Preserve aspect ratio
    float scaleX = frame_buffer->width * 0.5f;
    float scaleY = frame_buffer->height * 0.5f;

    vec2 screen_coord = {
        (world_coord.x + 1.0) * scaleX,
        (1.0 - (world_coord.y + 1.0)) * scaleY
    };
    return screen_coord;
}

// Wireframe mode
// -----------------------------------------------------------------------------
// Bresenham’s Line Drawing Algorithm
// Source: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void line(vec2 v0, vec2 v1, TGAImage *frame_buffer, TGAColor color) {
    int x0 = v0.x;
    int x1 = v1.x;
    int y0 = v0.y;
    int y1 = v1.y;

    int steep = 0;
    if (abs(x0 - x1) < abs(y0 - y1)) {
        swap_int(&x0, &y0);
        swap_int(&x1, &y1);
        steep = 1;
    }
    if (x0 > x1) {
        swap_int(&x0, &x1);
        swap_int(&y0, &y1);
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

void wire_frame(vec2 verts[3], TGAImage* frame_buffer, TGAColor color) {
    line(verts[0], verts[1], frame_buffer, color);
    line(verts[1], verts[2], frame_buffer, color);
    line(verts[2], verts[0], frame_buffer, color);
}

// Triangle rasterization
// -----------------------------------------------------------------------------
// Triangle rasterizer
// Source: http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
void rasterize(vec2 verts[3], TGAImage* frame_buffer, TGAColor color) {
    // Note: Z buffer is applied in here rasterization
    // https://web.eecs.utk.edu/~huangj/cs452/notes/452_rasterization.pdf

    vec2 vs1 = {verts[1].x - verts[0].x, verts[1].y - verts[0].y};
    vec2 vs2 = {verts[2].x - verts[0].x, verts[2].y - verts[0].y};

    // Compute min and max values
    int minX = verts[0].x;
    int minY = verts[0].y;
    int maxX = verts[0].x;
    int maxY = verts[0].y;

    for (int i = 1; i < 3; i++) {
        int x = verts[i].x;
        int y = verts[i].y;

        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }

    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            vec2 q = {x - verts[0].x, y - verts[0].y};
            float s = v2_cross(q, vs2) / v2_cross(vs1, vs2);
            float t = v2_cross(vs1, q) / v2_cross(vs1, vs2);

            // Inside the triangle
            if (s >= 0.0f && t >= 0.0f && s + t <= 1.0f) {
                setPixel(frame_buffer, x, y, color);
            }
        }
    }
}

// Drawing
// -----------------------------------------------------------------------------
void draw_triangle(TGAImage* frame_buffer, vec3 world_coords[3], TGAColor color) {
    vec2 screen_coords[3];

    // Iterate over the vertices of the triangle
    for (int i = 0; i < 3; ++i) {
        // TODO: Perspective matrix here
        screen_coords[i] = world_to_screen_coords(world_coords[i], frame_buffer);
        // TEMP: Shift the coords into camera for now
        screen_coords[i].y += 150;
    }

    // vec3 tri_normal = cross(
    //     sub(world_coords[2], world_coords[0]),
    //     sub(world_coords[1], world_coords[0])
    // );

    TGAColor orange = createTGAColor(255, 165, 0, 255);
    rasterize(screen_coords, frame_buffer, color);
    wire_frame(screen_coords, frame_buffer, orange);
}

void draw_model(TGAImage* frame_buffer, MeshData* mesh, TGAColor color) {
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
        draw_triangle(frame_buffer, triangle, color);
    }
}
#endif // SHAPES_HH