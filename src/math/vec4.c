#include"vec4.h"

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

vec4 vec3_to_vec4(const vec3 v, float w) {
    vec4 result;
    result.elem[0] = v.x;
    result.elem[1] = v.y;
    result.elem[2] = v.z;
    result.elem[3] = w;
    return result;
}

vec3 homogenize_vec4(const vec4 v) {
    float epsilon = 1e-6;
    float w = v.elem[3];
    if (fabs(w) < epsilon) {
        return (vec3){0.0f, 0.0f, 0.0f};
    }

    vec3 result = {
        v.elem[0] / w,
        v.elem[1] / w,
        v.elem[2] / w
    };
    return result;
}

vec4 color_scale(const float scale, const vec4 v) {
    vec4 result;
    result.elem[0] = v.elem[0] * scale;
    result.elem[1] = v.elem[1] * scale;
    result.elem[2] = v.elem[2] * scale;
    result.elem[3] = v.elem[3];
    return result;
}

vec4 vec4_add(const vec4 a, const vec4 b) {
    vec4 result;
    result.elem[0] = a.elem[0] + b.elem[0];
    result.elem[1] = a.elem[1] + b.elem[1];
    result.elem[2] = a.elem[2] + b.elem[2];
    result.elem[3] = a.elem[3] + b.elem[3];
    return result;
}

void print_vec4(const vec4 v) {
    printf("x: %.10f, y: %.10f, z: %.10f, w: %.10f \n",
        v.elem[0],
        v.elem[1],
        v.elem[2],
        v.elem[3]
    );
}
