#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>

#include "../math/vec4.h"
#include "../math/vec3.h"
#include "../math/mat4x4.h"

const extern bool FLAT_SHADING;
const extern bool SMOOTH_SHADING;
const extern bool PHONG_SHADING;

void vertex();

void fragment();

#endif // SHADER_H