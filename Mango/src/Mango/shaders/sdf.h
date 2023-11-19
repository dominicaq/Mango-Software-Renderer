#ifndef SDF_H
#define SDF_H

#include "../game/camera.h"
#include "../math/vec2.h"
#include "../math/vec3.h"
#include "../math/vec4.h"
#include "../math/mat4.h"
#include "../math/shader_math.h"
#include "../render/framedata.h"

struct {
    Vec3 position;
    float radius;
    Vec3 color;
    Vec3 ndc;
} typedef SDFSphere;

#define SDF_EPSILON 0.0001
#define SDF_MAX_MARCH_STEPS 255
#define SDF_START 0
#define SDF_END 100

// Render Pipeline
// -----------------------------------------------------------------------------
float scene_sdf(Vec3 sample_point);

void sdf_draw(Frame *frame, const Camera *camera);

Vec3 sdf_estimate_normal(Vec3 p);

// SDF Helpers

Vec3 sdf_ray_dir(Vec2 uv, Vec2 size, float fov);

float sdf_ray_march(Vec3 origin, Vec3 direction);

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