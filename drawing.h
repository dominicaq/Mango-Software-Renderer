#ifndef DRAWING_H
#define DRAWING_H

#include <float.h>

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
void rasterize(Frame *frame, vec3 verts[3], TGAColor color) {
    vec3 v0 = verts[0];
    vec3 v1 = verts[1];
    vec3 v2 = verts[2];

    // Bounding box
    int x_min = MAX(0, MIN(MIN(v0.x, v1.x), v2.x));
    int y_min = MAX(0, MIN(MIN(v0.y, v1.y), v2.y));
    int x_max = MIN(frame->width - 1, MAX(MAX(v0.x, v1.x), v2.x));
    int y_max = MIN(frame->height - 1, MAX(MAX(v0.y, v1.y), v2.y));

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
            P.z += (verts[0].z * 0.5 + 0.5) * bc_screen.x;
            P.z += (verts[1].z * 0.5 + 0.5) * bc_screen.y;
            P.z += (verts[2].z * 0.5 + 0.5) * bc_screen.z;


            // Apply z-buffer
            int buffer_index = P.x + P.y * frame->width;
            if (frame->zBuffer[buffer_index] < P.z) {
                frame->zBuffer[buffer_index] = ceil(P.z);
                setPixel(frame->framebuffer, P.x, P.y, color);
            }
        }
    }
}

void draw_triangle(Frame *frame, Triangle *triangle, TGAColor color) {
    vec3 screen_coords[3];

    // TODO: TEMP CODE PLACEMENT
    // Camera properties
    vec3 camera_pos = {0.0f, 0.0f, -5.0f};
    vec3 cam_euler_angles = {0.0f, 0.0f, 0.0f};
    Camera camera;
    camera.transform.position = camera_pos;
    camera.transform.eulerAngles = cam_euler_angles;
    camera.fov = 90.0f;
    camera.aspect = (float)(frame->width) / (float)(frame->height);
    camera.zNear = 0.01f;
    camera.zFar = 500.0f;

    Transform model_transform;
    vec3 model_pos = {0.0f, 0.0f, -5.0f};
    vec3 model_euler = {0.0f, 0.0f, 0.0f};
    vec3 model_scale = {6.0f, 6.0f, 6.0f};
    model_transform.position = model_pos;
    model_transform.eulerAngles = model_euler;
    model_transform.scale = model_scale;

    Mat4x4 model_matrix = create_model_matrix(model_transform);
    Mat4x4 view_matrix = view(&camera);
    Mat4x4 projection_matrix = perspective(&camera);

    // MVP Matrix: projection * view * model
    Mat4x4 mvp = mat_mul(projection_matrix, mat_mul(view_matrix, model_matrix));

    // Apply transformations to each vertex
    for (int i = 0; i < 3; ++i) {
        vec4 homogenous_coords = vec3_to_homogenous(triangle->vertices[i], 1.0f);
        vec4 new_pos_clip_space = mat_mul_vec4(mvp, homogenous_coords);

        // Convert from clip space to NDC by performing perspective divide
        vec3 new_pos_ndc = homogenous_to_vec3(new_pos_clip_space);
        screen_coords[i] = ndc_to_screen(
            frame->width,
            frame->height,
            new_pos_ndc
        );

        // Flat
        // screen_coords[i] = world_to_screen(
        //     frame->width,
        //     frame->height,
        //     triangle->vertices[i]
        // );
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
        rasterize(frame, screen_coords, face_lighting);
    }

    // Wireframe
    // TGAColor orange = createTGAColor(255, 165, 0, 255);
    // wire_frame(frame, screen_coords, orange);
}

void draw_model(Frame *frame, Model *mesh, TGAColor color) {
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
        draw_triangle(frame, &triangle, color);
    }
}

#endif // DRAWING_H