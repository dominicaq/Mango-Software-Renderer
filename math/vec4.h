#ifndef VEC4_H
#define VEC4_H

#include "mat4x4.h"
#include "vec3.h"

typedef struct {
    float elem[4];
} vec4;

vec4 mat_mul_vec4(const Mat4x4 m, const vec4 v) {
    vec4 ret;
    for (int i = 0; i < 4; ++i) {
        ret.elem[i] = 0.0f;
        for (int j = 0; j < 4; ++j) {
            ret.elem[i] += m.elem[i][j] * v.elem[j];
        }
    }
    return ret;
}

vec4 vec3_to_homogenous(const vec3 v, float w) {
    vec4 result;
    result.elem[0] = v.x;
    result.elem[1] = v.y;
    result.elem[2] = v.z;
    result.elem[3] = w;
    return result;
}

vec3 homogenous_to_vec3(const vec4 v) {
    if (v.elem[3] == 0.0f) {
        return (vec3){0.0f, 0.0f, 0.0f};
    }

    vec3 result = {
        v.elem[0] / v.elem[3],
        v.elem[1] / v.elem[3],
        v.elem[2] / v.elem[3]
    };
    return result;
}

#endif