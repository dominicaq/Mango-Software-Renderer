#ifndef SDF_H
#define SDF_H

#include "../math/vec2.h"
#include "../math/vec3.h"
#include "../math/vec4.h"
#include "../math/shader_math.h"

struct {
    float radius;
    Vec3 position;
    Vec3 transformed_position;
    Vec3 color;
} typedef SDFSphere;

// Signed Distance Field functions
// Source: https://iquilezles.org/articles/distfunctions/
// -----------------------------------------------------------------------------

float sdf_sphere(Vec3 p, float s);

float sdf_box(Vec3 p, Vec3 b);

float sdf_torus(Vec3 p, Vec2 t);

// Signed Distance Field Operations
// Source: https://iquilezles.org/articles/distfunctions/
// -----------------------------------------------------------------------------

float sdf_op_union(float d1, float d2);

float sdf_op_sub(float d1, float d2);

float sdf_op_intersect(float d1, float d2);

float sdf_op_smooth_union(float d1, float d2, float k);

float sdf_op_smooth_sub(float d1, float d2, float k);

float sdf_op_smooth_intersect(float d1, float d2, float k);

#endif