#include "shader_math.h"

float clamp(float value, float min, float max) {
    if (value >= max) {
        return max;
    } else if (value < min) {
        return min;
    }
    return value;
}

float lerp(float a, float b, float t) {
    return a * (1.0f - t) + b * t;
}

float q_rsqrt(float number) {
    union {
        float f;
        uint32_t i;
    } conv = {.f = number};
    conv.i = 0x5f3759df - (conv.i >> 1);
    conv.f *= 1.5F - (number * 0.5F * conv.f * conv.f);
    return conv.f;
}