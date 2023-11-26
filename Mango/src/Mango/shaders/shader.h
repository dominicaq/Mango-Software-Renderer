#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>

#include "ubo.h"

#include "../game/gameobject.h"
#include "../math/geometry.h"
#include "../math/mat4.h"
#include "../math/vec3.h"
#include "../math/vec4.h"
#include "../math/shader_math.h"

const extern bool FLAT_SHADING;
const extern bool SMOOTH_SHADING;
const extern bool PHONG_SHADING;

void vertex_shader(UBO *ubo, Vec4 a_position);

void fragment_shader(UBO *ubo, Vec3 frag_coord);

#endif // SHADER_H
