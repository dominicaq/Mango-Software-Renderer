#ifndef DRAWING_H
#define DRAWING_H

#include <float.h>
#include <stdbool.h>

#include "../game/camera.h"
#include "../game/gameobject.h"
#include "../math/geometry.h"
#include "../math/shader_math.h"
#include "../math/vec2.h"
#include "../math/vec3.h"
#include "../math/vec4.h"
#include "../shaders/sdf.h"
#include "../shaders/shader.h"
#include "framedata.h"

const extern Vec4 WIREFRAME_COLOR;

/*
 * Render pipeline
*/
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
    Vec3 position;
    Vec3 normal;
    Vec4 color;
    Vec2 uv;
} Vertex;

/*
 * draw_model - Draw a 3D model
 * @frame: The frame to draw the model on
 * @mesh: The 3D model mesh
 * @mvp: The Model-View-Projection matrix
 */
void draw_mesh(Frame *frame, Mesh *mesh, UBO *ubo);

/*
 * Clipping planes
*/

typedef struct {
    Vec3 normal;
    float distance;
} Plane;

#define NUM_CLIP_PLANES 5

extern Plane clip_planes[NUM_CLIP_PLANES];

void init_clip_planes();

#endif  // DRAWING_H
