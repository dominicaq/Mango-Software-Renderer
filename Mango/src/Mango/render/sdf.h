#ifndef SDF_H
#define SDF_H

#include "../math/vec3.h"
#include "../math/vec4.h"

struct {
    float radius;
    Vec3 position;
    Vec3 transformed_position;
    Vec3 color;
} typedef SDFSphere;

// Shapes source:
// https://iquilezles.org/articles/distfunctions/
float sdf_sphere(Vec3 p, float s);

#endif