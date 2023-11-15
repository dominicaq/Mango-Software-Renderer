#include "sdf.h"

float sdf_sphere(Vec3 p, float s) { return vec3_magnitude(p) - s; }