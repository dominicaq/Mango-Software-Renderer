#include "shader_math.h"

float clamp(float value, float min, float max) {
    if (value > max) {
        return max;
    } else if (value < min) {
        return min;
    }
    return value;
}

float lerp(float a, float b, float t) {
    return a * (1.0f - t) + b * t;
}