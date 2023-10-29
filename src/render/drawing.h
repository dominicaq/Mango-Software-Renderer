#ifndef DRAWING_H
#define DRAWING_H

#include <stdlib.h>
#include <float.h>
#include <stdbool.h>

#include "../math/vec3.h"
#include "../math/vec4.h"
#include "../math/geometry.h"
#include "../mesh/mesh.h"

#include "../shaders/shader.h"
#include "framedata.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
    Vec3 position;
    Vec3 normal;
    Vec4 color;
    Vec2 uv;
} Vertex;

const extern Vec4 WIREFRAME_COLOR;

// Wireframe mode
// -----------------------------------------------------------------------------
/*
 * line - Draw a line using Bresenham's Line Drawing Algorithm
 * @frame: The frame to draw the line on
 * @v0: The first endpoint of the line
 * @v1: The second endpoint of the line
 * @color: The color of the line
 *
 * This function draws a line on the frame using Bresenham's Line Drawing Algorithm.
 *
 * Source: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 */
void line(Frame *frame, Vec3 v0, Vec3 v1);

/*
 * wire_frame - Draw wireframe triangle
 * @frame: The frame to draw the wireframe on
 * @clip_space: An array of three 4D clip space coordinates
 * @color: The color of the wireframe
 *
 * This function draws a wireframe triangle on the frame.
 */
void wire_frame(Frame *frame, Vec3 screen_space[3]);

/*
 * draw - Draw a triangle
 * @frame: The frame to draw the triangle on
 * @triangle: The triangle to be drawn
 * @mvp: The Model-View-Projection matrix
 * @color: The color of the triangle
 * @wireframe: Boolean indicating whether to draw in wireframe mode
 */
void transform_triangle(Frame *frame, Vertex *vertices, UBO *ubo);

/*
 * draw_model - Draw a 3D model
 * @frame: The frame to draw the model on
 * @mesh: The 3D model mesh
 * @mvp: The Model-View-Projection matrix
 * @wireframe: Boolean indicating whether to draw in wireframe mode
 */
void draw_mesh(Frame *frame, Mesh *mesh, UBO *ubo);

void rasterize(Frame *frame, Vec3 ss[3], Vec3 model_space[3], Vec3 normals[3], UBO *ubo);

void draw_triangle(Frame *frame, Vec3 clip_space[3], Vec3 normals[3], UBO *ubo);

#endif // DRAWING_H
