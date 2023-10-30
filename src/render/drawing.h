#ifndef DRAWING_H
#define DRAWING_H

#include <float.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../game/gameobject.h"
#include "../math/geometry.h"
#include "../math/vec3.h"
#include "../math/vec4.h"
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

/*
 * draw_model - Draw a 3D model
 * @frame: The frame to draw the model on
 * @mesh: The 3D model mesh
 * @mvp: The Model-View-Projection matrix
 * @wireframe: Boolean indicating whether to draw in wireframe mode
 */
void draw_mesh(Frame *frame, Mesh *mesh, UBO *ubo);

#endif  // DRAWING_H
